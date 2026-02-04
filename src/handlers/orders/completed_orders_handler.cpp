#include "completed_orders_handler.hpp"

#include <schemas/openapi.hpp>
#include <userver/components/component_context.hpp>
#include <userver/formats/serialize/common_containers.hpp>

#include "repository_manager.hpp"

using namespace userver::formats;
using namespace userver::server;
using namespace chaotic::openapi;

namespace lavka {
CompletedOrdersHandler::CompletedOrdersHandler(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : HttpHandlerJsonBase(config, context),
      order_repository(
          context.FindComponent<RepositoryManager>().GetOrderRepository()) {}

json::Value CompletedOrdersHandler::HandleRequestJsonThrow(
    const http::HttpRequest&, const json::Value& request_json,
    request::RequestContext&) const {
    CompleteOrderRequestDto request_dto;
    try {
        request_dto = request_json.As<CompleteOrderRequestDto>();
    } catch (json::Exception& e) {
        LOG_INFO() << "CompletedOrderHandler cathed exception '" << e.what()
                   << "' with message: " << e.GetMessage();
        throw ClientError{};
    }

    std::vector<domain::Order> orders_to_update;
    for (auto complete_info : request_dto.complete_info) {
        domain::Order order;
        try {
            order = order_repository->GetById(complete_info.order_id);
        } catch (std::invalid_argument& e) {
            throw handlers::ClientError{};
        }
        order.completed_courier_id = complete_info.courier_id;
        order.completed_time = complete_info.complete_time;

        orders_to_update.push_back(order);
    }

    std::vector<domain::Order> updated_orders;
    try {
        updated_orders = order_repository->UpdateAll(orders_to_update);
    } catch (std::invalid_argument& e) {
        throw handlers::ClientError{};
    }

    std::vector<OrderDto> response_dto;
    for (domain::Order updated_order : updated_orders)
        response_dto.push_back(
            {updated_order.id, updated_order.weight, updated_order.regions,
             updated_order.delivery_hours, updated_order.cost,
             updated_order.completed_time});

    return json::ValueBuilder{response_dto}.ExtractValue();
}
}  // namespace lavka