#include "get_courier_handler.hpp"

namespace lavka {
std::string GetCourierHandler::HandleRequest(
    userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext& context) const {
    if (request.HasPathArg(kCourierIdPathArg)) {
        return "GetCourierHandler path arg : " +
               request.GetPathArg(kCourierIdPathArg) + "\n";
    }
    return "GetCourierHandler invalid request\n";
}

}  // namespace lavka