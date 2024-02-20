#pragma once

#include "policies/DefaultError.hpp"

#include <optional>
#include <print>
#include <string_view>

#define TRY(expression) ({                                                          \
    auto _ = (expression);                                                          \
    if (_.has_error())                                                              \
        return error::make_error(_.error());                                        \
    *_;                                                                             \
})

#define MUST(expression) ({                                                         \
    auto _ = (expression);                                                          \
    if (_.has_error())                                                              \
    {                                                                               \
        std::println(stderr, "Aborted because: {}", _.error().message());           \
        std::abort();                                                               \
    }                                                                               \
    *_;                                                                             \
})

#define SHOULD(expression) ({                                                       \
    auto _ = (expression);                                                          \
    if (_.has_error())                                                              \
    {                                                                               \
        std::println(stderr, "Failed because: {}", _.error().message());            \
        std::exit(1);                                                               \
    }                                                                               \
    *_;                                                                             \
})

namespace error {

    namespace {

    struct EmptyType {};

    }

template <class T>
concept error_policy_concept =
    std::is_move_constructible_v<T>
        && std::is_copy_constructible_v<T>
        && std::is_move_assignable_v<T>
        && std::is_copy_assignable_v<T>
        && requires (T error)
{
    error.message();
};

template <class T, error_policy_concept ErrorPolicy = DefaultError>
class ErrorOr
{
    template <class _T, error_policy_concept _ErrorPolicy> friend class ErrorOr;

public:
    using value_t = std::conditional_t<std::is_void_v<T>, EmptyType, T>;

    constexpr ErrorOr()  = default;
    constexpr ~ErrorOr() = default;

    // cppcheck-suppress noExplicitConstructor
    constexpr ErrorOr(value_t&& value): value_m(std::move(value)) {}

    // cppcheck-suppress noExplicitConstructor
    constexpr ErrorOr(value_t const& value): value_m(value) {}

    explicit constexpr ErrorOr(ErrorPolicy&& error) noexcept: error_m(std::move(error)) {}

    // cppcheck-suppress noExplicitConstructor
    constexpr ErrorOr(ErrorOr<EmptyType, ErrorPolicy> const& errorOr): error_m(errorOr.error()) {}
    // cppcheck-suppress noExplicitConstructor
    constexpr ErrorOr(ErrorOr<EmptyType, ErrorPolicy>&& errorOr) noexcept: error_m(std::move(errorOr.error_m)) {}

    constexpr ErrorOr& operator=(ErrorOr<EmptyType, ErrorPolicy> const& errorOr)
    {
        error_m = errorOr.error_m;
        return *this;
    }

    constexpr ErrorOr& operator=(ErrorOr<EmptyType, ErrorPolicy>&& errorOr) noexcept
    {
        error_m = std::exchange(errorOr.error_m, {});
        return *this;
    }

    constexpr auto value() const noexcept { return value_m.value(); }
    constexpr auto has_value() const noexcept { return value_m.has_value(); }
    constexpr auto error() const noexcept { return *error_m; }
    constexpr auto has_error() const noexcept { return error_m.has_value(); }

    constexpr auto operator!() const noexcept { return has_error(); }
    explicit constexpr operator bool() const noexcept { return !has_error(); }

    constexpr auto operator*() const noexcept
    {
        if constexpr (std::is_same_v<value_t, EmptyType>)
            return;
        else
            return *value_m;
    }

    constexpr auto operator*() noexcept
    {
        if constexpr (std::is_same_v<value_t, EmptyType>)
            return;
        else
            return std::move(*value_m);
    }

private:
    std::optional<ErrorPolicy> error_m {};
    std::optional<value_t> value_m {};
};

template <error_policy_concept ErrorPolicy = DefaultError>
constexpr auto make_error(std::string_view const format, auto&&... args)
    requires (std::is_same_v<ErrorPolicy, DefaultError>)
{
    return ErrorOr<EmptyType> { ErrorPolicy { std::vformat(format.data(), std::make_format_args(args...)) } };
}

template <error_policy_concept ErrorPolicy>
constexpr auto make_error(auto&&... args)
    requires (!std::is_same_v<ErrorPolicy, DefaultError>)
{
    return ErrorOr<EmptyType, ErrorPolicy> { ErrorPolicy { std::forward<decltype(args)>(args)... } };
}

template <error_policy_concept ErrorPolicy>
constexpr auto make_error(ErrorPolicy&& error)
{
    return ErrorOr<EmptyType, ErrorPolicy> { std::forward<decltype(error)>(error) };
}

}

