#pragma once
#include <userver/storages/postgres/io/io_fwd.hpp>
#include <userver/storages/postgres/io/pg_types.hpp>
#include <userver/utils/trivial_map.hpp>
#include <vector>

namespace lavka::postgres {
struct Courier {
    enum Type { kFoot, kBike, kAuto };

    std::int64_t id{};
    Type type;
    std::vector<std::int32_t> regions;
    std::vector<std::string> working_hours;
    std::optional<double> rating{};
};
}  // namespace lavka::postgres

template <>
struct userver::storages::postgres::io::CppToUserPg<
    lavka::postgres::Courier::Type> {
    static constexpr DBTypeName postgres_name = "lavka.courier_type";
    static constexpr USERVER_NAMESPACE::utils::TrivialBiMap enumerators =
        [](auto selector) {
            return selector()
                .Case("AUTO", lavka::postgres::Courier::Type::kAuto)
                .Case("BIKE", lavka::postgres::Courier::Type::kBike)
                .Case("FOOT", lavka::postgres::Courier::Type::kFoot);
        };
};