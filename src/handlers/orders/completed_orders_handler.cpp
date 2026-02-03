#include "completed_orders_handler.hpp"

#include <schemas/openapi.hpp>
#include <userver/formats/serialize/common_containers.hpp>

#include "postgres/order.hpp"

using namespace userver::formats;
using namespace userver::server;
using namespace chaotic::openapi;

namespace lavka {

json::Value CompletedOrdersHandler::HandleRequestJsonThrow(
    const http::HttpRequest&, const json::Value& request_json,
    request::RequestContext&) const {
    CompleteOrderRequestDto requestDto;
    try {
        requestDto = request_json.As<CompleteOrderRequestDto>();
    } catch (json::Exception& e) {
        LOG_INFO() << "CompletedOrderHandler cathed exception '" << e.what()
                   << "' with message: " << e.GetMessage();
        throw ClientError{};
    }

    std::vector<OrderDto> result;
    userver::storages::postgres::Transaction tr =
        GetPg().Begin("orders_completion_transaction",
                      userver::storages::postgres::ClusterHostType::kMaster,
                      userver::storages::postgres::Transaction::RW);
    for (auto completion : requestDto.complete_info) {
        auto res = tr.Execute(
            "UPDATE lavka.orders "
            "SET completed_courier_id = $1, completed_time = $2 "
            "WHERE id = $3 AND completed_courier_id IS NULL AND completed_time "
            "IS NULL "
            "RETURNING id, weight, regions, delivery_hours, cost, "
            "completed_time;",
            completion.courier_id,
            userver::storages::postgres::TimePointWithoutTz(
                completion.complete_time.GetTimePoint()),
            completion.order_id);

        if (res.IsEmpty()) {
            tr.Rollback();
            throw ClientError{};
        }

        result.push_back(res.AsSingleRow<postgres::Order>(
            userver::storages::postgres::kRowTag));
    }
    tr.Commit();

    return json::ValueBuilder{result}.ExtractValue();
}
}  // namespace lavka