#include "service_manager.hpp"

#include <userver/components/component.hpp>
#include <userver/components/component_base.hpp>
#include <userver/formats/json.hpp>
#include <userver/testsuite/testpoint.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/yaml_config/merge_schemas.hpp>

#include "infra/repository_manager.hpp"

namespace lavka {
ServiceManager::ServiceManager(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : ComponentBase(config, context),
      courier_service(std::make_shared<domain::CourierService>(
          context.FindComponent<RepositoryManager>().GetOrderRepository(),
          context.FindComponent<RepositoryManager>().GetCourierRepository(),
          config["number-of-ratings"].As<size_t>(10),
          config["part-size"].As<size_t>(3))),
      update_ratings_task(
          "update-ratings",
          userver::utils::PeriodicTask::Settings{std::chrono::days{1}},
          [this]() { this->courier_service->UpdateRatings(); }) {
    update_ratings_task.RegisterInTestsuite(
        context.FindComponent<userver::components::TestsuiteSupport>()
            .GetPeriodicTaskControl());
}

ServiceManager::~ServiceManager() { update_ratings_task.Stop(); }

domain::CourierServicePtr ServiceManager::GetCourierService() {
    return courier_service;
}

userver::yaml_config::Schema ServiceManager::GetStaticConfigSchema() {
    return userver::yaml_config::MergeSchemas<
        userver::components::ComponentBase>(R"(
type: object
description: manager of domain services
additionalProperties: false
properties:
    number-of-ratings:
        type: integer
        description: count of order ratings for courier rating calculation
    part-size:
        type: integer
        description: count of courier ratings that will be updated per iteration
)");
}
}  // namespace lavka