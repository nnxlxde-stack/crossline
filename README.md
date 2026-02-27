> English

# Crossline IDE

> A Python-first IDE with built-in local AI, written in modern C++23

## Table Of Contents

1. [General](#general)
    1. [Technical Requirements](#technical-requirements)
    2. [Features](#features)
    3. [Roadmap](#roadmap)
2. [Getting Started](/docs/GETTING_STARTED.md)
3. [Build Guide](#build-guide)
4. [Testing](/docs/TESTING.md)
5. [Project Information](/docs/PROJECT_INFO.md)
6. [Architecture](/docs/ARCHITECTURE.md)

## General

Crossline is a modern, lightweight IDE designed primarily for Python development at the MVP stage, with plans for multi-language support (JavaScript, TypeScript, C++, Rust, etc.). The entire IDE is built from scratch using **C++23** and a custom UI framework, emphasizing performance, extensibility, and integrated AI capabilities.

Unlike traditional Electron-based IDEs, Crossline runs natively with zero overhead and includes built-in AI assistance powered by small ONNX models (like Qwen 0.6B), enabling intelligent code completion and analysis without external API dependencies.

### Technical Requirements

**Build-time:**
- C++23 compiler (MSVC 17.4+, Clang 17+, GCC 13+)
- CMake 3.28+ with C++ modules support
- Python 3.9+ (for build tooling and MVP IDE)

**Runtime:**
- Windows 10+, Linux (glibc 2.31+), macOS 11+
- 512 MB RAM (base UI)
- 2+ GB RAM (with AI models loaded)

### Features

1. **Built-in AI via ONNX**  
   Local language models (Qwen 0.6B) integrated natively. No cloud dependencies, no latency beyond local inference.

2. **Fast & Beautiful UI**  
   Custom rendering layer built on modern graphics APIs (will support Vulkan, Metal, OpenGL 4.6). Responsive, accessible, themeable.

3. **Modern C++23 Practices**  
   Modules, concepts, ranges, `std::expected`, structured bindings, and more. Code is clean, type-safe, and verifiable at compile-time.

4. **Extensibility**  
   Plugin architecture (C++ modules + dynamic loading). Users can write extensions in C++ or Python.

5. **CMake with Module Support**  
   No more header-only hacks. Full C++20/23 module integration. Reproducible, hermetic builds.

6. **Light Testing Framework**  
   400-line testing library included. No external test runners, no framework bloat. Works cross-platform.

### Roadmap

**MVP (Early 2026)**
- [ ] Basic Python syntax highlighting and code folding
- [ ] LSP client for Python (via Pylance or pyright)
- [ ] Local AI: code completion suggestions
- [ ] Core UI framework (editor, file tree, terminal)
- [ ] Project opening/saving
- [ ] Multi-platform builds (Windows, Linux, macOS)

**Phase 2**
- [ ] Language support: JavaScript/TypeScript, C++, Rust
- [ ] Integrated terminal with shell support
- [ ] Debugger UI (DAP backend)
- [ ] Full Git integration
- [ ] Plugin marketplace (community extensions)

**Phase 3+**
- [ ] Distributed debugging (remote sessions)
- [ ] Collaborative editing
- [ ] Web IDE version (via WebAssembly)

## Getting Started

### Prerequisites

```bash
# Windows
# Install Visual Studio 2022+ (C++23 support)
# Install CMake 3.28+
# Install Git

# Linux (Ubuntu 22.04+)
sudo apt install build-essential cmake git python3 python3-pip

# macOS
# Install Xcode Command Line Tools
# Install CMake via Homebrew
```

### Clone & Build

```bash
git clone https://github.com/nnxlxde-stack/crossline.git
cd crossline
cmake --preset default
cmake --build build
```

See [Build Guide](#build-guide) for detailed instructions.

## Build Guide

### Configure

```bash
cmake --list-presets
cmake --preset default          # Debug build, development mode
cmake --preset release          # Optimized, stripped symbols
cmake --preset ci               # For CI/CD pipelines
```

### Compile

```bash
cmake --build build                    # Default (Debug)
cmake --build build --config Release   # or Release
cmake --build build --target tests     # Run test suite
```

### Install

```bash
cmake --install build --prefix ~/.local
# or system-wide (requires sudo):
cmake --install build --prefix /usr/local
```

### Development Workflow

1. **Make changes** to C++ source (`core/`, `tools/`)
2. **Rebuild:** `cmake --build build`
3. **Run tests:** `cmake --build build --target tests`
4. **Generate HTML report:** `python tools/test_results_to_html.py`

> Russian

# Crossline IDE

> Python-ориентированная IDE со встроенным и локальным AI, написанная на современном C++23

## Содержание

1. [Общее](#общее)
    1. [Технические требования](#технические-требования)
    2. [Возможности](#возможности)
    3. [Дорожная карта](#дорожная-карта)
2. [Первые шаги](/docs/GETTING_STARTED.md)
3. [Руководство по сборке](#руководство-по-сборке)
4. [Тестирование](/docs/TESTING.md)
5. [Информация о проекте](/docs/PROJECT_INFO.md)
6. [Архитектура](/docs/ARCHITECTURE.md)

## Общее

Crossline — это современная, легковесная IDE, разработанная в первую очередь для Python на этапе MVP, с планами поддержки других языков (JavaScript, TypeScript, C++, Rust и т. д.). Вся IDE написана с нуля на **C++23** с собственным UI фреймворком, с упором на производительность, расширяемость и встроенные возможности AI.

В отличие от традиционных IDE на Electron, Crossline работает нативно без оверхеда и включает встроенный AI на основе малых моделей ONNX (например, Qwen 0.6B), что позволяет интеллектуально дополнять код и проводить анализ без зависимости от внешних API.

### Технические требования

**На этапе сборки:**
- Компилятор C++23 (MSVC 17.4+, Clang 17+, GCC 13+)
- CMake 3.28+ с поддержкой C++ модулей
- Python 3.9+ (для инструментов и MVP IDE)

**Во время выполнения:**
- Windows 10+, Linux (glibc 2.31+), macOS 11+
- 512 МБ ОЗУ (базовый UI)
- 2+ ГБ ОЗУ (с загруженными моделями AI)

### Возможности

1. **Встроенный AI через ONNX**  
   Локальные языковые модели (Qwen 0.6B) интегрированы нативно. Без облачных зависимостей, без задержек сверх локального вывода.

2. **Быстрый и красивый UI**  
   Собственный слой рендеринга на основе современных графических API (будет поддерживать Vulkan, Metal, OpenGL 4.6). Отзывчивый, доступный, тематизируемый.

3. **Современные практики C++23**  
   Модули, концепции, ranges, `std::expected`, структурированные привязки и многое другое. Код чистый, типобезопасный и проверяемый во время компиляции.

4. **Расширяемость**  
   Архитектура плагинов (C++ модули + динамическая загрузка). Пользователи могут писать расширения на C++ или Python.

5. **CMake с поддержкой модулей**  
   Больше никаких хаков с заголовками. Полная интеграция модулей C++20/23. Воспроизводимые, герметичные сборки.

6. **Лёгкий фреймворк тестирования**  
   400-строчная библиотека тестирования включена. Нет внешних раннеров, нет перегруза фреймворка. Работает кроссплатформенно.

### Дорожная карта

**MVP (начало 2026)**
- [ ] Базовое выделение синтаксиса Python и складывание кода
- [ ] LSP клиент для Python (через Pylance или pyright)
- [ ] Локальный AI: предложения автодополнения
- [ ] Основной фреймворк UI (редактор, дерево файлов, терминал)
- [ ] Открытие/сохранение проектов
- [ ] Мультиплатформенные сборки (Windows, Linux, macOS)

**Этап 2**
- [ ] Поддержка языков: JavaScript/TypeScript, C++, Rust
- [ ] Встроенный терминал с поддержкой shell
- [ ] UI отладчика (DAP бэкенд)
- [ ] Полная интеграция Git
- [ ] Мартетплейс плагинов (расширения сообщества)

**Этап 3+**
- [ ] Распределённая отладка (удалённые сессии)
- [ ] Совместное редактирование
- [ ] Web IDE версия (через WebAssembly)

## Первые шаги

### Предварительные условия

```bash
# Windows
# Установить Visual Studio 2022+ (поддержка C++23)
# Установить CMake 3.28+
# Установить Git

# Linux (Ubuntu 22.04+)
sudo apt install build-essential cmake git python3 python3-pip

# macOS
# Установить Xcode Command Line Tools
# Установить CMake через Homebrew
```

### Клонирование и сборка

```bash
git clone https://github.com/nnxlxde-stack/crossline.git
cd crossline
cmake --preset default
cmake --build build
```

Подробнее в [Руководстве по сборке](#руководство-по-сборке).

## Руководство по сборке

### Настройка

```bash
cmake --list-presets
cmake --preset default          # Debug сборка, режим разработки
cmake --preset release          # Оптимизированная, удалены символы
cmake --preset ci               # Для CI/CD конвейеров
```

### Компиляция

```bash
cmake --build build                    # По умолчанию (Debug)
cmake --build build --config Release   # или Release
cmake --build build --target tests     # Запустить набор тестов
```

### Установка

```bash
cmake --install build --prefix ~/.local
# или системная (требует sudo):
cmake --install build --prefix /usr/local
```

### Рабочий процесс разработки

1. **Сделать изменения** в исходном коде C++ (`core/`, `tools/`)
2. **Пересобрать:** `cmake --build build`
3. **Запустить тесты:** `cmake --build build --target tests`
4. **Сгенерировать HTML отчёт:** `python tools/test_results_to_html.py`
