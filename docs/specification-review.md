# ESPresso retrofit specification review

ESPresso is a replacement controller project for the Kubo KBECM4842. The first milestone is a controller with deterministic host-side tests for brew states, heater and pump commands, and fault handling. A thermal model is optional for later PID work. Hardware integration depends on confirming the board, sensors, and electrical protection.

## Reported observations and assumptions

| Item | Reported observations | What remains open |
| --- | --- | --- |
| Thermoblock | Owner reports a 230 V, 800 W marking and 68 Ω cold resistance. The resistance implies about 778 W and 3.38 A at 230 V as a nominal cold calculation. | Both switches read closed at the time of measurement. Their roles, trip temperatures, reset behavior, wiring order, and the identity of the white wire remain unverified. |
| Pump | Label reportedly identifies an ODE P500-224A-AAB-20 rated 53 W, with a 1.5 MPa maximum and a 2 min on / 1 min off duty cycle. Owner reports 875 Ω in one direction and an open reading in the other. | The reading supports the internal-diode hypothesis. Pump topology, expected operating current, protector wiring and reset behavior, and the presence of an overpressure valve remain open. |
| Control and sensing | An ESP32, SSRs marked Fotek SSR-40 DA, and temperature probes are reported on hand. | Exact ESP32 board, sensor and mounting choice, SSR provenance and ratings, GPIO assignment, and pump-button behavior remain open. |
| Original board | The supply is reported to be non-isolated and the pump button to switch mains. | Treat the original board and pump-button circuit as mains-referenced. Confirm the isolation and input design before connecting controller electronics. |

The winding resistance and pump nameplate watts are insufficient to size the pump switch or protection. Verify the switching device against the actual load and its transient behavior. If the pump is half-wave rectified, compatibility with a zero-cross SSR is a specific concern. [Omron SSR load precautions](https://www.ia.omron.com/product/cautions/18/safety_precautions.html)

## First firmware milestone

1. Set up the PlatformIO native C++17 test target first. Keep the controller independent of ESP32 drivers. Use a fake clock, scripted inputs and temperature readings, and recorded actuator commands. Give temperature samples explicit validity and freshness so boot, stale data, and read failures can be tested separately. [PlatformIO Native documentation](https://docs.platformio.org/en/stable/platforms/native.html)
2. Verify the outputs are commanded off at boot, idle, and on fault. Cover readiness, brew and standby transitions, pump timing boundaries, heater pulse scheduling, and fault latching. As fault behavior is specified, test missing or invalid readings, implausible or stuck readings, over-temperature, failure to heat, runaway, controller stalls, repeated abnormal resets, and high enclosure or heatsink temperatures. Use scripted readings to represent a switch stuck on even after an off command. Test the actuator deadline with a fake timer that can advance while the control loop is stalled.
3. Run the host tests in CI once the target exists. Add a separate ESP32 Arduino build target after confirming the board and before adopting the pin map. Pin versions in the build configuration and scope Arduino dependencies to the ESP32 target. Use serial CSV for initial device diagnostics. Remote telemetry, OTA, dimming, and pressure control are later work.

## Optional thermal model for PID development

Exercise PID saturation and recovery with scripted inputs first. Add a simple thermoblock model if closed-loop scenarios help evaluate controller changes. Model block heating as heater input minus heat lost to the surroundings and carried away by water flow. Keep thermal mass, heat loss, flow, and sensor delay as adjustable simulation parameters. Use measured traces to fit the model and evaluate real warm-up, overshoot, and outlet temperature before adopting tuning values.

## Requirements to clarify

| Topic | Resolution needed |
| --- | --- |
| Temperature limits | The 93 °C brew target is a placeholder at the block sensor. The 120 °C and 135 °C values and the proposed margins to thermostat trips need review alongside sensor placement, sensor lag, and measured overshoot. Keep commissioning placeholders out of deployable defaults. |
| Heater timing | A scheduler running only every 250 ms cannot deliver a 20 ms pulse. Define a separate output timing mechanism or revise the pulse resolution. Specify rounding, window boundaries, and immediate cancellation on fault. |
| Fault response | Command both outputs off on fault in the first host-tested controller. Define stale-sample deadlines, whether high enclosure or heatsink temperatures trigger a fault or derating, and how reset history persists and tracks time. |
| Brew and pump duty | Define hold-to-run and toggle behavior, when a brew may start, and how brewing exits to ready or cooldown. Specify rolling-window accounting and restart behavior. A rolling 120 s per 180 s limit alone can allow repeated starts with short off intervals. Clarify how shorter runs interact with the label's 1 min off requirement before enabling the pump on the machine. |
| Thermal fault detection | A fixed temperature rise or lack of rise can also reflect residual heat, normal thermostat cycling, or water flow. Exercise those cases with scripted temperature traces for stuck readings, failure to heat, and runaway. Use commissioned traces before adopting field thresholds. |

## Hardware review before commissioning

- **Independent shutdown.** An SSR can fail short and continue energizing a load after its control input is off. A watchdog verifies command shutdown. Establish and verify the independent heater cutoff before energizing the retrofit. Retain the original thermal protection and confirm the function and reset behavior of both switches. [Omron SSR failure guidance](https://www.ia.omron.com/support/guide/18/further_information.html)
- **Mains and bench setup.** Review isolation, protective earth, Schuko plug reversal, the wiring design, and component selection for protection and switching. For a USB-only bench test, observe the command or SSR input with a low-voltage indicator.
- **Commissioning.** Use normal operating measurements to characterize the block and leave unidentified thermostat behavior open until documented or verified through an appropriate hardware review. Use simulation and isolated bench setups to inject faults that would otherwise require overheating or operating with a protection device disabled.
- **Pressure.** A 1.2 MPa sensor would have a lower range than the pump's 1.5 MPa nameplate maximum. Confirm the hydraulic relief provision, pressure limits and transient ratings of all wetted parts, and compatible fittings before selecting the sensor and tee. Firmware pressure limiting is a control feature. Establish mechanical protection independently.
