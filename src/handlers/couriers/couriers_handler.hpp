#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/storages/postgres/cluster.hpp>

#include "postgres/courier_repository.hpp"

namespace lavka {
class CouriersHandler : public userver::server::handlers::HttpHandlerJsonBase {
    postgres::CourierRepositoryPtr couriers_repository_ptr;

    userver::formats::json::Value GetCouriers(
        const userver::server::http::HttpRequest& request) const;
    userver::formats::json::Value PostCouriers(
        const userver::formats::json::Value& request_json) const;

   public:
    static constexpr std::string_view kName = "couriers-handler";

    CouriersHandler(const userver::components::ComponentConfig& config,
                    const userver::components::ComponentContext& context);

    userver::formats::json::Value HandleRequestJsonThrow(
        const userver::server::http::HttpRequest& request,
        const userver::formats::json::Value& request_json,
        userver::server::request::RequestContext& context) const override;
};
}  // namespace lavka