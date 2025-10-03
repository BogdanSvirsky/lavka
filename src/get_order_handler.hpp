#include <userver/server/handlers/http_handler_base.hpp>

namespace lavka {
class GetOrderHandler : public userver::server::handlers::HttpHandlerBase {
   public:
    static constexpr std::string_view kName = "get-order-handler";
    static constexpr std::string_view kOrderIdPathArg = "order_id";

    using userver::server::handlers::HttpHandlerBase::HttpHandlerBase;

    std::string HandleRequest(
        userver::server::http::HttpRequest& request,
        userver::server::request::RequestContext& context) const override;
};
}  // namespace lavka