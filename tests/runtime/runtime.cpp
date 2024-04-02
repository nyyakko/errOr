#include <gtest/gtest.h>

#include <liberror/ErrorOr.hpp>
#include <liberror/types/TraceError.hpp>

using namespace liberror;

struct S
{
    explicit constexpr S(std::string value) noexcept : value_m { value } { std::println("S::S(std::string)"); }
    explicit constexpr S(char const* value) noexcept : value_m { value } { std::println("S::S(char const*)"); }
    constexpr ~S() noexcept { std::println("S::~S()"); }
    constexpr S(S const& s) noexcept : value_m { s.value_m } { std::println("S::S(S const&)"); }
    constexpr S(S&& s) noexcept : value_m { std::move(s.value_m) } { std::println("S::S(S&&)"); }
    constexpr S& operator=(S const& s) { value_m = s.value_m; std::println("S::S operator=(S const&)"); return *this; }
    constexpr S& operator=(S&& s) noexcept { value_m = std::move(s.value_m); std::println("S::S operator=(S&&)"); return *this; }

    std::string value_m {};
};

TEST(compile_time, non_default_error_type_failure)
{
    auto result = [] () -> ErrorOr<std::string_view, TraceError> {
        return make_error<TraceError>("failure");
    }();
}

TEST(runtime, no_error)
{
    auto result = [] () -> ErrorOr<std::string_view> {
        return "69420";
    }();

    EXPECT_STREQ(result.value().data(), "69420");
}

TEST(runtime, single_error)
{
    auto result = [] () -> ErrorOr<std::string> {
        return make_error("error");
    }();

    EXPECT_STREQ(result.error().message().data(), "error");
}

TEST(runtime, multiple_error)
{
    auto result = [] () -> ErrorOr<std::string> {
        auto const error = [] () -> ErrorOr<std::string> {
            return make_error("first error {}", 69);
        }();
        if (error.has_error()) return make_error(error.error());
        return make_error("second error");
    }();

    EXPECT_STREQ(result.error().message().data(), "first error 69");
}

