#!/usr/bin/env python3
"""Генерация wav/ogg для assets (опционально, иначе — процедурный звук в игре)."""

from __future__ import annotations

import math
import struct
import wave
from pathlib import Path


def write_wav(path: Path, frequency: float, duration: float = 0.1, volume: float = 0.4) -> None:
    sample_rate = 22050
    n = int(sample_rate * duration)
    path.parent.mkdir(parents=True, exist_ok=True)
    with wave.open(str(path), "w") as wf:
        wf.setnchannels(1)
        wf.setsampwidth(2)
        wf.setframerate(sample_rate)
        frames = bytearray()
        for i in range(n):
            t = i / sample_rate
            env = 1.0 - i / n
            val = int(32767 * volume * env * math.sin(2 * math.pi * frequency * t))
            frames.extend(struct.pack("<h", val))
        wf.writeframes(frames)


def main() -> None:
    root = Path(__file__).resolve().parent.parent
    assets = root / "assets" / "sounds"
    write_wav(assets / "paddle_hit.wav", 440, 0.08)
    write_wav(assets / "wall_hit.wav", 280, 0.06)
    write_wav(assets / "score.wav", 150, 0.25, 0.5)
    write_wav(assets / "menu_select.wav", 520, 0.05)
    write_wav(assets / "game_over.wav", 110, 0.4, 0.45)
    print("Файлы созданы в", assets)


if __name__ == "__main__":
    main()
