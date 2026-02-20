#include "domain/entities/courier.hpp"
#include "domain/entities/order.hpp"
#include "infra/postgres/models/courier.hpp"
#include "infra/postgres/models/order.hpp"

namespace lavka::postgres::utils {
domain::Order ToDomain(const Order& postgres_order);

domain::Courier ToDomain(const Courier& postgres_courier);

std::optional<userver::utils::datetime::TimePointTz> Convert(
    std::optional<userver::storages::postgres::TimePointWithoutTz> time_point);

std::optional<userver::storages::postgres::TimePointWithoutTz> Convert(
    std::optional<userver::utils::datetime::TimePointTz> time_point);

std::optional<domain::Order::Rating> Convert(std::optional<Order::Rating>);

std::optional<Order::Rating> Convert(std::optional<domain::Order::Rating>);

Order ToPostgres(domain::Order& order);
}  // namespace lavka::postgres::utils