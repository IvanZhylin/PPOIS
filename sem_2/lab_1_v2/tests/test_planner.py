"""Unit tests for wedding planner service."""

from __future__ import annotations

import json
from pathlib import Path

import pytest

from wedding_planner.entities import WeddingPlan
from wedding_planner.exceptions import ValidationError
from wedding_planner.planner import WeddingPlannerService
from wedding_planner.storage import JsonPlanRepository


def test_set_newlyweds_and_add_guest(tmp_path: Path) -> None:
    data_file = tmp_path / "plan.json"
    service = WeddingPlannerService(JsonPlanRepository(str(data_file)))

    service.set_newlyweds("Anna", "Mark")
    service.add_guest("Olga")

    assert service.plan.newlyweds == ["Anna", "Mark"]
    assert service.plan.guests == ["Olga"]


def test_wedding_operations_update_entities(tmp_path: Path) -> None:
    data_file = tmp_path / "plan.json"
    service = WeddingPlannerService(JsonPlanRepository(str(data_file)))

    service.apply_choose_date_place("2026-09-12", "City Hall", "Royal Hall")
    service.apply_choose_outfit("Classic white dress", "Platinum rings")
    service.apply_prepare_menu(["Salad", "Steak", "Cake"])
    service.apply_organize_ceremony("Vows, exchange rings, dance")
    service.apply_prepare_photo_session("Park and riverside locations")

    plan = service.plan
    assert plan.ceremony.date == "2026-09-12"
    assert plan.ceremony.place == "City Hall"
    assert plan.banquet.venue == "Royal Hall"
    assert plan.bride_dress == "Classic white dress"
    assert plan.rings == "Platinum rings"
    assert plan.banquet.menu == ["Salad", "Steak", "Cake"]
    assert plan.ceremony.script == "Vows, exchange rings, dance"
    assert plan.ceremony.photo_plan == "Park and riverside locations"


def test_validation_errors_are_raised(tmp_path: Path) -> None:
    data_file = tmp_path / "plan.json"
    service = WeddingPlannerService(JsonPlanRepository(str(data_file)))

    with pytest.raises(ValidationError):
        service.set_newlyweds("", "Mark")

    with pytest.raises(ValidationError):
        service.add_guest(" ")

    with pytest.raises(ValidationError):
        service.apply_prepare_menu([])


def test_state_persisted_between_service_instances(
    tmp_path: Path,
) -> None:
    data_file = tmp_path / "plan.json"
    repository = JsonPlanRepository(str(data_file))

    first = WeddingPlannerService(repository)
    first.set_newlyweds("Kate", "John")
    first.add_guest("Bob")

    second = WeddingPlannerService(repository)
    assert second.plan.newlyweds == ["Kate", "John"]
    assert second.plan.guests == ["Bob"]


def test_plan_from_dict_safe_defaults() -> None:
    plan = WeddingPlan.from_dict({"newlyweds": ["A", "B"], "banquet": {"menu": [1, 2]}})
    assert plan.newlyweds == ["A", "B"]
    assert plan.banquet.menu == ["1", "2"]
    assert plan.ceremony.place == ""


def test_json_file_contains_serialized_plan(tmp_path: Path) -> None:
    data_file = tmp_path / "plan.json"
    service = WeddingPlannerService(JsonPlanRepository(str(data_file)))
    service.apply_organize_ceremony("Simple script")

    raw = json.loads(data_file.read_text(encoding="utf-8"))
    assert raw["ceremony"]["script"] == "Simple script"
