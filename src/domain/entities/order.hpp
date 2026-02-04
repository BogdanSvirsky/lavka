#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <userver/utils/datetime/timepoint_tz.hpp>
#include <vector>

namespace lavka::domain {
struct Order {
    std::int64_t id{};
    float weight{};
    std::int32_t regions{};
    std::vector<std::string> delivery_hours{};
    std::int32_t cost{};
    std::optional<std::int64_t> completed_courier_id{};
    std::optional<userver::utils::datetime::TimePointTz> completed_time{};
};
}  // namespace lavka::domain