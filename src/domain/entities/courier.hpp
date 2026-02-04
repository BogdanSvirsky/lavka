#pragma once
#include <userver/storages/postgres/io/io_fwd.hpp>
#include <userver/storages/postgres/io/pg_types.hpp>
#include <userver/utils/trivial_map.hpp>
#include <vector>

namespace lavka::domain {
struct Courier {
    enum Type { kFoot, kBike, kAuto };

    std::int64_t id{};
    Type type;
    std::vector<std::int32_t> regions;
    std::vector<std::string> working_hours;
};
}  // namespace lavka::domain