"""Загрузка JSON-конфигураций (единая ответственность)."""

from __future__ import annotations

import json
from pathlib import Path
from typing import Any


class ConfigLoader:
    """Читает конфигурационные файлы из каталога проекта."""

    def __init__(self, base_dir: Path) -> None:
        self._base = base_dir

    def load(self, relative_path: str) -> dict[str, Any]:
        path = self._base / relative_path
        with path.open(encoding="utf-8") as f:
            return json.load(f)

    @property
    def game(self) -> dict[str, Any]:
        return self.load("config/game.json")

    @property
    def menu(self) -> dict[str, Any]:
        return self.load("config/menu.json")

    @property
    def help_text(self) -> dict[str, Any]:
        return self.load("config/help.json")

    @property
    def audio(self) -> dict[str, Any]:
        return self.load("config/audio.json")
