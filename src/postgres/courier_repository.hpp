#pragma once
#include <userver/storages/postgres/cluster.hpp>
#include <vector>

#include "courier.hpp"

namespace lavka::postgres {
class CourierRepository {
    userver::storages::postgres::ClusterPtr pg_cluster_;

   public:
    CourierRepository(userver::storages::postgres::ClusterPtr pg_cluster_);

    std::vector<Courier> GetAll(int limit, int offset);

    // Throws an std::invalid_argument when can't create at least one argument
    std::vector<Courier> CreateAll(std::vector<Courier> couriers_to_create);

    // Throws an std::invalid_argument when can't find courier
    Courier GetById(std::int64_t id);
};

using CourierRepositoryPtr = std::shared_ptr<CourierRepository>;
}  // namespace lavka::postgres
