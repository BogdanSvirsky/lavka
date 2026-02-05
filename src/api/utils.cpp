#include "utils.hpp"

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
    return {order.id,      order.weight,
            order.regions, order.delivery_hours,
            order.cost,    order.completed_time};
}
}  // namespace lavka::api::utils