# ESP32 BLE Car

BLE-controlled car using an **ESP32-C3** (or regular **ESP32**) with a **DRV8833** dual motor driver, plus a **React Native** Android/iOS companion app.

## Projects

| Directory | Description |
|---|---|
| `esp32c3_car_ble/` | Arduino sketch for **ESP32-C3** (e.g. Super Mini). Safe GPIOs: 2, 3, 4, 5. |
| `esp32_car_ble/` | Arduino sketch for regular **ESP32** (e.g. NodeMCU-32S). Safe GPIOs: 32, 33, 25, 26. |
| `car-ble-remote/` | **React Native** mobile app — scan, connect, and drive via BLE. |

Older experiments are in `old/`.

## Firmware — Arduino (`esp32c3_car_ble` / `esp32_car_ble`)

### Requirements

- [Arduino IDE](https://www.arduino.cc/en/software) or [arduino-cli](https://arduino.github.io/arduino-cli/)
- [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32) (Board Manager URL: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`)

### Wiring

| DRV8833 | ESP32-C3 (Super Mini) | ESP32 (NodeMCU-32S) |
|---|---|---|
| IN1 (Motor 1) | GPIO 2 | GPIO 32 |
| IN2 (Motor 1) | GPIO 3 | GPIO 33 |
| IN1 (Motor 2) | GPIO 4 | GPIO 25 |
| IN2 (Motor 2) | GPIO 5 | GPIO 26 |

Power the DRV8833 from a battery pack (VM pin) — do **not** draw motor current from the dev board's 3.3V pin.

### Compile & upload

```bash
# ESP32-C3
arduino-cli compile --fqbn esp32:esp32:esp32c3 esp32c3_car_ble
arduino-cli upload --fqbn esp32:esp32:esp32c3 --port COM3 esp32c3_car_ble

# ESP32
arduino-cli compile --fqbn esp32:esp32:esp32 esp32_car_ble
arduino-cli upload --fqbn esp32:esp32:esp32 --port COM3 esp32_car_ble
```

Serial monitor: **115200 baud**.

### BLE protocol

- Device name: `ESP32C3-Car-BLE`
- Service UUID: `4fafc201-1fb5-459e-8fcc-c5c9c331914b`
- Characteristic UUID: `beb5483e-36e1-4688-b7f5-ea07361f1196`
- Commands (UTF-8 string, write): `forward`, `backward`, `left`, `right`, `stop`

## Mobile App — React Native (`car-ble-remote`)

Scans for the car's BLE advertisement and displays a D-pad to drive it.

### Requirements

- Node.js >= 18
- React Native development environment ([setup guide](https://reactnative.dev/docs/environment-setup))

### Run

```bash
cd car-ble-remote
npm install
npx react-native run-android   # or run-ios
```

### Permissions

On Android 12+ the app requests `BLUETOOTH_SCAN`, `BLUETOOTH_CONNECT`, and `ACCESS_FINE_LOCATION`. On older Android it requests just `ACCESS_FINE_LOCATION`.
