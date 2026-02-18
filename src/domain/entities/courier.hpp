#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace lavka::domain {
struct Courier {
    enum Type { kFoot, kBike, kAuto };
    using Id = std::int64_t;
    using Rating = std::optional<double>;

    Id id{};
    Type type;
    std::vector<std::int32_t> regions;
    std::vector<std::string> working_hours;
    Rating rating{};
};
}  // namespace lavka::domain