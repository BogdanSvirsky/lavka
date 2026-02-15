#include <gmock/gmock.h>

#include <userver/utest/utest.hpp>

#include "domain/entities/order.hpp"
#include "domain/services/courier_service.hpp"

using namespace lavka::domain;
using namespace ::testing;

class MockedOrderRepository final : public IOrderRepository {
   public:
    MOCK_METHOD(Order, GetById, (std::int64_t), (override));
    MOCK_METHOD(std::vector<Order>, GetAll, (int, int), (override));
    MOCK_METHOD(std::vector<Order>, CreateAll, (std::vector<Order>),
                (override));
    MOCK_METHOD(std::vector<Order>, UpdateAll, (std::vector<Order>),
                (override));
    MOCK_METHOD(std::vector<Order::Rating>, GetLastRatings,
                (std::int64_t id, int limit), (override));
};

class MockedCourierRepository final : public ICourierRepository {
   public:
    MOCK_METHOD(std::vector<Courier>, GetAll, (int, int), (override));
    MOCK_METHOD(std::vector<Courier>, CreateAll, (std::vector<Courier>),
                (override));
    MOCK_METHOD(Courier, GetById, (std::int64_t), (override));
    MOCK_METHOD(
        void, UpdateRatings,
        ((const std::vector<std::tuple<Courier::Id, Courier::Rating>>&)),
        (override));
};

UTEST(CourierService, CalculateRating) {
    auto order_repo = std::make_shared<MockedOrderRepository>();
    CourierService service(order_repo,
                           std::make_shared<MockedCourierRepository>(), 10, 0);

    EXPECT_CALL(*order_repo, GetLastRatings(0, 10))
        .WillOnce(Return(std::vector<Order::Rating>(10, Order::k5)))
        .WillRepeatedly(Return(std::vector<Order::Rating>{}));

    EXPECT_EQ(service.CalculateRating(0), 5.);
    EXPECT_EQ(service.CalculateRating(0), std::nullopt);

    EXPECT_CALL(*order_repo, GetLastRatings(1, 10))
        .WillOnce(Return(std::vector<Order::Rating>{
            Order::k5, Order::k5, Order::k2, Order::k4, Order::k4, Order::k4,
            Order::k5, Order::k3, Order::k3, Order::k2}))
        .WillRepeatedly(Return(std::vector<Order::Rating>{}));

    EXPECT_EQ(*service.CalculateRating(1), 4);

    EXPECT_CALL(*order_repo, GetLastRatings(2, 10))
        .WillOnce(Return(std::vector<Order::Rating>{
            Order::k1, Order::k3, Order::k5, Order::k2, Order::k4, Order::k1,
            Order::k4, Order::k2, Order::k5, Order::k3}))
        .WillRepeatedly(Return(std::vector<Order::Rating>{}));

    EXPECT_TRUE(abs(*service.CalculateRating(2) - 2.818181818181) < 1e-8);
}

UTEST(CourierService, UpdateRatings) {
    auto order_repo = std::make_shared<MockedOrderRepository>();
    auto courier_repo = std::make_shared<MockedCourierRepository>();
    CourierService service(order_repo, courier_repo, 10, 3);

    EXPECT_CALL(*order_repo, GetLastRatings(_, 10))
        .WillRepeatedly(Return(std::vector<Order::Rating>{}));
    EXPECT_CALL(*order_repo, GetLastRatings(0, 10))
        .WillOnce(Return(std::vector<Order::Rating>(10, Order::k5)))
        .WillRepeatedly(Return(std::vector<Order::Rating>{}));
    EXPECT_CALL(*order_repo, GetLastRatings(1, 10))
        .WillOnce(Return(std::vector<Order::Rating>{
            Order::k5, Order::k5, Order::k2, Order::k4, Order::k4, Order::k4,
            Order::k5, Order::k3, Order::k3, Order::k2}))
        .WillRepeatedly(Return(std::vector<Order::Rating>{}));
    EXPECT_CALL(*order_repo, GetLastRatings(3, 10))
        .WillOnce(Return(std::vector<Order::Rating>{
            Order::k1, Order::k3, Order::k5, Order::k2, Order::k4, Order::k1,
            Order::k4, Order::k2, Order::k5, Order::k3}))
        .WillRepeatedly(Return(std::vector<Order::Rating>{}));

    std::vector<std::tuple<Courier::Id, Courier::Rating>> updated_ratings1,
        updated_ratings2;
    EXPECT_CALL(*courier_repo, UpdateRatings(_))
        .WillOnce(SaveArg<0>(&updated_ratings1))
        .WillOnce(SaveArg<0>(&updated_ratings2));

    EXPECT_CALL(*courier_repo, GetAll(3, _))
        .WillRepeatedly(Return(std::vector<Courier>{}));
    EXPECT_CALL(*courier_repo, GetAll(3, 0))
        .WillOnce(
            Return(std::vector<Courier>{{.id = 0}, {.id = 1}, {.id = 2}}));
    EXPECT_CALL(*courier_repo, GetAll(3, 3))
        .WillOnce(Return(std::vector<Courier>{{.id = 3}}));

    service.UpdateRatings();

    EXPECT_EQ(updated_ratings1,
              (std::vector<std::tuple<Courier::Id, Courier::Rating>>{
                  {0, 5}, {1, 4}, {2, std::nullopt}}));

    EXPECT_EQ(std::get<1>(updated_ratings1[1]), 4);

    EXPECT_TRUE(updated_ratings2.size() == 1);
    EXPECT_TRUE((*std::get<1>(updated_ratings2[0]) - 2.818181818181) < 1e-8);
}
