#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include "common_handler.hpp"

namespace lavka {

class CouriersHandler : public lavka::CommonHandler {
    userver::formats::json::Value GetCouriers(
        const userver::server::http::HttpRequest& request) const;
    userver::formats::json::Value PostCouriers(
        const userver::formats::json::Value& request_json) const;

   public:
    static constexpr std::string_view kName = "couriers-handler";

    using CommonHandler::CommonHandler;

    userver::formats::json::Value HandleRequestJsonThrow(
        const userver::server::http::HttpRequest& request,
        const userver::formats::json::Value& request_json,
        userver::server::request::RequestContext& context) const override;
};
}  // namespace lavka