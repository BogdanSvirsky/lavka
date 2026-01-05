#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace lavka {
class CouriersHandler : public userver::server::handlers::HttpHandlerBase {
    std::string GetCouriers(userver::server::http::HttpRequest& request) const;
    std::string PostCouriers(userver::server::http::HttpRequest& request) const;

    userver::storages::postgres::ClusterPtr pg_cluster_;

   public:
    static constexpr std::string_view kName = "couriers-handler";

    CouriersHandler(const userver::components::ComponentConfig& config,
                    const userver::components::ComponentContext& context);

    std::string HandleRequest(
        userver::server::http::HttpRequest& request,
        userver::server::request::RequestContext& context) const override;
};
}  // namespace lavka