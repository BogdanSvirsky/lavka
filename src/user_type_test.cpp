#include "user_type.hpp"

#include <userver/utest/utest.hpp>

using namespace lavka;

UTEST(GetUserType, Basic) {
    EXPECT_EQ(GetUserType("username"), UserType::kFirstTime);
    EXPECT_EQ(GetUserType("username"), UserType::kKnown);
}