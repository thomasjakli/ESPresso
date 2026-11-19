#pragma once

#include <chrono>

namespace espresso {

using ControllerTime = std::chrono::milliseconds;

struct TemperatureSample {
    bool valid = false;
    ControllerTime captured_at{};
};

struct ControllerInputs {
    TemperatureSample temperature{};
    bool heat_requested = false;
    bool pump_requested = false;
    bool fault_detected = false;
};

struct ActuatorCommands {
    bool heater_on = false;
    bool pump_on = false;
};

class Controller {
public:
    explicit Controller(ControllerTime maximum_sample_age);

    // Both timestamps must come from the same monotonic clock.
    void update(ControllerTime now, const ControllerInputs& inputs);
    ActuatorCommands commands() const;

private:
    ControllerTime maximum_sample_age_;
    bool fault_latched_ = false;
    ActuatorCommands commands_{};
};

}  // namespace espresso
