#include <cstdint>
#include <optional>
#include <string>
#include <userver/storages/postgres/io/chrono.hpp>
#include <vector>

#include "schemas/openapi.hpp"

namespace lavka::postgres {
struct Order {
    std::int64_t id{};
    float weight{};
    std::int32_t regions{};
    std::vector<std::string> delivery_hours{};
    std::int32_t cost{};
    std::optional<userver::storages::postgres::TimePointWithoutTz>
        completed_time{};

    operator chaotic::openapi::OrderDto() const {
        chaotic::openapi::OrderDto result{id, weight, regions, delivery_hours,
                                          cost};
        if (completed_time) {
            result.completed_time = userver::utils::datetime::TimePointTz{
                completed_time->GetUnderlying()};
        }
        return result;
    }
};
}  // namespace lavka::postgres