"""Мяч и физика отскоков в духе классического Pong."""

from __future__ import annotations

from collections import deque
from dataclasses import dataclass, field
from typing import Any

import pygame

from src.entities.paddle import Paddle


@dataclass
class Ball:
    """Мяч с траекторией и следом (анимация полёта)."""

    x: float
    y: float
    radius: int
    vx: float
    vy: float
    speed: float
    color: tuple[int, int, int]
    trail_length: int = 8
    trail: deque[tuple[float, float]] = field(default_factory=deque)

    @classmethod
    def from_config(cls, cfg: dict[str, Any], center_x: float, center_y: float) -> Ball:
        return cls(
            x=center_x,
            y=center_y,
            radius=int(cfg["radius"]),
            vx=0.0,
            vy=0.0,
            speed=float(cfg["speed_initial"]),
            color=tuple(cfg["color"]),
            trail_length=int(cfg.get("trail_length", 8)),
        )

    def serve(self, direction: int, angle: float = 0.25) -> None:
        """direction: -1 влево (к ИИ), +1 вправо (к игроку)."""
        import math

        self.vx = direction * self.speed * math.cos(angle)
        self.vy = self.speed * math.sin(angle) * (1 if direction > 0 else -1)
        self.trail.clear()

    def update_trail(self) -> None:
        self.trail.appendleft((self.x, self.y))
        while len(self.trail) > self.trail_length:
            self.trail.pop()

    def move(self, dt: float) -> None:
        self.update_trail()
        self.x += self.vx * dt
        self.y += self.vy * dt

    def bounce_wall(self, top: float, bottom: float) -> bool:
        hit = False
        if self.y - self.radius <= top:
            self.y = top + self.radius
            self.vy = abs(self.vy)
            hit = True
        elif self.y + self.radius >= bottom:
            self.y = bottom - self.radius
            self.vy = -abs(self.vy)
            hit = True
        return hit

    def bounce_paddle(self, paddle: Paddle, speed_increase: float, max_speed: float) -> bool:
        if not self._collides_paddle(paddle):
            return False
        # Угол зависит от точки удара по ракетке (классическое поведение Pong)
        relative = (self.y - paddle.center_y()) / (paddle.height / 2)
        relative = max(-1.0, min(1.0, relative))
        import math

        angle = relative * (math.pi / 3)
        direction = 1 if paddle.x < self.x else -1
        self.speed = min(max_speed, self.speed + speed_increase)
        self.vx = direction * self.speed * math.cos(angle)
        self.vy = self.speed * math.sin(angle)
        # Выталкиваем мяч из ракетки
        if direction > 0:
            self.x = paddle.x + paddle.width + self.radius
        else:
            self.x = paddle.x - self.radius
        return True

    def _collides_paddle(self, paddle: Paddle) -> bool:
        return (
            self.x + self.radius >= paddle.x
            and self.x - self.radius <= paddle.x + paddle.width
            and self.y + self.radius >= paddle.y
            and self.y - self.radius <= paddle.y + paddle.height
        )

    def draw(self, surface: pygame.Surface, fade_trail: bool = True) -> None:
        if fade_trail:
            for i, (tx, ty) in enumerate(reversed(list(self.trail))):
                alpha = int(40 + 180 * (i + 1) / max(1, len(self.trail)))
                r = max(2, self.radius - (len(self.trail) - i))
                shade = tuple(min(255, c * alpha // 255) for c in self.color)
                pygame.draw.circle(surface, shade, (int(tx), int(ty)), r)
        pygame.draw.circle(surface, self.color, (int(self.x), int(self.y)), self.radius)
