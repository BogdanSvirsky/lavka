#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace lavka {
class GetCourierHandler : public userver::server::handlers::HttpHandlerBase {
    userver::storages::postgres::ClusterPtr pg_cluster_;

   public:
    static constexpr std::string_view kName = "get-courier-handler";
    static constexpr std::string_view kCourierIdPathArg = "courier_id";

    GetCourierHandler(const userver::components::ComponentConfig& config,
                      const userver::components::ComponentContext& context);

    std::string HandleRequest(
        userver::server::http::HttpRequest& request,
        userver::server::request::RequestContext& context) const override;
};
}  // namespace lavka