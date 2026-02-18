#include "service_manager.hpp"

#include <userver/components/component.hpp>
#include <userver/formats/json.hpp>
#include <userver/testsuite/testpoint.hpp>
#include <userver/testsuite/testsuite_support.hpp>

#include "infra/repository_manager.hpp"

namespace lavka {
ServiceManager::ServiceManager(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : ComponentBase(config, context),
      courier_service(std::make_shared<domain::CourierService>(
          context.FindComponent<RepositoryManager>().GetOrderRepository(),
          context.FindComponent<RepositoryManager>().GetCourierRepository(), 10,
          3)),
      update_ratings_task(
          "update-ratings",
          userver::utils::PeriodicTask::Settings{std::chrono::days{1}},
          [this]() {
              LOG_DEBUG() << "TASK STARTED!";
              this->courier_service->UpdateRatings();
              LOG_DEBUG() << "TASK ENDED!";
          }) {
    update_ratings_task.RegisterInTestsuite(
        context.FindComponent<userver::components::TestsuiteSupport>()
            .GetPeriodicTaskControl());
}

ServiceManager::~ServiceManager() { update_ratings_task.Stop(); }

domain::CourierServicePtr ServiceManager::GetCourierService() {
    return courier_service;
}
}  // namespace lavka