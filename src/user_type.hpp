#pragma once

#include <string>

namespace lavka {
enum class UserType { kFirstTime, kKnown };

UserType GetUserType(std::string username);
}  // namespace lavka