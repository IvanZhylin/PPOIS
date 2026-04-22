from __future__ import annotations

from dataclasses import dataclass

from train_app.models import Page, SearchCriteria, TrainRecord
from train_app.services import TrainService


@dataclass
class PaginationState:
    page_index: int = 0
    page_size: int = 10


class MainController:
    def __init__(self, service: TrainService) -> None:
        self._service = service
        self._state = PaginationState()

    def add_record(self, record: TrainRecord) -> None:
        self._service.add_record(record)

    def get_current_page(self) -> Page:
        records = self._service.list_records()
        page = self._service.paginate(records, self._state.page_index, self._state.page_size)
        if self._state.page_index >= page.total_pages:
            self._state.page_index = max(page.total_pages - 1, 0)
            page = self._service.paginate(records, self._state.page_index, self._state.page_size)
        return page

    def search(self, criteria: SearchCriteria, page_index: int, page_size: int) -> Page:
        records = self._service.search(criteria)
        page = self._service.paginate(records, page_index, page_size)
        if page_index >= page.total_pages:
            return self._service.paginate(records, max(page.total_pages - 1, 0), page_size)
        return page

    def delete(self, criteria: SearchCriteria) -> int:
        deleted = self._service.delete(criteria)
        self.get_current_page()
        return deleted

    def set_page_size(self, page_size: int) -> None:
        self._state.page_size = page_size
        self._state.page_index = 0

    def first_page(self) -> None:
        self._state.page_index = 0

    def prev_page(self) -> None:
        self._state.page_index = max(0, self._state.page_index - 1)

    def next_page(self) -> None:
        page = self.get_current_page()
        self._state.page_index = min(page.total_pages - 1, self._state.page_index + 1)

    def last_page(self) -> None:
        page = self.get_current_page()
        self._state.page_index = max(page.total_pages - 1, 0)

    def save_xml(self, file_path: str) -> None:
        self._service.save_xml(file_path)

    def load_xml(self, file_path: str) -> int:
        loaded = self._service.load_xml(file_path)
        self.first_page()
        return loaded

    def save_to_database_file(self, file_path: str) -> int:
        return self._service.save_to_database_file(file_path)

    def load_from_database_file(self, file_path: str) -> int:
        loaded = self._service.load_from_database_file(file_path)
        self.first_page()
        return loaded
