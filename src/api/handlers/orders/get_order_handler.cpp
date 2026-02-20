#include "get_order_handler.hpp"

#include "api/utils.hpp"
#include "schemas/openapi.hpp"

using namespace userver::formats;
using namespace userver::server;

namespace lavka::api {
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

    domain::Order order;
    try {
        order = order_repository->GetById(order_id);
    } catch (std::invalid_argument& e) {
        throw handlers::ClientError{};
    }
    OrderDto response_dto = utils::ToDto(order);

    return json::ValueBuilder{response_dto}.ExtractValue();
}
}  // namespace lavka::api