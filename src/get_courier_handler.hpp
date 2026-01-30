#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include "common_handler.hpp"

namespace lavka {
class GetCourierHandler : public lavka::CommonHandler {
   public:
    static constexpr std::string_view kName = "get-courier-handler";
    static constexpr std::string_view kCourierIdPathArg = "courier_id";

    using CommonHandler::CommonHandler;

    userver::formats::json::Value HandleRequestJsonThrow(
        const userver::server::http::HttpRequest& request,
        const userver::formats::json::Value& request_json,
        userver::server::request::RequestContext& context) const override;
};
}  // namespace lavka