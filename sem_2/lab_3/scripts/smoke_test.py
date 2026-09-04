#!/usr/bin/env python3
"""Автоматическая проверка запуска на Linux (без ручного ввода)."""

from __future__ import annotations

import sys
from pathlib import Path

import pygame

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT))

from src.application import PongApplication  # noqa: E402
from src.states.game_state import PongGameState  # noqa: E402
from src.states.help_state import HelpState  # noqa: E402
from src.states.menu_state import MenuState  # noqa: E402
from src.states.scores_state import ScoresState  # noqa: E402


def post_key(key: int) -> None:
    pygame.event.post(pygame.event.Event(pygame.KEYDOWN, key=key, unicode=""))


def state_name(app: PongApplication) -> str:
    cur = app._states.current
    return type(cur).__name__ if cur else "None"


def main() -> int:
    print("=== Smoke-тест Pong (Arch Linux) ===")
    print(f"Python: {sys.version.split()[0]}")
    print(f"pygame: {pygame.version.ver}")
    print(f"DISPLAY: {__import__('os').environ.get('DISPLAY', '(нет)')}")

    errors: list[str] = []

    try:
        app = PongApplication(ROOT)
    except Exception as exc:
        print(f"FAIL: инициализация: {exc}")
        return 1

    _run_frames(app, 2)
    print("OK: pygame.init(), окно и аудио")

    if not isinstance(app._states.current, MenuState):
        errors.append(f"Ожидалось MenuState, получено {state_name(app)}")
    else:
        app._states.current._index = 0

    # Справка (пункт 3: индекс 2)
    _select_menu_item(app, 2)
    _run_frames(app, 5)
    if not isinstance(app._states.current, HelpState):
        errors.append(f"Переход в справку: {state_name(app)}")
    else:
        print("OK: экран справки")
    post_key(pygame.K_ESCAPE)
    _run_frames(app, 3)

    if not isinstance(app._states.current, MenuState):
        errors.append(f"Возврат из справки: {state_name(app)}")

    # Рекорды (пункт 2: индекс 1)
    _select_menu_item(app, 1)
    _run_frames(app, 5)
    if not isinstance(app._states.current, ScoresState):
        errors.append(f"Переход в рекорды: {state_name(app)}")
    else:
        print("OK: таблица рекордов")
    post_key(pygame.K_ESCAPE)
    _run_frames(app, 3)

    # Игра (пункт 1: индекс 0)
    _select_menu_item(app, 0)
    _run_frames(app, 120)
    if not isinstance(app._states.current, PongGameState):
        errors.append(f"Старт игры: {state_name(app)}")
    else:
        gs = app._states.current
        print(
            f"OK: игровой процесс (счёт {gs._player_score}:{gs._ai_score}, "
            f"фаза {gs._phase.name})"
        )

    # Движение ракетки и кадры
    post_key(pygame.K_w)
    _run_frames(app, 60)
    post_key(pygame.K_w)  # KEYUP через отпускание — симулируем повторным не нужно
    pygame.event.post(pygame.event.Event(pygame.KEYUP, key=pygame.K_w))
    _run_frames(app, 30)

    # Выход через меню: пауза не нужна — шлём QUIT
    pygame.event.post(pygame.event.Event(pygame.QUIT))
    _run_frames(app, 2)

    if errors:
        for e in errors:
            print(f"FAIL: {e}")
        return 1

    print("=== Все проверки пройдены ===")
    return 0


def _select_menu_item(app: PongApplication, index: int) -> None:
    """Выбор пункта меню по индексу и Enter."""
    menu = app._states.current
    if isinstance(menu, MenuState):
        menu._index = index
    post_key(pygame.K_RETURN)


def _run_frames(app: PongApplication, n: int) -> None:
    for _ in range(n):
        if not app._running:
            break
        dt_ms = app._clock.tick(app._fps)
        dt = dt_ms / 1000.0
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                app._quit()
            else:
                app._states.handle_event(event)
        app._states.update(dt)
        app._screen.fill(app._bg)
        app._states.draw(app._screen)
        pygame.display.flip()
    if not app._running:
        app._audio.stop_music()
        pygame.quit()


if __name__ == "__main__":
    raise SystemExit(main())
