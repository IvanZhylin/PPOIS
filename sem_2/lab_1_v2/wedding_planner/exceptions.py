"""Исключения, специфичные для проекта."""


class WeddingPlannerError(Exception):
    """Базовая ошибка предметной области планировщика свадьбы."""


class ValidationError(WeddingPlannerError):
    """Выбрасывается при некорректных входных данных."""


class PersistenceError(WeddingPlannerError):
    """Выбрасывается, если данные нельзя загрузить или сохранить."""
