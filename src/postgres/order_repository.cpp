#include "order_repository.hpp"

using namespace userver::storages::postgres;

namespace lavka::postgres {
OrderRepository::OrderRepository(ClusterPtr pg_cluster)
    : pg_cluster_(pg_cluster) {}

Order OrderRepository::GetById(std::int64_t id) {
    auto res =
        pg_cluster_->Execute(ClusterHostType::kSlave,
                             "SELECT * FROM lavka.orders WHERE id = $1;", id);

    if (res.IsEmpty()) throw std::invalid_argument{"Bad order id!"};

    return res.AsSingleRow<Order>(kRowTag);
}

std::vector<Order> OrderRepository::GetAll(int limit, int offset) {
    if (limit < 1 || offset < 0)
        throw std::invalid_argument{"Bad limit or offset!"};

    auto orders_result_set = pg_cluster_
                                 ->Execute(ClusterHostType::kSlave,
                                           "SELECT * FROM lavka.orders "
                                           "ORDER BY id LIMIT $1 OFFSET $2;",
                                           limit, offset)
                                 .AsSetOf<Order>(kRowTag);

    std::vector<Order> result_vector;
    for (Order order : orders_result_set) result_vector.push_back(order);
    return result_vector;
}

std::vector<Order> OrderRepository::CreateAll(
    std::vector<Order> orders_to_create) {
    std::vector<Order> created_orders;

    Transaction tr =
        pg_cluster_->Begin("orders_creation_transaction",
                           ClusterHostType::kMaster, Transaction::RW);
    for (auto order_to_create : orders_to_create) {
        auto result_set = tr.Execute(
            "INSERT INTO lavka.orders(weight, regions, delivery_hours, "
            "cost) VALUES ($1, $2, $3, $4) RETURNING *",
            order_to_create.weight, order_to_create.regions,
            order_to_create.delivery_hours, order_to_create.cost);

        if (result_set.RowsAffected() != 1) {
            tr.Rollback();
            throw std::invalid_argument{"Mistake in orders to create!"};
        }

        created_orders.push_back(result_set.AsSingleRow<Order>(kRowTag));
    }
    tr.Commit();

    return created_orders;
}

std::vector<Order> OrderRepository::UpdateAll(
    std::vector<Order> orders_to_update) {
    std::vector<Order> updated_orders;

    Transaction tr =
        pg_cluster_->Begin("orders_completion_transaction",
                           ClusterHostType::kMaster, Transaction::RW);
    for (auto order_to_update : orders_to_update) {
        auto result_set = tr.Execute(
            "UPDATE lavka.orders "
            "SET weight = $2, regions = $3, delivery_hours = $4, cost = $5, "
            "completed_courier_id = $6, completed_time = $7 "
            "WHERE id = $1 "
            "RETURNING *",
            order_to_update.id, order_to_update.weight, order_to_update.regions,
            order_to_update.delivery_hours, order_to_update.cost,
            order_to_update.completed_courier_id,
            order_to_update.completed_time);

        if (result_set.RowsAffected() != 1) {
            tr.Rollback();
            throw std::invalid_argument{"Mistake in orders to update!"};
        }

        updated_orders.push_back(result_set.AsSingleRow<Order>(kRowTag));
    }
    tr.Commit();

    return updated_orders;
}
}  // namespace lavka::postgres