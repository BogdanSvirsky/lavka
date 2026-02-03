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

class CouriersRepository {
    userver::storages::postgres::ClusterPtr pg_cluster_;

   public:
    CouriersRepository(userver::storages::postgres::ClusterPtr pg_cluster_);

    std::vector<Courier> GetAll(int limit, int offset);

    // Throws an std::invalid_argument when can't create at least one argument
    std::vector<Courier> CreateAll(std::vector<CreateCourierRequest> couriers);

    // Throws an std::invalid_argument when can't find courier
    Courier GetById(std::int64_t id);
};

using CouriersRepositoryPtr = std::shared_ptr<CouriersRepository>;
}  // namespace lavka::postgres
