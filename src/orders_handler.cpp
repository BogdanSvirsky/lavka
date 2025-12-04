#include "orders_handler.hpp"
#include "schemas/openapi.hpp"

#include <userver/formats/json.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/server/http/http_response.hpp>

using namespace userver::server;
using namespace userver::formats;

namespace lavka {
std::string OrdersHandler::HandleRequest(http::HttpRequest& request,
                                         request::RequestContext&) const {
    json::Value response_json =
        json::ValueBuilder{chaotic::openapi::BadRequestResponse()}
            .ExtractValue();

    switch (request.GetMethod()) {
        case http::HttpMethod::kGet: {
            chaotic::openapi::OrderDto orderDto{
                0, 0, 0, {"08:00-09:00"}, 0, std::nullopt};
            std::vector<chaotic::openapi::OrderDto> resultArr(1);
            resultArr.push_back(orderDto);
            response_json = json::ValueBuilder{resultArr}.ExtractValue();
            break;
        }
        case http::HttpMethod::kPost: {
            request.GetHttpResponse().SetContentType(
                userver::http::content_type::kApplicationJson);
            json::Value request_json = json::FromString(request.RequestBody());

            auto request_dom =
                request_json.As<chaotic::openapi::CreateOrderRequest>();
            unsigned int id = 0;
            std::vector<chaotic::openapi::OrderDto> created_orders;
            for (auto order : request_dom.orders) {
                created_orders.push_back({id++, order.weight, order.regions,
                                          order.delivery_hours, order.cost});
            }
            response_json = json::ValueBuilder{created_orders}.ExtractValue();
            break;
        }
        default:
            request.GetHttpResponse().SetStatus(http::HttpStatus::BadRequest);
            break;
    }

    return json::ToString(response_json);
}
};  // namespace lavka