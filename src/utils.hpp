#pragma once
#include <userver/server/http/http_request.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <utility>

namespace lavka::utils {
// Extracts 'limit' and 'offset' query parameterss from request.
// If they are invalid, throws ClientError
std::pair<int, int> ExtractPagination(
    const userver::server::http::HttpRequest& request);

userver::storages::postgres::ClusterPtr GetDBCluster(
    const userver::components::ComponentContext& context);
}  // namespace lavka::utils