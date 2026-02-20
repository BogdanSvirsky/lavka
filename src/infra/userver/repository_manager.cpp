#include "repository_manager.hpp"

#include <userver/components/component.hpp>
#include <userver/storages/postgres/component.hpp>

#include "infra/postgres/repositories/courier_repository.hpp"
#include "infra/postgres/repositories/order_repository.hpp"

namespace lavka {
RepositoryManager::RepositoryManager(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : ComponentBase(config, context),
      pg_cluster_(
          context.FindComponent<userver::components::Postgres>("lavka-db")
              .GetCluster()),
      courier_repository(
          std::make_shared<postgres::CourierRepository>(pg_cluster_)),
      order_repository(
          std::make_shared<postgres::OrderRepository>(pg_cluster_)) {}

domain::ICourierRepositoryPtr RepositoryManager::GetCourierRepository() {
    return courier_repository;
}

domain::IOrderRepositoryPtr RepositoryManager::GetOrderRepository() {
    return order_repository;
}
}  // namespace lavka