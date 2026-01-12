#include <userver/http/common_headers.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace lavka {
class OrdersHandler : public userver::server::handlers::HttpHandlerBase {
    userver::storages::postgres::ClusterPtr pg_cluster_;

    std::string GetOrders(
        const userver::server::http::HttpRequest& request) const;
    std::string PostOrders(
        const userver::server::http::HttpRequest& request) const;

   public:
    static constexpr std::string_view kName = "orders-handler";

    OrdersHandler(const userver::components::ComponentConfig& config,
                  const userver::components::ComponentContext& context);

    std::string HandleRequest(
        userver::server::http::HttpRequest& request,
        userver::server::request::RequestContext& context) const override;
};
}  // namespace lavka