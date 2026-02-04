#include <userver/clients/dns/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>

#include "api/handlers/couriers/couriers_handler.hpp"
#include "api/handlers/couriers/get_courier_handler.hpp"
#include "api/handlers/orders/completed_orders_handler.hpp"
#include "api/handlers/orders/get_order_handler.hpp"
#include "api/handlers/orders/orders_handler.hpp"
#include "infrastructure/repository_manager.hpp"

int main(int argc, char* argv[]) {
    auto component_list = userver::components::MinimalServerComponentList()
                              .Append<lavka::api::OrdersHandler>()
                              .Append<lavka::api::CouriersHandler>()
                              .Append<lavka::api::CompletedOrdersHandler>()
                              .Append<lavka::api::GetOrderHandler>()
                              .Append<lavka::api::GetCourierHandler>()
                              .Append<userver::components::TestsuiteSupport>()
                              .Append<userver::components::Postgres>("lavka-db")
                              .Append<userver::clients::dns::Component>()
                              .Append<lavka::RepositoryManager>();

    return userver::utils::DaemonMain(argc, argv, component_list);
}