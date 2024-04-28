#pragma once

#include <ranges>
#include <sstream>

#ifdef __cpp_lib_stacktrace
#include <stacktrace>
#endif

#include <string_view>

namespace liberror {

class [[nodiscard]] TraceError
{
public:
    explicit constexpr TraceError(std::string_view message)
        : message_m { message }
#ifdef __cpp_lib_stacktrace
        , errorStack_m { std::stacktrace::current() }
#endif
    {}

    constexpr  TraceError() = default;
    constexpr ~TraceError() = default;

    constexpr TraceError(TraceError const& error)
        : message_m { error.message_m }
#ifdef __cpp_lib_stacktrace
        , errorStack_m { error.errorStack_m }
#endif
    {}

    constexpr TraceError(TraceError&& error) noexcept
        : message_m { std::move(error.message_m) }
#ifdef __cpp_lib_stacktrace
        , errorStack_m { std::move(error.errorStack_m) }
#endif
    {}

    constexpr TraceError& operator=(TraceError&& error) noexcept
    {
        message_m    = std::move(error.message_m);
#ifdef __cpp_lib_stacktrace
        errorStack_m = std::move(error.errorStack_m);
#endif
        return *this;
    }

    constexpr TraceError& operator=(TraceError const& error)
    {
        message_m    = error.message_m;
#ifdef __cpp_lib_stacktrace
        errorStack_m = error.errorStack_m;
#endif
        return *this;
    }

    [[nodiscard]] auto message() const
    {
#ifdef __cpp_lib_stacktrace
        std::stringstream builder {};

        builder << message_m << "\n" << "    Stacktrace:\n";

        for (auto const& [index, stack] : errorStack_m | std::views::drop(2) | std::views::enumerate)
        {
            builder << "\t" << "[" << index << "] " << stack << '\n';
        }

        return builder.str();
#else
        return "MISSING STACKTRACE LIBRARY";
#endif
    }

private:
    std::string message_m;
#ifdef __cpp_lib_stacktrace
    std::stacktrace errorStack_m;
#endif
};

} // liberror

