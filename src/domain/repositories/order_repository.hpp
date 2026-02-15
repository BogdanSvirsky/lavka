#pragma once

#include <memory>

#include "domain/entities/order.hpp"

namespace lavka::domain {
class IOrderRepository {
   public:
    // Throws an std::invalid_argument when id is invalid
    virtual Order GetById(std::int64_t id) = 0;

    virtual std::vector<Order> GetAll(int limit, int offset) = 0;

    // Throws an std::invalid_argument when can't create one of the orders
    virtual std::vector<Order> CreateAll(
        std::vector<Order> orders_to_create) = 0;

    // Throws an std::invalid_argument when can't update one of the orders
    virtual std::vector<Order> UpdateAll(
        std::vector<Order> orders_to_update) = 0;

    // Get ratings of the last orders that were complited by courier with id
    // `courier_id` sorted in new to old order
    virtual std::vector<Order::Rating> GetLastRatings(std::int64_t courier_id,
                                                      int limit) = 0;
};

using IOrderRepositoryPtr = std::shared_ptr<IOrderRepository>;
}  // namespace lavka::domain