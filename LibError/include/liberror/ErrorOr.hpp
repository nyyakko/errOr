#pragma once

#include "types/DefaultError.hpp"

#include <cstdlib>

#ifndef __cpp_lib_print
#include <fmt/format.h>

namespace liberror {
    using fmt::vformat;
    using fmt::make_format_args;
    using fmt::println;
}
#else
#include <format>
#include <print>

namespace liberror {
    using std::vformat;
    using std::make_format_args;
    using std::println;
}
#endif

#ifndef __cpp_lib_expected
#include <tl/expected.hpp>

namespace liberror {
    using namespace tl;
}
#else
#include <expected>

namespace liberror {
    template <class T, class E>
    using expected = std::expected<T, E>;

    template <class E>
    using unexpected = std::unexpected<E>;
}
#endif

#include <string_view>

#if defined(__clang__) || defined(__GNUC__)
#define TRY(expression) ({                                                              \
    using namespace liberror;                                                           \
    auto&& _ = (expression);                                                            \
    if (!_.has_value()) return make_error(_.error());                                   \
    std::move(_).value();                                                               \
})

#define MUST(expression) ({                                                             \
    using namespace liberror;                                                           \
    auto&& _ = (expression);                                                            \
    if (!_.has_value())                                                                 \
    {                                                                                   \
        liberor::println(stderr, "Aborted execution because: {}", _.error().message()); \
        std::abort();                                                                   \
    }                                                                                   \
    std::move(_).value();                                                               \
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

struct Void {};

template <class T, error_policy_concept ErrorPolicy = DefaultError>
using ErrorOr = expected<std::conditional_t<std::is_void_v<T>, Void, T>, ErrorPolicy>;

template <error_policy_concept ErrorPolicy = DefaultError>
constexpr auto make_error(std::string_view message)
{
    return unexpected<ErrorPolicy>(message);
}

template <error_policy_concept ErrorPolicy = DefaultError>
constexpr auto make_error(std::string_view format, auto&&... arguments)
{
    return unexpected<ErrorPolicy>(vformat(format, make_format_args(arguments...)));
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

