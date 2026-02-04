#pragma once
#include <userver/storages/postgres/cluster.hpp>

#include "domain/repositories/order_repository.hpp"

namespace lavka::postgres {
class OrderRepository final : public domain::IOrderRepository {
    userver::storages::postgres::ClusterPtr pg_cluster_;

   public:
    OrderRepository(userver::storages::postgres::ClusterPtr pg_cluster);

    // Throws an std::invalid_argument when id is invalid
    domain::Order GetById(std::int64_t id) override;

    std::vector<domain::Order> GetAll(int limit, int offset) override;

    // Throws an std::invalid_argument when can't create one of the orders
    std::vector<domain::Order> CreateAll(
        std::vector<domain::Order> orders_to_create) override;

    // Throws an std::invalid_argument when can't update one of the orders
    std::vector<domain::Order> UpdateAll(
        std::vector<domain::Order> orders_to_update) override;
};
}  // namespace lavka::postgres