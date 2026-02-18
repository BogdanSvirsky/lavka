#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/clients/http/component_core.hpp>
#include <userver/clients/http/middlewares/pipeline_component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>

#include "api/handlers/couriers/couriers_handler.hpp"
#include "api/handlers/couriers/get_courier_handler.hpp"
#include "api/handlers/orders/completed_orders_handler.hpp"
#include "api/handlers/orders/get_order_handler.hpp"
#include "api/handlers/orders/orders_handler.hpp"
#include "infra/repository_manager.hpp"
#include "infra/userver/service_manager.hpp"

int main(int argc, char* argv[]) {
    auto component_list =
        userver::components::MinimalServerComponentList()
            .Append<lavka::api::OrdersHandler>()
            .Append<lavka::api::CouriersHandler>()
            .Append<lavka::api::CompletedOrdersHandler>()
            .Append<lavka::api::GetOrderHandler>()
            .Append<lavka::api::GetCourierHandler>()
            .Append<userver::components::HttpClient>()
            .Append<userver::components::HttpClientCore>()
            .Append<userver::clients::http::MiddlewarePipelineComponent>()
            .Append<userver::components::TestsuiteSupport>()
            .Append<userver::server::handlers::TestsControl>()
            .Append<userver::components::Postgres>("lavka-db")
            .Append<userver::clients::dns::Component>()
            .Append<lavka::RepositoryManager>()
            .Append<lavka::ServiceManager>();

    return userver::utils::DaemonMain(argc, argv, component_list);
}