#include "completed_orders_handler.hpp"

#include <schemas/openapi.hpp>
#include <userver/formats/serialize/common_containers.hpp>

#include "api/utils.hpp"

using namespace userver::formats;
using namespace userver::server;

namespace lavka::api {
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
            LOG_DEBUG() << "Can't find order with id="
                        << complete_info.order_id;
            throw handlers::ClientError{};
        }
        order.completed_courier_id = complete_info.courier_id;
        order.completed_time = complete_info.complete_time;
        if (complete_info.rating)
            order.rating = domain::Order::Rating(complete_info.rating.value());

        orders_to_update.push_back(order);
    }

    std::vector<domain::Order> updated_orders;
    try {
        updated_orders = order_repository->UpdateAll(orders_to_update);
    } catch (std::invalid_argument& e) {
        LOG_DEBUG() << "Can't update orders!";
        throw handlers::ClientError{};
    }

    std::vector<OrderDto> response_dto;
    for (domain::Order updated_order : updated_orders)
        response_dto.push_back(utils::ToDto(updated_order));

    return json::ValueBuilder{response_dto}.ExtractValue();
}
}  // namespace lavka::api