#pragma once
#include <userver/storages/postgres/io/io_fwd.hpp>
#include <userver/storages/postgres/io/pg_types.hpp>
#include <userver/utils/trivial_map.hpp>
#include "schemas/openapi.hpp"

template <>
struct userver::storages::postgres::io::CppToUserPg<
    chaotic::openapi::CourierType> {
    static constexpr DBTypeName postgres_name = "lavka.courier_type";
    static constexpr USERVER_NAMESPACE::utils::TrivialBiMap enumerators =
        [](auto selector) {
            return selector()
                .Case("AUTO", ::chaotic::openapi::CourierType::kAuto)
                .Case("BIKE", ::chaotic::openapi::CourierType::kBike)
                .Case("FOOT", ::chaotic::openapi::CourierType::kFoot);
        };
};