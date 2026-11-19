# ESPresso agent guidance

- Coding agents are owners of this codebase. Take responsibility for code quality and long-term maintainability. Keep changes focused, preserve existing behavior unless the task changes it, and verify the behavior you touch.
- Consult the [specification review](docs/specification-review.md) when changing control behavior, hardware assumptions, or commissioning requirements. Record confirmed hardware decisions in `docs/` and keep unverified values out of deployable defaults.
- Keep controller logic independent of ESP32 drivers and test it on the host with deterministic inputs. Use a thermal model when it helps PID development.
- Report host tests, isolated bench tests, and hardware verification separately. Treat actuator commands and actual load power as distinct. Preserve independent heater protection.
