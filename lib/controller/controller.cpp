#include "controller.h"

namespace espresso {

Controller::Controller(ControllerTime maximum_sample_age)
    : maximum_sample_age_(maximum_sample_age) {}

void Controller::update(ControllerTime now, const ControllerInputs& inputs) {
    fault_latched_ = fault_latched_ || inputs.fault_detected;
    if (fault_latched_) {
        commands_ = {};
        return;
    }

    const auto& sample = inputs.temperature;
    const bool temperature_usable = sample.valid && sample.captured_at <= now &&
                                    now - sample.captured_at < maximum_sample_age_;
    commands_ = {inputs.heat_requested && temperature_usable, inputs.pump_requested};
}

ActuatorCommands Controller::commands() const {
    return commands_;
}

}  // namespace espresso
