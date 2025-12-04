#include "get_courier_handler.hpp"
#include "schemas/openapi.hpp"

using namespace userver::formats;
using namespace userver::server;
using namespace chaotic::openapi;

namespace lavka {
std::string GetCourierHandler::HandleRequest(
    userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext&) const {
    json::Value response =
        json::ValueBuilder{BadRequestResponse{}}.ExtractValue();

    if (request.HasPathArg(kCourierIdPathArg)) {
        int courierId = std::stoi(request.GetPathArg(kCourierIdPathArg));
        CourierDto response_dto{
            courierId, CourierDto::Courier_Type::kAuto, {54}, {"10:00-18:00"}};
        response = json::ValueBuilder{response_dto}.ExtractValue();
    } else {
        request.GetHttpResponse().SetStatus(http::HttpStatus::BadRequest);
    }

    return json::ToString(response);
}

}  // namespace lavka