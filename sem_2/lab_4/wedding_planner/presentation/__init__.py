"""Общие компоненты представления для CLI и GUI."""

from wedding_planner.presentation.plan_formatter import format_plan_text, format_snapshot_text
from wedding_planner.presentation.plan_snapshot import PlanSnapshot

__all__ = ["PlanSnapshot", "format_plan_text", "format_snapshot_text"]
