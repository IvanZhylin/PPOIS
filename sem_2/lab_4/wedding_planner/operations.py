"""Операции предметной области для планирования свадьбы."""

from __future__ import annotations

from abc import ABC, abstractmethod

from wedding_planner.entities import WeddingPlan
from wedding_planner.exceptions import ValidationError


class PlanOperation(ABC):
    """Базовый интерфейс операции (ISP и OCP)."""

    @abstractmethod
    def execute(self, plan: WeddingPlan) -> None:
        """Применяет изменения операции к переданному плану."""


class ChooseDateAndPlaceOperation(PlanOperation):
    """Устанавливает дату свадьбы и места церемонии и банкета."""

    def __init__(self, date: str, ceremony_place: str, banquet_venue: str) -> None:
        self._date = date.strip()
        self._ceremony_place = ceremony_place.strip()
        self._banquet_venue = banquet_venue.strip()

    def execute(self, plan: WeddingPlan) -> None:
        if not self._date:
            raise ValidationError("Дата свадьбы не может быть пустой.")
        if not self._ceremony_place:
            raise ValidationError("Место церемонии не может быть пустым.")
        if not self._banquet_venue:
            raise ValidationError("Место банкета не может быть пустым.")
        plan.ceremony.date = self._date
        plan.ceremony.place = self._ceremony_place
        plan.banquet.venue = self._banquet_venue


class ChooseWeddingOutfitOperation(PlanOperation):
    """Устанавливает платье невесты и кольца."""

    def __init__(self, dress: str, rings: str) -> None:
        self._dress = dress.strip()
        self._rings = rings.strip()

    def execute(self, plan: WeddingPlan) -> None:
        if not self._dress:
            raise ValidationError("Описание платья невесты не может быть пустым.")
        if not self._rings:
            raise ValidationError("Описание колец не может быть пустым.")
        plan.bride_dress = self._dress
        plan.rings = self._rings


class PrepareWeddingMenuOperation(PlanOperation):
    """Устанавливает меню банкета."""

    def __init__(self, menu_items: list[str]) -> None:
        self._menu_items = [item.strip() for item in menu_items if item.strip()]

    def execute(self, plan: WeddingPlan) -> None:
        if not self._menu_items:
            raise ValidationError("Меню не может быть пустым.")
        plan.banquet.menu = self._menu_items


class OrganizeCeremonyOperation(PlanOperation):
    """Устанавливает сценарий церемонии."""

    def __init__(self, script: str) -> None:
        self._script = script.strip()

    def execute(self, plan: WeddingPlan) -> None:
        if not self._script:
            raise ValidationError("Сценарий церемонии не может быть пустым.")
        plan.ceremony.script = self._script


class PreparePhotoSessionOperation(PlanOperation):
    """Устанавливает план подготовки к фотосессии."""

    def __init__(self, photo_plan: str) -> None:
        self._photo_plan = photo_plan.strip()

    def execute(self, plan: WeddingPlan) -> None:
        if not self._photo_plan:
            raise ValidationError("План фотосессии не может быть пустым.")
        plan.ceremony.photo_plan = self._photo_plan
