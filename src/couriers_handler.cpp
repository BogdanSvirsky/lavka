#include "couriers_handler.hpp"
#include <userver/components/component_context.hpp>
#include <userver/storages/postgres/component.hpp>

#include "schemas/openapi.hpp"

using namespace userver::server;
using namespace userver::formats;
using namespace chaotic::openapi;

namespace lavka {
CouriersHandler::CouriersHandler(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      pg_cluster_(
          context.FindComponent<userver::components::Postgres>("lavka-db")
              .GetCluster()) {}

std::string CouriersHandler::HandleRequest(http::HttpRequest& request,
                                           request::RequestContext&) const {
    json::Value response =
        json::ValueBuilder{BadRequestResponse{}}.ExtractValue();

    switch (request.GetMethod()) {
        case http::HttpMethod::kGet:
            return GetCouriers(request);
        case http::HttpMethod::kPost:
            return PostCouriers(request);
        default:
            request.GetHttpResponse().SetStatus(http::HttpStatus::BadRequest);
            return json::ToString(response);
    };
}

std::string CouriersHandler::GetCouriers(
    userver::server::http::HttpRequest& request) const {
    request.GetHttpResponse().SetContentType(
        userver::http::content_type::kApplicationJson);
    json::Value response =
        json::ValueBuilder{BadRequestResponse{}}.ExtractValue();
    std::string limitStr = request.GetArg("limit"),
                offsetStr = request.GetArg("offset");

    int limit = limitStr.empty() ? 1 : std::stoi(limitStr),
        offset = offsetStr.empty() ? 0 : std::stoi(offsetStr);

    if (limit < 1 && offset < 0) {
        request.GetHttpResponse().SetStatus(http::HttpStatus::BadRequest);
    } else {
        response = json::ValueBuilder{GetCouriersResponse{
                                          {{0,
                                            CourierDto::Courier_Type::kBike,
                                            {54},
                                            {"09:00-14:00"}}},
                                          limit,
                                          offset}}
                       .ExtractValue();
    }

    return json::ToString(response);
}

std::string CouriersHandler::PostCouriers(http::HttpRequest& request) const {
    json::Value response =
        json::ValueBuilder{BadRequestResponse{}}.ExtractValue();

    request.GetHttpResponse().SetContentType(
        userver::http::content_type::kApplicationJson);

    CreateCourierRequest request_dto;

    try {
        request_dto =
            json::FromString(request.RequestBody()).As<CreateCourierRequest>();
    } catch (json::Exception&) {
        request.GetHttpResponse().SetStatus(http::HttpStatus::BadRequest);
        return json::ToString(response);
    }

    CreateCouriersResponse response_dto;
    int id = 0;

    for (CreateCourierDto& courier : request_dto.couriers) {
        response_dto.couriers.push_back(CourierDto{
            id++, CourierDto::kCourier_TypeValues[(int)courier.courier_type],
            courier.regions, courier.working_hours});
    }

    response = json::ValueBuilder{response_dto}.ExtractValue();
    return json::ToString(response);
}
}  // namespace lavka