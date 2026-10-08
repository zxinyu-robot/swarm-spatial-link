#include "swarm_spatial_link/adaptive_qos.h"
#include "swarm_spatial_link/clock_offset_estimator.h"
#include "swarm_spatial_link/keyframe_scheduler.h"

#include <iomanip>
#include <iostream>
#include <vector>

int main() {
    using namespace swarm_spatial_link;

    AdaptiveQosController controller;
    const std::vector<LinkMetrics> replay{
        {0.01, 20.0, 2.0, 0.10, 80.0},
        {0.02, 25.0, 3.0, 0.15, 75.0},
        {0.12, 110.0, 18.0, 0.65, 20.0},
        {0.14, 130.0, 24.0, 0.70, 16.0},
        {0.31, 310.0, 60.0, 0.95, 4.0},
        {0.34, 340.0, 70.0, 0.97, 3.0},
        {0.10, 100.0, 20.0, 0.62, 18.0},
        {0.08, 85.0, 16.0, 0.58, 22.0},
        {0.02, 30.0, 3.0, 0.20, 70.0},
        {0.01, 22.0, 2.0, 0.12, 82.0},
    };

    std::cout << "adaptive DDS policy replay\n";
    NetworkMode previous = controller.mode();
    for (std::size_t index = 0; index < replay.size(); ++index) {
        const QosDecision decision =
            controller.update(replay[index], {.pose_uncertainty = 0.45});
        if (index == 0 || decision.mode != previous) {
            std::cout
                << "  sample=" << index
                << " mode=" << AdaptiveQosController::name(decision.mode)
                << " keyframe="
                << AdaptiveQosController::name(decision.keyframe.reliability)
                << '@' << decision.keyframe.target_rate_hz << "Hz"
                << " compression=" << decision.compression_level
                << " defer_submaps="
                << (decision.defer_submaps ? "yes" : "no") << '\n';
        }
        previous = decision.mode;
    }

    ClockOffsetEstimator clock;
    const std::int64_t base = 1'000'000'000;
    for (std::int64_t noise : {-20'000, 10'000, 0, 30'000, -10'000}) {
        clock.observe(base, base + 5'500'000 + noise, 1'000'000);
    }
    const ClockEstimate estimate = clock.estimate();
    std::cout << std::fixed << std::setprecision(3)
              << "clock offset=" << estimate.offset_ns / 1'000'000.0 << "ms"
              << " mad=" << estimate.median_absolute_deviation_ns / 1'000'000.0
              << "ms synchronized="
              << (estimate.synchronized ? "yes" : "no") << '\n';

    KeyframeScheduler scheduler;
    std::size_t published = 0;
    for (std::int64_t index = 0; index < 20; ++index) {
        published += scheduler.should_publish(
            index * 50'000'000,
            index == 11 ? 0.90 : 0.30,
            false,
            5.0);
    }
    std::cout << "keyframes published=" << published
              << " from 20 frontend updates\n";
    return 0;
}
