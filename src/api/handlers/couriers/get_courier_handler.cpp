#include "get_courier_handler.hpp"

#include <userver/components/component_context.hpp>
#include <userver/server/http/http_error.hpp>

#include "api/utils.hpp"
#include "infra/repository_manager.hpp"

using namespace userver::formats;
using namespace userver::server;

namespace lavka::api {

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
        std::int64_t courier_id;
        try {
            courier_id = std::stoll(request.GetPathArg(kCourierIdPathArg));
        } catch (std::exception& e) {
            LOG_INFO() << "Catched exception '" << e.what()
                       << "' in GetCourierHandler";
            throw ClientError{};
        }

        domain::Courier courier;
        try {
            courier = couriers_repository_ptr->GetById(courier_id);
        } catch (std::invalid_argument& e) {
            throw handlers::ClientError{};
        }

        return json::ValueBuilder{utils::ToDto(courier)}.ExtractValue();
    } else
        throw ClientError{};
}

}  // namespace lavka::api