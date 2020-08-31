#ifndef SERIO_LASTWATCHDATE_H
#define SERIO_LASTWATCHDATE_H

#include <chrono>
#include <string>

namespace serio::core {

class LastWatchDate {
public:
    LastWatchDate(std::chrono::system_clock::time_point date);
    LastWatchDate(long long timeSinceEpoch);
    std::string toString() const;
    long long getSinceEpoch() const;
    bool operator==(const LastWatchDate &rhs) const;
    bool operator!=(const LastWatchDate &rhs) const;
private:
    std::chrono::system_clock::time_point date;
};

}

#endif //SERIO_LASTWATCHDATE_H
