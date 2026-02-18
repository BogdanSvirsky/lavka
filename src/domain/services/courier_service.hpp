#include <cstdint>

#include "domain/repositories/courier_repository.hpp"
#include "domain/repositories/order_repository.hpp"

namespace lavka::domain {
class CourierService {
    const size_t kNumberOfRatings;
    const double kWeightsSum = ((1 + kNumberOfRatings) * kNumberOfRatings) / 2.;
    const size_t kPartSize;
    IOrderRepositoryPtr order_repository;
    ICourierRepositoryPtr courier_repository;

   public:
    // `number_of_ratings` - number of recent order ratings that will be used
    // for courier rating calculation
    // `part_size` - count of couriers that will be updated per iteration in
    // UpdateRatings
    CourierService(IOrderRepositoryPtr order_repository,
                   ICourierRepositoryPtr courier_repository,
                   size_t number_of_ratings, size_t part_size);

    // Calculates courier rating if it has at least `kOrdersCount` complited
    // orders with rating
    std::optional<double> CalculateRating(std::int64_t courier_id);

    void UpdateRatings();
};

using CourierServicePtr = std::shared_ptr<CourierService>;
}  // namespace lavka::domain