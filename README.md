# ESPresso

ESPresso is an ESP32 retrofit project for the Kubo KBECM4842 espresso machine.

## Features

### Working on the host

- Controller logic that starts with both actuators commanded off, blocks heating when temperature data is missing, invalid, or stale, and keeps both commands off after a fault latches.

### Planned

- Thermoblock temperature control, on/off pump control, and serial telemetry.
- Pump dimming, pressure sensing, and a simple UI in later stages.

## Documentation

- [Project setup and checks](docs/project.md)
- [Retrofit specification review](docs/specification-review.md)
