#include "orders_handler.hpp"

#include <userver/components/component_context.hpp>
#include <userver/formats/json.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/server/http/http_response.hpp>
#include <userver/storages/postgres/component.hpp>

#include "postgres/cpp_to_user_pg_map.hpp"  // IWYU pragma: keep
#include "postgres/types.hpp"
#include "schemas/openapi.hpp"
#include "utils.hpp"

using namespace userver::server;
using namespace userver::formats;

namespace lavka {

json::Value OrdersHandler::HandleRequestJsonThrow(
    const http::HttpRequest& request, const json::Value& request_json,
    userver::server::request::RequestContext&) const {
    switch (request.GetMethod()) {
        case http::HttpMethod::kGet:
            return GetOrders(request);
        case http::HttpMethod::kPost:
            return PostOrders(request_json);
        default:
            throw ClientError{};
    }
}

json::Value OrdersHandler::GetOrders(
    const userver::server::http::HttpRequest& request) const {
    auto [limit, offset] = lavka::utils::ExtractPagination(request);

    auto res =
        GetPg()
            .Execute(userver::storages::postgres::ClusterHostType::kSlave,
                     "SELECT id, weight, regions, delivery_hours, cost, "
                     "completed_time FROM "
                     "lavka.orders\n"
                     "ORDER BY id LIMIT $1 OFFSET $2;",
                     limit, offset)
            .AsSetOf<postgres::Order>(userver::storages::postgres::kRowTag);

    std::vector<chaotic::openapi::OrderDto> resultArr;
    for (postgres::Order order : res) {
        resultArr.push_back(order);
    }

    return json::ValueBuilder{resultArr}.ExtractValue();
}

json::Value OrdersHandler::PostOrders(const json::Value& request_json) const {
    chaotic::openapi::CreateOrderRequest request_dto;
    try {
        request_dto = request_json.As<chaotic::openapi::CreateOrderRequest>();
    } catch (json::Exception) {
        throw ClientError{};
    }

    std::vector<chaotic::openapi::OrderDto> result;

    for (chaotic::openapi::CreateOrderDto order : request_dto.orders) {
        postgres::Order created_order =
            GetPg()
                .Execute(
                    userver::storages::postgres::ClusterHostType::kMaster,
                    "INSERT INTO lavka.orders(weight, regions, delivery_hours, "
                    "cost) VALUES ($1, $2, $3, $4) RETURNING id, weight, "
                    "regions, "
                    "delivery_hours, cost, completed_time",
                    order.weight, order.regions, order.delivery_hours,
                    order.cost)
                .AsSingleRow<postgres::Order>(
                    userver::storages::postgres::kRowTag);
        result.push_back(created_order);
    }
    return json::ValueBuilder{result}.ExtractValue();
}
};  // namespace lavka