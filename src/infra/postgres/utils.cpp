#include "utils.hpp"

namespace lavka::postgres::utils {
domain::Order ToDomain(const Order& postgres_order) {
    return {postgres_order.id,
            postgres_order.weight,
            postgres_order.regions,
            postgres_order.delivery_hours,
            postgres_order.cost,
            postgres_order.completed_courier_id,
            Convert(postgres_order.completed_time),
            Convert(postgres_order.rating)};
}

domain::Courier ToDomain(const Courier& postgres_courier) {
    return {postgres_courier.id, domain::Courier::Type(postgres_courier.type),
            postgres_courier.regions, postgres_courier.working_hours,
            postgres_courier.rating.has_value()
                ? std::optional<double>{*postgres_courier.rating}
                : std::nullopt};
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

std::optional<domain::Order::Rating> Convert(
    std::optional<Order::Rating> order_rating) {
    return order_rating.has_value()
               ? std::optional<domain::Order::Rating>{domain::Order::Rating(
                     order_rating.value())}
               : std::nullopt;
}

std::optional<Order::Rating> Convert(
    std::optional<domain::Order::Rating> order_rating) {
    return order_rating.has_value()
               ? std::optional<Order::Rating>{Order::Rating(
                     order_rating.value())}
               : std::nullopt;
}

Order ToPostgres(domain::Order& order) {
    return {order.id,
            order.weight,
            order.regions,
            order.delivery_hours,
            order.cost,
            order.completed_courier_id,
            Convert(order.rating),
            Convert(order.completed_time)};
}
}  // namespace lavka::postgres::utils