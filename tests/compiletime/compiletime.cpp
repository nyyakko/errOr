#include <gtest/gtest.h>

#include <liberror/ErrorOr.hpp>
#include <liberror/types/TraceError.hpp>

#include <memory>

using namespace liberror;

consteval ErrorOr<size_t> inspect_string(std::string_view input, size_t current = 0)
{
    size_t index = current;

    for (; index < input.size() && input.at(index) != '>'; index += 1)
    {
        if (input.at(index) == '<')
        {
            index += 1;
            auto maybeIndex = inspect_string(input, index);
            if (maybeIndex.has_error()) { return make_error(maybeIndex.error()); }
            index = maybeIndex.value();
            if (index >= input.size() || input.at(index) != '>') return make_error("invalid string");
        }
    }

    return index;
}

TEST(compile_time, inspect_string_success)
{
    static_assert(inspect_string("<69420>").has_error() == false);
    static_assert(inspect_string("<<69420> is <42069>>").has_error() == false);
}

TEST(compile_time, inspect_string_failure)
{
    static_assert(inspect_string("<69420").has_error() == true);
    static_assert(inspect_string("<69420 is <420>").has_error() == true);
}

TEST(compile_time, convert_to_string_implicitly)
{
    auto constexpr result = [] () constexpr -> ErrorOr<std::string_view> {
        return "69420";
    }();

    static_assert(result.has_error() == false);
    static_assert(result.value() == "69420");
}

TEST(compile_time, convert_to_pair_implicitly)
{
    auto constexpr result = [] () -> ErrorOr<std::pair<int, std::string_view>> {
        return std::pair { 69, "420" };
    }();

    static_assert(result.has_error() == false);
    static_assert(result.value() == std::pair { 69, "420" });
}

TEST(compile_time, non_default_error_type_success)
{
    auto constexpr result = [] () -> ErrorOr<std::string_view, TraceError> {
        return "69420";
    }();

    static_assert(result.has_error() == false);
    static_assert(result.value() == "69420");
}

TEST(compile_time, return_move_only_type)
{
    (void)[] () -> ErrorOr<std::unique_ptr<int>> {
        std::unique_ptr<int> pointer {};
        return pointer;
    };
}

