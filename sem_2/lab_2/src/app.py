from __future__ import annotations

from pathlib import Path
from tkinter import Tk

from src.controller import MainController
from src.repository import SqliteTrainRepository
from src.services import TrainService
from src.view import MainWindow
from src.xml_storage import DomSaxXmlStorage


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
