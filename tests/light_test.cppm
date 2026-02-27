module;

#include <cassert>
#include <concepts>
#include <expected>
#include <format>
#include <functional>
#include <iostream>
#include <list>
#include <string>
#include <type_traits>

export module light_test;

namespace detail
{
    class jsonify
    {
    public:
        jsonify() noexcept : json_("{"), first_(true), expect_key_(true)
        {
        }

        template<std::convertible_to<std::string> T>
        jsonify& operator<<(T&& value) &
        {
            if (expect_key_)
            {
                if (!first_)
                {
                    json_ += ", ";
                }
                json_ += std::format("\"{}\": ", std::forward<T>(value));
                first_ = false;
            }
            else
            {
                json_ += std::format("\"{}\"", std::forward<T>(value));
            }

            expect_key_ = !expect_key_;
            return *this;
        }

        template<std::integral T>
        jsonify& operator<<(T value) &
        {
            if (expect_key_)
            {
                if (!first_)
                {
                    json_ += ", ";
                }
                json_ += std::format("\"{}\": ", value);
                first_ = false;
            }
            else
            {
                json_ += std::format("{}", value);
            }

            expect_key_ = !expect_key_;
            return *this;
        }

        jsonify& operator<<(bool value) &
        {
            if (expect_key_)
            {
                if (!first_)
                {
                    json_ += ", ";
                }
                json_ += std::format("\"{}\": ", value ? "true" : "false");
                first_ = false;
            }
            else
            {
                json_ += value ? "true" : "false";
            }

            expect_key_ = !expect_key_;
            return *this;
        }

        template<typename T>
        jsonify& add_array(const std::string& key, const std::vector<T>& values) &
        {
            *this << key;

            json_ += "[";
            for (size_t i = 0; i < values.size(); ++i)
            {
                if (i > 0)
                    json_ += ", ";
                *this << values[i];
            }
            json_ += "]";

            expect_key_ = true;
            return *this;
        }

        std::string str() const
        {
            return json_ + "}";
        }

        jsonify& operator>>(std::string& str) &
        {
            str = json_ + "}";
            return *this;
        }

    private:
        std::string json_;
        bool first_;
        bool expect_key_;
    };
}  // namespace detail

export namespace light_test
{

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

    class test_result
    {
    public:
        test_result(const std::string& test_name, const std::string& test_description = "")
            : test_name_(test_name), test_description_(test_description)
        {
        }

        void add_result(const std::string& case_name, const std::string& case_description, bool passed) noexcept
        {
            case_results_.push_back({case_name, case_description, passed, {}});
            if (passed)
                ++passed_count_;
            else
                ++failed_count_;
        }

        void add_error(
            const std::string& case_name, const std::string& case_description, const std::string& error
        ) noexcept
        {
            case_results_.push_back({case_name, case_description, false, error});
            ++failed_count_;
        }

        size_t total() const noexcept
        {
            return case_results_.size();
        }
        size_t passed() const noexcept
        {
            return passed_count_;
        }
        size_t failed() const noexcept
        {
            return failed_count_;
        }
        bool success() const noexcept
        {
            return failed_count_ == 0;
        }

        std::string to_json() const
        {
            detail::jsonify j;
            j << "test_name" << test_name_ << "test_description" << test_description_ << "total"
              << static_cast<long long>(total()) << "passed" << static_cast<long long>(passed()) << "failed"
              << static_cast<long long>(failed()) << "success" << success();

            if (!case_results_.empty())
            {
                std::string cases_json = "[";
                for (size_t i = 0; i < case_results_.size(); ++i)
                {
                    if (i > 0)
                        cases_json += ", ";
                    detail::jsonify case_j;
                    const auto& cr = case_results_[i];
                    case_j << "name" << cr.name << "description" << cr.description << "passed" << cr.passed;
                    if (!cr.error.empty())
                    {
                        case_j << "error" << cr.error;
                    }
                    cases_json += case_j.str();
                }
                cases_json += "]";
            }

            std::string result;
            j >> result;
            return result;
        }

    private:
        struct case_result
        {
            std::string name;
            std::string description;
            bool passed;
            std::string error;
        };

        std::string test_name_;
        std::string test_description_;
        std::vector<case_result> case_results_;
        size_t passed_count_ = 0;
        size_t failed_count_ = 0;
    };

    class test
    {
    public:
        test() = default;

        test(const std::string& name, const std::string& description = "") : name_(name), description_(description)
        {
        }

        test& add_case(const test_case& tc) &
        {
            cases_.push_back(tc);
            return *this;
        }

        test& add_case(test_case&& tc) &
        {
            cases_.push_back(std::move(tc));
            return *this;
        }

        template<class Func>
            requires std::is_invocable_r_v<bool, Func>
        test& add_case(const std::string& name, const std::string& description, Func&& f) &
        {
            cases_.emplace_back(name, description, std::forward<Func>(f));
            return *this;
        }

        const std::string& get_name() const noexcept
        {
            return name_;
        }

        const std::string& get_description() const noexcept
        {
            return description_;
        }

        const std::list<test_case>& get_cases() const noexcept
        {
            return cases_;
        }

        std::expected<test_result, std::string> run() const noexcept
        {
            test_result result(name_, description_);

            for (const auto& tc : cases_)
            {
                auto tc_result = tc.run();
                if (tc_result)
                {
                    result.add_result(tc.get_name(), tc.get_description(), *tc_result);
                }
                else
                {
                    result.add_error(tc.get_name(), tc.get_description(), tc_result.error());
                }
            }

            return result;
        }

    private:
        std::string name_;
        std::string description_;
        std::list<test_case> cases_;
    };

    class test_register
    {
    public:
        static test_register& instance() noexcept
        {
            static test_register register_;
            return register_;
        }

        void add_test(test&& t) &
        {
            tests_.push_back(std::move(t));
        }

        void add_test(const test& t) &
        {
            tests_.push_back(t);
        }

        const std::list<test>& get_tests() const noexcept
        {
            return tests_;
        }

        std::vector<test_result> run_all() const
        {
            std::vector<test_result> results;
            for (const auto& t : tests_)
            {
                auto result = t.run();
                if (result)
                {
                    results.push_back(*result);
                }
            }
            return results;
        }

    private:
        test_register() = default;
        ~test_register() = default;

        static inline std::list<test> tests_;
    };

    inline int run_all_tests_and_print(std::ostream& os = std::cout) noexcept
    {
        auto results = light_test::test_register::instance().run_all();

        size_t total_passed = 0;
        size_t total_failed = 0;

        for (const auto& result : results)
        {
            os << result.to_json() << std::endl;
            total_passed += result.passed();
            total_failed += result.failed();
        }

        // os << std::format(
        //     "\nSummary: {} passed, {} failed, {} total\n", total_passed, total_failed, total_passed + total_failed
        // );

        return total_failed == 0 ? 0 : 1;
    }

    template<class Func>
    auto make_test_case(const std::string& name, const std::string& description, Func&& f) -> test_case
    {
        return test_case(name, description, std::forward<Func&&>(f));
    }

    auto make_test(const std::string& name, const std::string& description, std::list<test_case>&& cases) -> test
    {
        test t(name, description);
        for (auto& tc : cases)
        {
            t.add_case(std::move(tc));
        }
        return t;
    }

    auto register_tests(std::initializer_list<test> tests) -> void
    {
        for (auto& t : tests)
        {
            test_register::instance().add_test(std::move(t));
        }
    }
}  // namespace light_test