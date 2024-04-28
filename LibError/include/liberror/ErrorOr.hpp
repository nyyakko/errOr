#pragma once

#include "types/DefaultError.hpp"

#include <fmt/format.h>

#ifndef __cpp_lib_expected
#include <tl/expected.hpp>
namespace liberror {
    using namespace tl;
}
#else
#include <expected>
namespace liberror {
    using std::expected;
}
#endif

#include <string_view>

#if defined(__clang__) || defined(__GNUC__)
#define TRY(expression) ({                                                          \
    using namespace liberror;                                                       \
    auto&& _ = (expression);                                                        \
    if (!_.has_value()) return make_error(_.error());                               \
    std::move(_).value();                                                           \
})

#define MUST(expression) ({                                                         \
    using namespace liberror;                                                       \
    auto&& _ = (expression);                                                        \
    if (!_.has_value())                                                             \
    {                                                                               \
        fmt::println(stderr, "Aborted execution because: {}", _.error().message()); \
        std::abort();                                                               \
    }                                                                               \
    std::move(_).value();                                                           \
})
#else
#error "Compiler doesn't support [compound-expressions](https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html)"
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
using ErrorOr = expected<T, ErrorPolicy>;

template <error_policy_concept ErrorPolicy = DefaultError>
constexpr auto make_error(std::string_view message)
{
    return unexpected<ErrorPolicy>(message);
}

template <error_policy_concept ErrorPolicy = DefaultError>
constexpr auto make_error(std::string_view format, auto&&... arguments)
{
    return unexpected<ErrorPolicy>(fmt::vformat(format, fmt::make_format_args(arguments...)));
}

template <error_policy_concept ErrorPolicy = DefaultError>
constexpr auto make_error(ErrorPolicy&& error) noexcept
{
    return unexpected<ErrorPolicy>(std::forward<ErrorPolicy>(error));
}

template <error_policy_concept ErrorPolicy = DefaultError>
constexpr auto make_error(ErrorPolicy& error) noexcept
{
    return unexpected<ErrorPolicy>(std::move(error));
}

} // liberror

