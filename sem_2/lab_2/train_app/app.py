from __future__ import annotations

from pathlib import Path
from tkinter import Tk

from train_app.controller import MainController
from train_app.repository import SqliteTrainRepository
from train_app.services import TrainService
from train_app.view import MainWindow
from train_app.xml_storage import DomSaxXmlStorage


def run() -> None:
    db_path = Path("data") / "train_records.db"
    db_path.parent.mkdir(parents=True, exist_ok=True)

    repository = SqliteTrainRepository(str(db_path))
    xml_storage = DomSaxXmlStorage()
    service = TrainService(repository, xml_storage)
    controller = MainController(service)

    root = Tk()
    MainWindow(root, controller)
    root.mainloop()
