#pragma once

#include <format>
#include <ranges>
#include <sstream>
#include <stacktrace>
#include <string_view>
#include <string>

namespace error {

class TraceableError
{
public:
    ~TraceableError() = default;

    explicit constexpr TraceableError(std::string_view const format, auto&&... args):
        message_m(std::vformat(format, std::make_format_args(std::forward<decltype(args)>(args)...))),
        errorStack_m(std::stacktrace::current())
    {}

    constexpr TraceableError(TraceableError const& error):
        message_m(error.message_m),
        errorStack_m(error.errorStack_m)
    {}

    constexpr TraceableError(TraceableError&& error):
        message_m(std::move(error.message_m)),
        errorStack_m(std::move(error.errorStack_m))
    {}

    constexpr TraceableError& operator=(TraceableError&& error)
    {
        message_m    = std::exchange(error.message_m, {});
        errorStack_m = std::exchange(error.errorStack_m, {});
        return *this;
    }

    constexpr TraceableError& operator=(TraceableError const& error)
    {
        message_m    = error.message_m;
        errorStack_m = error.errorStack_m;
        return *this;
    }

    auto message() const
    {
        std::stringstream stream {};
        stream << message_m << "\n" << "    Stacktrace:\n";

        for (auto const& [index, stack] : errorStack_m | std::views::drop(2) | std::views::enumerate)
        {
            stream << "\t" << "[" << index << "] " << stack << '\n';
        }

        return stream.str();
    }

private:
    std::string message_m {};
    std::stacktrace errorStack_m {};
};

}


