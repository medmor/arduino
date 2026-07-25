#!/usr/bin/env python3
"""Main entry point: listen to voice commands and drive the car."""

import os
import sys

from config import MODEL_PATH, SAMPLE_RATE, BLOCK_SIZE
from command_parser import parse_command
from motor_controller import CarController
from voice_listener import VoiceListener


def main() -> None:
    print("Pi Voice Car starting...")

    if not os.path.exists(MODEL_PATH):
        print(f"ERROR: Vosk model not found at {MODEL_PATH}")
        print("Run setup.sh on the Pi to download it, or set MODEL_PATH.")
        sys.exit(1)

    car = CarController()
    listener = VoiceListener(MODEL_PATH, sample_rate=SAMPLE_RATE, block_size=BLOCK_SIZE)

    print("Say a command: forward, backward, left, right, stop, quit")

    try:
        for result in listener.listen():
            text = result.get("text", "")
            if not text:
                continue

            print(f"Heard: '{text}'")
            command = parse_command(text)
            if not command:
                continue

            print(f"Action: {command}")
            if command == "quit":
                break
            elif command == "forward":
                car.forward()
            elif command == "backward":
                car.backward()
            elif command == "left":
                car.left()
            elif command == "right":
                car.right()
            elif command == "stop":
                car.stop()

    except KeyboardInterrupt:
        print("\nStopped by user.")
    finally:
        car.close()
        print("Pi Voice Car stopped.")


if __name__ == "__main__":
    main()
