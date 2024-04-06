module;

export module liberror;

import stl;

export import :types;

struct EmptyType {};

template <class T> concept non_empty_type_concept = !std::is_same_v<T, EmptyType>;
template <class T> concept empty_type_concept = !non_empty_type_concept<T>;

export namespace liberror {

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

template <class T, error_policy_concept ErrorPolicy = StandardError>
class [[nodiscard]] ErrorOr
{
    template <class, error_policy_concept> friend class ErrorOr;

public:
    using value_t =
        std::conditional_t
        <
            std::is_void_v<T>,
                EmptyType,
                std::conditional_t<
                    std::is_reference_v<T>,
                    std::reference_wrapper<std::remove_reference_t<T>>, T
                >
        >;

    using error_t = ErrorPolicy;

    constexpr  ErrorOr() = default;
    constexpr ~ErrorOr() = default;

    // cppcheck-suppress noExplicitConstructor
    constexpr ErrorOr(auto&& value) : value_m { std::forward<decltype(value)>(value) } {}

    template <non_empty_type_concept U>
    explicit constexpr ErrorOr(U&& value)
        noexcept (std::is_nothrow_constructible_v<value_t, U>)
            : value_m { std::in_place_type<value_t>, std::forward<U>(value) } {}

    template <non_empty_type_concept U>
    constexpr ErrorOr(ErrorOr<U, ErrorPolicy>&& errorOr) noexcept : value_m { std::move(errorOr.value()) } {}
    template <non_empty_type_concept U>
    constexpr ErrorOr(ErrorOr<U, ErrorPolicy> const& errorOr) : value_m { errorOr.value() } {}

    constexpr ErrorOr(ErrorOr<EmptyType, ErrorPolicy>&& errorOr) noexcept : value_m { std::move(errorOr.error()) } {}
    constexpr ErrorOr(ErrorOr<EmptyType, ErrorPolicy> const& errorOr) : value_m { errorOr.error() } {}

    // cppcheck-suppress noExplicitConstructor
    constexpr ErrorOr(ErrorPolicy&& error) noexcept : value_m { std::move(error) } {}
    // cppcheck-suppress noExplicitConstructor
    constexpr ErrorOr(ErrorPolicy const& error) : value_m { error } {}

    [[nodiscard]] constexpr auto has_value() const noexcept { return std::holds_alternative<value_t>(value_m); }

    [[nodiscard]] constexpr auto const& value() const&
    {
        if constexpr (!std::is_same_v<value_t, EmptyType>)
        {
            if constexpr (std::is_same_v<value_t, std::reference_wrapper<std::remove_reference_t<T>>>)
                return std::get<value_t>(value_m).get();
            else
                return std::get<value_t>(value_m);
        }
    }

    [[nodiscard]] constexpr auto& value() &
    {
        if constexpr (!std::is_same_v<value_t, EmptyType>)
        {
            if constexpr (std::is_same_v<value_t, std::reference_wrapper<std::remove_reference_t<T>>>)
                return std::get<value_t>(value_m).get();
            else
                return std::get<value_t>(value_m);
        }
    }

    [[nodiscard]] constexpr auto value() &&
        noexcept (std::is_nothrow_move_constructible_v<value_t>)
    {
        if constexpr (!std::is_same_v<value_t, EmptyType>)
        {
            return std::move(std::get<value_t>(value_m));
        }
    }

    [[gnu::warning("You're not getting a reference to, but a copy of T.")]]
    [[nodiscard]] constexpr auto value() &&
        noexcept (std::is_nothrow_move_constructible_v<value_t>)
        requires (std::is_same_v<value_t, std::reference_wrapper<std::remove_reference_t<T>>>)
    {
        if constexpr (!std::is_same_v<value_t, EmptyType>)
        {
            return std::move(std::get<value_t>(value_m).get());
        }
    }

    [[nodiscard]] constexpr auto has_error() const noexcept { return std::holds_alternative<error_t>(value_m); }
    [[nodiscard]] constexpr auto const& error() const& { return std::get<error_t>(value_m); }
    [[nodiscard]] constexpr auto& error() & { return std::get<error_t>(value_m); }
    [[nodiscard]] constexpr auto error() && noexcept { return std::move(std::get<error_t>(value_m)); }

    explicit constexpr operator bool() const noexcept { return has_value(); }

    constexpr ErrorOr& operator=(ErrorOr<EmptyType, ErrorPolicy>&& errorOr) noexcept
    {
        value_m = std::move(errorOr.error());
        return *this;
    }

    constexpr ErrorOr& operator=(ErrorOr<EmptyType, ErrorPolicy> const& errorOr)
    {
        value_m = errorOr.error();
        return *this;
    }

    template <non_empty_type_concept U>
    constexpr ErrorOr& operator=(ErrorOr<U, ErrorPolicy>&& errorOr) noexcept
    {
        value_m = std::move(errorOr);
        return *this;
    }

    template <non_empty_type_concept U>
    constexpr ErrorOr& operator=(ErrorOr<U, ErrorPolicy> const& errorOr)
    {
        value_m = errorOr;
        return *this;
    }

    constexpr ErrorOr& operator=(auto&& lhs)
        noexcept (std::is_nothrow_assignable_v<value_t, decltype(lhs)> &&
                  std::is_nothrow_constructible_v<value_t, decltype(lhs)>)
    {
        std::get<value_t>(value_m) = std::forward<decltype(lhs)>(lhs);
        return *this;
    }

private:
    std::variant<value_t, error_t> value_m {};
};

template <error_policy_concept ErrorPolicy = StandardError>
[[nodiscard]] constexpr ErrorOr<EmptyType, ErrorPolicy> make_error(std::string_view const format, auto&&... args)
{
    return ErrorOr<EmptyType, ErrorPolicy> { ErrorPolicy { std::vformat(format.data(), std::make_format_args(std::forward<decltype(args)>(args)...)) } };
}

template <error_policy_concept ErrorPolicy = StandardError>
[[nodiscard]] constexpr ErrorOr<EmptyType, ErrorPolicy> make_error(std::string_view message) noexcept
{
    return ErrorOr<EmptyType, ErrorPolicy> { ErrorPolicy { message } };
}

template <error_policy_concept ErrorPolicy = StandardError>
[[nodiscard]] constexpr ErrorOr<EmptyType, ErrorPolicy> make_error(ErrorPolicy& error)
{
    return ErrorOr<EmptyType, ErrorPolicy> { std::move(error) };
}

template <error_policy_concept ErrorPolicy = StandardError>
[[nodiscard]] constexpr ErrorOr<EmptyType, ErrorPolicy> make_error(ErrorPolicy&& error) noexcept
{
    return ErrorOr<EmptyType, ErrorPolicy> { std::move(error) };
}

template <error_policy_concept ErrorPolicy = StandardError>
[[nodiscard]] constexpr ErrorOr<EmptyType, ErrorPolicy> make_error(ErrorPolicy const& error)
{
    return ErrorOr<EmptyType, ErrorPolicy> { error };
}

} // liberror

