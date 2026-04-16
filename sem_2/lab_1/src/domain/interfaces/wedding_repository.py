from abc import ABC, abstractmethod
from datetime import date
from typing import List, Optional
from domain.entities.wedding import Wedding
from .repository import Repository


class WeddingRepository(Repository[Wedding]):

    @abstractmethod
    def find_by_bride(self, bride_name: str) -> List[Wedding]:
        pass

    @abstractmethod
    def find_by_groom(self, groom_name: str) -> List[Wedding]:
        pass

    @abstractmethod
    def find_by_date(self, date: date) -> List[Wedding]:
        pass

    @abstractmethod
    def find_by_venue(self, venue: str) -> List[Wedding]:
        pass

    @abstractmethod
    def find_upcoming(self, from_date: date) -> List[Wedding]:
        pass