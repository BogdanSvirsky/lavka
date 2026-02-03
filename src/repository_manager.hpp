#include <userver/components/component_base.hpp>
#include <userver/storages/postgres/cluster.hpp>

#include "postgres/couriers_repository.hpp"

namespace lavka {
class RepositoryManager final : public userver::components::ComponentBase {
    lavka::postgres::CouriersRepositoryPtr couriers_repository_ptr;

   public:
    static constexpr std::string_view kName = "repository-manager";

    RepositoryManager(const userver::components::ComponentConfig& config,
                      const userver::components::ComponentContext& context);

    lavka::postgres::CouriersRepositoryPtr GetCouriersRepository();
};
}  // namespace lavka

template <>
inline constexpr auto
    userver::components::kConfigFileMode<lavka::RepositoryManager> =
        ConfigFileMode::kNotRequired;
