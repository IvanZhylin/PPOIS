"""Экран справки с правилами."""

from __future__ import annotations

from typing import TYPE_CHECKING, Callable

import pygame

from src.interfaces import IGameState

if TYPE_CHECKING:
    from src.state_manager import StateManager


class HelpState(IGameState):
    def __init__(
        self,
        manager: StateManager,
        config: dict,
        screen_size: tuple[int, int],
        on_back: Callable[[], IGameState],
    ) -> None:
        self._manager = manager
        self._config = config
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
        w, h = self._size
        font_title = pygame.font.SysFont("dejavusans", 40)
        font_head = pygame.font.SysFont("dejavusans", 28)
        font_body = pygame.font.SysFont("dejavusans", 22)

        title = font_title.render(self._config.get("title", "Справка"), True, (255, 220, 80))
        surface.blit(title, (w // 2 - title.get_width() // 2, 32))

        y = 100
        for section in self._config.get("sections", []):
            head = font_head.render(section.get("heading", ""), True, (120, 200, 255))
            surface.blit(head, (48, y))
            y += head.get_height() + 8
            for line in section.get("lines", []):
                body = font_body.render(line, True, (210, 210, 220))
                surface.blit(body, (64, y))
                y += body.get_height() + 4
            y += 12

        footer = font_body.render(self._config.get("footer", ""), True, (160, 160, 180))
        surface.blit(footer, (w // 2 - footer.get_width() // 2, h - 40))
