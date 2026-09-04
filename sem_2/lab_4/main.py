"""Точка входа: CLI (л/р №1) или GUI (л/р №4)."""

from __future__ import annotations

import argparse

from wedding_planner.bootstrap import DEFAULT_DATA_FILE
from wedding_planner.cli import run_cli
from wedding_planner.gui import run_gui


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Планировщик свадьбы: консольный или графический режим.",
    )
    parser.add_argument(
        "--gui",
        action="store_true",
        help="Запустить графический интерфейс (лабораторная №4, MVP).",
    )
    parser.add_argument(
        "--data",
        default=DEFAULT_DATA_FILE,
        help=f"Путь к JSON-файлу состояния (по умолчанию: {DEFAULT_DATA_FILE}).",
    )
    args = parser.parse_args()

    if args.gui:
        run_gui(args.data)
    else:
        run_cli(args.data)


if __name__ == "__main__":
    main()
