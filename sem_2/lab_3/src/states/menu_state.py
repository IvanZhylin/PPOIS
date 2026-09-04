"""Главное меню."""

from __future__ import annotations

from typing import TYPE_CHECKING, Callable

import pygame

from src.interfaces import IAudioService, IGameState

if TYPE_CHECKING:
    from src.state_manager import StateManager


class MenuState(IGameState):
    """Меню: начать игру, рекорды, справка, выход."""

    def __init__(
        self,
        manager: StateManager,
        config: dict,
        screen_size: tuple[int, int],
        audio: IAudioService,
        on_start: Callable[[], IGameState],
        on_scores: Callable[[], IGameState],
        on_help: Callable[[], IGameState],
        on_exit: Callable[[], None],
    ) -> None:
        self._manager = manager
        self._config = config
        self._size = screen_size
        self._audio = audio
        self._on_start = on_start
        self._on_scores = on_scores
        self._on_help = on_help
        self._on_exit = on_exit
        self._index = 0
        self._font_title: pygame.font.Font | None = None
        self._font_item: pygame.font.Font | None = None
        self._font_hint: pygame.font.Font | None = None

    def _fonts(self) -> None:
        if self._font_title is None:
            self._font_title = pygame.font.SysFont("dejavusans", self._config.get("font_size_title", 72))
            self._font_item = pygame.font.SysFont("dejavusans", self._config.get("font_size_item", 36))
            self._font_hint = pygame.font.SysFont("dejavusans", self._config.get("font_size_hint", 22))

    def enter(self) -> None:
        # Музыка запускается один раз на уровне приложения, не при каждом входе в меню
        pass

    def exit(self) -> None:
        pass

    def _move_selection(self, delta: int, items: list) -> None:
        new_index = (self._index + delta) % len(items)
        if new_index != self._index:
            self._index = new_index
            self._audio.play_menu_select()

    def handle_event(self, event: pygame.event.Event) -> None:
        items = self._config.get("items", [])
        if event.type == pygame.KEYDOWN:
            if event.key in (pygame.K_UP, pygame.K_w):
                self._move_selection(-1, items)
            elif event.key in (pygame.K_DOWN, pygame.K_s):
                self._move_selection(1, items)
            elif event.key in (pygame.K_RETURN, pygame.K_SPACE):
                self._activate(items[self._index]["id"])

    def _activate(self, item_id: str) -> None:
        self._audio.play_menu_select()
        if item_id == "start":
            self._manager.change(self._on_start())
        elif item_id == "scores":
            self._manager.change(self._on_scores())
        elif item_id == "help":
            self._manager.change(self._on_help())
        elif item_id == "exit":
            self._on_exit()

    def update(self, dt: float) -> None:
        pass

    def draw(self, surface: pygame.Surface) -> None:
        self._fonts()
        w, h = self._size
        title = self._config.get("title", "PONG")
        subtitle = self._config.get("subtitle", "")

        title_surf = self._font_title.render(title, True, self._config.get("selected_color", (255, 220, 80)))
        surface.blit(title_surf, (w // 2 - title_surf.get_width() // 2, h // 6))

        if subtitle and self._font_hint:
            sub = self._font_hint.render(subtitle, True, self._config.get("normal_color", (200, 200, 210)))
            surface.blit(sub, (w // 2 - sub.get_width() // 2, h // 6 + title_surf.get_height() + 8))

        items = self._config.get("items", [])
        start_y = h // 2 - len(items) * 28
        for i, item in enumerate(items):
            color = (
                self._config.get("selected_color", (255, 220, 80))
                if i == self._index
                else self._config.get("normal_color", (200, 200, 210))
            )
            prefix = "▶ " if i == self._index else "  "
            text = self._font_item.render(prefix + item["label"], True, color)
            surface.blit(text, (w // 2 - text.get_width() // 2, start_y + i * 56))

        if self._font_hint:
            hint = self._font_hint.render(self._config.get("hint", ""), True, (140, 140, 160))
            surface.blit(hint, (w // 2 - hint.get_width() // 2, h - 48))
