#include "get_courier_handler.hpp"
#include <userver/server/http/http_error.hpp>
#include "cpp_to_user_pg_map.hpp"  // IWYU pragma: keep
#include "schemas/openapi.hpp"
#include "utils.hpp"

using namespace userver::formats;
using namespace userver::server;
using namespace chaotic::openapi;

namespace lavka {

GetCourierHandler::GetCourierHandler(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      pg_cluster_(lavka::utils::GetDBCluster(context)) {}

std::string GetCourierHandler::HandleRequest(
    userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext&) const {
    request.GetHttpResponse().SetContentType(
        userver::http::content_type::kApplicationJson);

    if (request.HasPathArg(kCourierIdPathArg)) {
        std::int64_t courierId;
        try {
            courierId = std::stoll(request.GetPathArg(kCourierIdPathArg));
        } catch (std::exception& e) {
            LOG_INFO() << "Catched exception '" << e.what()
                       << "' in GetCourierHandler";
            throw ClientError{};
        }

        auto res = pg_cluster_->Execute(
            userver::storages::postgres::ClusterHostType::kSlave,
            "SELECT id, type, regions, working_hours FROM lavka.couriers "
            "WHERE id = $1;",
            courierId);

        if (res.IsEmpty()) throw ClientError{};

        CourierDto courier =
            res.AsSingleRow<CourierDto>(userver::storages::postgres::kRowTag);

        json::Value response = json::ValueBuilder{courier}.ExtractValue();
        return json::ToString(response);
    } else
        throw ClientError{};
}

}  // namespace lavka