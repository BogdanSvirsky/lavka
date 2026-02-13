#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace lavka::domain {
struct Courier {
    enum Type { kFoot, kBike, kAuto };

    std::int64_t id{};
    Type type;
    std::vector<std::int32_t> regions;
    std::vector<std::string> working_hours;
    std::optional<double> rating{};
};
}  // namespace lavka::domain