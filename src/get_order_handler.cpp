#include "get_order_handler.hpp"
#include "schemas/openapi.hpp"

using namespace userver::formats;
using namespace userver::server;

namespace lavka {
std::string GetOrderHandler::HandleRequest(http::HttpRequest& request,
                                           request::RequestContext&) const {
    request.GetHttpResponse().SetContentType(
        userver::http::content_type::kApplicationJson);
    int order_id;
    if (request.HasPathArg(0)) {
        try {
            order_id = std::stoi(request.GetPathArg(0));
        } catch (...) {
            request.GetHttpResponse().SetStatus(http::HttpStatus::BadRequest);
            json::Value response =
                json::ValueBuilder{chaotic::openapi::BadRequestResponse{}}
                    .ExtractValue();
            return json::ToString(response);
        }
    } else {
        request.GetHttpResponse().SetStatus(http::HttpStatus::BadRequest);
        json::Value response =
            json::ValueBuilder{chaotic::openapi::BadRequestResponse{}}
                .ExtractValue();
        return json::ToString(response);
    }

    chaotic::openapi::OrderDto orderDto{order_id,        0, 0,
                                        {"08:00-09:00"}, 0, std::nullopt};
    json::Value response = json::ValueBuilder{orderDto}.ExtractValue();
    return json::ToString(response);
}

}  // namespace lavka