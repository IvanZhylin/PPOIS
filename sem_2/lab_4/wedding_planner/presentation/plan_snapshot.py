"""Снимок состояния плана для отображения во View."""

from __future__ import annotations

from dataclasses import dataclass

from wedding_planner.entities import WeddingPlan


@dataclass(frozen=True)
class PlanSnapshot:
    """Неизменяемое представление плана для UI (отделено от доменной модели)."""

    newlyweds: str
    guests: str
    guest_count: int
    bride_dress: str
    rings: str
    ceremony_date: str
    ceremony_place: str
    banquet_venue: str
    menu: str
    ceremony_script: str
    photo_plan: str

    @classmethod
    def from_plan(cls, plan: WeddingPlan) -> "PlanSnapshot":
        """Строит снимок из текущего доменного плана."""
        return cls(
            newlyweds=", ".join(plan.newlyweds) if plan.newlyweds else "—",
            guests=", ".join(plan.guests) if plan.guests else "—",
            guest_count=len(plan.guests),
            bride_dress=plan.bride_dress or "—",
            rings=plan.rings or "—",
            ceremony_date=plan.ceremony.date or "—",
            ceremony_place=plan.ceremony.place or "—",
            banquet_venue=plan.banquet.venue or "—",
            menu=", ".join(plan.banquet.menu) if plan.banquet.menu else "—",
            ceremony_script=plan.ceremony.script or "—",
            photo_plan=plan.ceremony.photo_plan or "—",
        )
