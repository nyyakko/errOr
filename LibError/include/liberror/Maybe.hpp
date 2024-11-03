#pragma once

#if !(defined(__clang__) || defined(__GNUC__))
#error "Compiler doesn't support [compound-expressions](https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html)"
#endif

#include "types/DefaultError.hpp"

#include <string_view>

#ifndef __cpp_lib_print
#include <fmt/format.h>
#define LIBERROR_FMT fmt
#else
#include <format>
#include <print>
#define LIBERROR_FMT std
#endif

#ifndef __cpp_lib_expected
#include <tl/expected.hpp>
#define LIBERROR_EXP tl
#else
#include <expected>
#define LIBERROR_EXP std
#endif

#define TRY(expression) ({                                                                   \
    using namespace liberror;                                                                \
    auto&& _ = (expression);                                                                 \
    if (!_.has_value()) return make_error(_.error());                                        \
    std::move(_).value();                                                                    \
})

#define MUST(expression) ({                                                                  \
    auto&& _ = (expression);                                                                 \
    if (!_.has_value())                                                                      \
    {                                                                                        \
        LIBERROR_FMT::println(stderr, "Aborted execution because: {}", _.error().message()); \
        std::abort();                                                                        \
    }                                                                                        \
    std::move(_).value();                                                                    \
})

#define THROW(fmt, ...) return liberror::make_error(fmt __VA_OPT__(,) __VA_ARGS__);

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
using Maybe = LIBERROR_EXP::expected<std::conditional_t<std::is_void_v<T>, Void, T>, ErrorPolicy>;

template <error_policy_concept ErrorPolicy = DefaultError>
constexpr auto make_error(std::string_view message)
{
    return LIBERROR_EXP::unexpected<ErrorPolicy>(message);
}

template <error_policy_concept ErrorPolicy = DefaultError>
constexpr auto make_error(std::string_view format, auto&&... arguments)
{
    return LIBERROR_EXP::unexpected<ErrorPolicy>(LIBERROR_FMT::vformat(format, LIBERROR_FMT::make_format_args(arguments...)));
}

template <error_policy_concept ErrorPolicy = DefaultError>
constexpr auto make_error(ErrorPolicy&& error) noexcept
{
    return LIBERROR_EXP::unexpected<ErrorPolicy>(std::forward<ErrorPolicy>(error));
}

template <error_policy_concept ErrorPolicy = DefaultError>
constexpr auto make_error(ErrorPolicy& error) noexcept
{
    return LIBERROR_EXP::unexpected<ErrorPolicy>(std::move(error));
}

} // liberror

