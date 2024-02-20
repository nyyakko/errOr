#pragma once

#include <string_view>
#include <string>

namespace error {

class DefaultError
{
public:
    constexpr DefaultError()  = default;
    ~DefaultError() = default;

    explicit constexpr DefaultError(std::string_view const message): message_m(message) {}
    constexpr DefaultError(DefaultError const& error): message_m(error.message_m) {}
    constexpr DefaultError(DefaultError&& error) noexcept: message_m(std::move(error.message_m)) {}

    constexpr DefaultError& operator=(DefaultError&& error) noexcept
    {
        message_m = std::exchange(error.message_m, {});
        return *this;
    }

    constexpr DefaultError& operator=(DefaultError const& error) noexcept
    {
        message_m = error.message_m;
        return *this;
    }

    constexpr auto const& message() const { return message_m; }

private:
    std::string message_m;
};

}
