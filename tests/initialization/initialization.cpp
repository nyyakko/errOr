#include <gtest/gtest.h>

#include <liberror/ErrorOr.hpp>

#include <array>
#include <string>
#include <print>

using namespace liberror;
using namespace std::literals;

static constexpr auto BUFFER_SIZE = 1024;

struct S
{
    explicit constexpr S(std::string value) : value_m { value } { std::println("S::S(std::string)"); }
    constexpr S(char const* value) : value_m { value } { std::println("S::S(char const*)"); }
    constexpr ~S() noexcept { std::println("S::~S()"); }
    constexpr S(S const& s) : value_m { s.value_m } { std::println("S::S(S const&)"); }
    constexpr S(S&& s) noexcept : value_m { std::move(s.value_m) } { std::println("S::S(S&&)"); }
    constexpr S& operator=(S const& s) { value_m = s.value_m; std::println("S::S operator=(S const&)"); return *this; }
    constexpr S& operator=(S&& s) noexcept { value_m = std::move(s.value_m); std::println("S::S operator=(S&&)"); return *this; }

    std::string value_m {};
};

int redirect_stdout_to_buffer(std::array<char, BUFFER_SIZE>& buffer)
{
    fflush(stdout);
    auto previousState = _dup(_fileno(stdout));
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    (void)freopen("NUL", "a", stdout);
#pragma clang diagnostic pop
    setvbuf(stdout, buffer.data(), _IOFBF, buffer.size());

    return previousState;
}

void restore_stdout(int state)
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    (void)freopen("NUL", "a", stdout);
#pragma clang diagnostic pop
    _dup2(state, _fileno(stdout));
    setvbuf(stdout, NULL, _IONBF, BUFFER_SIZE);
}

TEST(initialization, explicit_with_in_place_construction)
{
    std::array<char, BUFFER_SIZE> buffer {};
    auto const previousState = redirect_stdout_to_buffer(buffer);
    {
        auto result = [] -> ErrorOr<S> {
            ErrorOr<S> temp { "hello"s };
            return temp;
        }();
    }
    restore_stdout(previousState);
    EXPECT_STREQ(buffer.data(), "S::S(std::string)\nS::~S()\n");
}

TEST(initialization, explicit_with_in_place_construction_while_checking_for_internal_state)
{
    std::array<char, BUFFER_SIZE> buffer {};
    auto const previousState = redirect_stdout_to_buffer(buffer);
    {
        auto result = [] -> ErrorOr<S> {
            ErrorOr<S> temp { "hello" };
            auto const& value = temp.value();
            if (value.value_m != "hello"s) return make_error("failure");
            return temp;
        }();
    }
    restore_stdout(previousState);
    EXPECT_STREQ(buffer.data(), "S::S(char const*)\nS::S(S&&)\nS::~S()\nS::~S()\n");
}

TEST(initialization, implicit_with_in_place_construction)
{
    std::array<char, BUFFER_SIZE> buffer {};
    auto const previousState = redirect_stdout_to_buffer(buffer);
    {
        auto result = [] -> ErrorOr<S> {
            return "hello";
        }();
    }
    restore_stdout(previousState);
    EXPECT_STREQ(buffer.data(), "S::S(char const*)\nS::~S()\n");
}


TEST(initialization, no_extra_object_failure)
{
    std::array<char, BUFFER_SIZE> buffer {};
    auto const previousState = redirect_stdout_to_buffer(buffer);
    auto const result = [] -> ErrorOr<S> {
        return make_error("failure");
    }();
    std::println("{}", result.error().message());
    restore_stdout(previousState);
    EXPECT_STREQ(buffer.data(), "failure\n");
}

