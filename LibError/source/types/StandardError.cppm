module;

export module liberror:types.error.standard;

import stl;

export namespace liberror {

class [[nodiscard]] StandardError
{
public:
    constexpr explicit StandardError(std::string_view message) : message_m { message } {}

    constexpr StandardError() noexcept = default;
    constexpr ~StandardError() noexcept = default;

    constexpr StandardError(StandardError const& error) : message_m { error.message_m } {}
    constexpr StandardError(StandardError&& error) noexcept : message_m { std::move(error.message_m) } {}

    constexpr StandardError& operator=(StandardError&& error) noexcept
    {
        message_m = std::move(error.message_m);
        return *this;
    }

    constexpr StandardError& operator=(StandardError const& error)
    {
        message_m = error.message_m;
        return *this;
    }

    [[nodiscard]] constexpr auto const& message() const noexcept { return message_m; }

private:
    std::string message_m;
};

} // liberror
