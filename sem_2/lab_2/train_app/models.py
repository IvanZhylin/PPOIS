from __future__ import annotations

from dataclasses import dataclass
from datetime import datetime, timedelta
from typing import Optional


@dataclass(frozen=True)
class TrainRecord:
    train_number: str
    departure_station: str
    arrival_station: str
    departure_at: datetime
    arrival_at: datetime

    @property
    def travel_time(self) -> timedelta:
        return self.arrival_at - self.departure_at


@dataclass(frozen=True)
class SearchCriteria:
    train_number: Optional[str] = None
    departure_date: Optional[datetime] = None
    departure_time_from: Optional[datetime] = None
    departure_time_to: Optional[datetime] = None
    arrival_time_from: Optional[datetime] = None
    arrival_time_to: Optional[datetime] = None
    departure_station: Optional[str] = None
    arrival_station: Optional[str] = None
    travel_minutes_from: Optional[int] = None
    travel_minutes_to: Optional[int] = None


@dataclass(frozen=True)
class Page:
    items: list[TrainRecord]
    page_index: int
    page_size: int
    total_items: int

    @property
    def total_pages(self) -> int:
        if self.total_items == 0:
            return 1
        return (self.total_items + self.page_size - 1) // self.page_size
