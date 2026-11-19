#include <unity.h>

#include "controller.h"

using namespace std::chrono_literals;
using espresso::Controller;
using espresso::ControllerInputs;
using espresso::ControllerTime;

namespace {

constexpr auto sample_timeout = 100ms;

struct FakeClock {
    ControllerTime now{};

    void advance(ControllerTime elapsed) { now += elapsed; }
};

void test_startup_commands_both_outputs_off() {
    Controller controller(sample_timeout);

    TEST_ASSERT_FALSE(controller.commands().heater_on);
    TEST_ASSERT_FALSE(controller.commands().pump_on);
}

void test_heater_stays_off_until_a_valid_sample_arrives() {
    Controller controller(sample_timeout);
    FakeClock clock;
    ControllerInputs inputs;
    inputs.heat_requested = true;

    controller.update(clock.now, inputs);
    TEST_ASSERT_FALSE(controller.commands().heater_on);

    inputs.temperature = {true, clock.now};
    controller.update(clock.now, inputs);
    TEST_ASSERT_TRUE(controller.commands().heater_on);

    clock.advance(1ms);
    inputs.temperature = {false, clock.now};
    controller.update(clock.now, inputs);
    TEST_ASSERT_FALSE(controller.commands().heater_on);
}

void test_heater_turns_off_when_sample_reaches_its_age_limit() {
    Controller controller(sample_timeout);
    FakeClock clock;
    ControllerInputs inputs;
    inputs.heat_requested = true;
    inputs.temperature = {true, clock.now};

    clock.advance(sample_timeout - 1ms);
    controller.update(clock.now, inputs);
    TEST_ASSERT_TRUE(controller.commands().heater_on);

    clock.advance(1ms);
    controller.update(clock.now, inputs);
    TEST_ASSERT_FALSE(controller.commands().heater_on);

    inputs.temperature = {true, clock.now};
    controller.update(clock.now, inputs);
    TEST_ASSERT_TRUE(controller.commands().heater_on);
}

void test_future_sample_keeps_heater_off() {
    Controller controller(sample_timeout);
    FakeClock clock;
    ControllerInputs inputs;
    inputs.heat_requested = true;
    inputs.temperature = {true, clock.now + 1ms};

    controller.update(clock.now, inputs);
    TEST_ASSERT_FALSE(controller.commands().heater_on);
}

void test_fault_shuts_off_both_outputs_and_stays_latched() {
    Controller controller(sample_timeout);
    FakeClock clock;
    ControllerInputs inputs;
    inputs.heat_requested = true;
    inputs.pump_requested = true;
    inputs.temperature = {true, clock.now};

    controller.update(clock.now, inputs);
    TEST_ASSERT_TRUE(controller.commands().heater_on);
    TEST_ASSERT_TRUE(controller.commands().pump_on);

    inputs.fault_detected = true;
    controller.update(clock.now, inputs);
    TEST_ASSERT_FALSE(controller.commands().heater_on);
    TEST_ASSERT_FALSE(controller.commands().pump_on);

    clock.advance(1ms);
    inputs.fault_detected = false;
    inputs.temperature = {true, clock.now};
    controller.update(clock.now, inputs);
    TEST_ASSERT_FALSE(controller.commands().heater_on);
    TEST_ASSERT_FALSE(controller.commands().pump_on);
}

} // namespace

void setUp() {}
void tearDown() {}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_startup_commands_both_outputs_off);
    RUN_TEST(test_heater_stays_off_until_a_valid_sample_arrives);
    RUN_TEST(test_heater_turns_off_when_sample_reaches_its_age_limit);
    RUN_TEST(test_future_sample_keeps_heater_off);
    RUN_TEST(test_fault_shuts_off_both_outputs_and_stays_latched);
    return UNITY_END();
}
