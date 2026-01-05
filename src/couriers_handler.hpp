#include <userver/server/handlers/http_handler_base.hpp>

namespace lavka {
class CouriersHandler : public userver::server::handlers::HttpHandlerBase {

    std::string GetCouriers(userver::server::http::HttpRequest& request) const;
    std::string PostCouriers(userver::server::http::HttpRequest& request) const;

   public:
    static constexpr std::string_view kName = "couriers-handler";

    using userver::server::handlers::HttpHandlerBase::HttpHandlerBase;

    std::string HandleRequest(
        userver::server::http::HttpRequest& request,
        userver::server::request::RequestContext& context) const override;
};
}  // namespace lavka