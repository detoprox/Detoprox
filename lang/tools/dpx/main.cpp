#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <cstdlib>
#include "../../openapi/exporter.hpp"
#include "../../openapi/ts_client_gen.hpp"
#include "../../backend_c/http_runtime.h"

using namespace std;
using namespace dpx;

static void print_help() {
    cout << "Detoprox CLI (MVP)\n";
    cout << "Usage:\n";
    cout << "  dpx serve [file.dtp]        Run local HTTP server demo\n";
    cout << "  dpx export-openapi [file]   Generate openapi.yaml\n";
    cout << "  dpx gen-ts-client [file]    Generate client.ts\n";
    cout << "  dpx help                    Show help\n";
}

static void serve_demo() {
    cout << "[serve] Iniciando servidor en :3000 ...\n";
    if (dpx_init_net()!=0) { cerr << "net init fail\n"; return; }
    int srv = dpx_start_server(3000);
    if (srv < 0) { perror("bind/listen"); return; }
    cout << "Servidor HTTP corriendo en http://localhost:3000\n";
    while (1) {
        int cfd = dpx_accept(srv); if (cfd < 0) continue;
        char buf[65536]; int n = dpx_read(cfd, buf, sizeof(buf)-1);
        if (n <= 0) { dpx_close(cfd); continue; }
        buf[n]='\0';
        char method[8], path[1024]; int clen=0;
        dpx_parse_req(buf, method, sizeof(method), path, sizeof(path), &clen);

        if (strcmp(method,"OPTIONS")==0) {
            dpx_send_response(cfd,"204 No Content","text/plain","");
            dpx_close(cfd); continue;
        }
        if (strcmp(method,"GET")==0 && strcmp(path,"/")==0) {
            dpx_send_response(cfd,"200 OK","text/html",
                "<h1>Detoprox</h1><p>Servidor activo</p><a href=\"/static/index.html\">Abrir UI</a>");
            dpx_close(cfd); continue;
        }
        if (strncmp(path,"/static/",8)==0) {
            char fs[1024]; snprintf(fs,sizeof(fs),"lang/examples/ui_min/%s", path+8);
            if (dpx_send_file(cfd,fs)!=0)
                dpx_send_response(cfd,"404 Not Found","text/plain","not found");
            dpx_close(cfd); continue;
        }
        dpx_send_response(cfd,"404 Not Found","text/plain","not found");
        dpx_close(cfd);
    }
}

static void export_openapi() {
    Program prog;
    Model m; m.name="Producto";
    m.fields.push_back({"id","int",{}});
    m.fields.push_back({"nombre","text",{"required"}});
    m.fields.push_back({"precio","decimal(10,2)",{}});
    prog.models.push_back(m);

    Service s; s.basePath="/api/productos";
    s.routes.push_back({"GET","/"}); s.routes.push_back({"GET","/:id"});
    s.routes.push_back({"POST","/"}); s.routes.push_back({"PUT","/:id"});
    s.routes.push_back({"DELETE","/:id"});
    prog.services.push_back(s);

    auto r = OpenAPIExporter::fromProgram(prog,"Detoprox API","0.1.0");
    ofstream("openapi.yaml") << r.yaml;
    cout << "✅ openapi.yaml generado.\n";
}

static void gen_ts_client() {
    ifstream in("openapi.yaml");
    if(!in){ cerr<<"❌ openapi.yaml no encontrado. Ejecuta dpx export-openapi primero.\n"; return; }
    stringstream ss; ss<<in.rdbuf();
    auto ts = dpx_generate_typescript_client(ss.str());
    ofstream("client.ts")<<ts;
    cout<<"✅ client.ts generado.\n";
}

int main(int argc,char**argv){
    if(argc<2){ print_help(); return 0; }
    string cmd=argv[1];
    if(cmd=="serve") serve_demo();
    else if(cmd=="export-openapi") export_openapi();
    else if(cmd=="gen-ts-client") gen_ts_client();
    else print_help();
    return 0;
}
