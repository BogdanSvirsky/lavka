#include <gmock/gmock.h>

#include "domain//repositories/order_repository.hpp"

class MockedOrderRepository final : public lavka::domain::IOrderRepository {
   public:
    MOCK_METHOD(lavka::domain::Order, GetById, (std::int64_t), (override));
    MOCK_METHOD(std::vector<lavka::domain::Order>, GetAll, (int, int),
                (override));
    MOCK_METHOD(std::vector<lavka::domain::Order>, CreateAll,
                (std::vector<lavka::domain::Order>), (override));
    MOCK_METHOD(std::vector<lavka::domain::Order>, UpdateAll,
                (std::vector<lavka::domain::Order>), (override));
    MOCK_METHOD(std::vector<lavka::domain::Order::Rating>, GetLastRatings,
                (std::int64_t id, int limit), (override));
};