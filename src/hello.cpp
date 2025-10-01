#include <hello.hpp>

#include <greeting.hpp>
#include <user_type.hpp>

namespace lavka {

std::string Hello::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext&) const {
    const std::string name = request.GetArg("name");
    return SayHelloTo(name, GetUserType(name))  ;
}

}  // namespace lavka