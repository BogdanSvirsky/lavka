#include <user_type.hpp>
#include <userver/engine/mutex.hpp>
#include <vector>

namespace lavka {
UserType GetUserType(std::string username) {
    static std::vector<std::string> already_meet;
    static userver::engine::Mutex mutex;

    mutex.lock();
    for (std::string& meeted_name : already_meet) {
        if (meeted_name == username) {
            mutex.unlock();
            return UserType::kKnown;
        }
    }
    already_meet.push_back(username);
    mutex.unlock();
    return UserType::kFirstTime;
}
}  // namespace lavka