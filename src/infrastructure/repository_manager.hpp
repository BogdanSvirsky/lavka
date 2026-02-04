#include <userver/components/component_base.hpp>
#include <userver/storages/postgres/cluster.hpp>

#include "domain/repositories/courier_repository.hpp"
#include "domain/repositories/order_repository.hpp"

namespace lavka {
class RepositoryManager final : public userver::components::ComponentBase {
    userver::storages::postgres::ClusterPtr pg_cluster_;
    domain::ICourierRepositoryPtr courier_repository;
    domain::IOrderRepositoryPtr order_repository;

   public:
    static constexpr std::string_view kName = "repository-manager";

    RepositoryManager(const userver::components::ComponentConfig& config,
                      const userver::components::ComponentContext& context);

    domain::ICourierRepositoryPtr GetCourierRepository();
    domain::IOrderRepositoryPtr GetOrderRepository();
};
}  // namespace lavka

template <>
inline constexpr auto
    userver::components::kConfigFileMode<lavka::RepositoryManager> =
        ConfigFileMode::kNotRequired;
