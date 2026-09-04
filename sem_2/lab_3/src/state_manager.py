"""Переключение состояний (открыт для расширения новыми состояниями)."""

from __future__ import annotations

from typing import TYPE_CHECKING

import pygame

from src.interfaces import IGameState

if TYPE_CHECKING:
    pass


class StateManager:
    """Держит текущее состояние и делегирует цикл событий."""

    def __init__(self) -> None:
        self._current: IGameState | None = None
        self._next: IGameState | None = None

    @property
    def current(self) -> IGameState | None:
        return self._current

    def change(self, state: IGameState) -> None:
        self._next = state

    def _apply_pending(self) -> None:
        if self._next is None:
            return
        if self._current is not None:
            self._current.exit()
        self._current = self._next
        self._next = None
        self._current.enter()

    def handle_event(self, event: pygame.event.Event) -> None:
        if self._current:
            self._current.handle_event(event)

    def update(self, dt: float) -> None:
        self._apply_pending()
        if self._current:
            self._current.update(dt)
        self._apply_pending()

    def draw(self, surface: pygame.Surface) -> None:
        if self._current:
            self._current.draw(surface)
