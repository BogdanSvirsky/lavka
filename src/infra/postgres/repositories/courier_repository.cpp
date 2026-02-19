#include "courier_repository.hpp"

#include <stdexcept>

#include "infra/postgres/models/courier.hpp"
#include "infra/postgres/utils.hpp"

using namespace userver::storages::postgres;

namespace lavka::postgres {
CourierRepository::CourierRepository(ClusterPtr pg_cluster_)
    : pg_cluster_(pg_cluster_) {};

std::vector<domain::Courier> CourierRepository::CreateAll(
    const std::vector<domain::Courier>& couriers_to_create) {
    std::vector<domain::Courier> created_couriers;

    Transaction tr =
        pg_cluster_->Begin("couriers_creation_transaction",
                           ClusterHostType::kMaster, Transaction::RW);
    for (auto& courier_request : couriers_to_create) {
        auto res = tr.Execute(
            "INSERT INTO lavka.couriers (type, regions, working_hours) "
            "VALUES ($1, $2, $3) RETURNING *",
            Courier::Type(courier_request.type), courier_request.regions,
            courier_request.working_hours);

        if (res.RowsAffected() != 1) {
            tr.Rollback();
            throw std::invalid_argument{"Mistake in CreateCouriersRequest!"};
        }

        domain::Courier created_courier =
            utils::ToDomain(res.AsSingleRow<Courier>(kRowTag));

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
        "SELECT * FROM lavka.couriers ORDER BY id LIMIT $1 OFFSET $2;", limit,
        offset);

    std::vector<domain::Courier> couriers;
    for (Courier courier : result_set.AsSetOf<Courier>(kRowTag))
        couriers.push_back(utils::ToDomain(courier));

    return couriers;
}

domain::Courier CourierRepository::GetById(domain::Courier::Id id) {
    auto res =
        pg_cluster_->Execute(ClusterHostType::kSlave,
                             "SELECT * FROM lavka.couriers WHERE id = $1;", id);

    if (res.IsEmpty()) {
        throw std::invalid_argument{"Bad id!"};
    }

    Courier postgres_courier = res.AsSingleRow<Courier>(kRowTag);

    return utils::ToDomain(postgres_courier);
}

void CourierRepository::UpdateRatings(
    const std::vector<std::tuple<domain::Courier::Id, domain::Courier::Rating>>&
        ratings_to_update) {
    auto tr = pg_cluster_->Begin("courier-ratings-update",
                                 ClusterHostType::kMaster, Transaction::RW);
    for (auto [id, rating] : ratings_to_update) {
        auto res = tr.Execute(
            "UPDATE lavka.couriers SET rating = $1 WHERE id = $2 ", rating, id);
        if (res.RowsAffected() != 1) {
            tr.Rollback();
            throw std::invalid_argument{"Mistake in ratings to update!"};
        }
    }
    tr.Commit();
}
}  // namespace lavka::postgres