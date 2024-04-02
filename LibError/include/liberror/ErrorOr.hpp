#pragma once

#include "types/DefaultError.hpp"

#include <print>
#include <string_view>
#include <variant>

#if defined(__clang__) || defined(__GNUC__)
#define TRY(expression) ({                                                          \
    auto _ = (expression);                                                          \
    if (_.has_error())                                                              \
        return liberror::make_error(_.error());                                     \
    _.value();                                                                      \
})

#define MUST(expression) ({                                                         \
    auto _ = (expression);                                                          \
    if (_.has_error())                                                              \
    {                                                                               \
        std::println(stderr, "Aborted execution because: {}", _.error().message()); \
        std::abort();                                                               \
    }                                                                               \
    _.value();                                                                      \
})
#else
#error "Your compiler doesn't allow for [compound-expressions](https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html)"
#endif

namespace liberror {

    namespace {

    struct EmptyType {};

    }

template <class T>
concept error_policy_concept = requires (T error)
{
    std::is_constructible_v<T, std::string_view>;
    std::is_move_constructible_v<T>;
    std::is_move_assignable_v<T>;
    std::is_copy_constructible_v<T>;
    std::is_copy_assignable_v<T>;
    error.message();
};

template <class T, error_policy_concept ErrorPolicy = DefaultError>
class [[nodiscard]] ErrorOr
{
    template <class, error_policy_concept> friend class ErrorOr;

public:
    using value_t = std::conditional_t<std::is_void_v<T>, EmptyType, T>;
    using error_t = ErrorPolicy;

    constexpr  ErrorOr() = default;
    constexpr ~ErrorOr() = default;

    constexpr ErrorOr(ErrorOr<EmptyType, ErrorPolicy>&& errorOr) noexcept : value_m { std::move(errorOr.error()) } {}
    constexpr ErrorOr(ErrorOr<EmptyType, ErrorPolicy> const& errorOr) : value_m { errorOr.error() } {}

    constexpr ErrorOr(ErrorOr<value_t, ErrorPolicy>&& errorOr) noexcept
        requires (!std::is_same_v<value_t, EmptyType>) : value_m { std::move(errorOr.value_m) } {}
    constexpr ErrorOr(ErrorOr<value_t, ErrorPolicy> const& errorOr)
        requires (!std::is_same_v<value_t, EmptyType>) : value_m { errorOr.value_m } {}

    // cppcheck-suppress noExplicitConstructor
    constexpr ErrorOr(auto&& value) noexcept
        requires (std::is_nothrow_constructible_v<value_t, decltype(value)>)
            : value_m { std::in_place_type<value_t>, std::forward<decltype(value)>(value) } {}

    // cppcheck-suppress noExplicitConstructor
    constexpr ErrorOr(auto&& value) noexcept
            : value_m { std::in_place_type<value_t>, std::forward<decltype(value)>(value) } {}

    explicit constexpr ErrorOr(ErrorPolicy&& error) noexcept : value_m { std::move(error) } {}
    constexpr ErrorOr(ErrorPolicy const&) = delete;

    [[nodiscard]] constexpr decltype(auto) value() const { return std::get<value_t>(value_m); }
    [[nodiscard]] constexpr auto has_value() const noexcept { return std::holds_alternative<value_t>(value_m); }
    [[nodiscard]] constexpr auto error() const { return std::get<ErrorPolicy>(value_m); }
    [[nodiscard]] constexpr auto has_error() const noexcept { return std::holds_alternative<ErrorPolicy>(value_m); }

    constexpr ErrorOr& operator=(ErrorOr<EmptyType, ErrorPolicy>&& errorOr) noexcept
    {
        value_m = std::move(errorOr.error());
        return *this;
    }

    constexpr ErrorOr& operator=(ErrorOr<EmptyType, ErrorPolicy> const& errorOr) noexcept
    {
        value_m = errorOr.error();
        return *this;
    }

    constexpr ErrorOr& operator=(ErrorOr<EmptyType, ErrorPolicy>&& errorOr) noexcept
        requires (!std::is_same_v<value_t, EmptyType>)
    {
        value_m = std::move(errorOr);
        return *this;
    }

    constexpr ErrorOr& operator=(ErrorOr<EmptyType, ErrorPolicy> const& errorOr) noexcept
        requires (!std::is_same_v<value_t, EmptyType>)
    {
        value_m = errorOr;
        return *this;
    }

    [[nodiscard]] explicit constexpr operator bool() const noexcept { return has_value(); }

private:
    std::variant<value_t, ErrorPolicy> value_m {};
};

template <error_policy_concept ErrorPolicy = DefaultError>
[[nodiscard]] constexpr auto make_error(std::string_view const format, auto&&... args) noexcept
{
    return ErrorOr<EmptyType, ErrorPolicy> { ErrorPolicy { std::vformat(format.data(), std::make_format_args(std::forward<decltype(args)>(args)...)) } };
}

template <error_policy_concept ErrorPolicy = DefaultError>
[[nodiscard]] constexpr auto make_error(std::string_view message) noexcept
{
    return ErrorOr<EmptyType, ErrorPolicy> { ErrorPolicy { message } };
}

template <error_policy_concept ErrorPolicy = DefaultError>
[[nodiscard]] constexpr auto make_error(ErrorPolicy&& error) noexcept
{
    return ErrorOr<EmptyType, ErrorPolicy> { std::forward<ErrorPolicy>(error) };
}

} // liberror

