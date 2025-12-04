#include "couriers_handler.hpp"
#include "schemas/openapi.hpp"

using namespace userver::server;
using namespace userver::formats;
using namespace chaotic::openapi;

namespace lavka {
std::string CouriersHandler::HandleRequest(http::HttpRequest& request,
                                           request::RequestContext&) const {
    json::Value response =
        json::ValueBuilder{BadRequestResponse{}}.ExtractValue();

    switch (request.GetMethod()) {
        case http::HttpMethod::kGet: {
            std::string limitStr = request.GetArg("limit"),
                        offsetStr = request.GetArg("offset");

            int limit = limitStr.empty() ? 1 : std::stoi(limitStr),
                offset = offsetStr.empty() ? 0 : std::stoi(offsetStr);

            if (limit < 1 && offset < 0) {
                request.GetHttpResponse().SetStatus(
                    http::HttpStatus::BadRequest);
                break;
            }

            response = json::ValueBuilder{GetCouriersResponse{
                                              {{0,
                                                CourierDto::Courier_Type::kBike,
                                                {54},
                                                {"09:00-14:00"}}},
                                              limit,
                                              offset}}
                           .ExtractValue();
            break;
        }
        case http::HttpMethod::kPost: {
            request.GetHttpResponse().SetContentType(
                userver::http::content_type::kApplicationJson);

            CreateCourierRequest request_dto =
                json::FromString(request.RequestBody())
                    .As<CreateCourierRequest>();

            CreateCouriersResponse response_dto;
            int id = 0;

            for (CreateCourierDto& courier : request_dto.couriers) {
                response_dto.couriers.push_back(CourierDto{
                    id++,
                    CourierDto::kCourier_TypeValues[(int)courier.courier_type],
                    courier.regions, courier.working_hours});
            }

            response = json::ValueBuilder{response_dto}.ExtractValue();
            break;
        }
        default:
            request.GetHttpResponse().SetStatus(http::HttpStatus::BadRequest);
            break;
    };
    return json::ToString(response);
}
}  // namespace lavka