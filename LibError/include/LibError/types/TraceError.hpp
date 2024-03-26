#pragma once

#include <format>
#include <ranges>
#include <sstream>
#include <stacktrace>
#include <string_view>

namespace liberror {

class [[nodiscard]] TraceError
{
public:
    constexpr  TraceError() = default;
    constexpr ~TraceError() = default;

    explicit constexpr TraceError(std::string_view format, auto&&... args)
        : message_m(std::vformat(format, std::make_format_args(std::forward<decltype(args)>(args)...)))
        , errorStack_m(std::stacktrace::current())
    {}

    constexpr TraceError(TraceError const& error) noexcept
        : message_m(error.message_m)
        , errorStack_m(error.errorStack_m)
    {}

    constexpr TraceError(TraceError&& error) noexcept
        : message_m(std::move(error.message_m))
        , errorStack_m(std::move(error.errorStack_m))
    {}

    constexpr TraceError& operator=(TraceError&& error) noexcept
    {
        message_m    = std::move(error.message_m);
        errorStack_m = std::move(error.errorStack_m);
        return *this;
    }

    constexpr TraceError& operator=(TraceError const& error) noexcept
    {
        message_m    = error.message_m;
        errorStack_m = error.errorStack_m;
        return *this;
    }

    [[nodiscard]] auto message() const noexcept
    {
        std::stringstream builder {};

        builder << message_m << "\n" << "    Stacktrace:\n";

        for (auto const& [index, stack] : errorStack_m | std::views::drop(2) | std::views::enumerate)
        {
            builder << "\t" << "[" << index << "] " << stack << '\n';
        }

        return builder.str();
    }

private:
    std::string message_m {};
    std::stacktrace errorStack_m {};
};

} // error

