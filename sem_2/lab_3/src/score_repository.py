"""Работа с таблицей рекордов."""

from __future__ import annotations

import json
from datetime import date
from pathlib import Path
from typing import Any

from src.interfaces import IScoreRepository


class JsonScoreRepository(IScoreRepository):
    """Сохранение рекордов в JSON-файл."""

    def __init__(self, path: Path) -> None:
        self._path = path
        self._data = self._read()

    def _read(self) -> dict[str, Any]:
        if not self._path.exists():
            return {"entries": [], "max_entries": 10}
        with self._path.open(encoding="utf-8") as f:
            return json.load(f)

    def _write(self) -> None:
        self._path.parent.mkdir(parents=True, exist_ok=True)
        with self._path.open("w", encoding="utf-8") as f:
            json.dump(self._data, f, ensure_ascii=False, indent=2)

    def get_entries(self) -> list[dict[str, Any]]:
        entries = list(self._data.get("entries", []))
        entries.sort(key=lambda e: e.get("score", 0), reverse=True)
        return entries

    def get_top_score(self) -> int:
        entries = self.get_entries()
        if not entries:
            return 0
        return int(entries[0].get("score", 0))

    def is_high_score(self, score: int) -> bool:
        entries = self.get_entries()
        max_entries = int(self._data.get("max_entries", 10))
        if len(entries) < max_entries:
            return True
        return score > int(entries[-1].get("score", 0))

    def beats_first_place(self, score: int) -> bool:
        """Результат выше первой строчки таблицы."""
        entries = self.get_entries()
        if not entries:
            return score > 0
        return score > int(entries[0].get("score", 0))

    def add_entry(self, name: str, score: int) -> None:
        entry = {
            "name": (name or "Игрок")[:12],
            "score": score,
            "date": date.today().isoformat(),
        }
        entries = self.get_entries()
        entries.append(entry)
        entries.sort(key=lambda e: e.get("score", 0), reverse=True)
        max_entries = int(self._data.get("max_entries", 10))
        self._data["entries"] = entries[:max_entries]
        self._write()
