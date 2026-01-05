#include <userver/clients/dns/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/testsuite/testsuite_support.hpp>

#include <userver/utils/daemon_run.hpp>
#include "completed_orders_handler.hpp"
#include "couriers_handler.hpp"
#include "get_courier_handler.hpp"
#include "get_order_handler.hpp"
#include "orders_handler.hpp"

int main(int argc, char* argv[]) {
    auto component_list = userver::components::MinimalServerComponentList()
                              .Append<lavka::OrdersHandler>()
                              .Append<lavka::CouriersHandler>()
                              .Append<lavka::CompletedOrdersHandler>()
                              .Append<lavka::GetOrderHandler>()
                              .Append<lavka::GetCourierHandler>()
                              .Append<userver::components::TestsuiteSupport>()
                              .Append<userver::components::Postgres>("lavka-db")
                              .Append<userver::clients::dns::Component>();

    return userver::utils::DaemonMain(argc, argv, component_list);
}