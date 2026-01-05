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

    switch (request.GetMethod()) {      // TODO: split into separated methods
        case http::HttpMethod::kGet: {  // TODO: add a limit & offset
            chaotic::openapi::OrderDto orderDto{
                0, 0, 0, {"08:00-09:00"}, 0, std::nullopt};
            std::vector<chaotic::openapi::OrderDto> resultArr;
            resultArr.push_back(orderDto);
            response_json = json::ValueBuilder{resultArr}.ExtractValue();
            break;
        }
        case http::HttpMethod::kPost: {
            chaotic::openapi::CreateOrderRequest request_dto;
            try {
                request_dto = json::FromString(request.RequestBody())
                                  .As<chaotic::openapi::CreateOrderRequest>();
            } catch (json::Exception) {
                request.GetHttpResponse().SetStatus(
                    http::HttpStatus::kBadRequest);
                return json::ToString(response_json);
            }

            unsigned int id = 0;
            std::vector<chaotic::openapi::OrderDto> created_orders;
            for (auto order : request_dto.orders) {
                created_orders.push_back({id++, order.weight, order.regions,
                                          order.delivery_hours, order.cost});
            }

            request.GetHttpResponse().SetContentType(
                userver::http::content_type::kApplicationJson);
            response_json = json::ValueBuilder{created_orders}.ExtractValue();
            break;
        }
        default:
            request.GetHttpResponse().SetStatus(http::HttpStatus::kBadRequest);
            break;
    }

    return json::ToString(response_json);
}
};  // namespace lavka