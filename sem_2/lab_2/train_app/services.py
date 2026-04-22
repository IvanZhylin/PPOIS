from __future__ import annotations

from train_app.models import Page, SearchCriteria, TrainRecord
from train_app.protocols import ITrainRepository, IXmlStorage
from train_app.repository import SqliteTrainRepository


class TrainService:
    def __init__(self, repository: ITrainRepository, xml_storage: IXmlStorage) -> None:
        self._repository = repository
        self._xml_storage = xml_storage

    def add_record(self, record: TrainRecord) -> None:
        self._repository.add(record)

    def list_records(self) -> list[TrainRecord]:
        return self._repository.list_all()

    def search(self, criteria: SearchCriteria) -> list[TrainRecord]:
        return self._repository.find(criteria)

    def delete(self, criteria: SearchCriteria) -> int:
        return self._repository.delete(criteria)

    def save_xml(self, file_path: str) -> None:
        self._xml_storage.save(file_path, self._repository.list_all())

    def load_xml(self, file_path: str) -> int:
        records = self._xml_storage.load(file_path)
        self._repository.replace_all(records)
        return len(records)

    def save_to_database_file(self, db_file_path: str) -> int:
        target_repo = SqliteTrainRepository(db_file_path)
        records = self._repository.list_all()
        target_repo.replace_all(records)
        return len(records)

    def load_from_database_file(self, db_file_path: str) -> int:
        source_repo = SqliteTrainRepository(db_file_path)
        records = source_repo.list_all()
        self._repository.replace_all(records)
        return len(records)

    @staticmethod
    def paginate(records: list[TrainRecord], page_index: int, page_size: int) -> Page:
        start = page_index * page_size
        end = start + page_size
        return Page(
            items=records[start:end],
            page_index=page_index,
            page_size=page_size,
            total_items=len(records),
        )
