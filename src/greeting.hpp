#pragma once

#include <string>
#include <string_view>
#include <user_type.hpp>

namespace lavka {

std::string SayHelloTo(std::string_view name, UserType type);

}  // namespace lavka