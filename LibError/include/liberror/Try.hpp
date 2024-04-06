#pragma once

#if defined(__clang__) || defined(__GNUC__)

#define TRY(expression) ({                                                          \
    auto _ = (expression);                                                          \
    if (_.has_error())                                                              \
        return liberror::make_error(_.error());                                     \
    std::move(_).value();                                                           \
})

#else
#error "Your compiler doesn't allow for [compound-expressions](https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html)"
#endif
