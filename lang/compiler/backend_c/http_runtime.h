#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <sys/stat.h>
#include <arpa/inet.h>

#ifndef DPX_HTTP_MAX
#define DPX_HTTP_MAX 65536
#endif

// === Basic HTTP Runtime (C99/C++17 compatible) ===
// Provides: simple HTTP server, CORS support, static files, query parsing.
// Used internally by Detoprox transpiled C backends.

static int dpx_init_net() { return 0; }

static int dpx_start_server(int port) {
    int srv = socket(AF_INET, SOCK_STREAM, 0);
    if (srv < 0) return -1;
    int opt = 1;
    setsockopt(srv, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    if (bind(srv, (struct sockaddr*)&addr, sizeof(addr)) < 0) return -2;
    if (listen(srv, 64) < 0) return -3;
    return srv;
}

static int dpx_accept(int srv) {
    struct sockaddr_in caddr;
    socklen_t len = sizeof(caddr);
    return accept(srv, (struct sockaddr*)&caddr, &len);
}

static int dpx_read(int cfd, char* buf, int cap) { return read(cfd, buf, cap); }
static void dpx_close(int fd) { close(fd); }

static int dpx_send_response(int cfd, const char* status, const char* ctype, const char* body) {
    char head[1024];
    int blen = strlen(body);
    int hlen = snprintf(head, sizeof(head),
        "HTTP/1.1 %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %d\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n"
        "Access-Control-Allow-Headers: Content-Type, Authorization\r\n"
        "\r\n",
        status, ctype, blen);
    write(cfd, head, hlen);
    write(cfd, body, blen);
    return 0;
}

static int dpx_send_response_raw(int cfd, const char* status, const char* ctype, const char* body, int blen) {
    char head[512];
    int hlen = snprintf(head, sizeof(head),
        "HTTP/1.1 %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %d\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n"
        "Access-Control-Allow-Headers: Content-Type, Authorization\r\n"
        "\r\n",
        status, ctype, blen);
    write(cfd, head, hlen);
    write(cfd, body, blen);
    return 0;
}

// Serve static files from ./public/
static int dpx_send_file(int cfd, const char* path) {
    FILE* f = fopen(path, "rb");
    if (!f) return -1;
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* buf = malloc(size);
    if (!buf) { fclose(f); return -2; }
    fread(buf, 1, size, f);
    fclose(f);
    char head[512];
    snprintf(head, sizeof(head),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %ld\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "\r\n", size);
    write(cfd, head, strlen(head));
    write(cfd, buf, size);
    free(buf);
    return 0;
}

// Parse HTTP request line + headers
static int dpx_parse_req(const char* req, char* method, int mcap, char* path, int pcap, int* clen) {
    *clen = 0;
    const char* sp1 = strchr(req, ' ');
    if (!sp1) return 1;
    int ml = sp1 - req;
    if (ml >= mcap) ml = mcap - 1;
    memcpy(method, req, ml);
    method[ml] = '\0';
    const char* sp2 = strchr(sp1 + 1, ' ');
    if (!sp2) return 1;
    int pl = sp2 - (sp1 + 1);
    if (pl >= pcap) pl = pcap - 1;
    memcpy(path, sp1 + 1, pl);
    path[pl] = '\0';
    const char* cl = strstr(req, "Content-Length:");
    if (cl) *clen = atoi(cl + 15);
    return 0;
}

// Match basic pattern /api/foo/:id
static int dpx_match_seg(const char* pat, const char* path) {
    const char* p = pat;
    const char* s = path;
    while (*p && *s) {
        if (*p == ':') {
            while (*s && *s != '/') s++;
            while (*p && *p != '/') p++;
        } else if (*p != *s) {
            return 0;
        } else {
            p++; s++;
        }
    }
    return (*p == *s);
}

// === Querystring decoding (for pagination & search) ===
static void dpx_url_decode(char* s) {
    char* w = s;
    for (char* r = s; *r; r++) {
        if (*r == '+') *w++ = ' ';
        else if (*r == '%' && isxdigit((unsigned char)r[1]) && isxdigit((unsigned char)r[2])) {
            int hi = isdigit(r[1]) ? r[1]-'0' : 10 + (tolower(r[1])-'a');
            int lo = isdigit(r[2]) ? r[2]-'0' : 10 + (tolower(r[2])-'a');
            *w++ = (char)((hi<<4)|lo);
            r += 2;
        } else *w++ = *r;
    }
    *w = '\0';
}

static int dpx_get_query_value(const char* path, const char* key, char* out, int outcap) {
    const char* q = strchr(path, '?');
    if (!q) return 1;
    q++;
    size_t klen = strlen(key);
    while (*q) {
        const char* e = strchr(q, '&');
        if (!e) e = q + strlen(q);
        const char* eq = strchr(q, '=');
        if (eq && eq < e) {
            if ((size_t)(eq - q) == klen && strncmp(q, key, klen) == 0) {
                int n = (int)(e - (eq + 1));
                if (n >= outcap) n = outcap - 1;
                memcpy(out, eq + 1, n);
                out[n] = '\0';
                dpx_url_decode(out);
                return 0;
            }
        }
        q = (*e) ? e + 1 : e;
    }
    return 1;
}
