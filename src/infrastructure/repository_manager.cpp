#include "repository_manager.hpp"

#include <userver/components/component.hpp>
#include <userver/storages/postgres/component.hpp>

#include "postgres/courier_repository.hpp"
#include "postgres/order_repository.hpp"

namespace lavka {
RepositoryManager::RepositoryManager(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : ComponentBase(config, context),
      pg_cluster_(
          context.FindComponent<userver::components::Postgres>("lavka-db")
              .GetCluster()),
      courier_repository(std::shared_ptr<domain::ICourierRepository>(
          new postgres::CourierRepository(pg_cluster_))),
      order_repository(std::shared_ptr<domain::IOrderRepository>(
          new postgres::OrderRepository(pg_cluster_))) {}

domain::ICourierRepositoryPtr RepositoryManager::GetCourierRepository() {
    return courier_repository;
}

domain::IOrderRepositoryPtr RepositoryManager::GetOrderRepository() {
    return order_repository;
}
}  // namespace lavka