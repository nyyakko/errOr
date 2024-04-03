#pragma once

#include <string>

namespace liberror {

class [[nodiscard]] DefaultError
{
public:
    constexpr explicit DefaultError(std::string_view message) : message_m { message } {}

    constexpr  DefaultError() noexcept = default;
    constexpr ~DefaultError() noexcept = default;

    constexpr DefaultError(DefaultError const& error) : message_m { error.message_m } {}
    constexpr DefaultError(DefaultError&& error) noexcept : message_m { std::move(error.message_m) } {}

    constexpr DefaultError& operator=(DefaultError&& error) noexcept
    {
        message_m = std::move(error.message_m);
        return *this;
    }

    constexpr DefaultError& operator=(DefaultError const& error)
    {
        message_m = error.message_m;
        return *this;
    }

    [[nodiscard]] constexpr auto const& message() const noexcept { return message_m; }

private:
    std::string message_m;
};

} // liberror

