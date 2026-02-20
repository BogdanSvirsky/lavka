#pragma once
#include <userver/storages/postgres/cluster.hpp>
#include <vector>

#include "domain/repositories/courier_repository.hpp"

namespace lavka::postgres {
class CourierRepository final : public domain::ICourierRepository {
    userver::storages::postgres::ClusterPtr pg_cluster_;

   public:
    CourierRepository(userver::storages::postgres::ClusterPtr pg_cluster_);

    std::vector<domain::Courier> GetAll(int limit, int offset) override;

    // Throws an std::invalid_argument when can't create at least one argument
    std::vector<domain::Courier> CreateAll(
        const std::vector<domain::Courier>& couriers_to_create) override;

    // Throws an std::invalid_argument when can't find courier
    domain::Courier GetById(domain::Courier::Id) override;

    void UpdateRatings(
        const std::vector<std::tuple<domain::Courier::Id,
                                     domain::Courier::Rating>>&) override;
};
}  // namespace lavka::postgres
