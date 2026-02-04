#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/storages/postgres/cluster.hpp>

#include "domain/repositories/order_repository.hpp"

namespace lavka {
class CompletedOrdersHandler
    : public userver::server::handlers::HttpHandlerJsonBase {
    domain::IOrderRepositoryPtr order_repository;

   public:
    static constexpr std::string_view kName = "completed-orders-handler";

    CompletedOrdersHandler(
        const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& context);

    userver::formats::json::Value HandleRequestJsonThrow(
        const userver::server::http::HttpRequest& request,
        const userver::formats::json::Value& request_json,
        userver::server::request::RequestContext& context) const override;
};
}  // namespace lavka