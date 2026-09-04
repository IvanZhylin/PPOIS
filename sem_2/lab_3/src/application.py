"""Сборка приложения и фабрика состояний."""

from __future__ import annotations

from pathlib import Path

import pygame

from src.audio_manager import create_audio_manager
from src.config_loader import ConfigLoader
from src.score_repository import JsonScoreRepository
from src.state_manager import StateManager
from src.states.game_over_state import GameOverState
from src.states.game_state import PongGameState
from src.states.help_state import HelpState
from src.states.menu_state import MenuState
from src.states.scores_state import ScoresState


class PongApplication:
    """Точка сборки зависимостей и игрового цикла."""

    def __init__(self, base_dir: Path) -> None:
        self._base = base_dir
        self._config = ConfigLoader(base_dir)
        game_cfg = self._config.game
        disp = game_cfg["display"]
        self._width = int(disp["width"])
        self._height = int(disp["height"])
        self._title = disp.get("title", "Pong")
        self._fps = int(disp.get("fps", 60))
        self._bg = tuple(disp["background_color"])

        pygame.init()
        pygame.display.set_caption(self._title)
        self._screen = pygame.display.set_mode((self._width, self._height))
        self._clock = pygame.time.Clock()

        self._audio = create_audio_manager(base_dir, self._config.audio)
        self._scores = JsonScoreRepository(base_dir / "data/highscores.json")
        self._states = StateManager()
        self._running = True

        self._states.change(self._create_menu())
        # Первый кадр активирует меню; музыка — один раз на всё приложение
        self._states.update(0)

    def _screen_size(self) -> tuple[int, int]:
        return self._width, self._height

    def _create_menu(self) -> MenuState:
        return MenuState(
            manager=self._states,
            config=self._config.menu,
            screen_size=self._screen_size(),
            audio=self._audio,
            on_start=self._create_game,
            on_scores=self._create_scores,
            on_help=self._create_help,
            on_exit=self._quit,
        )

    def _create_help(self) -> HelpState:
        return HelpState(
            manager=self._states,
            config=self._config.help_text,
            screen_size=self._screen_size(),
            on_back=self._create_menu,
        )

    def _create_scores(self) -> ScoresState:
        return ScoresState(
            manager=self._states,
            repository=self._scores,
            screen_size=self._screen_size(),
            on_back=self._create_menu,
        )

    def _create_game(self) -> PongGameState:
        return PongGameState(
            manager=self._states,
            game_config=self._config.game,
            screen_size=self._screen_size(),
            audio=self._audio,
            scores=self._scores,
            on_menu=self._create_menu,
            on_game_over=self._create_game_over,
        )

    def _create_game_over(self, player_score: int, beats_top: bool) -> GameOverState:
        return GameOverState(
            manager=self._states,
            repository=self._scores,
            screen_size=self._screen_size(),
            player_score=player_score,
            show_high_score_dialog=beats_top,
            on_menu=self._create_menu,
            on_restart=self._create_game,
        )

    def _quit(self) -> None:
        self._running = False

    def run(self) -> None:
        """Событийно-ориентированный главный цикл."""
        self._audio.play_music()
        while self._running:
            dt_ms = self._clock.tick(self._fps)
            dt = dt_ms / 1000.0

            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self._quit()
                else:
                    self._states.handle_event(event)

            self._states.update(dt)
            self._screen.fill(self._bg)
            self._states.draw(self._screen)
            pygame.display.flip()

        self._audio.stop_music()
        pygame.quit()
