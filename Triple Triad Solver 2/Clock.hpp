#pragma once
#include <chrono>
#include <iomanip>

namespace Clock {
    static std::chrono::steady_clock::time_point timeLast;

    static void init() {
        timeLast = std::chrono::steady_clock::now();
    }

    static bool intervalPassed(int intervalInSeconds) {
        auto now = std::chrono::steady_clock::now();
        std::chrono::seconds interval(intervalInSeconds);  // Convert seconds to chrono duration
        return (now - timeLast) >= interval;
    }

    static void intervalReset() {
        timeLast = std::chrono::steady_clock::now();
    }

    static void printProgressEveryXseconds(float currentValue, float maxValue, int timeInSeconds) {
        if (intervalPassed(timeInSeconds)) {
            std::cout << "Progress: " << std::fixed << std::setprecision(2)
                << ((currentValue / maxValue) * 100) << "%" << std::endl;
            intervalReset();
        }
    }
}