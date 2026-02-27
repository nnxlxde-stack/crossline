> English

# Crossline Project Information

## Vision

Crossline is a next-generation integrated development environment (IDE) that reimagines how developers work with code. By combining native performance, modern language design, and integrated AI capabilities, Crossline delivers an uncompromising development experience without compromises on speed or features.

### Core Philosophy

1. **Native First:** No Electron, no web bloat. Written entirely in C++23 with a custom rendering pipeline.
2. **Local AI:** Developers own their intelligence. Models run locally; your code never leaves your machine.
3. **Zero Friction:** From startup time to keystroke latency, every millisecond matters.
4. **Modern Practices:** Leverage C++23 language features instead of working around them.

## Why This Project?

Today's IDEs have fallen into a trap:

- **VSCode & Atom:** Built on Electron (Chromium), consuming gigabytes of RAM for each instance.
- **JetBrains IDEs:** Powerful but heavy, written in Kotlin/Java, requiring significant resources.
- **Vim/Emacs:** Fast but not beginner-friendly, steep learning curve.

Crossline bridges this gap:
- **Lightweight:** Base IDE < 50 MB, with optional AI models (~500 MB–2 GB).
- **Intelligent:** AI assistance is built-in, not bolted on (no "Sign in to OpenAI" pop-ups).
- **Modern:** Uses C++23 features like modules, concepts, and `std::expected` for safer, cleaner code.
- **Accessible:** GUI-first, but scriptable and extensible.

## Target Users (MVP)

- **Python developers** (primary)
- **Hobbyists & academics** wanting a lightweight, intelligent environment
- **Embedded developers** bootstrapping project setups

## Future Users (Phase 2+)

- Web developers (JavaScript/TypeScript)
- Systems programmers (C++, Rust)
- Data scientists (Jupyter-like notebooks)
- Game developers (multi-language support)

## Technical Decision Rationale

### Why C++23?

- **Performance:** Native execution, no VM overhead, predictable latency.
- **Type Safety:** Modules + concepts catch errors at compile-time.
- **Expressiveness:** Ranges, structured bindings, pattern matching (future), semantic reflection.
- **Maturity:** Standard toolchain, proven in production systems.

### Why Custom UI?

- **Ownership:** Full control over rendering, layout, and responsiveness.
- **Performance:** Avoid web/Electron overhead; direct GPU access where available.
- **Theming:** Efficient, data-driven styling (not CSS bloat).
- **Accessibility:** Native APIs for screen readers, keyboard navigation.

### Why Local AI (ONNX)?

- **Privacy:** No telemetry, no cloud vendor lock-in.
- **Latency:** Sub-100ms response for intelligent features.
- **Cost:** No API calls = no subscriptions.
- **Reliability:** Works offline.

## Dependencies

### Build-time
- CMake 3.28+
- C++23 compiler (MSVC, Clang, GCC)
- Python 3.9+ (build tooling)

### Runtime
- None (base IDE)
- ONNX Runtime (optional, for AI features)
- GLFW 3+ (cross-platform windowing, future)

### No External Runtime Dependencies
Unlike Electron, Node.js, JVM, or .NET Runtime required.

## Architecture Overview

```
crossline/
├── core/
│   ├── main.cpp                    # Entry point
│   └── user-interface/             # UI framework & components
│       ├── CMakeLists.txt
│       ├── lib_main.cpp
│       └── modules/
│           ├── crossline_ui.cppm   # Main UI module
│           ├── crossline_ui_context.cppm
│           └── crossline_ui_reactive.cppm    # Reactive data binding
├── tests/
│   ├── light_test.cppm            # Testing framework
│   ├── main.cpp
│   ├── CMakeLists.txt
│   └── crossline_ui/               # UI subsystem tests
│       ├── crossline_ui_reactive_tests.cppm
│       └── crossline_ui_reactive.cppm
├── tools/
│   ├── run_tests.bat               # Test runner script
│   └── test_results_to_html.py     # Test report generator
└── external/
    └── GLFW/                       # Cross-platform windowing API
```

## Key Modules (C++23)

| Module | Purpose | Status |
|--------|---------|--------|
| `light_test` | Testing framework | ✅ Complete |
| `crossline_ui` | Main UI library | 🚧 In Development |
| `crossline_ui_reactive` | Reactive state management | 🚧 In Development |
| `crossline_ui_context` | Global app context | 🚧 In Development |
| `onnx_runtime` (future) | AI model inference | 📋 Planned |
| `lsp_client` (future) | Language Server Protocol | 📋 Planned |

## Testing Strategy

Crossline uses a lightweight, in-house testing framework (`light_test`) that:
- Requires zero external dependencies
- Produces JSON output for CI integration
- Generates HTML reports via Python script
- Works cross-platform without modifications

See [TESTING.md](TESTING.md) for details.

## Build Philosophy

- **Hermetic builds:** No reliance on system-wide libraries (except OS SDKs).
- **Reproducible:** Same inputs → same outputs, always.
- **Fast:** Incremental builds, parallel compilation, minimal linking.
- **Transparent:** `cmake --list-presets` shows all available configurations.

## Contributing

Future contribution guidelines will be documented in `CONTRIBUTING.md`.

Current focus areas:
- [ ] Core UI framework stability
- [ ] Python LSP integration
- [ ] Basic syntax highlighting
- [ ] Reactive state management improvements

---

> Russian

# Информация о проекте Crossline

## Видение

Crossline — это IDE нового поколения, переосмысляющая, как разработчики работают с кодом. Объединяя нативную производительность, современный дизайн языка и встроенные возможности AI, Crossline обеспечивает непреклонный опыт разработки без компромиссов в скорости или функциях.

### Базовая философия

1. **Native First:** Нет Electron, нет веб-раздутости. Написано полностью на C++23 с собственным конвейером рендеринга.
2. **Локальный AI:** Разработчики владеют своим интеллектом. Модели работают локально; ваш код никогда не покидает вашу машину.
3. **Нулевое трение:** От времени запуска до задержки нажатия клавиши, каждая миллисекунда имеет значение.
4. **Современные практики:** Используйте возможности C++23 вместо работы вокруг них.

## Почему этот проект?

Современные IDE зашли в тупик:

- **VSCode & Atom:** Построены на Electron (Chromium), потребляя гигабайты оперативной памяти на каждый экземпляр.
- **JetBrains IDE:** Мощные, но тяжёлые, написаны на Kotlin/Java, требуют значительных ресурсов.
- **Vim/Emacs:** Быстрые, но не удобны для начинающих, крутая кривая обучения.

Crossline закрывает этот пробел:
- **Легковесная:** Базовая IDE < 50 МБ, с опциональными моделями AI (~500 МБ–2 ГБ).
- **Интеллектуальная:** AI встроена внутрь, а не приклеена (без всплывающих окон "Войди в OpenAI").
- **Современная:** Использует возможности C++23 типа модули, концепции и `std::expected` для более безопасного, чистого кода.
- **Доступная:** Ориентирована на GUI, но скриптуемая и расширяемая.

## Целевые пользователи (MVP)

- **Разработчики на Python** (первичные)
- **Любители и академики**, ищущие легковесное, интеллектуальное окружение
- **Встроенные разработчики (Embedded)** при начальной загрузке проектов

## Будущие пользователи (этап 2+)

- Веб-разработчики (JavaScript/TypeScript)
- Системные программисты (C++, Rust)
- Специалисты по данным (ноутбуки типа Jupyter)
- Разработчики игр (поддержка нескольких языков)

## Обоснование технических решений

### Почему C++23?

- **Производительность:** Нативное выполнение, отсутствие оверхеда VM, предсказуемая задержка.
- **Типобезопасность:** Модули + концепции ловят ошибки во время компиляции.
- **Выразительность:** Ranges, структурированные привязки, pattern matching (будущее), семантическое отражение.
- **Зрелость:** Стандартная цепочка инструментов, доказано в производственных системах.

### Почему собственный UI?

- **Владение:** Полный контроль над рендерингом, макетом и отзывчивостью.
- **Производительность:** Избегаем оверхеда web/Electron; прямой доступ к GPU где доступен.
- **Тематизация:** Эффективный, управляемый данными стиль (не CSS раздутость).
- **Доступность:** Нативные API для программ чтения с экрана, навигация по клавиатуре.

### Почему локальный AI (ONNX)?

- **Приватность:** Нет телеметрии, нет привязки к облачному поставщику.
- **Задержка:** Подответ < 100ms для интеллектуальных функций.
- **Стоимость:** Нет API вызовов = нет подписок.
- **Надёжность:** Работает офлайн.

## Зависимости

### На этапе сборки
- CMake 3.28+
- Компилятор C++23 (MSVC, Clang, GCC)
- Python 3.9+ (инструменты сборки)

### Во время выполнения
- Нет (базовая IDE)
- ONNX Runtime (опционально, для функций AI)
- GLFW 3+ (кроссплатформенное окна, будущее)

### Нет внешних зависимостей во время выполнения
В отличие от Electron, Node.js, JVM или .NET Runtime.

## Обзор архитектуры

```
crossline/
├── core/
│   ├── main.cpp                    # Точка входа
│   └── user-interface/             # UI фреймворк и компоненты
│       ├── CMakeLists.txt
│       ├── lib_main.cpp
│       └── modules/
│           ├── crossline_ui.cppm   # Основной модуль UI
│           ├── crossline_ui_context.cppm
│           └── crossline_ui_reactive.cppm    # Реактивная привязка данных
├── tests/
│   ├── light_test.cppm            # Фреймворк тестирования
│   ├── main.cpp
│   ├── CMakeLists.txt
│   └── crossline_ui/               # Тесты подсистемы UI
│       ├── crossline_ui_reactive_tests.cppm
│       └── crossline_ui_reactive.cppm
├── tools/
│   ├── run_tests.bat               # Скрипт раннера тестов
│   └── test_results_to_html.py     # Генератор отчётов тестов
└── external/
    └── GLFW/                       # Кроссплатформенный API окна
```

## Ключевые модули (C++23)

| Модуль | Назначение | Статус |
|--------|-----------|--------|
| `light_test` | Фреймворк тестирования | ✅ Завершен |
| `crossline_ui` | Основная UI библиотека | 🚧 В разработке |
| `crossline_ui_reactive` | Управление реактивным состоянием | 🚧 В разработке |
| `crossline_ui_context` | Глобальный контекст приложения | 🚧 В разработке |
| `onnx_runtime` (будущее) | Вывод модели AI | 📋 Запланирован |
| `lsp_client` (будущее) | Language Server Protocol | 📋 Запланирован |

## Стратегия тестирования

Crossline использует легковесный, внутренний фреймворк тестирования (`light_test`), который:
- Требует нулевых внешних зависимостей
- Выводит JSON для интеграции CI
- Генерирует HTML отчёты через Python скрипт
- Работает кроссплатформенно без модификаций

Подробнее в [TESTING.md](TESTING.md).

## Философия сборки

- **Герметичные сборки:** Нет полагания на системные библиотеки (кроме OS SDK).
- **Воспроизводимые:** Те же входные данные → те же выходные, всегда.
- **Быстрые:** Инкрементальные сборки, параллельная компиляция, минимальная линковка.
- **Прозрачные:** `cmake --list-presets` показывает все доступные конфигурации.

## Основной вклад

Будущие рекомендации по внесению вклада будут задокументированы в `CONTRIBUTING.md`.

Области текущего внимания:
- [ ] Стабильность основного UI фреймворка
- [ ] Интеграция Python LSP
- [ ] Базовое выделение синтаксиса
- [ ] Улучшения управления реактивным состоянием
