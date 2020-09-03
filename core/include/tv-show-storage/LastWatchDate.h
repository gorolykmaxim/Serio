#ifndef SERIO_LASTWATCHDATE_H
#define SERIO_LASTWATCHDATE_H

#include <chrono>
#include <string>

namespace serio::core {

class LastWatchDate {
public:
    explicit LastWatchDate(std::chrono::system_clock::time_point date);
    explicit LastWatchDate(long long timeSinceEpoch);
    [[nodiscard]] std::string toString() const;
    [[nodiscard]] long long getSinceEpoch() const;
    bool operator==(const LastWatchDate &rhs) const;
    bool operator!=(const LastWatchDate &rhs) const;
private:
    std::chrono::system_clock::time_point date;
};

}

#endif //SERIO_LASTWATCHDATE_H
