#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <userver/utils/datetime/timepoint_tz.hpp>
#include <vector>

#include "courier.hpp"

namespace lavka::domain {
struct Order {
    enum Rating { k1 = 1, k2 = 2, k3 = 3, k4 = 4, k5 = 5 };
    using Id = std::int64_t;

    Id id{};
    double weight{};
    std::int32_t regions{};
    std::vector<std::string> delivery_hours{};
    std::int32_t cost{};
    std::optional<Courier::Id> completed_courier_id{};
    std::optional<userver::utils::datetime::TimePointTz> completed_time{};
    std::optional<Rating> rating{};
};
}  // namespace lavka::domain