#include "App.h"
#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>

namespace {
volatile std::sig_atomic_t stopRequested = 0;

void handleSignal(int signal) {
    if (signal == SIGINT) {
        stopRequested = 1;
    }
}
} // namespace

void App::simulate(const double deltaTime) {
}

void App::run() {
    std::signal(SIGINT, handleSignal);

    using Clock = std::chrono::steady_clock;

    constexpr double fixedDeltaTime = 1.0F / 60.0F;

    auto previousTime = Clock::now();
    double accumulator = 0.0;

    double staticsAccumulator = 0.0;
    int tickCount = 0;

    while (stopRequested != 1) {
        const auto currentTime = Clock::now();

        const std::chrono::duration<double> elapsed = currentTime - previousTime;

        accumulator += elapsed.count();
        staticsAccumulator += elapsed.count();

        while (accumulator >= fixedDeltaTime) {
            simulate(fixedDeltaTime);
            tickCount++;
            accumulator -= fixedDeltaTime;
        }

        if (staticsAccumulator >= 1.0) {
            std::cout << tickCount << "\n";
            tickCount = 0;
            staticsAccumulator -= 1.0;
        }

        previousTime = currentTime;

        double remainingSeconds = fixedDeltaTime - accumulator;
        if (remainingSeconds > 0.0) {
            std::this_thread::sleep_for(std::chrono::duration<double>(remainingSeconds));
        }
    }

    std::cout << "Server Stopped.\n";
}
