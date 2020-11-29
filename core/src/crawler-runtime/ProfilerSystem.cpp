#include "ProfilerSystem.h"
#include <iostream>
#include <iomanip>

namespace serio {
class UseCoutPrecision {
public:
    UseCoutPrecision(std::streamsize newPrecision) {
        precision = std::cout.precision();
        std::cout << std::fixed;
        std::cout << std::setprecision(newPrecision);
    }
    virtual ~UseCoutPrecision() {
        std::cout << std::defaultfloat;
        std::cout << std::setprecision(precision);
    }
private:
    std::streamsize precision;
};

ProfilerSystem::ProfilerSystem() : startTime(std::chrono::system_clock::now()), lastRecordStartTime(startTime) {}

void ProfilerSystem::update(const std::string &system) {

    const auto currentTime = updateLastSystemDuration();
    lastRecordStartTime = currentTime;
    lastSystem = system;
}

void ProfilerSystem::displayResults() {
    const UseCoutPrecision precision(2);
    const auto currentTime = updateLastSystemDuration();
    const auto totalDuration = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - startTime);
    std::cout << "Profiler results:" << std::endl;
    for (const auto& result: systemToExecutionTime) {
        const auto duration = result.second;
        const auto percentage = duration * 100.0 / totalDuration;
        std::cout << result.first << ": " << duration.count() / 1000.0 << "ms (" << percentage << "%)" << std::endl;
    }
    std::cout << "Total duration: " << totalDuration.count() / 1000.0 << "ms" << std::endl << std::endl;
}

std::chrono::system_clock::time_point ProfilerSystem::updateLastSystemDuration() {
    const auto currentTime = std::chrono::system_clock::now();
    if (!lastSystem.empty()) {
        systemToExecutionTime[lastSystem] += std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastRecordStartTime);
    }
    return currentTime;
}
}