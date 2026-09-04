"""Управление звуком: файлы из конфига или процедурная генерация."""

from __future__ import annotations

import math
from pathlib import Path
from typing import Any

import pygame

from src.interfaces import IAudioService


def _mixer_available() -> bool:
    """Проверка доступности SDL_mixer (на Arch без libsdl2_mixer — нет)."""
    if not hasattr(pygame, "mixer"):
        return False
    try:
        if isinstance(pygame.mixer, pygame.MissingModule):  # type: ignore[attr-defined]
            return False
    except AttributeError:
        pass
    try:
        pygame.mixer.get_init()
        return True
    except (NotImplementedError, pygame.error, ModuleNotFoundError):
        try:
            pygame.mixer.init(frequency=22050, size=-16, channels=1, buffer=512)
            return True
        except (NotImplementedError, pygame.error, ModuleNotFoundError):
            return False


class NullAudioManager(IAudioService):
    """Заглушка, если mixer недоступен — игра запускается без звука."""

    def play_music(self) -> None: ...
    def stop_music(self) -> None: ...
    def play_paddle_hit(self) -> None: ...
    def play_wall_hit(self) -> None: ...
    def play_score(self) -> None: ...
    def play_menu_select(self) -> None: ...
    def play_game_over(self) -> None: ...


def create_audio_manager(base_dir: Path, config: dict[str, Any]) -> IAudioService:
    if _mixer_available():
        return AudioManager(base_dir, config)
    return NullAudioManager()


def _make_tone(
    frequency: float,
    duration: float = 0.08,
    volume: float = 0.3,
    sample_rate: int = 22050,
) -> pygame.mixer.Sound:
    """Синтез короткого тона (если нет wav-файлов)."""
    n_samples = int(sample_rate * duration)
    buf = bytearray()
    for i in range(n_samples):
        t = i / sample_rate
        envelope = 1.0 - (i / n_samples)
        sample = int(
            127
            + 127
            * volume
            * envelope
            * math.sin(2 * math.pi * frequency * t)
        )
        buf.append(max(0, min(255, sample)))
    sound = pygame.mixer.Sound(buffer=bytes(buf))
    sound.set_volume(volume)
    return sound


def _write_ambient_music_wav(path: Path, sample_rate: int = 22050) -> None:
    """Тихая фоновая петля без резких «пиков» (для mixer.music)."""
    import struct
    import wave

    duration = 12.0
    n_samples = int(sample_rate * duration)
    path.parent.mkdir(parents=True, exist_ok=True)
    with wave.open(str(path), "w") as wf:
        wf.setnchannels(1)
        wf.setsampwidth(2)
        wf.setframerate(sample_rate)
        frames = bytearray()
        for i in range(n_samples):
            t = i / sample_rate
            # Плавный пад, без скачков нот
            env = 0.55 + 0.45 * math.sin(2 * math.pi * t / duration)
            sample = int(
                8000
                * env
                * (
                    0.6 * math.sin(2 * math.pi * 110 * t)
                    + 0.25 * math.sin(2 * math.pi * 164.81 * t)
                )
            )
            frames.extend(struct.pack("<h", max(-32767, min(32767, sample))))
        wf.writeframes(frames)


class AudioManager(IAudioService):
    """Загрузка звуков из конфигурации с запасным синтезом."""

    def __init__(self, base_dir: Path, config: dict[str, Any]) -> None:
        self._config = config
        self._base = base_dir
        self._master = float(config.get("master_volume", 0.7))
        self._music_vol = float(config.get("music_volume", 0.4)) * self._master
        self._sfx_vol = float(config.get("sfx_volume", 0.8)) * self._master
        self._music_enabled = bool(config.get("music_enabled", True))
        self._sfx_enabled = bool(config.get("sfx_enabled", True))
        self._procedural = bool(config.get("procedural_fallback", True))
        self._sounds: dict[str, pygame.mixer.Sound | None] = {}
        self._music_playing = False
        self._menu_sfx_channel: pygame.mixer.Channel | None = None
        self._init_mixer()
        self._load_sounds()
        self._prepare_music()

    def _init_mixer(self) -> None:
        if not _mixer_available():
            raise RuntimeError("pygame.mixer недоступен")

    def _load_file_or_none(self, rel: str) -> pygame.mixer.Sound | None:
        path = self._base / rel
        if path.exists():
            try:
                s = pygame.mixer.Sound(str(path))
                return s
            except pygame.error:
                return None
        return None

    def _load_sounds(self) -> None:
        files = self._config.get("files", {})
        mapping = {
            "paddle_hit": ("paddle_hit", 440),
            "wall_hit": ("wall_hit", 280),
            "score": ("score", 150),
            "menu_select": ("menu_select", 520),
            "game_over": ("game_over", 110),
        }
        for key, (cfg_key, freq) in mapping.items():
            rel = files.get(cfg_key, "")
            loaded = self._load_file_or_none(rel) if rel else None
            if loaded is None and self._procedural:
                loaded = _make_tone(freq, duration=0.1 if key != "game_over" else 0.35, volume=0.35)
            self._sounds[key] = loaded

    def _prepare_music(self) -> None:
        """Фон — через mixer.music (один поток, без наложения)."""
        files = self._config.get("files", {})
        music_rel = files.get("music", "")
        path = self._base / music_rel if music_rel else None
        if path is None or not path.exists():
            if self._procedural:
                path = self._base / "assets/sounds/_ambient_loop.wav"
                if not path.exists():
                    _write_ambient_music_wav(path)
            else:
                return
        try:
            pygame.mixer.music.load(str(path))
        except pygame.error:
            pass

    def _play(self, name: str) -> None:
        if not self._sfx_enabled:
            return
        sound = self._sounds.get(name)
        if sound:
            sound.set_volume(self._sfx_vol)
            sound.play()

    def play_music(self) -> None:
        if not self._music_enabled or self._music_playing:
            return
        if not pygame.mixer.music.get_busy():
            pygame.mixer.music.set_volume(self._music_vol)
            pygame.mixer.music.play(loops=-1)
            self._music_playing = True

    def stop_music(self) -> None:
        pygame.mixer.music.stop()
        self._music_playing = False

    def play_paddle_hit(self) -> None:
        self._play("paddle_hit")

    def play_wall_hit(self) -> None:
        self._play("wall_hit")

    def play_score(self) -> None:
        self._play("score")

    def play_menu_select(self) -> None:
        if not self._sfx_enabled:
            return
        sound = self._sounds.get("menu_select")
        if not sound:
            return
        if self._menu_sfx_channel and self._menu_sfx_channel.get_busy():
            self._menu_sfx_channel.stop()
        sound.set_volume(self._sfx_vol)
        self._menu_sfx_channel = sound.play()

    def play_game_over(self) -> None:
        self._play("game_over")
