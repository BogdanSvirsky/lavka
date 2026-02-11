#include "domain/entities/order.hpp"
#include "order.hpp"

namespace lavka::postgres::utils {
domain::Order ToDomain(const Order& postgres_order);

std::optional<userver::utils::datetime::TimePointTz> Convert(
    std::optional<userver::storages::postgres::TimePointWithoutTz> time_point);

std::optional<userver::storages::postgres::TimePointWithoutTz> Convert(
    std::optional<userver::utils::datetime::TimePointTz> time_point);

std::optional<domain::Order::Rating> Convert(std::optional<Order::Rating>);

std::optional<Order::Rating> Convert(std::optional<domain::Order::Rating>);

Order ToPostgres(domain::Order& order);
}  // namespace lavka::postgres::utils