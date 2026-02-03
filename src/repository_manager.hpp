#include <userver/components/component_base.hpp>
#include <userver/storages/postgres/cluster.hpp>

#include "postgres/courier_repository.hpp"

namespace lavka {
class RepositoryManager final : public userver::components::ComponentBase {
    lavka::postgres::CourierRepositoryPtr couriers_repository_ptr;

   public:
    static constexpr std::string_view kName = "repository-manager";

    RepositoryManager(const userver::components::ComponentConfig& config,
                      const userver::components::ComponentContext& context);

    lavka::postgres::CourierRepositoryPtr GetCouriersRepository();
};
}  // namespace lavka

template <>
inline constexpr auto
    userver::components::kConfigFileMode<lavka::RepositoryManager> =
        ConfigFileMode::kNotRequired;
