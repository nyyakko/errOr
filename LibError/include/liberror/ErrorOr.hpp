#pragma once

#include "types/DefaultError.hpp"

#include <print>
#include <string_view>
#include <expected>

#if defined(__clang__) || defined(__GNUC__)
#define TRY(expression) ({                                                          \
    using namespace liberror;                                                       \
    auto&& _ = (expression);                                                        \
    if (!_.has_value()) return make_error(_.error());                               \
    _.value();                                                                      \
})

#define MUST(expression) ({                                                         \
    using namespace liberror;                                                       \
    auto&& _ = (expression);                                                        \
    if (!_.has_value())                                                             \
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

template <class T>
concept error_policy_concept = requires (T error)
{
    std::is_constructible_v<T, std::string_view>;
    std::is_move_constructible_v<T>;
    std::is_move_assignable_v<T>;
    std::is_copy_constructible_v<T>;
    std::is_copy_assignable_v<T>;
    std::is_nothrow_constructible_v<T>;
    std::is_nothrow_move_constructible_v<T>;
    std::is_nothrow_move_assignable_v<T>;
    error.message();
};

template <class T, error_policy_concept ErrorPolicy = DefaultError>
using ErrorOr = std::expected<T, ErrorPolicy>;

template <error_policy_concept ErrorPolicy = DefaultError>
constexpr auto make_error(std::string_view message)
{
    return std::unexpected<ErrorPolicy>(message);
}

template <error_policy_concept ErrorPolicy = DefaultError>
constexpr auto make_error(std::string_view format, auto&&... arguments)
{
    return std::unexpected<ErrorPolicy>(std::vformat(format, std::make_format_args(std::forward<decltype(arguments)>(arguments)...)));
}

template <error_policy_concept ErrorPolicy = DefaultError>
constexpr auto make_error(ErrorPolicy&& error) noexcept
{
    return std::unexpected<ErrorPolicy>(std::forward<ErrorPolicy>(error));
}

template <error_policy_concept ErrorPolicy = DefaultError>
constexpr auto make_error(ErrorPolicy& error) noexcept
{
    return std::unexpected<ErrorPolicy>(std::move(error));
}

} // liberror

