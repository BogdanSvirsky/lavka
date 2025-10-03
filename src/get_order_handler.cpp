#include "get_order_handler.hpp"

namespace lavka {
std::string GetOrderHandler::HandleRequest(
    userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext& context) const {
    if (request.HasPathArg(kOrderIdPathArg)) {
        return "GetOrderHandler path arg : " +
               request.GetPathArg(kOrderIdPathArg) + "\n";
    }
    return "GetOrderHandler : no path arg\n";
}

}  // namespace lavka