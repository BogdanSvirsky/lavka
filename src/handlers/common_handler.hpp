#pragma once
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace lavka {
class CommonHandler : public userver::server::handlers::HttpHandlerJsonBase {
    userver::storages::postgres::ClusterPtr pg_cluster_;

   public:
    CommonHandler(const userver::components::ComponentConfig& config,
                  const userver::components::ComponentContext& context);

   protected:
    userver::storages::postgres::Cluster& GetPg() const;
};
}  // namespace lavka