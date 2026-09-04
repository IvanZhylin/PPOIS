"""Частицы для анимации «смерти» мяча при пропуске."""

from __future__ import annotations

import random
from dataclasses import dataclass

import pygame


@dataclass
class Particle:
    x: float
    y: float
    vx: float
    vy: float
    life: float
    color: tuple[int, int, int]
    size: float


class GoalExplosion:
    """Взрыв при пропуске мяча (анимация исчезновения)."""

    def __init__(
        self,
        x: float,
        y: float,
        color: tuple[int, int, int],
        count: int,
        duration: float,
    ) -> None:
        self._particles: list[Particle] = []
        for _ in range(count):
            angle = random.uniform(0, 6.28)
            speed = random.uniform(80, 280)
            self._particles.append(
                Particle(
                    x=x,
                    y=y,
                    vx=speed * random.uniform(-1, 1),
                    vy=speed * random.uniform(-1, 1),
                    life=duration * random.uniform(0.6, 1.0),
                    color=color,
                    size=random.uniform(3, 8),
                )
            )
        self._duration = duration
        self._elapsed = 0.0

    @property
    def finished(self) -> bool:
        return self._elapsed >= self._duration

    def update(self, dt: float) -> None:
        self._elapsed += dt
        for p in self._particles:
            p.life -= dt
            p.x += p.vx * dt
            p.y += p.vy * dt
            p.vy += 120 * dt

    def draw(self, surface: pygame.Surface) -> None:
        for p in self._particles:
            if p.life <= 0:
                continue
            alpha = max(0, min(255, int(255 * p.life / self._duration)))
            c = tuple(min(255, v * alpha // 255) for v in p.color)
            pygame.draw.circle(surface, c, (int(p.x), int(p.y)), int(p.size))


class ScorePopup:
    """Краткая анимация начисления очка."""

    def __init__(self, text: str, x: float, y: float, duration: float) -> None:
        self.text = text
        self.x = x
        self.y = y
        self.duration = duration
        self.elapsed = 0.0

    @property
    def finished(self) -> bool:
        return self.elapsed >= self.duration

    def update(self, dt: float) -> None:
        self.elapsed += dt

    def draw(self, surface: pygame.Surface, font: pygame.font.Font) -> None:
        t = self.elapsed / self.duration
        offset = int(30 * t)
        alpha = int(255 * (1 - t))
        color = (255, 220, 80, alpha)
        img = font.render(self.text, True, color[:3])
        surface.blit(img, (int(self.x - img.get_width() / 2), int(self.y - offset)))
