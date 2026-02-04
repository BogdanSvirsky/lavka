#pragma once
#include <userver/server/http/http_request.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/utils/trivial_map.hpp>
#include <utility>

#include "postgres/courier.hpp"
#include "schemas/openapi.hpp"

namespace lavka::utils {
// Extracts 'limit' and 'offset' query parameterss from request.
// If they are invalid, throws ClientError
std::pair<int, int> ExtractPagination(
    const userver::server::http::HttpRequest& request);

constexpr chaotic::openapi::CourierType TranslateCourierType(
    postgres::Courier::Type type) {
    return chaotic::openapi::kCourierTypeValues[type];
}
constexpr postgres::Courier::Type TranslateCourierType(
    chaotic::openapi::CourierType type) {
    return (postgres::Courier::Type)((int)type);
}

}  // namespace lavka::utils