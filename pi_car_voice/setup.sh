#!/bin/bash
# Run this script on the Raspberry Pi to install dependencies and download the Vosk model.
set -e

echo "Updating package lists..."
sudo apt-get update

echo "Installing system packages..."
sudo apt-get install -y \
    python3-pip \
    python3-venv \
    libportaudio2 \
    portaudio19-dev \
    wget \
    unzip

PROJECT_DIR="$HOME/pi_car_voice"
VENV_DIR="$HOME/pi_car_voice_env"
MODEL_DIR="$HOME/vosk-model-small-en-us-0.15"
MODEL_ZIP="$HOME/vosk-model-small-en-us-0.15.zip"

echo "Creating Python virtual environment..."
python3 -m venv "$VENV_DIR"
source "$VENV_DIR/bin/activate"

echo "Installing Python packages..."
pip install --upgrade pip
pip install -r "$PROJECT_DIR/requirements.txt"

if [ ! -d "$MODEL_DIR" ]; then
    echo "Downloading Vosk small English model..."
    wget -O "$MODEL_ZIP" "https://alphacephei.com/vosk/models/vosk-model-small-en-us-0.15.zip"
    unzip -q "$MODEL_ZIP" -d "$HOME"
    rm "$MODEL_ZIP"
    echo "Model downloaded to $MODEL_DIR"
else
    echo "Vosk model already exists."
fi

echo ""
echo "Setup complete. Run the car with:"
echo "  source $VENV_DIR/bin/activate"
echo "  python $PROJECT_DIR/main.py"
