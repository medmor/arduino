import os

# Motor pins (BCM GPIO numbering)
MOTOR_LEFT_FORWARD = int(os.environ.get("MOTOR_LEFT_FORWARD", "23"))
MOTOR_LEFT_BACKWARD = int(os.environ.get("MOTOR_LEFT_BACKWARD", "24"))
MOTOR_RIGHT_FORWARD = int(os.environ.get("MOTOR_RIGHT_FORWARD", "7"))
MOTOR_RIGHT_BACKWARD = int(os.environ.get("MOTOR_RIGHT_BACKWARD", "8"))

# Motor timing/speed
DEFAULT_SPEED = float(os.environ.get("DEFAULT_SPEED", "0.8"))
DEFAULT_MOVE_DURATION = float(os.environ.get("DEFAULT_MOVE_DURATION", "0.5"))
DEFAULT_TURN_DURATION = float(os.environ.get("DEFAULT_TURN_DURATION", "0.35"))

# Audio settings
SAMPLE_RATE = int(os.environ.get("SAMPLE_RATE", "16000"))
BLOCK_SIZE = int(os.environ.get("BLOCK_SIZE", "8000"))

# Vosk model path
MODEL_PATH = os.environ.get("MODEL_PATH", os.path.expanduser("~/vosk-model-small-en-us-0.15"))

# Voice command phrases (action -> list of recognized phrases)
COMMANDS = {
    "forward": ["forward", "ahead", "go", "go forward", "straight", "move forward"],
    "backward": ["backward", "back", "reverse", "retreat", "go back", "move back"],
    "left": ["left", "turn left", "go left"],
    "right": ["right", "turn right", "go right"],
    "stop": ["stop", "halt", "freeze", "idle", "stay"],
    "quit": ["quit", "exit", "shutdown", "goodbye"],
}
