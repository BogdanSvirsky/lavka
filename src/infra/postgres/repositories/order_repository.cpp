#include "order_repository.hpp"

#include "infra/postgres/models/order.hpp"
#include "infra/postgres/utils.hpp"

using namespace userver::storages::postgres;

namespace lavka::postgres {
OrderRepository::OrderRepository(ClusterPtr pg_cluster)
    : pg_cluster_(pg_cluster) {}

domain::Order OrderRepository::GetById(std::int64_t id) {
    auto res =
        pg_cluster_->Execute(ClusterHostType::kSlave,
                             "SELECT * FROM lavka.orders WHERE id = $1;", id);
    LOG_DEBUG() << res.RowsAffected() << " orders found with id " << id;
    if (res.IsEmpty()) throw std::invalid_argument{"Bad order id!"};

    return utils::ToDomain(res.AsSingleRow<Order>(kRowTag));
}

std::vector<domain::Order> OrderRepository::GetAll(int limit, int offset) {
    if (limit < 1 || offset < 0)
        throw std::invalid_argument{"Bad limit or offset!"};

    auto result_set = pg_cluster_->Execute(ClusterHostType::kSlave,
                                           "SELECT * FROM lavka.orders "
                                           "ORDER BY id LIMIT $1 OFFSET $2;",
                                           limit, offset);
    auto orders_result_set = result_set.AsSetOf<Order>(kRowTag);

    std::vector<domain::Order> result_vector;
    for (Order order : orders_result_set)
        result_vector.push_back(utils::ToDomain(order));
    return result_vector;
}

std::vector<domain::Order> OrderRepository::CreateAll(
    std::vector<domain::Order> orders_to_create) {
    std::vector<domain::Order> created_orders;
    Transaction tr =
        pg_cluster_->Begin("orders_creation_transaction",
                           ClusterHostType::kMaster, Transaction::RW);
    for (auto order_to_create : orders_to_create) {
        auto result_set = tr.Execute(
            "INSERT INTO lavka.orders(weight, regions, delivery_hours, "
            "cost, completed_courier_id, rating, completed_time) VALUES ($1, "
            "$2, $3, $4, $5, $6, $7) RETURNING *",
            order_to_create.weight, order_to_create.regions,
            order_to_create.delivery_hours, order_to_create.cost,
            order_to_create.completed_courier_id,
            utils::Convert(order_to_create.rating),
            utils::Convert(order_to_create.completed_time));

        if (result_set.RowsAffected() != 1) {
            tr.Rollback();
            throw std::invalid_argument{"Mistake in orders to create!"};
        }

        created_orders.push_back(
            utils::ToDomain(result_set.AsSingleRow<Order>(kRowTag)));
    }
    tr.Commit();

    return created_orders;
}

std::vector<domain::Order> OrderRepository::UpdateAll(
    std::vector<domain::Order> orders_to_update) {
    std::vector<domain::Order> updated_orders;

    Transaction tr =
        pg_cluster_->Begin("orders_completion_transaction",
                           ClusterHostType::kMaster, Transaction::RW);
    for (auto order_to_update : orders_to_update) {
        auto result_set = tr.Execute(
            "UPDATE lavka.orders "
            "SET weight = $2, regions = $3, delivery_hours = $4, cost = "
            "$5, completed_courier_id = $6, completed_time = $7, rating = $8 "
            "WHERE id = $1 "
            "RETURNING *",
            order_to_update.id, order_to_update.weight, order_to_update.regions,
            order_to_update.delivery_hours, order_to_update.cost,
            order_to_update.completed_courier_id,
            utils::Convert(order_to_update.completed_time),
            utils::Convert(order_to_update.rating));

        if (result_set.RowsAffected() != 1) {
            tr.Rollback();
            throw std::invalid_argument{"Mistake in orders to update!"};
        }

        updated_orders.push_back(
            utils::ToDomain(result_set.AsSingleRow<Order>(kRowTag)));
    }
    tr.Commit();

    return updated_orders;
}

std::vector<domain::Order::Rating> OrderRepository::GetLastRatings(
    std::int64_t courier_id, int limit) {
    auto result_set = pg_cluster_->Execute(
        ClusterHostType::kSlave,
        "SELECT rating FROM lavka.orders "
        "WHERE completed_courier_id = $1 AND rating IS NOT NULL "
        "ORDER BY completed_time DESC LIMIT $2",
        courier_id, limit);
    auto postgres_orders = result_set.AsContainer<std::vector<Order::Rating>>();
    std::vector<domain::Order::Rating> result;
    result.resize(postgres_orders.size());
    std::transform(postgres_orders.cbegin(), postgres_orders.cend(),
                   result.begin(),
                   [](auto rating) { return domain::Order::Rating(rating); });
    return result;
}
}  // namespace lavka::postgres