# Pi Voice Controlled Car

Offline voice-controlled Raspberry Pi robot car using a DRV8833 motor driver and Vosk speech recognition.

## Hardware

- Raspberry Pi 4 Model B (or similar)
- DRV8833 dual motor driver module
- 2x DC motors + chassis
- USB microphone
- Motor power supply (battery pack; voltage must match your motors)

## Wiring (BCM GPIO numbers)

| DRV8833 Pin | Pi GPIO | Function        |
|-------------|---------|-----------------|
| AIN1        | GPIO 23 | Left motor FWD  |
| AIN2        | GPIO 24 | Left motor BWD  |
| BIN1        | GPIO 7  | Right motor FWD |
| BIN2        | GPIO 8  | Right motor BWD |
| VCC         | 3.3V    | Logic supply    |
| GND         | GND     | Common ground   |
| VM          | VBAT    | Motor supply    |

**Important:** connect the battery negative and Pi GND together.

Physical pin locations on the 40-pin header:
- GPIO 7  = pin 26
- GPIO 8  = pin 24
- GPIO 23 = pin 16
- GPIO 24 = pin 18

## Voice commands

- "forward" / "go" / "straight"
- "backward" / "back" / "reverse"
- "left" / "turn left"
- "right" / "turn right"
- "stop" / "halt"
- "quit" / "exit" / "shutdown"

## Deployment

From this workspace on Windows, run the PowerShell deploy script:

```powershell
.\deploy_pi_car.ps1
```

Then SSH into the Pi and run the setup script:

```bash
ssh medmor@192.168.1.7
bash ~/pi_car_voice/setup.sh
```

## Run manually

```bash
source ~/pi_car_voice_env/bin/activate
python ~/pi_car_voice/main.py
```

## Run as a service

```bash
sudo cp ~/pi_car_voice/pi_car_voice.service /etc/systemd/system/
sudo systemctl daemon-reload
sudo systemctl enable pi_car_voice.service
sudo systemctl start pi_car_voice.service
```

View logs:

```bash
sudo journalctl -u pi_car_voice -f
```

## Configuration

Edit `config.py` or set environment variables to override defaults, e.g.:

```bash
export DEFAULT_SPEED=0.6
export DEFAULT_MOVE_DURATION=0.7
export MOTOR_LEFT_FORWARD=23
```
