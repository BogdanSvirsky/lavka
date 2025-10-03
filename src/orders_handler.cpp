#include "orders_handler.hpp"

#include <userver/server/http/http_response.hpp>

namespace lavka {
std::string OrdersHandler::HandleRequest(
    userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext&) const {
    // TODO: add request check with HasArg
    switch (request.GetMethod()) {
        case userver::server::http::HttpMethod::kGet:
            return "kGet";
        case userver::server::http::HttpMethod::kPost:
            return "kPost";
        default:
            break;
    }

    return "Invalid request";
}
};  // namespace lavka