#include "utils.hpp"

namespace lavka::postgres::utils {
domain::Order ToDomain(const Order& postgres_order) {
    return {postgres_order.id,
            postgres_order.weight,
            postgres_order.regions,
            postgres_order.delivery_hours,
            postgres_order.cost,
            postgres_order.completed_courier_id,
            Convert(postgres_order.completed_time)};
}

std::optional<userver::utils::datetime::TimePointTz> Convert(
    std::optional<userver::storages::postgres::TimePointWithoutTz> time_point) {
    return time_point.has_value()
               ? std::optional<
                     userver::utils::datetime::TimePointTz>{time_point.value()}
               : std::nullopt;
}

std::optional<userver::storages::postgres::TimePointWithoutTz> Convert(
    std::optional<userver::utils::datetime::TimePointTz> time_point) {
    return time_point.has_value()
               ? std::optional<userver::storages::postgres::
                                   TimePointWithoutTz>{time_point.value()}
               : std::nullopt;
}
}  // namespace lavka::postgres::utils