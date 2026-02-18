#pragma once
#include <userver/server/http/http_request.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/utils/trivial_map.hpp>
#include <utility>

#include "domain/entities/courier.hpp"
#include "domain/entities/order.hpp"
#include "schemas/openapi.hpp"

namespace lavka::api::utils {
// Extracts 'limit' and 'offset' query parameterss from request.
// If they are invalid, throws ClientError
std::pair<int, int> ExtractPagination(
    const userver::server::http::HttpRequest& request);

OrderDto ToDto(const domain::Order& order);

CourierDto ToDto(const domain::Courier& courier);
}  // namespace lavka::api::utils