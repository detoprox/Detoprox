# 👋 Hola, soy Detoprox

### 🧠 Creando el lenguaje donde las ideas se convierten en sistemas.

**Detoprox** es un proyecto de innovación tecnológica que busca unir creatividad, programación y automatización en una sola plataforma.

Inspirado en **C/C++**, este lenguaje nace con una visión clara:
> *“Construir sistemas que piensen, no solo que ejecuten.”*

---

### 🌍 Nuestra visión
Desarrollar un lenguaje expansible, moderno y humano, capaz de integrarse con **IA, web, y automatización**, sin perder la lógica estructurada que nos da control total sobre el sistema.

---

### 💡 En desarrollo
- Diseño de sintaxis moderna y modular  
- Compilador base experimental  
- Integración con HTML, CSS y JS  
- Comunidad global en formación  

---

### 🌐 Conéctate con nosotros
🔹 [YouTube - @detoprox](https://youtube.com/@detoprox)  
🔹 [GitHub - Proyecto Detoprox](https://github.com/detoprox)  
🔹 Discord (próximamente)

---

> *“De todo programas” — ese fue el sueño que inició todo.*

📘 **Detoprox** — *Where ideas become systems.*

# 🧠 Detoprox — Donde las ideas se convierten en sistemas

> **Filosofía:** “Where ideas become systems / Donde las ideas se convierten en sistemas.”  
> **Estado actual:** Fase 2 — *Desarrollo técnico (MVP)*  
> **Repositorio:** https://github.com/detoprox/Detoprox

---

## 🚀 ¿Qué es Detoprox?

Detoprox no es solo un lenguaje de programación: es un **sistema generador de sistemas**.  
Su propósito es unificar **backend, frontend, automatización e inteligencia artificial** en una sola plataforma coherente, eficiente y ética.

- ⚙️ Bajo nivel cuando se necesita rendimiento.  
- 🧩 Alto nivel cuando se busca productividad.  
- 🌍 Multiplataforma y full-stack por diseño.  
- 💠 Con filosofía ética: **DTPX**, su moneda de utilidad, recompensa aportes reales (código, documentación, educación).

---

## 🏗️ Estructura actual del proyecto

# 🧱 Estructura actual del proyecto

```plaintext
Detoprox/
├── lang/                         # Núcleo técnico del lenguaje
│   ├── compiler/backend_c/       # Runtimes en C (HTTP + SQLite + JSON)
│   ├── tools/
│   │   ├── openapi/              # Generadores OpenAPI 3.0 + cliente TypeScript
│   │   ├── react/                # Hooks de integración con React
│   │   └── dpx/                  # CLI mínima (serve / export-openapi / gen-ts-client)
│   ├── examples/ui_min/          # Interfaz HTML mínima conectada a la API
│   └── BUILD.md                  # Guía técnica de compilación y pruebas
├── docs/                         # (Próximamente) Documentación oficial
└── README.md                     # Este archivo.

---

##⚙️ ¿Qué puede hacer Detoprox hoy?

✅ **Backend real en C (runtime estable):**  
Levanta un servidor HTTP, maneja CORS, sirve archivos estáticos y se comunica con SQLite usando consultas seguras.  

✅ **Interfaz web lista:**  
Incluye una UI mínima (`examples/ui_min/index.html`) para probar el CRUD desde el navegador.  

✅ **Generación automática de documentación y SDKs:**  
Con `exporter.hpp` y `ts_client_gen.hpp` puedes generar:  
- `openapi.yaml` (documentación REST/GraphQL)  
- `client.ts` (SDK tipado para TypeScript/React)  

✅ **Integración React inmediata:**  
Los hooks `useDetoproxQuery` y `useDetoproxMutation` ya permiten consumir la API Detoprox desde cualquier app React o Vite.

✅ **CLI mínima funcional (`dpx`):**  
Compilable desde `lang/tools/dpx/main.cpp` y usable con tres comandos base:

| Comando | Descripción |
|----------|--------------|
| `dpx serve` | Levanta un servidor HTTP de prueba y sirve la UI mínima. |
| `dpx export-openapi` | Genera `openapi.yaml` desde un modelo simulado (`Producto`). |
| `dpx gen-ts-client` | Crea `client.ts` (cliente TypeScript tipado) desde el OpenAPI generado. |

📌 Ejemplo rápido:
```bash
./bin/dpx export-openapi
./bin/dpx gen-ts-client
./bin/dpx serve

Luego abre http://localhost:3000/static/index.html en el navegador.
🧠 Filosofía técnica

##Detoprox se basa en el patrón Hexa-Use (Hexagonal + Use Cases + DDD-lite):

Domain: entidades, reglas y eventos.

Application: casos de uso, puertos, lógica.

Adapters: HTTP, DB, IA, automatización.

Config: wiring y entorno.

Este diseño mantiene la lógica del dominio pura, con adaptadores reemplazables y escalabilidad real para futuras integraciones (WASM, gRPC, IA, blockchain).

| Etapa    | Meta                                                              |
| -------- | ----------------------------------------------------------------- |
| **F2.8** | Búsqueda y orden generalizados en múltiples campos.               |
| **F2.9** | CLI completa con parsing real de `.dtp`.                          |
| **F3**   | Comunidad open source: documentación, tutoriales y canal Discord. |
| **F4**   | Integración con DTPX (moneda de utilidad y gobernanza).           |
| **F5**   | Ecosistema total: Detoprox Hub, Academy y Chain.                  |

#🤝 Cómo aportar

##👨‍💻 Desarrolladores:
Puedes contribuir con runtime, CLI, documentación o ejemplos.

##🧩 Diseñadores y comunicadores:
Ayuda con identidad visual, comunidad y contenido educativo.

##💰 Colaboradores tempranos:
Obtendrán reconocimiento y acceso al programa DTPX (recompensas por contribución real).

##📬 Contacto / comunidad:

GitHub: @detoprox

YouTube: Próximamente — Devlogs y avances del lenguaje
Discord: Próximamente — Comunidad técnica

##🌟 Cierre

Detoprox busca redefinir la relación entre el código, el conocimiento y el valor.
No se trata de escribir software, sino de crear sistemas que generen oportunidades reales.
