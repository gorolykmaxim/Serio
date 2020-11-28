#ifndef SERIO_PROFILERSYSTEM_H
#define SERIO_PROFILERSYSTEM_H

#include <string>
#include <chrono>
#include <map>

namespace serio {
class ProfilerSystem {
public:
    ProfilerSystem();
    void update(const std::string& system);
    void displayResults();
private:
    std::chrono::system_clock::time_point startTime;
    std::string lastSystem;
    std::chrono::system_clock::time_point lastRecordStartTime;
    std::map<std::string, std::chrono::microseconds> systemToExecutionTime;
};
}

#endif //SERIO_PROFILERSYSTEM_H
