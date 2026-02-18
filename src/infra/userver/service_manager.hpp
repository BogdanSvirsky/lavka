#include <userver/components/component_base.hpp>
#include <userver/utils/periodic_task.hpp>

#include "domain/services/courier_service.hpp"

namespace lavka {
class ServiceManager : public userver::components::ComponentBase {
    const domain::CourierServicePtr courier_service;
    userver::utils::PeriodicTask update_ratings_task;

   public:
    static constexpr std::string_view kName = "service-manager";
    ServiceManager(const userver::components::ComponentConfig& config,
                   const userver::components::ComponentContext& context);

    ~ServiceManager();

    domain::CourierServicePtr GetCourierService();
};
}  // namespace lavka

template <>
inline constexpr auto
    userver::components::kConfigFileMode<lavka::ServiceManager> =
        ConfigFileMode::kNotRequired;
