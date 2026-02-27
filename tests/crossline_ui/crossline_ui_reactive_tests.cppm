module;

#include <string>
#include <stdexcept>

export module crossline_ui_reactive_tests;

import light_test;
import crossline_ui_reactive;

using namespace light_test;

// Тест 1: Базовая установка и получение значения
bool test_basic_set_get()
{
    crossline::ui::reactive<int> r(42);
    if (r.get() != 42)
        return false;

    r.set(100);
    return r.get() == 100;
}

// Тест 2: Наблюдение за изменениями
bool test_subscriber_notification()
{
    int observed_value = 0;
    int call_count = 0;

    crossline::ui::reactive<int> r(10);
    r.add_subscriber(
        [&](const int& v)
        {
            observed_value = v;
            ++call_count;
        }
    );

    r.set(20);

    return observed_value == 20 && call_count == 1;
}

// Тест 3: Множественные подписчики
bool test_multiple_subscribers()
{
    int count1 = 0, count2 = 0;

    crossline::ui::reactive<int> r(0);
    r.add_subscriber([&](const int&) { ++count1; });
    r.add_subscriber([&](const int&) { ++count2; });

    r.set(42);

    return count1 == 1 && count2 == 1;
}

// Тест 4: Оптимизация - не уведомлять при том же значении
bool test_no_notification_on_same_value()
{
    int call_count = 0;

    crossline::ui::reactive<int> r(10);
    r.add_subscriber([&](const int&) { ++call_count; });

    r.set(10);
    r.set(10);

    return call_count == 0;
}

// Тест 5: Оператор присваивания
bool test_assignment_operator()
{
    int observed_value = 0;

    crossline::ui::reactive<int> r(5);
    r.add_subscriber([&](const int& v) { observed_value = v; });

    r = 15;

    return observed_value == 15 && r.get() == 15;
}

// Тест 6: Работа с разными типами
bool test_different_types()
{
    crossline::ui::reactive<std::string> r_str("hello");
    std::string observed_str;

    r_str.add_subscriber([&](const std::string& v) { observed_str = v; });
    r_str.set("world");

    if (observed_str != "world" || r_str.get() != "world")
        return false;

    crossline::ui::reactive<bool> r_bool(false);
    bool observed_bool = true;

    r_bool.add_subscriber([&](bool v) { observed_bool = v; });
    r_bool.set(true);

    return observed_bool == true && r_bool.get() == true;
}

// Тест 7: Исключения в подписчиках не должны ломать систему
bool test_exception_handling()
{
    crossline::ui::reactive<int> r(0);

    r.add_subscriber([&](const int&) { throw std::runtime_error("bad subscriber"); });

    bool good_subscriber_called = false;
    r.add_subscriber([&](const int&) { good_subscriber_called = true; });

    try
    {
        r.set(42);
        return good_subscriber_called;
    }
    catch (...)
    {
        return false;
    }
}

bool test_move_semantics()
{
    int observed = 0;

    crossline::ui::reactive<int> r1(10);
    r1.add_subscriber([&](const int& v) { observed = v; });

    auto r2 = std::move(r1);

    r2.set(20);

    return observed == 20;
}

export void register_crossline_ui_reactive_tests()
{
    auto t = make_test(
        "Crossline UI Reactive Tests", "Comprehensive tests for reactive pattern",
        {make_test_case("basic_set_get", "Basic set and get operations", test_basic_set_get),
         make_test_case(
             "subscriber_notification", "Subscribers receive notifications on value change",
             test_subscriber_notification
         ),
         make_test_case(
             "multiple_subscribers", "Multiple subscribers all receive notifications", test_multiple_subscribers
         ),
         make_test_case(
             "no_notification_on_same_value", "No notification when value doesn't actually change",
             test_no_notification_on_same_value
         ),
         make_test_case("assignment_operator", "Assignment operator works and notifies", test_assignment_operator),
         make_test_case("different_types", "Works with different data types", test_different_types),
         make_test_case(
             "exception_handling", "Exceptions in subscribers don't break notifications", test_exception_handling
         ),
         make_test_case("move_semantics", "Move semantics preserve subscriptions", test_move_semantics)}
    );
    register_tests({t});
}