#pragma once
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// === Minimal JSON helpers for Detoprox (C99/C++17) ===
// NOT a full JSON parser — just enough for simple POST/PUT validation.
// Supports: presence check, string/number/bool extraction for flat objects.

// Returns 1 if key (as `"name"`) appears anywhere in body (raw search).
static int dpx_json_has_key(const char* body, const char* quoted_key) {
    // quoted_key must be like "\"field\""
    return strstr(body, quoted_key) ? 1 : 0;
}

// Extracts a JSON string value for "key": "value" into out.
// - quoted_key must be like "\"field\"" (including quotes).
// - Returns 0 on success, 1 on not found/invalid.
static int dpx_json_find_string_value(const char* body, const char* quoted_key,
                                      char* out, int outcap) {
    const char* p = body;
    int klen = (int)strlen(quoted_key);
    while ((p = strstr(p, quoted_key))) {
        const char* q = p + klen;
        while (*q && isspace((unsigned char)*q)) q++;
        if (*q != ':') { p++; continue; }
        q++;
        while (*q && isspace((unsigned char)*q)) q++;
        if (*q != '\"') { p++; continue; }
        q++; // inside string
        int i = 0;
        while (*q && *q != '\"' && i < outcap - 1) {
            if (*q == '\\' && q[1]) { // skip escaped char
                q++;
                if (!*q) break;
            }
            out[i++] = *q++;
        }
        out[i] = '\0';
        return 0;
    }
    return 1;
}

// Extracts a JSON number value for "key": 123.45 into *out.
// - quoted_key must be like "\"field\"" (including quotes).
// - Accepts leading +/-, digits, and one dot.
// - Returns 0 on success, 1 on not found/invalid.
static int dpx_json_find_number_value(const char* body, const char* quoted_key, double* out) {
    const char* p = strstr(body, quoted_key);
    if (!p) return 1;
    const char* q = p + strlen(quoted_key);
    while (*q && isspace((unsigned char)*q)) q++;
    if (*q != ':') return 1;
    q++;
    while (*q && isspace((unsigned char)*q)) q++;
    char buf[64];
    int i = 0, dots = 0;
    if (*q == '+' || *q == '-') buf[i++] = *q++;
    while (*q && (isdigit((unsigned char)*q) || *q == '.')) {
        if (*q == '.') { if (dots++) break; }
        if (i < (int)sizeof(buf) - 1) buf[i++] = *q;
        q++;
    }
    buf[i] = '\0';
    if (i == 0) return 1;
    *out = atof(buf);
    return 0;
}

// Extracts a JSON boolean value for "key": true/false into *out (1/0).
// - quoted_key must be like "\"field\"" (including quotes).
// - Returns 0 on success, 1 on not found/invalid token.
static int dpx_json_find_bool_value(const char* body, const char* quoted_key, int* out) {
    const char* p = strstr(body, quoted_key);
    if (!p) return 1;
    const char* q = p + strlen(quoted_key);
    while (*q && isspace((unsigned char)*q)) q++;
    if (*q != ':') return 1;
    q++;
    while (*q && isspace((unsigned char)*q)) q++;
    if (strncmp(q, "true", 4) == 0) { *out = 1; return 0; }
    if (strncmp(q, "false", 5) == 0) { *out = 0; return 0; }
    return 1;
}
