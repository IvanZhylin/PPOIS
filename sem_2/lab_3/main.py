#!/usr/bin/env python3
"""Запуск игры Pong."""

from pathlib import Path

from src.application import PongApplication


def main() -> None:
    base_dir = Path(__file__).resolve().parent
    app = PongApplication(base_dir)
    app.run()


if __name__ == "__main__":
    main()
