"""Сервис приложения, связывающий доменную логику и сохранение."""

from __future__ import annotations

from wedding_planner.entities import WeddingPlan
from wedding_planner.exceptions import ValidationError
from wedding_planner.operations import (
    ChooseDateAndPlaceOperation,
    ChooseWeddingOutfitOperation,
    OrganizeCeremonyOperation,
    PlanOperation,
    PreparePhotoSessionOperation,
    PrepareWeddingMenuOperation,
)
from wedding_planner.storage import PlanRepository


class WeddingPlannerService:
    """Высокоуровневые сценарии использования планировщика."""

    def __init__(self, repository: PlanRepository) -> None:
        self._repository = repository
        self._plan = repository.load()

    @property
    def plan(self) -> WeddingPlan:
        """Возвращает текущее состояние плана."""
        return self._plan

    def set_newlyweds(self, partner_one: str, partner_two: str) -> None:
        operation_values = [partner_one.strip(), partner_two.strip()]
        if not all(operation_values):
            raise ValidationError("Необходимо указать имена обоих молодоженов.")
        self._plan.newlyweds = operation_values
        self._repository.save(self._plan)

    def add_guest(self, guest_name: str) -> None:
        guest_name = guest_name.strip()
        if not guest_name:
            raise ValidationError("Имя гостя не может быть пустым.")
        if guest_name in self._plan.guests:
            raise ValidationError("Такой гость уже добавлен.")
        self._plan.guests.append(guest_name)
        self._repository.save(self._plan)

    def apply_choose_date_place(
        self,
        date: str,
        ceremony_place: str,
        banquet_venue: str,
    ) -> None:
        self._apply_operation(
            ChooseDateAndPlaceOperation(date, ceremony_place, banquet_venue),
        )

    def apply_choose_outfit(self, dress: str, rings: str) -> None:
        self._apply_operation(ChooseWeddingOutfitOperation(dress, rings))

    def apply_prepare_menu(self, menu_items: list[str]) -> None:
        self._apply_operation(PrepareWeddingMenuOperation(menu_items))

    def apply_organize_ceremony(self, script: str) -> None:
        self._apply_operation(OrganizeCeremonyOperation(script))

    def apply_prepare_photo_session(self, photo_plan: str) -> None:
        self._apply_operation(PreparePhotoSessionOperation(photo_plan))

    def _apply_operation(self, operation: PlanOperation) -> None:
        operation.execute(self._plan)
        self._repository.save(self._plan)
