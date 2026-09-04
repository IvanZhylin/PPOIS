"""Игровой процесс Pong."""

from __future__ import annotations

from enum import Enum, auto
from typing import TYPE_CHECKING, Any, Callable

import pygame

from src.ai_controller import PaddleAI
from src.effects.particles import GoalExplosion, ScorePopup
from src.entities.ball import Ball
from src.entities.paddle import Paddle
from src.interfaces import IAudioService, IGameState, IScoreRepository

if TYPE_CHECKING:
    from src.state_manager import StateManager


class PlayPhase(Enum):
    SERVING = auto()
    PLAYING = auto()
    POINT_PAUSE = auto()
    GOAL_ANIM = auto()
    FINISHED = auto()


class PongGameState(IGameState):
    """Основная логика матча против ИИ."""

    def __init__(
        self,
        manager: StateManager,
        game_config: dict[str, Any],
        screen_size: tuple[int, int],
        audio: IAudioService,
        scores: IScoreRepository,
        on_menu: Callable[[], IGameState],
        on_game_over: Callable[[int, bool], IGameState],
    ) -> None:
        self._manager = manager
        self._cfg = game_config
        self._size = screen_size
        self._audio = audio
        self._scores = scores
        self._on_menu = on_menu
        self._on_game_over = on_game_over

        disp = game_config["display"]
        self._bg = tuple(disp["background_color"])
        court = game_config["court"]
        self._margin = int(court["margin"])

        rules = game_config["rules"]
        self._points_to_win = int(rules["points_to_win"])
        self._win_by_two = bool(rules.get("win_by_two", True))
        self._serve_delay = int(rules["serve_delay_ms"]) / 1000.0
        self._point_pause = int(rules["pause_after_point_ms"]) / 1000.0

        anim = game_config["animations"]
        self._explosion_count = int(anim["goal_explosion_particles"])
        self._explosion_dur = int(anim["goal_explosion_duration_ms"]) / 1000.0
        self._flash_dur = int(anim["paddle_hit_flash_ms"]) / 1000.0
        self._popup_dur = int(anim["score_popup_duration_ms"]) / 1000.0

        self._player_score = 0
        self._ai_score = 0
        self._phase = PlayPhase.SERVING
        self._phase_timer = 0.0
        self._serve_direction = 1
        self._explosion: GoalExplosion | None = None
        self._popup: ScorePopup | None = None
        self._paused = False

        self._player: Paddle | None = None
        self._ai_paddle: Paddle | None = None
        self._ball: Ball | None = None
        self._ai: PaddleAI | None = None
        self._keys_down: set[int] = set()

    def _court_bounds(self) -> tuple[float, float, float, float]:
        w, h = self._size
        m = self._margin
        return m, m, w - m, h - m

    def _reset_entities(self) -> None:
        left, top, right, bottom = self._court_bounds()
        w, h = self._size
        p_cfg = self._cfg["paddle"]
        margin_edge = int(p_cfg["margin_from_edge"])

        self._player = Paddle.from_config(
            p_cfg,
            x=left + margin_edge,
            y=(top + bottom) / 2 - int(p_cfg["height"]) / 2,
            is_player=True,
        )
        self._ai_paddle = Paddle.from_config(
            p_cfg,
            x=right - margin_edge - int(p_cfg["width"]),
            y=(top + bottom) / 2 - int(p_cfg["height"]) / 2,
            is_player=False,
        )
        self._ball = Ball.from_config(self._cfg["ball"], (left + right) / 2, (top + bottom) / 2)
        self._ball.trail_length = int(self._cfg["ball"].get("trail_length", 8))
        self._ai = PaddleAI(self._cfg["ai"])

    def enter(self) -> None:
        self._player_score = 0
        self._ai_score = 0
        self._phase = PlayPhase.SERVING
        self._phase_timer = self._serve_delay
        self._serve_direction = 1
        self._reset_entities()

    def exit(self) -> None:
        pass

    def handle_event(self, event: pygame.event.Event) -> None:
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                if self._paused:
                    self._paused = False
                else:
                    self._paused = True
            elif event.key == pygame.K_m and self._paused:
                self._manager.change(self._on_menu())
            else:
                self._keys_down.add(event.key)
        elif event.type == pygame.KEYUP:
            self._keys_down.discard(event.key)

    def _player_direction(self) -> int:
        up = {pygame.K_w, pygame.K_UP}
        down = {pygame.K_s, pygame.K_DOWN}
        if self._keys_down & up:
            return -1
        if self._keys_down & down:
            return 1
        return 0

    def _check_win(self) -> str | None:
        """Возвращает 'player' или 'ai' при победе."""
        p, a = self._player_score, self._ai_score
        target = self._points_to_win
        if self._win_by_two:
            if p >= target and p >= a + 2:
                return "player"
            if a >= target and a >= p + 2:
                return "ai"
            return None
        if p >= target:
            return "player"
        if a >= target:
            return "ai"
        return None

    def update(self, dt: float) -> None:
        if self._paused or self._player is None or self._ball is None:
            return

        if self._phase == PlayPhase.FINISHED:
            return

        if self._popup and not self._popup.finished:
            self._popup.update(dt)

        if self._phase == PlayPhase.GOAL_ANIM:
            if self._explosion:
                self._explosion.update(dt)
                if self._explosion.finished:
                    self._explosion = None
                    self._phase = PlayPhase.POINT_PAUSE
                    self._phase_timer = self._point_pause
            return

        if self._phase in (PlayPhase.SERVING, PlayPhase.POINT_PAUSE):
            self._phase_timer -= dt
            if self._phase_timer <= 0:
                self._begin_serve()
            return

        left, top, right, bottom = self._court_bounds()
        player = self._player
        ai_paddle = self._ai_paddle
        ball = self._ball

        player.move(self._player_direction(), dt, top, bottom)
        player.update(dt)
        if ai_paddle and self._ai:
            self._ai.update(ai_paddle, ball, dt, top, bottom)
            ai_paddle.update(dt)

        ball.move(dt)
        b_cfg = self._cfg["ball"]

        if ball.bounce_wall(top, bottom):
            self._audio.play_wall_hit()

        hit_player = ball.bounce_paddle(
            player,
            float(b_cfg["speed_increase_on_paddle"]),
            float(b_cfg["speed_max"]),
        )
        hit_ai = False
        if ai_paddle:
            hit_ai = ball.bounce_paddle(
                ai_paddle,
                float(b_cfg["speed_increase_on_paddle"]),
                float(b_cfg["speed_max"]),
            )

        if hit_player:
            player.flash(self._flash_dur)
            self._audio.play_paddle_hit()
        if hit_ai:
            ai_paddle.flash(self._flash_dur)
            self._audio.play_paddle_hit()

        # Пропуск мяча
        if ball.x < left:
            self._register_point(scored_by_ai=True)
        elif ball.x > right:
            self._register_point(scored_by_ai=False)

    def _register_point(self, scored_by_ai: bool) -> None:
        assert self._ball is not None
        if scored_by_ai:
            self._ai_score += 1
            self._serve_direction = -1
            popup_text = "Очко ИИ"
        else:
            self._player_score += 1
            self._serve_direction = 1
            popup_text = "+1"

        self._audio.play_score()
        self._explosion = GoalExplosion(
            self._ball.x,
            self._ball.y,
            self._ball.color,
            self._explosion_count,
            self._explosion_dur,
        )
        w, h = self._size
        self._popup = ScorePopup(popup_text, w / 2, h / 3, self._popup_dur)
        self._phase = PlayPhase.GOAL_ANIM
        self._ball.vx = self._ball.vy = 0

        winner = self._check_win()
        if winner:
            self._phase = PlayPhase.FINISHED
            self._audio.play_game_over()
            beats_top = (
                winner == "player"
                and self._scores.beats_first_place(self._player_score)
            )
            self._manager.change(
                self._on_game_over(self._player_score, beats_top)
            )

    def _begin_serve(self) -> None:
        assert self._ball is not None
        left, top, right, bottom = self._court_bounds()
        self._ball.x = (left + right) / 2
        self._ball.y = (top + bottom) / 2
        self._ball.speed = float(self._cfg["ball"]["speed_initial"])
        self._ball.serve(self._serve_direction)
        self._phase = PlayPhase.PLAYING

    def draw(self, surface: pygame.Surface) -> None:
        surface.fill(self._bg)
        w, h = self._size
        left, top, right, bottom = self._court_bounds()

        # Центральная линия
        dash = int(self._cfg["court"].get("center_line_dash", 20))
        gap = int(self._cfg["court"].get("center_line_gap", 15))
        cx = w // 2
        y = int(top)
        while y < bottom:
            pygame.draw.rect(surface, (60, 60, 90), (cx - 2, y, 4, dash))
            y += dash + gap

        font = pygame.font.SysFont("dejavusans", 48)
        score_l = font.render(str(self._player_score), True, (200, 200, 210))
        score_r = font.render(str(self._ai_score), True, (200, 200, 210))
        surface.blit(score_l, (w // 4 - score_l.get_width() // 2, 24))
        surface.blit(score_r, (3 * w // 4 - score_r.get_width() // 2, 24))

        if self._player:
            self._player.draw(surface)
        if self._ai_paddle:
            self._ai_paddle.draw(surface)
        if self._ball and self._phase != PlayPhase.GOAL_ANIM:
            self._ball.draw(surface, fade_trail=bool(self._cfg["ball"].get("trail_fade", True)))
        if self._explosion:
            self._explosion.draw(surface)
        if self._popup and not self._popup.finished:
            self._popup.draw(surface, pygame.font.SysFont("dejavusans", 36))

        if self._phase == PlayPhase.SERVING:
            hint = pygame.font.SysFont("dejavusans", 24).render("Подача...", True, (160, 160, 180))
            surface.blit(hint, (w // 2 - hint.get_width() // 2, h - 60))

        if self._paused:
            overlay = pygame.Surface(self._size, pygame.SRCALPHA)
            overlay.fill((0, 0, 0, 140))
            surface.blit(overlay, (0, 0))
            pfont = pygame.font.SysFont("dejavusans", 36)
            txt = pfont.render("Пауза — Esc: продолжить, M: меню", True, (255, 255, 255))
            surface.blit(txt, (w // 2 - txt.get_width() // 2, h // 2 - 18))
