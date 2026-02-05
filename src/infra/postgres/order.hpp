#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <userver/storages/postgres/io/chrono.hpp>
#include <vector>

namespace lavka::postgres {
struct Order {
    std::int64_t id{};
    float weight;
    std::int32_t regions;
    std::vector<std::string> delivery_hours;
    std::int32_t cost;
    std::optional<std::int64_t> completed_courier_id{};
    std::optional<userver::storages::postgres::TimePointWithoutTz>
        completed_time{};
};
}  // namespace lavka::postgres