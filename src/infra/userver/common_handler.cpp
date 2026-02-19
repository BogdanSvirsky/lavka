#include "common_handler.hpp"

#include <userver/components/component_context.hpp>

#include "repository_manager.hpp"

namespace lavka::api {
CommonHandler::CommonHandler(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : HttpHandlerJsonBase(config, context),
      courier_repository(
          context.FindComponent<RepositoryManager>().GetCourierRepository()),
      order_repository(
          context.FindComponent<RepositoryManager>().GetOrderRepository()) {}
}  // namespace lavka::api