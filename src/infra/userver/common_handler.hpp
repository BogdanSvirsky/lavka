#pragma once
#include <userver/server/handlers/http_handler_json_base.hpp>

#include "domain/repositories/courier_repository.hpp"
#include "domain/repositories/order_repository.hpp"

namespace lavka::api {
class CommonHandler : public userver::server::handlers::HttpHandlerJsonBase {
   public:
    CommonHandler(const userver::components::ComponentConfig& config,
                  const userver::components::ComponentContext& context);

   protected:
    domain::ICourierRepositoryPtr courier_repository;
    domain::IOrderRepositoryPtr order_repository;
};
}  // namespace lavka::api