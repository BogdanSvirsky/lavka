#include "get_courier_handler.hpp"
#include <userver/server/http/http_error.hpp>
#include "cpp_to_user_pg_map.hpp"  // IWYU pragma: keep
#include "schemas/openapi.hpp"

using namespace userver::formats;
using namespace userver::server;
using namespace chaotic::openapi;

namespace lavka {

json::Value GetCourierHandler::HandleRequestJsonThrow(
    const http::HttpRequest& request, const json::Value&,
    request::RequestContext&) const {
    if (request.HasPathArg(kCourierIdPathArg)) {
        std::int64_t courierId;
        try {
            courierId = std::stoll(request.GetPathArg(kCourierIdPathArg));
        } catch (std::exception& e) {
            LOG_INFO() << "Catched exception '" << e.what()
                       << "' in GetCourierHandler";
            throw ClientError{};
        }

        auto res = GetPg().Execute(
            userver::storages::postgres::ClusterHostType::kSlave,
            "SELECT id, type, regions, working_hours FROM lavka.couriers "
            "WHERE id = $1;",
            courierId);

        if (res.IsEmpty()) throw ClientError{};

        CourierDto courier =
            res.AsSingleRow<CourierDto>(userver::storages::postgres::kRowTag);

        return json::ValueBuilder{courier}.ExtractValue();
    } else
        throw ClientError{};
}

}  // namespace lavka