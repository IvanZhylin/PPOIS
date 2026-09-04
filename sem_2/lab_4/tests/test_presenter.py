"""Тесты презентера MVP (без tkinter)."""

from __future__ import annotations

from pathlib import Path

from wedding_planner.gui.presenter import WeddingPlannerPresenter
from wedding_planner.planner import WeddingPlannerService
from wedding_planner.presentation.plan_snapshot import PlanSnapshot
from wedding_planner.storage import JsonPlanRepository


class MockPlanView:
    """Заглушка View для проверки презентера."""

    def __init__(self) -> None:
        self.presenter: WeddingPlannerPresenter | None = None
        self.snapshots: list[PlanSnapshot] = []
        self.errors: list[str] = []
        self.infos: list[str] = []

    def attach_presenter(self, presenter: WeddingPlannerPresenter) -> None:
        self.presenter = presenter

    def show_plan(self, snapshot: PlanSnapshot) -> None:
        self.snapshots.append(snapshot)

    def show_error(self, message: str) -> None:
        self.errors.append(message)

    def show_info(self, message: str) -> None:
        self.infos.append(message)

    def run(self) -> None:
        pass


def test_presenter_refreshes_view_on_start(tmp_path: Path) -> None:
    data_file = tmp_path / "plan.json"
    service = WeddingPlannerService(JsonPlanRepository(str(data_file)))
    view = MockPlanView()

    WeddingPlannerPresenter(service, view)

    assert len(view.snapshots) == 1
    assert view.snapshots[0].newlyweds == "—"


def test_presenter_updates_plan_and_shows_info(tmp_path: Path) -> None:
    data_file = tmp_path / "plan.json"
    service = WeddingPlannerService(JsonPlanRepository(str(data_file)))
    view = MockPlanView()
    presenter = WeddingPlannerPresenter(service, view)

    presenter.set_newlyweds("Анна", "Марк")

    assert service.plan.newlyweds == ["Анна", "Марк"]
    assert view.snapshots[-1].newlyweds == "Анна, Марк"
    assert view.infos[-1] == "Молодожены сохранены."


def test_presenter_shows_validation_error(tmp_path: Path) -> None:
    data_file = tmp_path / "plan.json"
    service = WeddingPlannerService(JsonPlanRepository(str(data_file)))
    view = MockPlanView()
    presenter = WeddingPlannerPresenter(service, view)

    presenter.add_guest("   ")

    assert view.errors
    assert "пуст" in view.errors[-1].lower()


def test_presenter_all_operations(tmp_path: Path) -> None:
    data_file = tmp_path / "plan.json"
    service = WeddingPlannerService(JsonPlanRepository(str(data_file)))
    view = MockPlanView()
    presenter = WeddingPlannerPresenter(service, view)

    presenter.set_newlyweds("Катя", "Иван")
    presenter.add_guest("Ольга")
    presenter.choose_date_place("2026-06-01", "ЗАГС", "Ресторан")
    presenter.choose_outfit("Белое платье", "Золотые кольца")
    presenter.prepare_menu("Салат, Стейк, Торт")
    presenter.organize_ceremony("Обмен кольцами")
    presenter.prepare_photo_session("Парк")

    snapshot = view.snapshots[-1]
    assert snapshot.newlyweds == "Катя, Иван"
    assert snapshot.guest_count == 1
    assert snapshot.ceremony_date == "2026-06-01"
    assert snapshot.bride_dress == "Белое платье"
    assert "Салат" in snapshot.menu


def test_presenter_duplicate_guest_error(tmp_path: Path) -> None:
    data_file = tmp_path / "plan.json"
    service = WeddingPlannerService(JsonPlanRepository(str(data_file)))
    service.add_guest("Пётр")
    view = MockPlanView()
    presenter = WeddingPlannerPresenter(service, view)

    presenter.add_guest("Пётр")
    assert any("уже" in err.lower() for err in view.errors)
