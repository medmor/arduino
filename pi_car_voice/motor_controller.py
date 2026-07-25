"""DRV8833 dual motor driver control using gpiozero."""

from time import sleep
from gpiozero import Motor

from config import (
    MOTOR_LEFT_FORWARD,
    MOTOR_LEFT_BACKWARD,
    MOTOR_RIGHT_FORWARD,
    MOTOR_RIGHT_BACKWARD,
    DEFAULT_SPEED,
    DEFAULT_MOVE_DURATION,
    DEFAULT_TURN_DURATION,
)


class CarController:
    """Controls a two-motor robot car with a DRV8833 driver."""

    def __init__(self) -> None:
        self.left_motor = Motor(
            forward=MOTOR_LEFT_FORWARD,
            backward=MOTOR_LEFT_BACKWARD,
        )
        self.right_motor = Motor(
            forward=MOTOR_RIGHT_FORWARD,
            backward=MOTOR_RIGHT_BACKWARD,
        )

    def forward(self, duration: float = DEFAULT_MOVE_DURATION, speed: float = DEFAULT_SPEED) -> None:
        self.left_motor.forward(speed)
        self.right_motor.forward(speed)
        sleep(duration)
        self.stop()

    def backward(self, duration: float = DEFAULT_MOVE_DURATION, speed: float = DEFAULT_SPEED) -> None:
        self.left_motor.backward(speed)
        self.right_motor.backward(speed)
        sleep(duration)
        self.stop()

    def left(self, duration: float = DEFAULT_TURN_DURATION, speed: float = DEFAULT_SPEED) -> None:
        # Pivot left: left motor back, right motor forward
        self.left_motor.backward(speed)
        self.right_motor.forward(speed)
        sleep(duration)
        self.stop()

    def right(self, duration: float = DEFAULT_TURN_DURATION, speed: float = DEFAULT_SPEED) -> None:
        # Pivot right: left motor forward, right motor back
        self.left_motor.forward(speed)
        self.right_motor.backward(speed)
        sleep(duration)
        self.stop()

    def stop(self) -> None:
        self.left_motor.stop()
        self.right_motor.stop()

    def close(self) -> None:
        self.stop()
        self.left_motor.close()
        self.right_motor.close()
