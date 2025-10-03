#include "couriers_handler.hpp"

namespace lavka {
std::string CouriersHandler::HandleRequest(
    userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext& context) const {
    return "Request was handled";
}
}  // namespace lavka