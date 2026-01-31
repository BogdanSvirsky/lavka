#include "get_order_handler.hpp"

#include "postgres/types.hpp"
#include "schemas/openapi.hpp"

using namespace userver::formats;
using namespace userver::server;

namespace lavka {

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

    auto res = GetPg().Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT id, weight, regions, delivery_hours, cost, completed_time "
        "FROM lavka.orders WHERE id = $1;",
        order_id);

    if (res.IsEmpty()) throw ClientError{};

    chaotic::openapi::OrderDto order = res.AsSingleRow<lavka::postgres::Order>(
        userver::storages::postgres::kRowTag);

    return json::ValueBuilder{order}.ExtractValue();
}

}  // namespace lavka