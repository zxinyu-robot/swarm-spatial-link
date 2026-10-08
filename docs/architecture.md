# Architecture

## Design boundary

Swarm Spatial Link sits between robot-local state estimation and edge-side graph optimization. It owns message contracts, transport policy selection, timing metadata and keyframe admission. It does not own scan matching, factor construction, graph optimization, route planning or flight control.

## Data classes

### Pose

High-rate, latest-value state. Old poses should not accumulate behind newer poses, so the default policy is `BestEffort + KeepLast(1)`.

### Health

Low-rate but operationally critical diagnostics. Health remains reliable with high priority across all network modes.

### Keyframe

Selected LiDAR/visual data used by cooperative localization and loop closure. Healthy and constrained links use reliable delivery. Critical links prefer selected best-effort frames to avoid retry backlog, unless uncertainty, a loop candidate or a mission-critical flag overrides that policy.

### Submap

Large, delay-tolerant map blocks. Critical links defer submaps while preserving pose and health traffic.

### Mission

Skill-level task commands and acknowledgements. A production adapter should map these to a separate reliable, authenticated DDS partition.

## Policy transitions

Link classification observes packet loss, one-way latency and queue fill ratio:

- Healthy: all metrics remain below healthy thresholds.
- Critical: any metric crosses its critical threshold.
- Constrained: all intermediate conditions.

A candidate mode must remain stable for a configured number of observations before activation. This prevents reliability and rate parameters from oscillating around a threshold.

## Time consistency

PTP is preferred when supported by the wired or managed wireless topology. NTP provides a fallback, but source timestamps and estimated offsets must remain explicit in every spatial message.

`ClockOffsetEstimator` uses:

```text
offset = receive_time - source_time - round_trip_time / 2
```

The median rejects isolated delay spikes, and median absolute deviation represents residual jitter. This model is a transport health estimate, not a replacement for a full PTP servo.

## Shared-memory boundary

Large same-host payloads should remain in System V/POSIX shared memory while DDS carries a bounded descriptor:

- segment or pool identity;
- immutable buffer offset and size;
- schema/version;
- producer epoch and lease deadline;
- checksum;
- robot, session and sequence identifiers.

The production design must handle producer crashes, stale leases, generation reuse and consumer backpressure. Those mechanisms are not implemented in the current public core.

## Edge fusion

The edge receiver validates sequence and clock metadata before translating keyframes or submaps into Graph-SLAM constraints. Late or duplicated messages must be idempotent. Global corrections should carry a graph version so robot-local consumers never combine state from incompatible map epochs.
