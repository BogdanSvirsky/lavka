#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <userver/storages/postgres/io/chrono.hpp>
#include <userver/utils/trivial_map.hpp>
#include <vector>

namespace lavka::postgres {
struct Order {
    enum Rating { k1 = 1, k2 = 2, k3 = 3, k4 = 4, k5 = 5 };

    std::int64_t id{};
    double weight;
    std::int32_t regions;
    std::vector<std::string> delivery_hours;
    std::int32_t cost;
    std::optional<std::int64_t> completed_courier_id{};
    std::optional<Rating> rating;
    std::optional<userver::storages::postgres::TimePointWithoutTz>
        completed_time{};
};
}  // namespace lavka::postgres

template <>
struct userver::storages::postgres::io::CppToUserPg<
    lavka::postgres::Order::Rating> {
    static constexpr DBTypeName postgres_name = "lavka.rating";
    static constexpr userver::utils::TrivialBiMap enumerators =
        [](auto selector) {
            return selector()
                .Case("1", lavka::postgres::Order::Rating::k1)
                .Case("2", lavka::postgres::Order::Rating::k2)
                .Case("3", lavka::postgres::Order::Rating::k3)
                .Case("4", lavka::postgres::Order::Rating::k4)
                .Case("5", lavka::postgres::Order::Rating::k5);
        };
};