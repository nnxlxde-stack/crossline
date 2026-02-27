@echo off
setlocal enabledelayedexpansion

chcp 65001 > nul

:: Инициализация цветов для Windows
call :init_colors

:: Функция для красивого вывода
call :print_header "Light Test Runner"
echo.

:: Конфигурация проекта (тихо)
call :print_step "Configuring project"
cmake -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE --no-warn-unused-cli -S . -B ./build -G Ninja > nul 2>&1

if %errorlevel% neq 0 (
    call :print_error "Configuration failed"
    echo.
    call :print_info "Running with verbose output to see error:"
    cmake -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE --no-warn-unused-cli -S . -B ./build -G Ninja
    exit /b %errorlevel%
)
call :print_success "Configuration complete"
echo.

:: Сборка тестов (тихо)
call :print_step "Building tests"
cmake --build ./build --config Debug --target tests -- > nul 2>&1

if %errorlevel% neq 0 (
    call :print_error "Build failed"
    echo.
    call :print_info "Running with verbose output to see error:"
    cmake --build ./build --config Debug --target tests --
    exit /b %errorlevel%
)
call :print_success "Build complete"
echo.

:: Запуск тестов
call :print_step "Running tests"

:: Создаём временный файл для вывода
set "temp_file=%TEMP%\test_output_%RANDOM%.tmp"
set "done_file=%TEMP%\test_done_%RANDOM%.tmp"

:: Запускаем тесты
call :print_info "Executing test suite..."

:: Убираем сложную анимацию и запускаем просто
.\build\tests\tests.exe > "%temp_file%" 2>&1
set "test_result=%errorlevel%"

:: Парсим результаты
set "total=0"
set "passed=0"
set "failed=0"

:: Более надёжный парсинг
if exist "%temp_file%" (
    for /f "tokens=1-7" %%a in ('type "%temp_file%" ^| findstr "Summary:"') do (
        set "passed=%%c"
        set "failed=%%e"
        set "total=%%g"
        
        :: Убираем возможные лишние символы
        set "passed=!passed:,=!"
        set "failed=!failed:,=!"
        set "total=!total:,=!"
    )
)

:: Копируем в results.json
if exist "%temp_file%" (
    copy "%temp_file%" .\tests\results.json > nul
    del "%temp_file%"
) else (
    call :print_warning "No test output generated"
    set "test_result=1"
)

:: Выводим результаты
echo.
call :print_divider
echo.

if %test_result% equ 0 (
    call :print_success "All tests passed! 🎉"
) else (
    call :print_error "Some tests failed! ❌"
)

echo.
call :print_stats "TOTAL" "%total%"
call :print_stats "PASSED" "%passed%"
call :print_stats "FAILED" "%failed%"

:: Считаем процент успешных
if %total% gtr 0 (
    set /a "success_rate=passed * 100 / total" 2>nul
) else (
    set "success_rate=0"
)

:: Проверяем, что success_rate число
if "!success_rate!"=="" set "success_rate=0"

:: Прогресс-бар
call :print_progress_bar %success_rate%

echo.
call :print_info "Results saved to: .\tests\results.json"

:: Если есть Python, генерируем HTML отчёт
where python > nul 2>&1
if %errorlevel% equ 0 (
    echo.
    call :print_step "Generating HTML report"
    
    if exist ".\tools\test_results_to_html.py" (
        python .\tools\test_results_to_html.py > nul 2>&1
        
        if !errorlevel! equ 0 (
            call :print_success "HTML report generated: tests\results.html"
            
            :: Спрашиваем, открыть ли HTML страницу
            echo.
            call :print_info "Do you want to open the HTML report? (y/n)"
            set /p "open_choice="
            
            if /i "!open_choice!"=="y" (
                call :print_step "Opening HTML report..."
                if exist "tests\results.html" (
                    start tests\results.html
                    call :print_success "Browser opened"
                ) else (
                    call :print_warning "HTML file not found"
                )
            ) else (
                call :print_info "Skipping HTML report opening"
            )
        ) else (
            call :print_warning "HTML report generation failed"
        )
    ) else (
        call :print_warning "HTML generator script not found"
    )
) else (
    echo.
    call :print_warning "Python not found - skipping HTML report generation"
)

echo.
call :print_divider
echo.
call :print_success "Done! 🚀"
echo.

exit /b %test_result%

:: Инициализация цветов для Windows
:init_colors
:: Проверяем, поддерживает ли консоль ANSI
for /f "tokens=4,5 delims=." %%a in ('ver') do set "version=%%a%%b"
if "%version%" GEQ "1000" (
    :: Windows 10 и выше - поддерживают ANSI
    set "GREEN=[92m"
    set "YELLOW=[93m"
    set "RED=[91m"
    set "CYAN=[96m"
    set "WHITE=[97m"
    set "RESET=[0m"
) else (
    :: Старые Windows - без цветов
    set "GREEN="
    set "YELLOW="
    set "RED="
    set "CYAN="
    set "WHITE="
    set "RESET="
)
goto :eof

:: Функции для красивого вывода
:print_header
echo %CYAN%╔════════════════════════════════════════════════════════════╗%RESET%
echo %CYAN%║%RESET%                    %WHITE%%~1%RESET%                     %CYAN%║%RESET%
echo %CYAN%╚════════════════════════════════════════════════════════════╝%RESET%
goto :eof

:print_step
echo %YELLOW%⚙ %~1%RESET%
goto :eof

:print_success
echo %GREEN%✓ %~1%RESET%
goto :eof

:print_error
echo %RED%✗ %~1%RESET%
goto :eof

:print_info
echo %CYAN%ℹ %~1%RESET%
goto :eof

:print_warning
echo %YELLOW%⚠ %~1%RESET%
goto :eof

:print_divider
echo %CYAN%────────────────────────────────────────────────────────────────%RESET%
goto :eof

:print_stats
set "label=%~1"
set "value=%~2"
set "spaces=                    "
set "padded_label=%label%%spaces%"
set "padded_label=!padded_label:~0,10!"

if "%label%"=="PASSED" (
    echo %GREEN%!padded_label! : %value%%RESET%
) else if "%label%"=="FAILED" (
    echo %RED%!padded_label! : %value%%RESET%
) else (
    echo %WHITE%!padded_label! : %value%%RESET%
)
goto :eof

:print_progress_bar
set "rate=%~1"
set "bar_width=50"
set /a "filled=rate * bar_width / 100" 2>nul
set /a "empty=bar_width - filled" 2>nul

:: Проверяем, что filled и empty числа
if "!filled!"=="" set "filled=0"
if "!empty!"=="" set "empty=%bar_width%"

set "bar="
for /l %%i in (1,1,%filled%) do set "bar=!bar!█"
for /l %%i in (1,1,%empty%) do set "bar=!bar!░"

echo %CYAN%Progress : [%RESET%!bar!%CYAN%] %rate%%%%RESET%
goto :eof