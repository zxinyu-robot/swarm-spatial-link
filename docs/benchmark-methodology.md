# Benchmark Methodology

## Required measurements

### CPU and copy cost

Compare ROS 2 serialized transport and shared-memory transport with identical payload size, publication rate and consumer count. Record:

- producer and consumer CPU time;
- bytes copied per message;
- allocation count;
- resident memory;
- dropped and overwritten buffers.

Report steady-state values after warm-up and avoid combining DDS discovery cost with the data-path result.

### Link reliability

Apply controlled loss, latency, jitter and bandwidth using a reproducible network emulator. Record each input trace alongside:

- offered and delivered keyframe rate;
- stale-frame discard rate;
- DDS retransmissions;
- queue occupancy;
- end-to-end age at the fusion consumer.

A reduction in dropped frames must be reported together with the change in offered load and latency.

### Map-fusion latency

Measure from robot-side keyframe eligibility to the publication of the corresponding accepted edge graph version. Split the result into:

1. local admission and shared-memory handoff;
2. DDS serialization and network transit;
3. edge receive/decode;
4. constraint generation;
5. graph optimization and map publication.

### Clock quality

Report offset, P50/P95 absolute offset, median absolute deviation and holdover behavior separately for PTP and NTP.

## Reproducible protocol

1. Record robot/edge hardware, power mode and temperature.
2. Pin DDS implementation, version, discovery mode and full QoS.
3. Use the same rosbag and map for every policy comparison.
4. Run at least five trials and publish raw timestamp traces.
5. Separate synthetic emulation, lab Mesh and field-network results.
6. Preserve robot/session/sequence IDs to identify duplicates and gaps.

## Historical engineering figures

The associated private engineering project reported:

- shared-memory data-path CPU reduction: approximately 35%–50%;
- keyframe drop-rate reduction after QoS tuning: approximately 20%–40%;
- keyframe synchronization: approximately 10–20 Hz;
- multi-robot submap-fusion latency: approximately 100–300 ms.

These figures are project context, not results reproduced by this repository. The original bags, point clouds, DDS traces, hardware setup and full fusion pipeline are not public.

## Public-repository policy

CI verifies policy transitions, scheduler behavior and clock-estimator robustness only. Future performance reports should include raw inputs, environment metadata and scripts under a versioned `benchmarks/` directory.
