#include "common_handler.hpp"

#include <userver/components/component.hpp>
#include <userver/storages/postgres/component.hpp>

namespace lavka {
CommonHandler::CommonHandler(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : HttpHandlerJsonBase(config, context),
      pg_cluster_(
          context.FindComponent<userver::components::Postgres>("lavka-db")
              .GetCluster()) {}

userver::storages::postgres::Cluster& CommonHandler::GetPg() const {
    return *pg_cluster_;
}
}  // namespace lavka