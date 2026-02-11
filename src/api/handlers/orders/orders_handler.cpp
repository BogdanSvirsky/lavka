#include "orders_handler.hpp"

#include <userver/components/component_context.hpp>
#include <userver/formats/json.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/server/http/http_response.hpp>
#include <userver/storages/postgres/component.hpp>

#include "api/utils.hpp"
#include "infra/repository_manager.hpp"
#include "schemas/openapi.hpp"

using namespace userver::server;
using namespace userver::formats;

namespace lavka::api {
OrdersHandler::OrdersHandler(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : HttpHandlerJsonBase(config, context),
      order_repository(
          context.FindComponent<RepositoryManager>().GetOrderRepository()) {}

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
    auto [limit, offset] = utils::ExtractPagination(request);

    auto orders = order_repository->GetAll(limit, offset);
    std::vector<OrderDto> response_dto;
    for (domain::Order order : orders) {
        response_dto.push_back(utils::ToDto(order));
    }

    return json::ValueBuilder{response_dto}.ExtractValue();
}

json::Value OrdersHandler::PostOrders(const json::Value& request_json) const {
    CreateOrderRequest request_dto;
    try {
        request_dto = request_json.As<CreateOrderRequest>();
    } catch (json::Exception) {
        throw ClientError{};
    }

    std::vector<domain::Order> orders_to_create;
    for (auto create_order_dto : request_dto.orders)
        orders_to_create.push_back({
            .weight = create_order_dto.weight,
            .regions = create_order_dto.regions,
            .delivery_hours = create_order_dto.delivery_hours,
            .cost = create_order_dto.cost,
        });

    auto created_orders = order_repository->CreateAll(orders_to_create);

    std::vector<OrderDto> response_dto;
    for (domain::Order created_order : created_orders)
        response_dto.push_back(utils::ToDto(created_order));

    return json::ValueBuilder{response_dto}.ExtractValue();
}
};  // namespace lavka::api