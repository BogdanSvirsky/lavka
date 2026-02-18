#include "courier_service.hpp"

namespace lavka::domain {
CourierService::CourierService(IOrderRepositoryPtr order_repository,
                               ICourierRepositoryPtr courier_repository,
                               size_t number_of_ratings, size_t part_size)
    : kNumberOfRatings(number_of_ratings),
      kPartSize(part_size),
      order_repository(order_repository),
      courier_repository(courier_repository) {}

Courier::Rating CourierService::CalculateRating(std::int64_t courier_id) {
    std::vector<Order::Rating> last_ratings =
        order_repository->GetLastRatings(courier_id, kNumberOfRatings);
    if (last_ratings.size() != kNumberOfRatings) return std::nullopt;

    unsigned long long weighted_sum = 0;
    for (size_t i = 0; i < kNumberOfRatings; i++)
        weighted_sum += (kNumberOfRatings - i) * last_ratings[i];

    return weighted_sum / kWeightsSum;
}

void CourierService::UpdateRatings() {
    for (size_t current_position = 0;; current_position += kPartSize) {
        std::vector<Courier> couriers_to_update =
            courier_repository->GetAll(kPartSize, current_position);
        if (couriers_to_update.empty()) break;

        std::vector<std::tuple<Courier::Id, Courier::Rating>> updated_ratings;
        for (auto& courier : couriers_to_update)
            updated_ratings.push_back(
                std::make_tuple(courier.id, CalculateRating(courier.id)));

        courier_repository->UpdateRatings(updated_ratings);
    }
}
}  // namespace lavka::domain