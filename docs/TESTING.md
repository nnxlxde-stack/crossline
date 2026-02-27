> English

# Testing with LightTest

## Table Of Contents
1. [Tool Overview](#tool-overview)
    1. [Key Functionality](#key-functionality)
    2. [Result Reporting](#result-reporting)
2. [Use Cases](#use-cases)
3. [Best Practices](#best-practices)
4. [Running the tests](#running-the-tests)
5. [Generating HTML Reports](#generating-html-reports)
    1. [Overview](#overview)
    2. [How It Works](#how-it-works)
    3. [Features](#features)
    4. [Usage](#usage)
6. [Example](#example)
    1. [Writing Tests](#writing-tests)
    2. [Registering Tests](#registering-tests)
    3. [Calling from main](#calling-from-main)
7. [Main File](/tests/light_test.cppm)
## Tool Overview
This tool using for testing crossline functionaluty and presents unit-test framework for easily write cross-platform tests.
### Key Functionality
```cpp
    class test_case
    {
    public:
        test_case() noexcept = default;

        template<class Func>
            requires std::is_invocable_r_v<bool, Func>
        test_case(const std::string& name, const std::string& description, Func&& f)
            : name_(name), description_(description), invokable_(std::forward<Func>(f))
        {
        }

        ~test_case() noexcept = default;

        const std::string& get_name() const noexcept
        {
            return name_;
        }

        const std::string& get_description() const noexcept
        {
            return description_;
        }

        std::expected<bool, std::string> run() const noexcept
        {
            try
            {
                return invokable_();
            }
            catch (const std::exception& e)
            {
                return std::unexpected(std::format("C++ Exception: {}", e.what()));
            }
            catch (...)
            {
                return std::unexpected("Unknown C++ Exception");
            }
        }

    private:
        std::string name_;
        std::string description_;
        std::function<bool()> invokable_;
    };
```
The test case class provides complex test case description and
returns detailed results.  A companion `test_result` class collects
multiple case outcomes, counts successes/failures and can emit JSON via
the small `detail::jsonify` helper that is also defined in
`light_test`.

### Result Reporting
After all cases have been evaluated the JSON string can be written to
`stdout` or a file.  The start‑up script `tools/run_tests.bat` captures
this output, runs the tests, and then invokes
`tests/test_results_to_html.py` to produce a human‑readable HTML report
(`results.html`) alongside the raw JSON (`results.json`).

## Use Cases
* **Unit tests** – verify single functions, classes or modules.
* **Integration** – drive subsystems such as `crossline_ui` or the
  renderer.
* **Regression** – record failing examples to prevent future
  breakage.
* **Cross‑platform** – identical code on Windows, Linux and macOS.

## Best Practices
* Write one logical assertion per `test_case`.
* Give each case a clear name and description.
* Prefer local state; avoid globals.
* Return `true` for pass, `false` for failure, or throw with an error
  message.
* Group related cases with `test_result` and report totals.

## Running the tests

The CMake target `tests` compiles and runs all registered tests:

```sh
cmake --build build --target tests
```

Raw JSON results are printed to stdout. To generate an HTML report,
use `test_results_to_html.py` (see below).

## Generating HTML Reports

### Overview

While `run_all_tests_and_print()` outputs raw JSON, the script
`tools/test_results_to_html.py` transforms this JSON into a beautifully
formatted HTML document. The script is written in Python and requires
no external dependencies.

### How It Works

1. **Reading JSON:** the script loads `tests/results.json` (a file generated
   via JSON output in main).
2. **Code formatting:** parses CPPM test files and displays the source code
   of each test in the HTML report.
3. **Visualization:** generates a dark theme with Bulma CSS and embedded
   JavaScript logic.
4. **Statistics:** displays a summary of passed/failed tests with success
   percentage.

### Features

* **Auto-expand failures:** rows marked `has-text-danger` (red) are
  automatically expanded on page load.
* **Syntax highlighting:** C++ expressions are color-coded: keywords (blue),
  strings (green), numbers (pink).
* **Interactive toggles:** each test can be expanded by clicking its header
  to view code and error messages.

### Usage

If you ran the test target manually, invoke the script:

```bash
python tools/test_results_to_html.py
```

The result is saved to `tests/results.html` (opens in any web browser).

## Example

### Writing Tests

Each test is a simple function returning `bool`:

```cpp
// In tests/crossline_ui/crossline_ui_reactive_tests.cppm

bool test_basic_set_get()
{
    crossline::ui::reactive<int> r(42);
    if (r.get() != 42)
        return false;

    r.set(100);
    return r.get() == 100;
}

bool test_subscriber_notification()
{
    int observed_value = 0;
    int call_count = 0;

    crossline::ui::reactive<int> r(10);
    r.add_subscriber([&](const int& v) {
        observed_value = v;
        ++call_count;
    });

    r.set(20);
    return observed_value == 20 && call_count == 1;
}
```

### Registering Tests

Wrap test functions in `make_test_case`, collect them in a set via `make_test`,
and register:

```cpp
export void register_crossline_ui_reactive_tests()
{
    auto t = make_test(
        "Crossline UI Reactive Tests",
        "Comprehensive tests for reactive pattern",
        {make_test_case(
             "basic_set_get",
             "Basic set and get operations",
             test_basic_set_get
         ),
         make_test_case(
             "subscriber_notification",
             "Subscribers receive notifications on value change",
             test_subscriber_notification
         )
        }
    );
    register_tests({t});
}
```

### Calling from main

```cpp
// tests/main.cpp
import crossline_ui_reactive_tests;
import light_test;

int main() {
    register_crossline_ui_reactive_tests();
    return light_test::run_all_tests_and_print();
}
```

Function `run_all_tests_and_print()` executes all registered tests,
prints JSON — one line per test suite — and returns exit code 0 on success
or 1 on any failure.

> Russian

# Тестирование с LightTest

## Содержание
1. [Обзор инструмента](#обзор-инструмента)
    1. [Основные возможности](#основные-возможности)
    2. [Отчет о результатах](#отчет-о-результатах)
2. [Сценарии использования](#сценарии-использования)
3. [Лучшие практики](#лучшие-практики)
4. [Запуск тестов](#запуск-тестов)
5. [Генерация HTML-отчётов](#генерация-html-отчётов)
    1. [Обзор](#обзор)
    2. [Как это работает](#как-это-работает)
    3. [Особенности](#особенности)
    4. [Использование](#использование)
6. [Пример](#пример)
    1. [Написание тестов](#написание-тестов)
    2. [Регистрация тестов](#регистрация-тестов)
    3. [Вызов из main](#вызов-из-main)
7. [Основной исходник](/tests/light_test.cppm)

## Обзор инструмента
LightTest — минималистичный фреймворк на C++ без зависимостей,
использующийся в проекте crossline для написания и выполнения
тестов. Тесты компилируются как обычные C++‑модули и включаются в
цель `tests` при сборке CMake.

### Основные возможности

Фреймворк построен на трёх ключевых классах:

* **`test_case`:** оборачивает функцию, возвращающую `bool`, с именем и
  описанием. Защищён от исключений C++.
* **`test_result`:** аккумулирует результаты одного или нескольких кейсов,
  ведёт счётчик прошедших/провалившихся, генерирует JSON.
* **`test_register`:** глобальный реестр (синглтон), где регистрируются все
  наборы тестов через `register_tests()`.

Функция `run_all_tests_and_print()` прогоняет все зарегистрированные тесты
и выводит JSON в `stdout`.

### Отчет о результатах

Каждый выполненный набор тестов выводится одной JSON‑строкой со следующей
структурой:

```json
{
  "test_name": "Crossline UI Reactive Tests",
  "test_description": "Comprehensive tests for reactive pattern",
  "total": 8,
  "passed": 7,
  "failed": 1,
  "success": false,
  "case_results": [
    {"name": "basic_set_get", "description": "...", "passed": true},
    {"name": "subscriber_notification", "description": "...", "passed": false, "error": "..."}
  ]
}
```

JSON‑строка перенаправляется в файл `tests/results.json`, откуда её
обрабатывает скрипт `tools/test_results_to_html.py` для создания красивого
HTML‑отчёта.

## Сценарии использования
* **Юнит‑тесты:** проверка отдельных функций или классов.
* **Интеграционные:** нагрузка подсистем, например `crossline_ui`.
* **Регрессионные:** воспроизведение багов для предотвращения
повторения.
* **Кросс‑платформенные:** единый код работает на Windows, Linux и macOS.

## Лучшие практики
* Один `test_case` на одну логическую проверку.
* Дайте понятные имена и описания.
* Избегайте глобального состояния, пользуйтесь RAII.
* Возвращайте `true` при успехе, `false` при провале, либо бросайте
строку с сообщением об ошибке.
* Группируйте кейсы через `test_result`, выводите итоги.

## Запуск тестов

Цель CMake `tests` компилирует и запускает все зарегистрированные тесты:

```sh
cmake --build build --target tests
```

JSON‑результаты выводятся в `stdout`. Для генерации HTML‑отчёта
используйте `test_results_to_html.py` (см. ниже).

## Пример

### Написание тестов

Каждый тест — это простая функция, возвращающая `bool`:

```cpp
// В файле tests/crossline_ui/crossline_ui_reactive_tests.cppm

bool test_basic_set_get()
{
    crossline::ui::reactive<int> r(42);
    if (r.get() != 42)
        return false;

    r.set(100);
    return r.get() == 100;
}

bool test_subscriber_notification()
{
    int observed_value = 0;
    int call_count = 0;

    crossline::ui::reactive<int> r(10);
    r.add_subscriber([&](const int& v) {
        observed_value = v;
        ++call_count;
    });

    r.set(20);
    return observed_value == 20 && call_count == 1;
}
```

### Регистрация тестов

Оберните функции в `make_test_case`, соберите в набор через `make_test` и
зарегистрируйте:

```cpp
export void register_crossline_ui_reactive_tests()
{
    auto t = make_test(
        "Crossline UI Reactive Tests",
        "Comprehensive tests for reactive pattern",
        {make_test_case(
             "basic_set_get",
             "Basic set and get operations",
             test_basic_set_get
         ),
         make_test_case(
             "subscriber_notification",
             "Subscribers receive notifications on value change",
             test_subscriber_notification
         )
        }
    );
    register_tests({t});
}
```

### Вызов из main

```cpp
// tests/main.cpp
import crossline_ui_reactive_tests;
import light_test;

int main() {
    register_crossline_ui_reactive_tests();
    return light_test::run_all_tests_and_print();
}
```

Функция `run_all_tests_and_print()` выполнит все зарегистрированные
тесты, выведет JSON (по одной строке на каждый набор) и вернёт код выхода
0 при успехе или 1 при отказе.

## Генерация HTML отчётов

### Обзор

В то время как `run_all_tests_and_print()` выводит сырой JSON, скрипт
`tools/test_results_to_html.py` преобразует этот JSON в красиво оформленный
HTML‑документ. Скрипт написан на Python и не требует внешних зависимостей.

### Как это работает

1. **Чтение JSON:** скрипт загружает `tests/results.json` (файл,
   сгенерированный из вывода main).
2. **Форматирование кода:** парсит CPPM‑файлы тестов и показывает исходный
   код каждого теста в HTML‑отчёте.
3. **Визуализация:** генерирует тёмный интерфейс с использованием Bulma CSS
   и встроенной JavaScript логики.
4. **Статистика:** выводит сводку пройденных/непройденных тестов с
   процентом успеха.

### Особенности

* **Автораскрытие упавших тестов:** строки с меткой `has-text-danger` (красные)
  автоматически раскрываются при загрузке отчёта.
* **Подсветка синтаксиса:** выражения на C++ раскрашены: ключевые слова
  (синие), строки (зелёные), числа (розовые).
* **Интерактивные свёртки:** каждый тест можно развернуть щелчком по
  заголовку, чтобы увидеть код и сообщения об ошибках.

### Использование

После выполнения цели `tests` вручную запустите скрипт:

```bash
python tools/test_results_to_html.py
```

Результат сохранится в `tests/results.html` (открывается в любом браузере).