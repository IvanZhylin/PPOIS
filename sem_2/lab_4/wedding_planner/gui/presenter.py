"""Презентер MVP: связывает View и сервис приложения (модель л/р №1)."""

from __future__ import annotations

from collections.abc import Callable

from wedding_planner.exceptions import WeddingPlannerError
from wedding_planner.gui.contracts import IPlanView
from wedding_planner.planner import WeddingPlannerService
from wedding_planner.presentation.plan_snapshot import PlanSnapshot


class WeddingPlannerPresenter:
    """Обрабатывает команды UI и обновляет отображение состояния модели."""

    def __init__(self, service: WeddingPlannerService, view: IPlanView) -> None:
        self._service = service
        self._view = view
        view.attach_presenter(self)
        self.refresh_plan()

    def refresh_plan(self) -> None:
        """Синхронизирует View с актуальным состоянием сервиса."""
        snapshot = PlanSnapshot.from_plan(self._service.plan)
        self._view.show_plan(snapshot)

    def set_newlyweds(self, partner_one: str, partner_two: str) -> None:
        self._run_action(
            lambda: self._service.set_newlyweds(partner_one, partner_two),
            "Молодожены сохранены.",
        )

    def add_guest(self, guest_name: str) -> None:
        self._run_action(
            lambda: self._service.add_guest(guest_name),
            "Гость добавлен.",
        )

    def choose_date_place(
        self,
        date: str,
        ceremony_place: str,
        banquet_venue: str,
    ) -> None:
        self._run_action(
            lambda: self._service.apply_choose_date_place(
                date,
                ceremony_place,
                banquet_venue,
            ),
            "Дата и места сохранены.",
        )

    def choose_outfit(self, dress: str, rings: str) -> None:
        self._run_action(
            lambda: self._service.apply_choose_outfit(dress, rings),
            "Свадебный наряд сохранён.",
        )

    def prepare_menu(self, menu_text: str) -> None:
        items = [item.strip() for item in menu_text.split(",")]
        self._run_action(
            lambda: self._service.apply_prepare_menu(items),
            "Меню сохранено.",
        )

    def organize_ceremony(self, script: str) -> None:
        self._run_action(
            lambda: self._service.apply_organize_ceremony(script),
            "Организация церемонии сохранена.",
        )

    def prepare_photo_session(self, photo_plan: str) -> None:
        self._run_action(
            lambda: self._service.apply_prepare_photo_session(photo_plan),
            "План фотосессии сохранён.",
        )

    def _run_action(self, action: Callable[[], None], success_message: str) -> None:
        try:
            action()
            self.refresh_plan()
            self._view.show_info(success_message)
        except (WeddingPlannerError, ValueError) as error:
            self._view.show_error(str(error))
