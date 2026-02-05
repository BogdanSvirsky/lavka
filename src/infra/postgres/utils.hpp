#include "domain/entities/order.hpp"
#include "order.hpp"

namespace lavka::postgres::utils {
domain::Order ToDomain(const Order& postgres_order);

std::optional<userver::utils::datetime::TimePointTz> Convert(
    std::optional<userver::storages::postgres::TimePointWithoutTz> time_point);

std::optional<userver::storages::postgres::TimePointWithoutTz> Convert(
    std::optional<userver::utils::datetime::TimePointTz> time_point);
}  // namespace lavka::postgres::utils