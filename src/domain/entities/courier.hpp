#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace lavka::domain {
struct Courier {
    enum Type { kFoot, kBike, kAuto };

    std::int64_t id{};
    Type type;
    std::vector<std::int32_t> regions;
    std::vector<std::string> working_hours;
};
}  // namespace lavka::domain