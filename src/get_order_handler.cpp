#include "get_order_handler.hpp"
#include "db/types.hpp"
#include "schemas/openapi.hpp"
#include "utils.hpp"

using namespace userver::formats;
using namespace userver::server;

namespace lavka {
GetOrderHandler::GetOrderHandler(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      pg_cluster_(lavka::utils::GetDBCluster(context)) {}
std::string GetOrderHandler::HandleRequest(http::HttpRequest& request,
                                           request::RequestContext&) const {
    request.GetHttpResponse().SetContentType(
        userver::http::content_type::kApplicationJson);

    int order_id;
    if (request.HasPathArg(kOrderIdPathArg)) {
        try {
            order_id = std::stoi(request.GetPathArg(kOrderIdPathArg));
        } catch (...) {
            throw ClientError{};
        }
    } else
        throw ClientError{};

    auto res = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT id, weight, regions, delivery_hours, cost, completed_time "
        "FROM lavka.orders WHERE id = $1;",
        order_id);

    if (res.IsEmpty()) throw ClientError{};

    chaotic::openapi::OrderDto order =
        res.AsSingleRow<lavka::db::Order>(userver::storages::postgres::kRowTag);

    json::Value response = json::ValueBuilder{order}.ExtractValue();
    return json::ToString(response);
}

}  // namespace lavka