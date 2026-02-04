#include "courier_repository.hpp"

#include <stdexcept>

#include "courier.hpp"

using namespace userver::storages::postgres;

namespace lavka::postgres {
CourierRepository::CourierRepository(ClusterPtr pg_cluster_)
    : pg_cluster_(pg_cluster_) {};

std::vector<domain::Courier> CourierRepository::CreateAll(
    std::vector<domain::Courier> couriers_to_create) {
    std::vector<domain::Courier> created_couriers;

    Transaction tr =
        pg_cluster_->Begin("couriers_creation_transaction",
                           ClusterHostType::kMaster, Transaction::RW);
    for (auto& courier_request : couriers_to_create) {
        auto res = tr.Execute(
            "INSERT INTO lavka.couriers (type, regions, working_hours) "
            "VALUES ($1, $2, $3)"
            "RETURNING id, type, regions, working_hours",
            Courier::Type(courier_request.type), courier_request.regions,
            courier_request.working_hours);

        if (res.RowsAffected() != 1) {
            tr.Rollback();
            throw std::invalid_argument{"Mistake in CreateCouriersRequest!"};
        }

        domain::Courier created_courier = res.AsSingleRow<Courier>(kRowTag);

        created_couriers.push_back(created_courier);
    }
    tr.Commit();

    return created_couriers;
}

std::vector<domain::Courier> CourierRepository::GetAll(int limit, int offset) {
    if (limit < 1 || offset < 0)
        throw std::invalid_argument{"Bad limit or offset!"};

    auto result_set = pg_cluster_->Execute(
        ClusterHostType::kSlave,
        "SELECT id, type, regions, working_hours FROM lavka.couriers\n"
        "ORDER BY id LIMIT $1 OFFSET $2;",
        limit, offset);

    std::vector<domain::Courier> couriers;
    for (Courier courier : result_set.AsSetOf<Courier>(kRowTag))
        couriers.push_back(courier);

    return couriers;
}

domain::Courier CourierRepository::GetById(std::int64_t id) {
    auto res = pg_cluster_->Execute(
        ClusterHostType::kSlave,
        "SELECT id, type, regions, working_hours FROM lavka.couriers "
        "WHERE id = $1;",
        id);

    if (res.IsEmpty()) {
        throw std::invalid_argument{"Bad id!"};
    }

    return res.AsSingleRow<Courier>(kRowTag);
}
}  // namespace lavka::postgres