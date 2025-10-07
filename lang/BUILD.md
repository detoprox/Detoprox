# Detoprox — Paquete `lang/` (MVP runtimes + herramientas)

Este paquete contiene **runtimes en C** y **herramientas de exportación/generación** que usará el compilador/CLI del lenguaje Detoprox cuando lo cableemos.  
Es **auto-contenible** y no rompe nada del repo.

## Contenido


> Nota: el **transpiler/CLI** aún no está integrado aquí para no tocar tu build actual.  
> En una siguiente tanda añadiremos `lang/tools/dpx/` con comandos y wiring.

---

## Requisitos

- **Compilador C/C++**: gcc/clang (C++17 para herramientas header-only).
- **SQLite** dev:
  - Debian/Ubuntu: `sudo apt install libsqlite3-dev`
  - macOS (Homebrew): `brew install sqlite`
  - Windows (MSYS2): `pacman -S mingw-w64-x86_64-sqlite3`

---

## 1) Smoke test rápido: servidor HTTP + estáticos (sin transpiler)

Este test verifica el runtime HTTP y el servido de estáticos.  
No implementa CRUD; eso llegará al cablear el transpiler.

1. Crea una carpeta `public/` junto al ejecutable y copia la UI mínima:
   - Copia `lang/examples/ui_min/index.html` a `public/index.html` (o crea el archivo manualmente).

2. Crea un archivo temporal `main.c` (local, **no hace falta subirlo al repo**) con este contenido:

```c
#include "lang/compiler/backend_c/http_runtime.h"
#include <stdio.h>
#include <string.h>

// Demo: sirve / (HTML mínimo) y /static/index.html desde ./public

int main() {
  if (dpx_init_net()!=0) { fprintf(stderr, "net init fail\n"); return 1; }
  int srv = dpx_start_server(3000);
  if (srv < 0) { perror("bind/listen"); return 1; }
  printf("HTTP server on :3000\n");

  while (1) {
    int cfd = dpx_accept(srv); if (cfd < 0) continue;
    char buf[65536]; int n = dpx_read(cfd, buf, sizeof(buf)-1);
    if (n <= 0) { dpx_close(cfd); continue; }
    buf[n] = '\0';

    char method[8], path[1024]; int clen=0;
    if (dpx_parse_req(buf, method, sizeof(method), path, sizeof(path), &clen) != 0) {
      dpx_send_response(cfd, "400 Bad Request", "text/plain", "bad request");
      dpx_close(cfd); continue;
    }

    if (strcmp(method, "OPTIONS") == 0) {
      dpx_send_response(cfd, "204 No Content", "text/plain", "");
      dpx_close(cfd); continue;
    }

    // / -> landing muy simple
    if (strcmp(method, "GET")==0 && strcmp(path, "/")==0) {
      dpx_send_response(cfd, "200 OK", "text/html",
        "<h1>Detoprox</h1><p>Servidor vivo</p><a href=\"/static/index.html\">UI mínima</a>");
      dpx_close(cfd); continue;
    }

    // /static/* -> ./public/*
    if (strncmp(path, "/static/", 8) == 0) {
      char fs[1024];
      snprintf(fs, sizeof(fs), "public/%s", path + 8);
      if (dpx_send_file(cfd, fs) != 0) {
        dpx_send_response(cfd, "404 Not Found", "text/plain", "not found");
      }
      dpx_close(cfd); continue;
    }

    dpx_send_response(cfd, "404 Not Found", "text/plain", "not found");
    dpx_close(cfd);
  }
  return 0;
}

## Compila y ejecuta (Linux/macOS):
{
   cc -O2 -std=c11 main.c -o server
./server

}

Prueba:

Navega a: http://localhost:3000/

Abre: http://localhost:3000/static/index.html

Preflight CORS:
curl -i -X OPTIONS http://localhost:3000/

Si quieres servir otros archivos, colócalos en ./public/ y accede con /static/<ruta>.#
