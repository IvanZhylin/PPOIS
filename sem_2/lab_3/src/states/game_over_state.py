"""Завершение игры и диалог нового рекорда."""

from __future__ import annotations

from typing import TYPE_CHECKING, Callable

import pygame

from src.interfaces import IGameState, IScoreRepository

if TYPE_CHECKING:
    from src.state_manager import StateManager


class GameOverState(IGameState):
    def __init__(
        self,
        manager: StateManager,
        repository: IScoreRepository,
        screen_size: tuple[int, int],
        player_score: int,
        show_high_score_dialog: bool,
        on_menu: Callable[[], IGameState],
        on_restart: Callable[[], IGameState],
    ) -> None:
        self._manager = manager
        self._repo = repository
        self._size = screen_size
        self._score = player_score
        self._dialog = show_high_score_dialog
        self._on_menu = on_menu
        self._on_restart = on_restart
        self._name = ""
        self._saved = False
        self._cursor_timer = 0.0

    def enter(self) -> None:
        if self._dialog and self._score > 0:
            # Автосохранение не делаем — ждём ввода имени
            pass
        elif self._repo.is_high_score(self._score) and self._score > 0:
            self._repo.add_entry("Игрок", self._score)
            self._saved = True

    def exit(self) -> None:
        pass

    def handle_event(self, event: pygame.event.Event) -> None:
        if event.type == pygame.KEYDOWN:
            if self._dialog and not self._saved:
                if event.key == pygame.K_RETURN and self._name.strip():
                    self._repo.add_entry(self._name.strip(), self._score)
                    self._saved = True
                elif event.key == pygame.K_BACKSPACE:
                    self._name = self._name[:-1]
                elif event.unicode and len(self._name) < 12 and event.unicode.isprintable():
                    self._name += event.unicode
            else:
                if event.key == pygame.K_RETURN:
                    self._manager.change(self._on_restart())
                elif event.key == pygame.K_ESCAPE:
                    self._manager.change(self._on_menu())

    def update(self, dt: float) -> None:
        self._cursor_timer += dt

    def draw(self, surface: pygame.Surface) -> None:
        w, h = self._size
        font_big = pygame.font.SysFont("dejavusans", 48)
        font_mid = pygame.font.SysFont("dejavusans", 32)
        font_small = pygame.font.SysFont("dejavusans", 24)

        title = font_big.render("Игра окончена", True, (255, 220, 80))
        surface.blit(title, (w // 2 - title.get_width() // 2, h // 4))

        result = font_mid.render(f"Ваш счёт: {self._score}", True, (220, 220, 230))
        surface.blit(result, (w // 2 - result.get_width() // 2, h // 4 + 70))

        y = h // 2
        if self._dialog and not self._saved:
            congrats = font_mid.render("Поздравляем! Новый рекорд!", True, (120, 255, 160))
            surface.blit(congrats, (w // 2 - congrats.get_width() // 2, y))
            y += 50
            prompt = font_small.render("Введите имя и нажмите Enter:", True, (200, 200, 210))
            surface.blit(prompt, (w // 2 - prompt.get_width() // 2, y))
            y += 36
            cursor = "|" if int(self._cursor_timer * 2) % 2 == 0 else " "
            name_line = font_mid.render((self._name or "") + cursor, True, (255, 255, 255))
            box = pygame.Rect(w // 2 - 160, y - 4, 320, 40)
            pygame.draw.rect(surface, (50, 50, 70), box, border_radius=6)
            pygame.draw.rect(surface, (120, 180, 255), box, 2, border_radius=6)
            surface.blit(name_line, (w // 2 - name_line.get_width() // 2, y))
        elif self._saved and self._dialog:
            saved = font_mid.render("Рекорд сохранён!", True, (120, 255, 160))
            surface.blit(saved, (w // 2 - saved.get_width() // 2, y))

        hint_y = h - 80
        hints = [
            "Enter — сыграть снова",
            "Esc — главное меню",
        ]
        for i, line in enumerate(hints):
            t = font_small.render(line, True, (140, 140, 160))
            surface.blit(t, (w // 2 - t.get_width() // 2, hint_y + i * 28))
