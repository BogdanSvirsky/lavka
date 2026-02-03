#pragma once
#include <userver/storages/postgres/cluster.hpp>

#include "order.hpp"

namespace lavka::postgres {
class OrderRepository {
    userver::storages::postgres::ClusterPtr pg_cluster_;

   public:
    OrderRepository(userver::storages::postgres::ClusterPtr pg_cluster);

    // Throws an std::invalid_argument when id is invalid
    Order GetById(std::int64_t id);

    std::vector<Order> GetAll(int limit, int offset);

    // Throws an std::invalid_argument when can't create one of the orders
    std::vector<Order> CreateAll(std::vector<Order> orders_to_create);

    // Throws an std::invalid_argument when can't update one of the orders
    std::vector<Order> UpdateAll(std::vector<Order> orders_to_update);
};

using OrderRepositoryPtr = std::shared_ptr<OrderRepository>;
}  // namespace lavka::postgres