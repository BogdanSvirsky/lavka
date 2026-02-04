#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <userver/storages/postgres/io/chrono.hpp>
#include <vector>

#include "domain/entities/order.hpp"

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

    operator domain::Order() const {
        domain::Order order{
            id, weight, regions, delivery_hours, cost, completed_courier_id};
        if (completed_time.has_value())
            order.completed_time =
                userver::utils::datetime::TimePointTz{completed_time.value()};
        return order;
    }
};
}  // namespace lavka::postgres