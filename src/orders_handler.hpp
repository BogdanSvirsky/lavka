#include <userver/http/common_headers.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

namespace lavka {
class OrdersHandler : public userver::server::handlers::HttpHandlerBase {
   public:
    static constexpr std::string_view kName = "orders-handler";

    using userver::server::handlers::HttpHandlerBase::HttpHandlerBase;

    std::string HandleRequest(
        userver::server::http::HttpRequest& request,
        userver::server::request::RequestContext& context) const override;
};
}  // namespace lavka