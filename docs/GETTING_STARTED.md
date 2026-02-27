> English

# Getting Started with Crossline

## Quick Setup (5 minutes)

### Prerequisites

**Windows:**
```powershell
# Check if you have Visual Studio 2022+
& "${env:ProgramFiles}\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

# If not installed, download from:
# https://visualstudio.microsoft.com/

# Install CMake 3.28+
choco install cmake  # or download from https://cmake.org/download/
```

**Linux (Ubuntu 22.04+):**
```bash
sudo apt update
sudo apt install build-essential cmake git python3 python3-pip
```

**macOS:**
```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install CMake via Homebrew
brew install cmake git
```

### Clone the Repository

```bash
git clone https://github.com/yourusername/crossline.git
cd crossline
```

### Build

```bash
# Configure with default (Debug) preset
cmake --preset default

# Build all targets
cmake --build build

# Run tests
cmake --build build --target tests
```

## Project Structure at a Glance

```
crossline/
├── core/                    # Main application
│   ├── main.cpp             # Entry point
│   └── user-interface/      # UI framework (C++23 modules)
│       ├── CMakeLists.txt
│       └── modules/
│           ├── crossline_ui.cppm
│           ├── crossline_ui_context.cppm
│           └── crossline_ui_reactive.cppm
├── tests/                   # Test suite
│   ├── light_test.cppm      # Testing framework (400 LOC)
│   ├── main.cpp
│   └── crossline_ui/        # UI tests
├── docs/                    # Documentation
│   ├── TESTING.md           # How to write tests
│   ├── PROJECT_INFO.md      # Project vision & rationale
│   ├── ARCHITECTURE.md      # Design & module layout
│   └── README.md            # Overview (you are here)
├── tools/                   # Build & test tools
│   ├── run_tests.bat        # Test runner (Windows)
│   └── test_results_to_html.py  # Generate HTML reports
└── CMakeLists.txt           # Root configuration
```

## Common Tasks

### Running Tests

```bash
# Build and run test suite
cmake --build build --target tests

# Generate HTML report
python tools/test_results_to_html.py

# Open results in browser
start build/results.html          # Windows
open build/results.html           # macOS
xdg-open build/results.html       # Linux
```

### Writing a New Test

1. Create a test function in a `.cppm` file:

```cpp
bool test_my_feature() {
    // Arrange
    auto widget = create_button("Click me");
    
    // Act
    widget.click();
    
    // Assert
    return widget.was_clicked();
}
```

2. Register it:

```cpp
export void register_my_tests() {
    auto t = make_test(
        "My Test Suite",
        "Testing new features",
        {make_test_case("my_feature", "Feature description", test_my_feature)}
    );
    register_tests({t});
}
```

3. Import and call in `tests/main.cpp`:

```cpp
import my_tests;

int main() {
    register_my_tests();
    return light_test::run_all_tests_and_print();
}
```

### Modifying the UI Framework

All UI code is in `core/user-interface/modules/`. 

To add a new widget:

1. Declare in `crossline_ui.cppm`:
```cpp
export class my_widget {
    // public interface
};
```

2. Implement in the same file or separate module

3. Update `core/user-interface/CMakeLists.txt` if needed

4. Test with a new test case in `tests/crossline_ui/`

### Checking Build Success

```bash
# Verbose output (if build fails)
cmake --build build --verbose

# Specific target
cmake --build build --target crossline-user-interface

# Check for errors
cmake --build build 2>&1 | grep -i error
```

## Development Workflow

### Typical Day

```bash
# 1. Start of day: update and build
git pull
cmake --build build

# 2. Make changes (edit .cppm, .cpp files)
# ... edit code ...

# 3. Test immediately
cmake --build build --target tests

# 4. Iteratively test as you code
# ... edit code ...
cmake --build build
cmake --build build --target tests

# 5. Before committing, ensure all tests pass
cmake --build build --target tests
python tools/test_results_to_html.py

# 6. View results if any fail
open build/results.html

# 7. Commit and push
git add .
git commit -m "Feature: add new widget"
git push
```

## Editing Setup

### Recommended Tools

**VS Code + Extensions:**
- C/C++ (ms-vscode.cpptools)
- CMake Tools (ms-vscode.cmake-tools)
- Clang-Format (xaver.clang-format)

**Visual Studio 2022:**
- Built-in C++ and CMake support
- Integrated test explorer

**CLion (JetBrains):**
- Full C++20/23 support
- Excellent CMake integration

### Editor Configuration

Copy `.clangd` to your workspace root for IDE-aware code completion:

```yaml
# .clangd (included in repo)
CompileFlags:
    Add: [-std=c++23, -fmodules-ts]
```

## Troubleshooting

### "CMake version too old"

```bash
cmake --version  # Check version

# Update CMake
# Windows: choco upgrade cmake
# macOS: brew upgrade cmake
# Linux: sudo apt upgrade cmake
```

### "C++ compiler doesn't support C++23"

```bash
# Check compiler
cmake --build build --verbose 2>&1 | grep range

# Update compiler:
# Windows: Install Visual Studio 2022 (17.4+)
# macOS: xcode-select --install (12.0+)
# Linux: sudo apt install g++-13 (or g++-14)
```

### "Module not found" errors

```bash
# Clean and reconfigure
rm -rf build
cmake --preset default
cmake --build build
```

### Tests fail on first run

- Ensure CMake configured with C++23 support
- Check `build/CMakeFiles/CMakeConfigureLog.yaml` for details
- Run `cmake --build build --verbose` to see compilation commands

## Next Steps

1. **Explore the codebase:** Read `core/user-interface/modules/crossline_ui_reactive.cppm`
2. **Write a test:** Follow [TESTING.md](/docs/TESTING.md)
3. **Understand architecture:** Review [ARCHITECTURE.md](/docs/ARCHITECTURE.md)
4. **Check the roadmap:** See [PROJECT_INFO.md](/docs/PROJECT_INFO.md) for upcoming work

## Getting Help

- **Documentation:** Check `/docs/` folder
- **Code comments:** Look for `TODO:` and `FIXME:` markers
- **Tests as examples:** See `tests/crossline_ui/` for usage patterns

---

> Russian

# Начало работы с Crossline

## Быстрая настройка (5 минут)

### Предварительные условия

**Windows:**
```powershell
# Проверьте, есть ли у вас Visual Studio 2022+
& "${env:ProgramFiles}\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

# Если не установлен, загрузите с:
# https://visualstudio.microsoft.com/

# Установите CMake 3.28+
choco install cmake  # или загрузите с https://cmake.org/download/
```

**Linux (Ubuntu 22.04+):**
```bash
sudo apt update
sudo apt install build-essential cmake git python3 python3-pip
```

**macOS:**
```bash
# Установить инструменты командной строки Xcode
xcode-select --install

# Установить CMake через Homebrew
brew install cmake git
```

### Клонируйте репозиторий

```bash
git clone https://github.com/yourusername/crossline.git
cd crossline
```

### Сборка

```bash
# Настроить с предустановкой по умолчанию (Debug)
cmake --preset default

# Построить все цели
cmake --build build

# Запустить тесты
cmake --build build --target tests
```

## Структура проекта с первого взгляда

```
crossline/
├── core/                    # Основное приложение
│   ├── main.cpp             # Точка входа
│   └── user-interface/      # UI фреймворк (C++23 модули)
│       ├── CMakeLists.txt
│       └── modules/
│           ├── crossline_ui.cppm
│           ├── crossline_ui_context.cppm
│           └── crossline_ui_reactive.cppm
├── tests/                   # Набор тестов
│   ├── light_test.cppm      # Фреймворк тестирования (400 строк)
│   ├── main.cpp
│   └── crossline_ui/        # Тесты UI
├── docs/                    # Документация
│   ├── TESTING.md           # Как писать тесты
│   ├── PROJECT_INFO.md      # Видение и обоснование проекта
│   ├── ARCHITECTURE.md      # Проектирование и макет модулей
│   └── README.md            # Обзор (вы здесь)
├── tools/                   # Инструменты сборки и тестирования
│   ├── run_tests.bat        # Раннер тестов (Windows)
│   └── test_results_to_html.py  # Генерение HTML отчётов
└── CMakeLists.txt           # Основная конфигурация
```

## Общие задачи

### Запуск тестов

```bash
# Построить и запустить набор тестов
cmake --build build --target tests

# Сгенерировать HTML отчёт
python tools/test_results_to_html.py

# Открыть результаты в браузере
start build/results.html          # Windows
open build/results.html           # macOS
xdg-open build/results.html       # Linux
```

### Написание нового теста

1. Создайте функцию теста в файле `.cppm`:

```cpp
bool test_my_feature() {
    // Подготовка
    auto widget = create_button("Нажмите меня");
    
    // Действие
    widget.click();
    
    // Утверждение
    return widget.was_clicked();
}
```

2. Зарегистрируйте его:

```cpp
export void register_my_tests() {
    auto t = make_test(
        "Мой набор тестов",
        "Тестирование новых функций",
        {make_test_case("my_feature", "Описание функции", test_my_feature)}
    );
    register_tests({t});
}
```

3. Импортируйте и вызовите в `tests/main.cpp`:

```cpp
import my_tests;

int main() {
    register_my_tests();
    return light_test::run_all_tests_and_print();
}
```

### Изменение UI фреймворка

Весь код UI находится в `core/user-interface/modules/`. 

Чтобы добавить новый виджет:

1. Объявите в `crossline_ui.cppm`:
```cpp
export class my_widget {
    // публичный интерфейс
};
```

2. Реализуйте в том же файле или отдельном модуле

3. Обновите `core/user-interface/CMakeLists.txt` если нужно

4. Протестируйте с новым кейсом теста в `tests/crossline_ui/`

### Проверка успеха сборки

```bash
# Подробный вывод (если сборка терпит неудачу)
cmake --build build --verbose

# Конкретная цель
cmake --build build --target crossline-user-interface

# Проверить ошибки
cmake --build build 2>&1 | grep -i error
```

## Рабочий процесс разработки

### Типичный день

```bash
# 1. Начало дня: обновление и сборка
git pull
cmake --build build

# 2. Сделать изменения (отредактировать .cppm, .cpp файлы)
# ... отредактируйте код ...

# 3. Сразу же протестировать
cmake --build build --target tests

# 4. Итеративно тестировать по мере кодирования
# ... отредактируйте код ...
cmake --build build
cmake --build build --target tests

# 5. Перед коммитом убедитесь, что все тесты пройдены
cmake --build build --target tests
python tools/test_results_to_html.py

# 6. Просмотрите результаты, если какие-то не прошли
open build/results.html

# 7. Коммит и отправка
git add .
git commit -m "Feature: добавить новый виджет"
git push
```

## Настройка редактирования

### Рекомендуемые инструменты

**VS Code + расширения:**
- C/C++ (ms-vscode.cpptools)
- CMake Tools (ms-vscode.cmake-tools)
- Clang-Format (xaver.clang-format)

**Visual Studio 2022:**
- Встроенная поддержка C++ и CMake
- Интегрированный обозреватель тестов

**CLion (JetBrains):**
- Полная поддержка C++20/23
- Отличная интеграция CMake

### Конфигурация редактора

Скопируйте `.clangd` в корень вашей рабочей области для IDE-ной автодополнения:

```yaml
# .clangd (включён в репо)
CompileFlags:
    Add: [-std=c++23, -fmodules-ts]
```

## Устранение неполадок

### "Версия CMake слишком старая"

```bash
cmake --version  # Проверьте версию

# Обновить CMake
# Windows: choco upgrade cmake
# macOS: brew upgrade cmake
# Linux: sudo apt upgrade cmake
```

### "Компилятор C++ не поддерживает C++23"

```bash
# Проверьте компилятор
cmake --build build --verbose 2>&1 | grep range

# Обновить компилятор:
# Windows: Установить Visual Studio 2022 (17.4+)
# macOS: xcode-select --install (12.0+)
# Linux: sudo apt install g++-13 (или g++-14)
```

### Ошибки "Модуль не найден"

```bash
# Очистить и переконфигурировать
rm -rf build
cmake --preset default
cmake --build build
```

### Тесты не проходят при первом запуске

- Убедитесь, что CMake настроен с поддержкой C++23
- Проверьте `build/CMakeFiles/CMakeConfigureLog.yaml` для деталей
- Запустите `cmake --build build --verbose` для просмотра команд компиляции

## Следующие шаги

1. **Изучите кодовую базу:** Прочитайте `core/user-interface/modules/crossline_ui_reactive.cppm`
2. **Напишите тест:** Следуйте [TESTING.md](/docs/TESTING.md)
3. **Поймите архитектуру:** Проверьте [ARCHITECTURE.md](/docs/ARCHITECTURE.md)
4. **Проверьте дорожную карту:** Смотрите [PROJECT_INFO.md](/docs/PROJECT_INFO.md) для предстоящей работы

## Получение помощи

- **Документация:** Проверьте папку `/docs/`
- **Комментарии кода:** Ищите отметки `TODO:` и `FIXME:`
- **Тесты как примеры:** Смотрите `tests/crossline_ui/` для паттернов использования
