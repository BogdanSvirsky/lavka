#include "courier_repository.hpp"

#include <stdexcept>

namespace lavka::postgres {
CourierRepository::CourierRepository(
    userver::storages::postgres::ClusterPtr pg_cluster_)
    : pg_cluster_(pg_cluster_) {};

std::vector<Courier> CourierRepository::CreateAll(
    std::vector<CreateCourierRequest> create_couriers_request) {
    std::vector<Courier> created_couriers;

    userver::storages::postgres::Transaction tr = pg_cluster_->Begin(
        "couriers_creation_transaction",
        userver::storages::postgres::ClusterHostType::kMaster,
        userver::storages::postgres::Transaction::RW);
    for (CreateCourierRequest& courier_request : create_couriers_request) {
        auto res = tr.Execute(
            "INSERT INTO lavka.couriers (type, regions, working_hours) "
            "VALUES ($1, $2, $3)"
            "RETURNING id, type, regions, working_hours",
            courier_request.type, courier_request.regions,
            courier_request.working_hours);

        if (res.RowsAffected() != 1) {
            tr.Rollback();
            throw std::invalid_argument{"Mistake in CreateCouriersRequest!"};
        }

        Courier createdCourier =
            res.AsSingleRow<Courier>(userver::storages::postgres::kRowTag);

        created_couriers.push_back(createdCourier);
    }
    tr.Commit();

    return created_couriers;
}

std::vector<Courier> CourierRepository::GetAll(int limit, int offset) {
    if (limit < 1 || offset < 0)
        throw std::invalid_argument{"Bad limit or offset!"};

    auto query_result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT id, type, regions, working_hours FROM lavka.couriers\n"
        "ORDER BY id LIMIT $1 OFFSET $2;",
        limit, offset);

    std::vector<Courier> couriers;
    for (auto courier :
         query_result.AsSetOf<Courier>(userver::storages::postgres::kRowTag))
        couriers.push_back(courier);

    return couriers;
}

Courier CourierRepository::GetById(std::int64_t id) {
    auto res = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT id, type, regions, working_hours FROM lavka.couriers "
        "WHERE id = $1;",
        id);

    if (res.IsEmpty()) {
        throw std::invalid_argument{"Bad id!"};
    }

    return res.AsSingleRow<Courier>(userver::storages::postgres::kRowTag);
}
}  // namespace lavka::postgres