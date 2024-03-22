#include <gtest/gtest.h>

#include <err_or/ErrorOr.hpp>

using namespace error;

TEST(runtime, no_error)
{
    auto result = [] -> ErrorOr<std::string> {
        return "69420";
    }();

    EXPECT_STREQ(result.value().c_str(), "69420");
}

TEST(runtime, single_error)
{
    auto result = [] -> ErrorOr<std::string> {
        return make_error("error");
    }();

    EXPECT_STREQ(result.error().message().c_str(), "error");
}

TEST(runtime, multiple_error)
{
    auto result = [] -> ErrorOr<std::string> {
        (void)TRY([] -> ErrorOr<std::string> {
            return make_error("first error");
        }());

        return make_error("second error");
    }();

    EXPECT_STREQ(result.error().message().c_str(), "first error");
}

