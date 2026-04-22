from __future__ import annotations

from datetime import datetime
from xml.dom import minidom
from xml.sax import ContentHandler, parse

from train_app.models import TrainRecord
from train_app.protocols import IXmlStorage

DATETIME_FORMAT = "%Y-%m-%d %H:%M"


class DomSaxXmlStorage(IXmlStorage):
    def save(self, file_path: str, records: list[TrainRecord]) -> None:
        document = minidom.Document()
        root = document.createElement("train_records")
        document.appendChild(root)

        for record in records:
            item = document.createElement("record")
            root.appendChild(item)

            self._append_text(document, item, "train_number", record.train_number)
            self._append_text(document, item, "departure_station", record.departure_station)
            self._append_text(document, item, "arrival_station", record.arrival_station)
            self._append_text(document, item, "departure_at", record.departure_at.strftime(DATETIME_FORMAT))
            self._append_text(document, item, "arrival_at", record.arrival_at.strftime(DATETIME_FORMAT))

        with open(file_path, "w", encoding="utf-8") as output:
            output.write(document.toprettyxml(indent="  ", encoding=None))

    def _append_text(self, document: minidom.Document, parent, tag_name: str, value: str) -> None:
        node = document.createElement(tag_name)
        node.appendChild(document.createTextNode(value))
        parent.appendChild(node)

    def load(self, file_path: str) -> list[TrainRecord]:
        handler = TrainRecordSaxHandler()
        parse(file_path, handler)
        return handler.records


class TrainRecordSaxHandler(ContentHandler):
    def __init__(self) -> None:
        super().__init__()
        self.records: list[TrainRecord] = []
        self._current_tag = ""
        self._current_data: dict[str, str] = {}
        self._buffer: list[str] = []

    def startElement(self, name: str, attrs) -> None:  # noqa: N802
        self._current_tag = name
        self._buffer = []
        if name == "record":
            self._current_data = {}

    def characters(self, content: str) -> None:
        if self._current_tag:
            self._buffer.append(content)

    def endElement(self, name: str) -> None:  # noqa: N802
        value = "".join(self._buffer).strip()
        if name in {"train_number", "departure_station", "arrival_station", "departure_at", "arrival_at"}:
            self._current_data[name] = value
        if name == "record":
            self.records.append(
                TrainRecord(
                    train_number=self._current_data["train_number"],
                    departure_station=self._current_data["departure_station"],
                    arrival_station=self._current_data["arrival_station"],
                    departure_at=datetime.strptime(self._current_data["departure_at"], DATETIME_FORMAT),
                    arrival_at=datetime.strptime(self._current_data["arrival_at"], DATETIME_FORMAT),
                )
            )
        self._current_tag = ""
        self._buffer = []
