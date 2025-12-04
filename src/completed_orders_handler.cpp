#include <completed_orders_handler.hpp>
#include <schemas/openapi.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <vector>

using namespace userver::formats;
using namespace userver::server;
using namespace chaotic::openapi;

namespace lavka {
std::string CompletedOrdersHandler::HandleRequest(
    http::HttpRequest& request, request::RequestContext&) const {
    request.GetHttpResponse().SetContentType(
        userver::http::content_type::kApplicationJson);

    if (request.GetMethod() != http::HttpMethod::kPost) {
        request.GetHttpResponse().SetStatus(http::HttpStatus::BadRequest);
        return json::ToString(
            json::ValueBuilder{BadRequestResponse()}.ExtractValue());
    }

    CompleteOrderRequestDto requestDto =
        json::FromString(request.RequestBody()).As<CompleteOrderRequestDto>();

    std::vector<OrderDto> responseDto;

    for (auto orderInfo : requestDto.complete_info) {
        responseDto.push_back(OrderDto{orderInfo.order_id,
                                       1.5,
                                       54,
                                       {"08:00-09:00"},
                                       100,
                                       orderInfo.complete_time});
    }

    return json::ToString(json::ValueBuilder{responseDto}.ExtractValue());
}
}  // namespace lavka