"""Ракетка игрока или ИИ."""

from __future__ import annotations

from dataclasses import dataclass
from typing import Any

import pygame


@dataclass
class Paddle:
    """Прямоугольная ракетка с вертикальным движением."""

    x: float
    y: float
    width: int
    height: int
    speed: float
    color: tuple[int, int, int]
    is_player: bool = True
    flash_timer: float = 0.0

    @classmethod
    def from_config(
        cls,
        cfg: dict[str, Any],
        x: float,
        y: float,
        is_player: bool,
    ) -> Paddle:
        color_key = "color_player" if is_player else "color_ai"
        return cls(
            x=x,
            y=y,
            width=int(cfg["width"]),
            height=int(cfg["height"]),
            speed=float(cfg["speed"]),
            color=tuple(cfg[color_key]),
            is_player=is_player,
        )

    @property
    def rect(self) -> pygame.Rect:
        return pygame.Rect(int(self.x), int(self.y), self.width, self.height)

    def move(self, direction: int, dt: float, top: float, bottom: float) -> None:
        """direction: -1 вверх, +1 вниз, 0 — стоп."""
        self.y += direction * self.speed * dt
        self.y = max(top, min(bottom - self.height, self.y))

    def center_y(self) -> float:
        return self.y + self.height / 2

    def flash(self, duration: float) -> None:
        self.flash_timer = duration

    def update(self, dt: float) -> None:
        if self.flash_timer > 0:
            self.flash_timer = max(0.0, self.flash_timer - dt)

    def draw(self, surface: pygame.Surface) -> None:
        color = self.color
        if self.flash_timer > 0:
            color = (
                min(255, color[0] + 80),
                min(255, color[1] + 80),
                min(255, color[2] + 80),
            )
        pygame.draw.rect(surface, color, self.rect, border_radius=4)
