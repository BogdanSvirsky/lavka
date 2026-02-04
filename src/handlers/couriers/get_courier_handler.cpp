#include "get_courier_handler.hpp"

#include <userver/components/component_context.hpp>
#include <userver/server/http/http_error.hpp>

#include "repository_manager.hpp"
#include "schemas/openapi.hpp"
#include "utils.hpp"

using namespace userver::formats;
using namespace userver::server;
using namespace chaotic::openapi;

namespace lavka {

GetCourierHandler::GetCourierHandler(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : HttpHandlerJsonBase(config, context),
      couriers_repository_ptr(
          context.FindComponent<RepositoryManager>().GetCourierRepository()) {}

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

        postgres::Courier courier;
        try {
            courier = couriers_repository_ptr->GetById(courierId);
        } catch (std::invalid_argument& e) {
            throw handlers::ClientError{};
        }

        CourierDto courier_dto{courier.id,
                               utils::TranslateCourierType(courier.type),
                               courier.regions, courier.working_hours};
        return json::ValueBuilder{courier_dto}.ExtractValue();
    } else
        throw ClientError{};
}

}  // namespace lavka