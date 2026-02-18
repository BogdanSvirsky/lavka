#include "utils.hpp"

#include <optional>
#include <userver/logging/log.hpp>
#include <userver/server/handlers/exceptions.hpp>

namespace lavka::api::utils {
std::pair<int, int> ExtractPagination(
    const userver::server::http::HttpRequest& request) {
    int limit = 0, offset = -1;
    try {
        std::string limitStr = request.GetArg("limit"),
                    offsetStr = request.GetArg("offset");

        LOG_DEBUG() << "offsetStr: " << offsetStr;

        limit = limitStr.empty() ? 1 : std::stoi(limitStr);
        offset = offsetStr.empty() ? 0 : std::stoi(offsetStr);

        LOG_DEBUG() << "offset: " << offset;
    } catch (std::exception& e) {
        LOG_INFO() << "Catched exception " << e.what()
                   << " in ExtractPagination()";
        throw userver::server::handlers::ClientError();
    }

    if (limit < 1 || offset < 0) throw userver::server::handlers::ClientError();
    return {limit, offset};
}

OrderDto ToDto(const domain::Order& order) {
    return {order.id,
            order.weight,
            order.regions,
            order.delivery_hours,
            order.cost,
            order.completed_time,
            order.rating.has_value()
                ? std::optional<Rating>{Rating(order.rating.value())}
                : std::nullopt};
}

CourierDto ToDto(const domain::Courier& courier) {
    return {courier.id, CourierType(courier.type), courier.regions,
            courier.working_hours,
            courier.rating.has_value() ? std::optional<double>{*courier.rating}
                                       : std::nullopt};
}
}  // namespace lavka::api::utils