"""Форматирование плана для вывода в CLI и GUI."""

from __future__ import annotations

from wedding_planner.entities import WeddingPlan
from wedding_planner.presentation.plan_snapshot import PlanSnapshot


def format_snapshot_text(snapshot: PlanSnapshot) -> str:
    """Возвращает многострочное описание по снимку состояния."""
    lines = [
        "Текущий план свадьбы",
        "─" * 32,
        f"Молодожены: {snapshot.newlyweds}",
        f"Гости ({snapshot.guest_count}): {snapshot.guests}",
        f"Платье невесты: {snapshot.bride_dress}",
        f"Кольца: {snapshot.rings}",
        f"Дата церемонии: {snapshot.ceremony_date}",
        f"Место церемонии: {snapshot.ceremony_place}",
        f"Место банкета: {snapshot.banquet_venue}",
        f"Меню: {snapshot.menu}",
        f"Сценарий церемонии: {snapshot.ceremony_script}",
        f"План фотосессии: {snapshot.photo_plan}",
    ]
    return "\n".join(lines)


def format_plan_text(plan: WeddingPlan) -> str:
    """Возвращает многострочное текстовое описание плана."""
    return format_snapshot_text(PlanSnapshot.from_plan(plan))
