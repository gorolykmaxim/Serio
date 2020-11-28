#include "ProfilerSystem.h"
#include <iostream>

namespace serio {
ProfilerSystem::ProfilerSystem() : startTime(std::chrono::system_clock::now()), lastRecordStartTime(startTime) {}

void ProfilerSystem::update(const std::string &system) {
    const auto currentTime = std::chrono::system_clock::now();
    if (!lastSystem.empty()) {
        systemToExecutionTime[lastSystem] += std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastRecordStartTime);
    }
    lastRecordStartTime = currentTime;
    lastSystem = system;
}

void ProfilerSystem::displayResults() {
    const auto currentTime = std::chrono::system_clock::now();
    systemToExecutionTime[lastSystem] += std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastRecordStartTime);
    const auto totalDuration = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - startTime).count();
    const auto totalDurationMs = totalDuration / 1000.0;
    std::cout << "Profiler results:" << std::endl;
    for (const auto& result: systemToExecutionTime) {
        const auto durationInMs = result.second.count() / 1000.0;
        const auto percentage = durationInMs / totalDurationMs * 100;
        std::cout << result.first << ": " << durationInMs << "ms (" << percentage << "%)" << std::endl;
    }
    std::cout << "Total duration: " << totalDurationMs << "ms" << std::endl << std::endl;
}
}