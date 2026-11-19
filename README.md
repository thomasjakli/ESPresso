# ESPresso

ESPresso is a planned ESP32 retrofit for the Kubo KBECM4842 espresso machine. The initial scope covers thermoblock temperature control, on/off pump control, and serial telemetry. Pump dimming, pressure sensing, and a simple UI are later work. Steam mode, scale integration, and display hardware are outside the initial scope.

The project is at the specification stage. The first implementation milestone is a controller with deterministic host-side tests. A thermoblock model is optional for later PID work.

- [Retrofit specification review](docs/specification-review.md) records the host testing plan, requirements to clarify, and open hardware decisions.

## Markdown linting

With Node.js 22 or later, run these commands from the repository root.

```sh
npm ci
npm run lint:markdown
```

GitHub Actions runs the same commands for pull requests.
