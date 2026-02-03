#pragma once
#include <userver/storages/postgres/cluster.hpp>
#include <vector>

#include "courier.hpp"

namespace lavka::postgres {
struct CreateCourierRequest {
    lavka::postgres::Courier::Type type;
    std::vector<std::int32_t> regions;
    std::vector<std::string> working_hours;
};

class CourierRepository {
    userver::storages::postgres::ClusterPtr pg_cluster_;

   public:
    CourierRepository(userver::storages::postgres::ClusterPtr pg_cluster_);

    std::vector<Courier> GetAll(int limit, int offset);

    // Throws an std::invalid_argument when can't create at least one argument
    std::vector<Courier> CreateAll(std::vector<CreateCourierRequest> couriers);

    // Throws an std::invalid_argument when can't find courier
    Courier GetById(std::int64_t id);
};

using CourierRepositoryPtr = std::shared_ptr<CourierRepository>;
}  // namespace lavka::postgres
