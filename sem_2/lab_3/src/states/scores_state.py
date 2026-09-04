"""Таблица рекордов."""

from __future__ import annotations

from typing import TYPE_CHECKING, Callable

import pygame

from src.interfaces import IGameState, IScoreRepository

if TYPE_CHECKING:
    from src.state_manager import StateManager


class ScoresState(IGameState):
    def __init__(
        self,
        manager: StateManager,
        repository: IScoreRepository,
        screen_size: tuple[int, int],
        on_back: Callable[[], IGameState],
    ) -> None:
        self._manager = manager
        self._repo = repository
        self._size = screen_size
        self._on_back = on_back

    def enter(self) -> None:
        pass

    def exit(self) -> None:
        pass

    def handle_event(self, event: pygame.event.Event) -> None:
        if event.type == pygame.KEYDOWN and event.key in (pygame.K_ESCAPE, pygame.K_RETURN):
            self._manager.change(self._on_back())

    def update(self, dt: float) -> None:
        pass

    def draw(self, surface: pygame.Surface) -> None:
        w, _ = self._size
        font_title = pygame.font.SysFont("dejavusans", 44)
        font_row = pygame.font.SysFont("dejavusans", 28)
        font_hint = pygame.font.SysFont("dejavusans", 22)

        title = font_title.render("Таблица рекордов", True, (255, 220, 80))
        surface.blit(title, (w // 2 - title.get_width() // 2, 40))

        header = font_row.render("  №    Имя          Очки    Дата", True, (140, 180, 220))
        surface.blit(header, (w // 2 - header.get_width() // 2, 110))

        entries = self._repo.get_entries()
        y = 160
        for i, e in enumerate(entries, start=1):
            row = font_row.render(
                f"{i:2}.  {e.get('name', '?'):<12}  {e.get('score', 0):>3}     {e.get('date', '')}",
                True,
                (220, 220, 230) if i == 1 else (180, 180, 195),
            )
            surface.blit(row, (w // 2 - row.get_width() // 2, y))
            y += 36

        if not entries:
            empty = font_row.render("Пока нет записей", True, (160, 160, 180))
            surface.blit(empty, (w // 2 - empty.get_width() // 2, 200))

        hint = font_hint.render("Esc / Enter — в меню", True, (140, 140, 160))
        surface.blit(hint, (w // 2 - hint.get_width() // 2, self._size[1] - 48))
