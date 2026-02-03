#include "repository_manager.hpp"

#include <userver/components/component.hpp>
#include <userver/storages/postgres/component.hpp>

namespace lavka {
RepositoryManager::RepositoryManager(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : ComponentBase(config, context),
      couriers_repository_ptr(
          std::make_shared<lavka::postgres::CourierRepository>(
              context.FindComponent<userver::components::Postgres>("lavka-db")
                  .GetCluster())) {}

lavka::postgres::CourierRepositoryPtr
RepositoryManager::GetCouriersRepository() {
    return couriers_repository_ptr;
}
}  // namespace lavka