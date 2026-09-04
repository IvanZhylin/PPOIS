"""Абстракции состояний и сервисов (принцип инверсии зависимостей)."""

from __future__ import annotations

from abc import ABC, abstractmethod
from typing import TYPE_CHECKING, Any

if TYPE_CHECKING:
    import pygame


class IGameState(ABC):
    """Состояние приложения: меню, игра, справка и т.д."""

    @abstractmethod
    def enter(self) -> None:
        """Вызывается при переходе в состояние."""

    @abstractmethod
    def exit(self) -> None:
        """Вызывается при выходе из состояния."""

    @abstractmethod
    def handle_event(self, event: pygame.event.Event) -> None:
        """Обработка события pygame."""

    @abstractmethod
    def update(self, dt: float) -> None:
        """Обновление логики (dt в секундах)."""

    @abstractmethod
    def draw(self, surface: pygame.Surface) -> None:
        """Отрисовка кадра."""


class IScoreRepository(ABC):
    """Хранение таблицы рекордов."""

    @abstractmethod
    def get_entries(self) -> list[dict[str, Any]]:
        """Список записей, отсортированный по убыванию счёта."""

    @abstractmethod
    def get_top_score(self) -> int:
        """Лучший результат или 0."""

    @abstractmethod
    def is_high_score(self, score: int) -> bool:
        """Проверка, побит ли рекорд таблицы."""

    @abstractmethod
    def beats_first_place(self, score: int) -> bool:
        """Результат выше первой строки таблицы."""

    @abstractmethod
    def add_entry(self, name: str, score: int) -> None:
        """Добавить запись и сохранить на диск."""


class IAudioService(ABC):
    """Звуковые эффекты и музыка."""

    @abstractmethod
    def play_music(self) -> None: ...

    @abstractmethod
    def stop_music(self) -> None: ...

    @abstractmethod
    def play_paddle_hit(self) -> None: ...

    @abstractmethod
    def play_wall_hit(self) -> None: ...

    @abstractmethod
    def play_score(self) -> None: ...

    @abstractmethod
    def play_menu_select(self) -> None: ...

    @abstractmethod
    def play_game_over(self) -> None: ...
