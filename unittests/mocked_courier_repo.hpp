#include <gmock/gmock.h>

#include "domain/repositories/courier_repository.hpp"

class MockedCourierRepository final : public lavka::domain::ICourierRepository {
   public:
    MOCK_METHOD(std::vector<lavka::domain::Courier>, GetAll, (int, int),
                (override));
    MOCK_METHOD(std::vector<lavka::domain::Courier>, CreateAll,
                (const std::vector<lavka::domain::Courier>&), (override));
    MOCK_METHOD(lavka::domain::Courier, GetById, (lavka::domain::Courier::Id),
                (override));
    MOCK_METHOD(
        void, UpdateRatings,
        ((const std::vector<std::tuple<lavka::domain::Courier::Id,
                                       lavka::domain::Courier::Rating>>&)),
        (override));
};