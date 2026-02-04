#include "couriers_handler.hpp"

#include <userver/components/component.hpp>
#include <userver/server/handlers/exceptions.hpp>

#include "infrastructure/repository_manager.hpp"
#include "schemas/openapi.hpp"
#include "utils.hpp"

using namespace userver::server;
using namespace userver::formats;

namespace lavka::api {
CouriersHandler::CouriersHandler(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : HttpHandlerJsonBase(config, context),
      couriers_repository_ptr(context.FindComponent<lavka::RepositoryManager>()
                                  .GetCourierRepository()) {}

json::Value CouriersHandler::HandleRequestJsonThrow(
    const http::HttpRequest& request, const json::Value& request_json,
    userver::server::request::RequestContext&) const {
    switch (request.GetMethod()) {
        case http::HttpMethod::kGet:
            return GetCouriers(request);
        case http::HttpMethod::kPost:
            return PostCouriers(request_json);
        default:
            throw handlers::ClientError();
    };
}

json::Value CouriersHandler::GetCouriers(
    const userver::server::http::HttpRequest& request) const {
    auto [limit, offset] = lavka::utils::ExtractPagination(request);

    LOG_DEBUG() << limit << " " << offset;

    auto couriers = couriers_repository_ptr->GetAll(limit, offset);

    GetCouriersResponse response_dto{.limit = limit, .offset = offset};
    for (auto courier : couriers) {
        response_dto.couriers.push_back({courier.id, CourierType(courier.type),
                                         courier.regions,
                                         courier.working_hours});
    }

    return json::ValueBuilder{response_dto}.ExtractValue();
}

json::Value CouriersHandler::PostCouriers(
    const json::Value& request_json) const {
    CreateCourierRequest request_dto;

    try {
        request_dto = request_json.As<CreateCourierRequest>();
    } catch (json::Exception& e) {
        LOG_INFO() << "Catched exception " << e.what()
                   << " with message: " << e.GetMessage();
        throw handlers::ClientError();
    }

    std::vector<domain::Courier> couriers_to_create;

    for (CreateCourierDto dto : request_dto.couriers)
        couriers_to_create.push_back(
            {.type = domain::Courier::Type(dto.courier_type),
             .regions = dto.regions,
             .working_hours = dto.working_hours});

    std::vector<domain::Courier> created_couriers;
    try {
        created_couriers =
            couriers_repository_ptr->CreateAll(couriers_to_create);
    } catch (std::invalid_argument& e) {
        LOG_DEBUG() << e.what();
        throw handlers::ClientError{};
    }

    CreateCouriersResponse response_dto;
    for (auto& created_courier : created_couriers)
        response_dto.couriers.push_back(
            {created_courier.id, CourierType(created_courier.type),
             created_courier.regions, created_courier.working_hours});

    return json::ValueBuilder{response_dto}.ExtractValue();
}
}  // namespace lavka::api