"""Контракты представления для паттерна MVP."""

from __future__ import annotations

from typing import TYPE_CHECKING, Protocol

from wedding_planner.presentation.plan_snapshot import PlanSnapshot

if TYPE_CHECKING:
    from wedding_planner.gui.presenter import WeddingPlannerPresenter


class IPlanView(Protocol):
    """Пассивное представление: отображает данные и делегирует действия презентеру."""

    def attach_presenter(self, presenter: "WeddingPlannerPresenter") -> None:
        """Связывает View с презентером (инициализация MVP)."""

    def show_plan(self, snapshot: PlanSnapshot) -> None:
        """Обновляет блок отображения текущего состояния модели."""

    def show_error(self, message: str) -> None:
        """Показывает сообщение об ошибке валидации или домена."""

    def show_info(self, message: str) -> None:
        """Показывает информационное сообщение об успешной операции."""

    def run(self) -> None:
        """Запускает главный цикл интерфейса."""
