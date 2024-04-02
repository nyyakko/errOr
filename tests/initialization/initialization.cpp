#include <gtest/gtest.h>

#include <liberror/ErrorOr.hpp>

#include <array>
#include <string>
#include <print>

using namespace liberror;

static constexpr auto BUFFER_SIZE = 1024;

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

struct S
{
    constexpr S(std::string) noexcept { std::println("S::S(std::string)"); }
    constexpr S(char const*) noexcept { std::println("S::S(char const*)"); }
    constexpr ~S() noexcept { std::println("S::~S()"); }
    constexpr S(S const&) noexcept { std::println("S::S(S const&)"); }
    constexpr S(S&&) noexcept { std::println("S::S(S&&)"); }
    constexpr S& operator=(S const&) noexcept { std::println("S::S operator=(S const&)"); return *this; }
    constexpr S& operator=(S&&) noexcept { std::println("S::S operator=(S&&)"); return *this; }
};

TEST(initialization, explicit_with_in_place_construction)
{
    std::array<char, BUFFER_SIZE> buffer {};
    auto const previousState = redirect_stdout_to_buffer(buffer);
    (void)[] -> ErrorOr<S> {
        using namespace std::literals;
        ErrorOr<S> temp { "hello"s };
        return temp;
    }();
    restore_stdout(previousState);
    EXPECT_STREQ(buffer.data(), "S::S(std::string)\nS::~S()\n");
}

TEST(initialization, implicit_with_in_place_construction)
{
    std::array<char, BUFFER_SIZE> buffer {};
    auto const previousState = redirect_stdout_to_buffer(buffer);
    (void)[] -> ErrorOr<S> {
        return "hello";
    }();
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

