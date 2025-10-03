#include "completed_orders_handler.hpp"

namespace lavka {
std::string CompletedOrdersHandler::HandleRequest(
    userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext& context) const {
    return "Request was handled\n";
}
}  // namespace lavka