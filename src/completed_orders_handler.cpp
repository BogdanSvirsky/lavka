#include <completed_orders_handler.hpp>
#include <schemas/openapi.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include "db/types.hpp"
#include "utils.hpp"

using namespace userver::formats;
using namespace userver::server;
using namespace chaotic::openapi;

namespace lavka {
CompletedOrdersHandler::CompletedOrdersHandler(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      pg_cluster_(lavka::utils::GetDBCluster(context)) {}

std::string CompletedOrdersHandler::HandleRequest(
    http::HttpRequest& request, request::RequestContext&) const {
    request.GetHttpResponse().SetContentType(
        userver::http::content_type::kApplicationJson);

    CompleteOrderRequestDto requestDto;
    try {
        requestDto = json::FromString(request.RequestBody())
                         .As<CompleteOrderRequestDto>();
    } catch (json::Exception& e) {
        LOG_INFO() << "CompletedOrderHandler cathed exception '" << e.what()
                   << "' with message: " << e.GetMessage();
        throw ClientError{};
    }

    std::vector<OrderDto> result;
    for (auto completion : requestDto.complete_info) {
        auto res = pg_cluster_->Execute(
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

        result.push_back(
            res.AsSingleRow<db::Order>(userver::storages::postgres::kRowTag));
    }

    json::Value response_json = json::ValueBuilder{result}.ExtractValue();
    return json::ToString(response_json);
}
}  // namespace lavka