#include <gmock/gmock.h>

#include "domain//repositories/order_repository.hpp"

class MockedOrderRepository final : public lavka::domain::IOrderRepository {
   public:
    MOCK_METHOD(lavka::domain::Order, GetById, (lavka::domain::Courier::Id),
                (override));
    MOCK_METHOD(std::vector<lavka::domain::Order>, GetAll, (int, int),
                (override));
    MOCK_METHOD(std::vector<lavka::domain::Order>, CreateAll,
                (const std::vector<lavka::domain::Order>&), (override));
    MOCK_METHOD(std::vector<lavka::domain::Order>, UpdateAll,
                (const std::vector<lavka::domain::Order>&), (override));
    MOCK_METHOD(std::vector<lavka::domain::Order::Rating>, GetLastRatings,
                (lavka::domain::Courier::Id id, int limit), (override));
};