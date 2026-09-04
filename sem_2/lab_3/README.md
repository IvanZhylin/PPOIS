# Лабораторная работа: игра Pong

Графическое приложение на Python с библиотекой **pygame**, построенное на **событийно-ориентированной** модели (цикл `pygame.event.get()` + состояния). Правила соответствуют классическому Pong: две ракетки, мяч, счёт до заданного числа очков, отскоки от стен и ракеток с изменением угла и скорости.

## Запуск

```bash
cd sem_2/lab_3
python -m venv .venv
source .venv/bin/activate   # Windows: .venv\Scripts\activate
pip install -r requirements.txt
python main.py
```

### Arch Linux

Рекомендуется зависимость **pygame-ce** из `requirements.txt` (импорт по-прежнему `import pygame`).  
Если используете пакет `pygame` из pip и нет звука, установите системную библиотеку и переустановите pygame:

```bash
sudo pacman -S sdl2_mixer
pip install --force-reinstall --no-binary :all: pygame
```

Автопроверка запуска:

```bash
.venv/bin/python scripts/smoke_test.py
```

Опционально — сгенерировать wav-файлы (иначе звук синтезируется в памяти):

```bash
python scripts/generate_sounds.py
```

## Меню

При старте отображаются команды:

| Пункт | Действие |
|--------|----------|
| Начать игру | Матч игрок (слева) против ИИ (справа) |
| Таблица рекордов | Просмотр сохранённых результатов |
| Справка | Правила и управление |
| Выход | Закрытие приложения |

Управление в меню: **↑/↓** или **W/S**, **Enter** — выбор, **Esc** — назад (в подэкранах).

## Игровой процесс

- **W/S** или **↑/↓** — ракетка игрока.
- **Esc** — пауза; **M** в паузе — возврат в меню.
- Победа при достижении `points_to_win` (по умолчанию 11); при `win_by_two: true` нужно опережение в 2 очка после счёта 10:10.
- После пропуска мяча — анимация «взрыва» (исчезновение мяча), затем подача в сторону проигравшего ракетку.
- След мяча — анимация полёта; вспышка ракетки при ударе.

## Рекорды

Файл `data/highscores.json`. Если счёт игрока **выше первой строки** таблицы, после матча показывается диалог с поздравлением и полем ввода имени (**Enter** — сохранить).

## Конфигурация (JSON)

| Файл | Назначение |
|------|------------|
| `config/game.json` | Размер экрана, мяч, ракетки, ИИ, правила, анимации |
| `config/menu.json` | Пункты и оформление меню |
| `config/help.json` | Текст справки |
| `config/audio.json` | Громкость и пути к звукам |
| `data/highscores.json` | Таблица рекордов |

Все характеристики сущностей и правила читаются при старте; для изменения баланса достаточно отредактировать JSON без правки кода.

## Звук

- Фоновая музыка в меню и во время игры.
- Эффекты: удар о ракетку, стену, гол, меню, конец игры.
- Пути в `config/audio.json`; при отсутствии файлов и `procedural_fallback: true` звук генерируется процедурно.

## Архитектура и SOLID

```
main.py
└── src/application.py          # сборка и игровой цикл
    ├── ConfigLoader            # SRP: только чтение JSON
    ├── JsonScoreRepository     # SRP: рекорды (IScoreRepository)
    ├── AudioManager            # SRP: звук (IAudioService)
    ├── StateManager            # переключение IGameState
    ├── entities/               # Ball, Paddle
    ├── effects/                # GoalExplosion, ScorePopup
    ├── ai_controller.py        # логика ИИ отдельно от отрисовки
    └── states/                 # Menu, Help, Scores, PongGame, GameOver
```

- **S (Single Responsibility)** — отдельные классы для конфигурации, звука, рекордов, ИИ, сущностей, каждого экрана.
- **O (Open/Closed)** — новые экраны добавляются как реализации `IGameState` без изменения цикла в `PongApplication.run`.
- **L (Liskov)** — любое состояние взаимозаменяемо через `StateManager`.
- **I (Interface Segregation)** — узкие интерфейсы `IGameState`, `IScoreRepository`, `IAudioService`.
- **D (Dependency Inversion)** — состояния зависят от абстракций (`IAudioService`, `IScoreRepository`), а не от конкретных реализаций.

## Структура каталогов

```
lab_3/
├── main.py
├── requirements.txt
├── README.md
├── config/
├── data/
├── assets/sounds/
├── scripts/generate_sounds.py
└── src/
```

## Автор и вариант

Вариант: **Pong**. Язык: **Python 3**. Комментарии и тексты интерфейса — на **русском** языке.
