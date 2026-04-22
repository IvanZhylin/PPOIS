from __future__ import annotations

from abc import ABC, abstractmethod

from train_app.models import SearchCriteria, TrainRecord


class ITrainRepository(ABC):
    @abstractmethod
    def add(self, record: TrainRecord) -> None:
        raise NotImplementedError

    @abstractmethod
    def list_all(self) -> list[TrainRecord]:
        raise NotImplementedError

    @abstractmethod
    def replace_all(self, records: list[TrainRecord]) -> None:
        raise NotImplementedError

    @abstractmethod
    def find(self, criteria: SearchCriteria) -> list[TrainRecord]:
        raise NotImplementedError

    @abstractmethod
    def delete(self, criteria: SearchCriteria) -> int:
        raise NotImplementedError


class IXmlStorage(ABC):
    @abstractmethod
    def save(self, file_path: str, records: list[TrainRecord]) -> None:
        raise NotImplementedError

    @abstractmethod
    def load(self, file_path: str) -> list[TrainRecord]:
        raise NotImplementedError
