#include "get_order_handler.hpp"

#include <userver/components/component_context.hpp>

#include "postgres/order.hpp"
#include "repository_manager.hpp"
#include "schemas/openapi.hpp"

using namespace userver::formats;
using namespace userver::server;

namespace lavka {
GetOrderHandler::GetOrderHandler(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : HttpHandlerJsonBase(config, context),
      order_repository(
          context.FindComponent<RepositoryManager>().GetOrderRepository()) {}

json::Value GetOrderHandler::HandleRequestJsonThrow(
    const http::HttpRequest& request, const json::Value&,
    request::RequestContext&) const {
    int order_id;
    if (request.HasPathArg(kOrderIdPathArg)) {
        try {
            order_id = std::stoi(request.GetPathArg(kOrderIdPathArg));
        } catch (...) {
            throw ClientError{};
        }
    } else
        throw ClientError{};

    postgres::Order order;
    try {
        order = order_repository->GetById(order_id);
    } catch (std::invalid_argument& e) {
        throw handlers::ClientError{};
    }

    return json::ValueBuilder{chaotic::openapi::OrderDto(order)}.ExtractValue();
}

}  // namespace lavka