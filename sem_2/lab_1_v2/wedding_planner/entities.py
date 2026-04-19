"""Ключевые сущности предметной области для планирования свадьбы."""

from __future__ import annotations

from dataclasses import asdict, dataclass, field
from typing import Any


@dataclass
class Ceremony:
    """Детали свадебной церемонии."""

    date: str = ""
    place: str = ""
    script: str = ""
    photo_plan: str = ""


@dataclass
class Banquet:
    """Детали банкета и меню."""

    venue: str = ""
    menu: list[str] = field(default_factory=list)


@dataclass
class WeddingPlan:
    """Агрегированное состояние плана свадьбы."""

    newlyweds: list[str] = field(default_factory=list)
    guests: list[str] = field(default_factory=list)
    bride_dress: str = ""
    rings: str = ""
    ceremony: Ceremony = field(default_factory=Ceremony)
    banquet: Banquet = field(default_factory=Banquet)

    def to_dict(self) -> dict[str, Any]:
        """Сериализует объект в словарь."""
        return asdict(self)

    @classmethod
    def from_dict(cls, data: dict[str, Any]) -> "WeddingPlan":
        """Создает экземпляр плана из словаря с безопасными значениями."""
        ceremony_data = data.get("ceremony", {})
        banquet_data = data.get("banquet", {})
        return cls(
            newlyweds=list(data.get("newlyweds", [])),
            guests=list(data.get("guests", [])),
            bride_dress=str(data.get("bride_dress", "")),
            rings=str(data.get("rings", "")),
            ceremony=Ceremony(
                date=str(ceremony_data.get("date", "")),
                place=str(ceremony_data.get("place", "")),
                script=str(ceremony_data.get("script", "")),
                photo_plan=str(ceremony_data.get("photo_plan", "")),
            ),
            banquet=Banquet(
                venue=str(banquet_data.get("venue", "")),
                menu=[str(item) for item in banquet_data.get("menu", [])],
            ),
        )
