#pragma once
#include <vector>

#include "domain/entities/courier.hpp"

namespace lavka::domain {
class ICourierRepository {
   public:
    virtual std::vector<Courier> GetAll(int limit, int offset) = 0;

    // Throws an std::invalid_argument when can't create at least one argument
    virtual std::vector<Courier> CreateAll(
        std::vector<Courier> couriers_to_create) = 0;

    // Throws an std::invalid_argument when can't find courier
    virtual Courier GetById(std::int64_t id) = 0;
};

using ICourierRepositoryPtr = std::shared_ptr<ICourierRepository>;
}  // namespace lavka::domain
