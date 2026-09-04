"""Простой ИИ для правой ракетки (отдельный класс — SRP)."""

from __future__ import annotations

from typing import Any

from src.entities.ball import Ball
from src.entities.paddle import Paddle


class PaddleAI:
    """Следует за мячом с ограниченной скоростью и задержкой реакции."""

    def __init__(self, config: dict[str, Any]) -> None:
        self._delay = float(config.get("reaction_delay", 0.08))
        self._speed_factor = float(config.get("max_speed_factor", 0.92))
        self._target_y: float | None = None
        self._timer = 0.0

    def update(self, paddle: Paddle, ball: Ball, dt: float, top: float, bottom: float) -> None:
        self._timer += dt
        if self._timer >= self._delay:
            self._timer = 0.0
            self._target_y = ball.y - paddle.height / 2

        if self._target_y is None:
            return

        diff = self._target_y - paddle.y
        direction = 0
        threshold = 8
        if diff < -threshold:
            direction = -1
        elif diff > threshold:
            direction = 1

        # Ограничиваем эффективную скорость ИИ
        original_speed = paddle.speed
        paddle.speed = original_speed * self._speed_factor
        paddle.move(direction, dt, top, bottom)
        paddle.speed = original_speed
