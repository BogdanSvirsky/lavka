#include <userver/components/component_base.hpp>
#include <userver/storages/postgres/cluster.hpp>

#include "postgres/courier_repository.hpp"
#include "postgres/order_repository.hpp"

namespace lavka {
class RepositoryManager final : public userver::components::ComponentBase {
    userver::storages::postgres::ClusterPtr pg_cluster_;
    lavka::postgres::CourierRepositoryPtr courier_repository;
    lavka::postgres::OrderRepositoryPtr order_repository;

   public:
    static constexpr std::string_view kName = "repository-manager";

    RepositoryManager(const userver::components::ComponentConfig& config,
                      const userver::components::ComponentContext& context);

    lavka::postgres::CourierRepositoryPtr GetCourierRepository();
    lavka::postgres::OrderRepositoryPtr GetOrderRepository();
};
}  // namespace lavka

template <>
inline constexpr auto
    userver::components::kConfigFileMode<lavka::RepositoryManager> =
        ConfigFileMode::kNotRequired;
