"""Unit tests for wedding planner service (unittest style)."""

from __future__ import annotations

import json
import tempfile
import unittest
from pathlib import Path

from wedding_planner.entities import WeddingPlan
from wedding_planner.exceptions import ValidationError
from wedding_planner.planner import WeddingPlannerService
from wedding_planner.storage import JsonPlanRepository


class TestWeddingPlanner(unittest.TestCase):
    """Test case for wedding planner service."""

    def setUp(self) -> None:
        """Create a temporary directory and file path for each test."""
        self.temp_dir = tempfile.TemporaryDirectory()
        self.data_file = Path(self.temp_dir.name) / "plan.json"
        self.addCleanup(self.temp_dir.cleanup)

    def test_set_newlyweds_and_add_guest(self) -> None:
        service = WeddingPlannerService(JsonPlanRepository(str(self.data_file)))

        service.set_newlyweds("Anna", "Mark")
        service.add_guest("Olga")

        self.assertEqual(service.plan.newlyweds, ["Anna", "Mark"])
        self.assertEqual(service.plan.guests, ["Olga"])

    def test_wedding_operations_update_entities(self) -> None:
        service = WeddingPlannerService(JsonPlanRepository(str(self.data_file)))

        service.apply_choose_date_place("2026-09-12", "City Hall", "Royal Hall")
        service.apply_choose_outfit("Classic white dress", "Platinum rings")
        service.apply_prepare_menu(["Salad", "Steak", "Cake"])
        service.apply_organize_ceremony("Vows, exchange rings, dance")
        service.apply_prepare_photo_session("Park and riverside locations")

        plan = service.plan
        self.assertEqual(plan.ceremony.date, "2026-09-12")
        self.assertEqual(plan.ceremony.place, "City Hall")
        self.assertEqual(plan.banquet.venue, "Royal Hall")
        self.assertEqual(plan.bride_dress, "Classic white dress")
        self.assertEqual(plan.rings, "Platinum rings")
        self.assertEqual(plan.banquet.menu, ["Salad", "Steak", "Cake"])
        self.assertEqual(plan.ceremony.script, "Vows, exchange rings, dance")
        self.assertEqual(plan.ceremony.photo_plan, "Park and riverside locations")

    def test_validation_errors_are_raised(self) -> None:
        service = WeddingPlannerService(JsonPlanRepository(str(self.data_file)))

        with self.assertRaises(ValidationError):
            service.set_newlyweds("", "Mark")

        with self.assertRaises(ValidationError):
            service.add_guest(" ")

        with self.assertRaises(ValidationError):
            service.apply_prepare_menu([])

    def test_state_persisted_between_service_instances(self) -> None:
        repository = JsonPlanRepository(str(self.data_file))

        first = WeddingPlannerService(repository)
        first.set_newlyweds("Kate", "John")
        first.add_guest("Bob")

        second = WeddingPlannerService(repository)
        self.assertEqual(second.plan.newlyweds, ["Kate", "John"])
        self.assertEqual(second.plan.guests, ["Bob"])

    def test_plan_from_dict_safe_defaults(self) -> None:
        plan = WeddingPlan.from_dict(
            {"newlyweds": ["A", "B"], "banquet": {"menu": [1, 2]}}
        )
        self.assertEqual(plan.newlyweds, ["A", "B"])
        self.assertEqual(plan.banquet.menu, ["1", "2"])
        self.assertEqual(plan.ceremony.place, "")

    def test_json_file_contains_serialized_plan(self) -> None:
        service = WeddingPlannerService(JsonPlanRepository(str(self.data_file)))
        service.apply_organize_ceremony("Simple script")

        raw = json.loads(self.data_file.read_text(encoding="utf-8"))
        self.assertEqual(raw["ceremony"]["script"], "Simple script")


if __name__ == "__main__":
    unittest.main()