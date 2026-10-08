#ifndef SWARM_SPATIAL_LINK_MESSAGE_CONTRACT_H
#define SWARM_SPATIAL_LINK_MESSAGE_CONTRACT_H

#include <cstddef>
#include <cstdint>
#include <string>

namespace swarm_spatial_link {

enum class MessageClass {
    Pose,
    Health,
    Keyframe,
    Submap,
    Mission,
};

struct MessageEnvelope {
    std::uint32_t robot_id{};
    std::uint32_t session_id{};
    std::uint64_t sequence{};
    std::int64_t source_time_ns{};
    MessageClass message_class{MessageClass::Pose};
    std::string frame_id;
    double uncertainty{};
    std::size_t payload_bytes{};
};

enum class Reliability {
    BestEffort,
    Reliable,
};

struct TopicPolicy {
    Reliability reliability{Reliability::BestEffort};
    std::size_t history_depth{1};
    double target_rate_hz{};
    std::int64_t deadline_ms{};
    std::uint8_t priority{};
};

}  // namespace swarm_spatial_link

#endif
