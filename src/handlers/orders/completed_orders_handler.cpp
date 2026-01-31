#include "completed_orders_handler.hpp"

#include <schemas/openapi.hpp>
#include <userver/formats/serialize/common_containers.hpp>

#include "postgres/types.hpp"

using namespace userver::formats;
using namespace userver::server;
using namespace chaotic::openapi;

namespace lavka {

userver::formats::json::Value CompletedOrdersHandler::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest&,
    const userver::formats::json::Value& request_json,
    userver::server::request::RequestContext&) const {
    CompleteOrderRequestDto requestDto;
    try {
        requestDto = request_json.As<CompleteOrderRequestDto>();
    } catch (json::Exception& e) {
        LOG_INFO() << "CompletedOrderHandler cathed exception '" << e.what()
                   << "' with message: " << e.GetMessage();
        throw ClientError{};
    }

    std::vector<OrderDto> result;
    for (auto completion : requestDto.complete_info) {
        auto res = GetPg().Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
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

        if (res.IsEmpty()) throw ClientError{};

        result.push_back(res.AsSingleRow<postgres::Order>(
            userver::storages::postgres::kRowTag));
    }

    return json::ValueBuilder{result}.ExtractValue();
}
}  // namespace lavka