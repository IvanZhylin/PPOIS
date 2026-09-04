"""Фабрика приложения: общая инициализация для CLI и GUI."""

from __future__ import annotations

from wedding_planner.planner import WeddingPlannerService
from wedding_planner.storage import JsonPlanRepository

DEFAULT_DATA_FILE = "data/wedding_plan.json"


def create_service(data_file: str = DEFAULT_DATA_FILE) -> WeddingPlannerService:
    """Создаёт сервис планировщика с JSON-хранилищем (DIP)."""
    repository = JsonPlanRepository(data_file)
    return WeddingPlannerService(repository)
