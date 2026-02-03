#include <userver/http/common_headers.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/storages/postgres/cluster.hpp>

#include "postgres/order_repository.hpp"

namespace lavka {
class OrdersHandler : public userver::server::handlers::HttpHandlerJsonBase {
    lavka::postgres::OrderRepositoryPtr order_repository;

    userver::formats::json::Value GetOrders(
        const userver::server::http::HttpRequest& request) const;
    userver::formats::json::Value PostOrders(
        const userver::formats::json::Value& request_json) const;

   public:
    static constexpr std::string_view kName = "orders-handler";

    OrdersHandler(const userver::components::ComponentConfig& config,
                  const userver::components::ComponentContext& context);

    userver::formats::json::Value HandleRequestJsonThrow(
        const userver::server::http::HttpRequest& request,
        const userver::formats::json::Value& request_json,
        userver::server::request::RequestContext& context) const override;
};
}  // namespace lavka