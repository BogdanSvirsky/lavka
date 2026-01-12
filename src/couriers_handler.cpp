#include "couriers_handler.hpp"
#include <userver/server/handlers/exceptions.hpp>
#include "cpp_to_user_pg_map.hpp"  // IWYU pragma: keep

#include "schemas/openapi.hpp"
#include "utils.hpp"

using namespace userver::server;
using namespace userver::formats;
using namespace chaotic::openapi;

namespace lavka {
CouriersHandler::CouriersHandler(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      pg_cluster_(lavka::utils::GetDBCluster(context)) {}

std::string CouriersHandler::HandleRequest(http::HttpRequest& request,
                                           request::RequestContext&) const {
    request.GetHttpResponse().SetContentType(
        userver::http::content_type::kApplicationJson);

    switch (request.GetMethod()) {
        case http::HttpMethod::kGet:
            return GetCouriers(request);
        case http::HttpMethod::kPost:
            return PostCouriers(request);
        default:
            throw handlers::ClientError();
    };
}

std::string CouriersHandler::GetCouriers(
    userver::server::http::HttpRequest& request) const {
    auto [limit, offset] = lavka::utils::ExtractPagination(request);

    LOG_DEBUG() << limit << " " << offset;

    auto res = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT id, type, regions, working_hours FROM lavka.couriers\n"
        "ORDER BY id LIMIT $1 OFFSET $2;",
        limit, offset);

    GetCouriersResponse response_dto{.limit = limit, .offset = offset};
    for (CourierDto courier :
         res.AsSetOf<CourierDto>(userver::storages::postgres::kRowTag)) {
        response_dto.couriers.push_back(courier);
    }

    json::Value response = json::ValueBuilder{response_dto}.ExtractValue();
    return json::ToString(response);
}

std::string CouriersHandler::PostCouriers(http::HttpRequest& request) const {
    CreateCourierRequest request_dto;

    try {
        request_dto =
            json::FromString(request.RequestBody()).As<CreateCourierRequest>();
    } catch (json::Exception& e) {
        LOG_INFO() << "Catched exception " << e.what()
                   << " with message: " << e.GetMessage();
        throw handlers::ClientError();
    }

    CreateCouriersResponse response_dto;

    for (CreateCourierDto courier : request_dto.couriers) {
        auto res = pg_cluster_->Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "INSERT INTO lavka.couriers (type, regions, working_hours) "
            "VALUES ($1, $2, $3)"
            "RETURNING id, type, regions, working_hours",
            courier.courier_type, courier.regions, courier.working_hours);

        CourierDto created_courier =
            res.AsSingleRow<CourierDto>(userver::storages::postgres::kRowTag);

        response_dto.couriers.push_back(created_courier);
    }

    json::Value response = json::ValueBuilder{response_dto}.ExtractValue();
    return json::ToString(response);
}
}  // namespace lavka