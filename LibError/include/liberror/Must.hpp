#pragma once

#if defined(__clang__) || defined(__GNUC__)

#define MUST(expression) ({                                                         \
    auto _ = (expression);                                                          \
    if (_.has_error())                                                              \
    {                                                                               \
        std::println(stderr, "Aborted execution because: {}", _.error().message()); \
        std::abort();                                                               \
    }                                                                               \
    std::move(_).value();                                                           \
})

#else
#error "Your compiler doesn't allow for [compound-expressions](https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html)"
#endif
