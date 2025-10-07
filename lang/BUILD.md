lang/
├─ BUILD.md                      # este archivo
├─ compiler/
│  └─ backend_c/
│     ├─ http_runtime.h          # servidor HTTP básico + CORS + estáticos + querystring
│     ├─ sqlite_runtime.h        # helpers SQLite (prepared statements)
│     └─ json_parse_min.h        # parser JSON mínimo (string/number/bool)
├─ tools/
│  ├─ openapi/
│  │  ├─ exporter.hpp            # generador OpenAPI 3.0 (header-only, C++17)
│  │  └─ ts_client_gen.hpp       # generador cliente TypeScript (header-only)
│  └─ react/
│     └─ hooks.ts                # useDetoproxQuery / useDetoproxMutation
└─ examples/
   └─ ui_min/
      └─ index.html              # UI mínima para pruebas manuales
Requisitos

Compilador C/C++: gcc/clang (C++17 para herramientas header-only).

SQLite dev:

Debian/Ubuntu: sudo apt install libsqlite3-dev

macOS (Homebrew): brew install sqlite

Windows (MSYS2): pacman -S mingw-w64-x86_64-sqlite3
