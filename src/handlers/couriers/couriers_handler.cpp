#include "couriers_handler.hpp"

#include <userver/server/handlers/exceptions.hpp>

#include "postgres/cpp_to_user_pg_map.hpp"  // IWYU pragma: keep
#include "schemas/openapi.hpp"
#include "utils.hpp"

using namespace userver::server;
using namespace userver::formats;
using namespace chaotic::openapi;

namespace lavka {
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

    auto res = GetPg().Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT id, type, regions, working_hours FROM lavka.couriers\n"
        "ORDER BY id LIMIT $1 OFFSET $2;",
        limit, offset);

    GetCouriersResponse response_dto{.limit = limit, .offset = offset};
    for (CourierDto courier :
         res.AsSetOf<CourierDto>(userver::storages::postgres::kRowTag)) {
        response_dto.couriers.push_back(courier);
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

    CreateCouriersResponse response_dto;

    for (CreateCourierDto courier : request_dto.couriers) {
        auto res = GetPg().Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "INSERT INTO lavka.couriers (type, regions, working_hours) "
            "VALUES ($1, $2, $3)"
            "RETURNING id, type, regions, working_hours",
            courier.courier_type, courier.regions, courier.working_hours);

        CourierDto created_courier =
            res.AsSingleRow<CourierDto>(userver::storages::postgres::kRowTag);

        response_dto.couriers.push_back(created_courier);
    }

    return json::ValueBuilder{response_dto}.ExtractValue();
}
}  // namespace lavka