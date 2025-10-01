#include <greeting.hpp>

#include <userver/utest/utest.hpp>

using lavka::UserType;

UTEST(SayHelloTo, Basic) {
    EXPECT_EQ(lavka::SayHelloTo("Developer", UserType::kFirstTime),
              "Hello, Developer!\n");
    EXPECT_EQ(lavka::SayHelloTo({}, UserType::kFirstTime),
              "Hello, unknown user!\n");

    EXPECT_EQ(lavka::SayHelloTo("Developer", UserType::kKnown),
              "Hi again, Developer!\n");
    EXPECT_EQ(lavka::SayHelloTo({}, UserType::kKnown),
              "Hi again, unknown user!\n");
}