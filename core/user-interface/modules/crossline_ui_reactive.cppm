module;

#include <functional>
#include <type_traits>

export module crossline_ui_reactive;

template<class T>
concept comparable_with_not_eq = requires { T::operator!=; } || requires(const T& a, const T& b) {
    { a != b } -> std::same_as<bool>;
};

export namespace crossline::ui
{
    template<class T>
    class reactive
    {
    public:
        template<class... Args>
        reactive(Args&&... args) : obj_(std::forward<Args&&>(args)...), subscribers_({})
        {
        }
        reactive(const reactive&) noexcept = delete;
        reactive& operator=(const reactive&) noexcept = delete;
        reactive(reactive&&) noexcept = default;
        reactive& operator=(reactive&&) noexcept = default;
        ~reactive() noexcept = default;

        void set(const T& v) noexcept
        {
            if (&obj_ == &v)
                return;
            if constexpr (comparable_with_not_eq<T>)
            {
                if (obj_ != v)
                {
                    obj_ = v;
                    notify();
                }
            }
            else
            {
                obj_ = v;
                notify();
            }
        }

        void set(T&& v) noexcept
        {
            if constexpr (comparable_with_not_eq<T>)
            {
                if (obj_ != v)
                {
                    obj_ = std::move(v);
                    notify();
                }
            }
            else
            {
                obj_ = std::move(v);
                notify();
            }
        }

        const T& get() const noexcept
        {
            return obj_;
        }

        template<class Func>
            requires std::is_invocable_r_v<void, Func, const T&>
        void add_subscriber(Func&& f) noexcept
        {
            subscribers_.emplace_back(std::forward<Func>(f));
        }

        reactive& operator=(const T& v) & noexcept
        {
            set(v);
            return *this;
        }

        reactive& operator=(T&& v) & noexcept
        {
            set(std::move(v));
            return *this;
        }

        reactive& operator=(const T& v) && = delete;
        reactive& operator=(T&& v) && = delete;

    private:
        T obj_;
        std::list<std::move_only_function<void(const T&)>> subscribers_;

        void notify() const
        {
            for (auto& sub : subscribers_)
            {
                try
                {
                    if (sub)
                        sub(obj_);
                }
                catch (...)
                {
                    // no-op
                }
            }
        }
    };
}  // namespace crossline::ui