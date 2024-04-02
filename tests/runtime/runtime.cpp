#include <gtest/gtest.h>

#include <liberror/ErrorOr.hpp>

using namespace liberror;

TEST(runtime, no_error)
{
    auto result = [] () -> ErrorOr<std::string> {
        return "69420";
    }();

    EXPECT_STREQ(result.value().c_str(), "69420");
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

        if (error.has_error())
        {
            return make_error(error.error());
        }

        return make_error("second error");
    }();

    EXPECT_STREQ(result.error().message().data(), "first error 69");
}

