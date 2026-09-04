"""Абстракции и реализации слоя сохранения."""

from __future__ import annotations

import json
from abc import ABC, abstractmethod
from pathlib import Path

from wedding_planner.entities import WeddingPlan
from wedding_planner.exceptions import PersistenceError


class PlanRepository(ABC):
    """Абстракция репозитория для сохранения данных (DIP)."""

    @abstractmethod
    def load(self) -> WeddingPlan:
        """Загружает план из хранилища."""

    @abstractmethod
    def save(self, plan: WeddingPlan) -> None:
        """Сохраняет план в хранилище."""


class JsonPlanRepository(PlanRepository):
    """Реализация репозитория плана на основе JSON-файла."""

    def __init__(self, file_path: str) -> None:
        self._path = Path(file_path)

    def load(self) -> WeddingPlan:
        if not self._path.exists():
            return WeddingPlan()
        try:
            raw_data = self._path.read_text(encoding="utf-8")
            parsed = json.loads(raw_data)
        except (OSError, json.JSONDecodeError) as error:
            raise PersistenceError(f"Не удалось загрузить данные: {error}") from error
        if not isinstance(parsed, dict):
            raise PersistenceError("Сохраненные данные JSON имеют неверный формат.")
        return WeddingPlan.from_dict(parsed)

    def save(self, plan: WeddingPlan) -> None:
        try:
            self._path.parent.mkdir(parents=True, exist_ok=True)
            self._path.write_text(
                json.dumps(plan.to_dict(), ensure_ascii=False, indent=2),
                encoding="utf-8",
            )
        except OSError as error:
            raise PersistenceError(f"Не удалось сохранить данные: {error}") from error
