#pragma once

#include <string>

namespace liberror {

class [[nodiscard]] DefaultError
{
public:
    constexpr  DefaultError() = default;
    constexpr ~DefaultError() = default;

    explicit constexpr DefaultError(std::string_view message) noexcept: message_m(message) {}
    constexpr DefaultError(DefaultError const& error) noexcept: message_m(error.message_m) {}
    constexpr DefaultError(DefaultError&& error) noexcept: message_m(std::move(error.message_m)) {}

    constexpr DefaultError& operator=(DefaultError&& error) noexcept
    {
        message_m = std::move(error.message_m);
        return *this;
    }

    constexpr DefaultError& operator=(DefaultError const& error) noexcept
    {
        message_m = error.message_m;
        return *this;
    }

    [[nodiscard]] constexpr auto const& message() const noexcept { return message_m; }

private:
    std::string message_m;
};

} // error

