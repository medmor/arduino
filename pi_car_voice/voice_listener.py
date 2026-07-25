"""Offline speech recognition using Vosk + sounddevice."""

import json
import queue
import sys
from typing import Iterator

import sounddevice as sd
from vosk import Model, KaldiRecognizer


class VoiceListener:
    """Streams microphone audio and yields Vosk recognition results."""

    def __init__(self, model_path: str, sample_rate: int = 16000, block_size: int = 8000) -> None:
        self.sample_rate = sample_rate
        self.block_size = block_size
        self.audio_queue: queue.Queue[bytes] = queue.Queue()
        print(f"Loading Vosk model from {model_path}...")
        self.model = Model(model_path)
        self.recognizer = KaldiRecognizer(self.model, sample_rate)
        print("Model loaded.")

    def _audio_callback(self, indata, frames, time_info, status) -> None:
        if status:
            print(f"Audio status: {status}", file=sys.stderr)
        self.audio_queue.put(bytes(indata))

    def listen(self) -> Iterator[dict]:
        """Yield JSON result dicts from Vosk as commands are spoken."""
        print(f"Listening at {self.sample_rate} Hz (say a command)...")
        with sd.RawInputStream(
            samplerate=self.sample_rate,
            blocksize=self.block_size,
            dtype="int16",
            channels=1,
            callback=self._audio_callback,
        ):
            while True:
                data = self.audio_queue.get()
                if self.recognizer.AcceptWaveform(data):
                    try:
                        yield json.loads(self.recognizer.Result())
                    except json.JSONDecodeError:
                        continue
