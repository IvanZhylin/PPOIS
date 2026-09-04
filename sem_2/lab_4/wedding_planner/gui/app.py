"""Точка входа графического приложения."""

from __future__ import annotations

from wedding_planner.bootstrap import create_service
from wedding_planner.exceptions import PersistenceError
from wedding_planner.gui.presenter import WeddingPlannerPresenter
from wedding_planner.gui.tk_view import TkWeddingPlannerView


def run_gui(data_file: str) -> None:
    """Запускает GUI с JSON-хранилищем и паттерном MVP."""
    try:
        service = create_service(data_file)
        view = TkWeddingPlannerView()
        WeddingPlannerPresenter(service, view)
        view.run()
    except PersistenceError as error:
        raise SystemExit(f"Критическая ошибка: {error}") from error
