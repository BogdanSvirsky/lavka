#include "infra/userver/common_handler.hpp"

namespace lavka::api {
class OrdersHandler : public CommonHandler {
    userver::formats::json::Value GetOrders(
        const userver::server::http::HttpRequest& request) const;
    userver::formats::json::Value PostOrders(
        const userver::formats::json::Value& request_json) const;

   public:
    static constexpr std::string_view kName = "orders-handler";

    using CommonHandler::CommonHandler;

    userver::formats::json::Value HandleRequestJsonThrow(
        const userver::server::http::HttpRequest& request,
        const userver::formats::json::Value& request_json,
        userver::server::request::RequestContext& context) const override;
};
}  // namespace lavka::api