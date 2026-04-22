from __future__ import annotations

import sqlite3
from datetime import datetime

from train_app.models import SearchCriteria, TrainRecord
from train_app.protocols import ITrainRepository


def _dt_to_str(value: datetime) -> str:
    return value.isoformat(timespec="minutes")


def _str_to_dt(value: str) -> datetime:
    return datetime.fromisoformat(value)


class SqliteTrainRepository(ITrainRepository):
    def __init__(self, db_path: str) -> None:
        self._db_path = db_path
        self._ensure_schema()

    def _connect(self) -> sqlite3.Connection:
        return sqlite3.connect(self._db_path)

    def _ensure_schema(self) -> None:
        with self._connect() as conn:
            conn.execute(
                """
                CREATE TABLE IF NOT EXISTS train_records (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    train_number TEXT NOT NULL,
                    departure_station TEXT NOT NULL,
                    arrival_station TEXT NOT NULL,
                    departure_at TEXT NOT NULL,
                    arrival_at TEXT NOT NULL
                )
                """
            )
            conn.commit()

    def add(self, record: TrainRecord) -> None:
        with self._connect() as conn:
            conn.execute(
                """
                INSERT INTO train_records (
                    train_number, departure_station, arrival_station, departure_at, arrival_at
                ) VALUES (?, ?, ?, ?, ?)
                """,
                (
                    record.train_number,
                    record.departure_station,
                    record.arrival_station,
                    _dt_to_str(record.departure_at),
                    _dt_to_str(record.arrival_at),
                ),
            )
            conn.commit()

    def list_all(self) -> list[TrainRecord]:
        with self._connect() as conn:
            rows = conn.execute(
                """
                SELECT train_number, departure_station, arrival_station, departure_at, arrival_at
                FROM train_records
                ORDER BY departure_at
                """
            ).fetchall()
        return [
            TrainRecord(
                train_number=row[0],
                departure_station=row[1],
                arrival_station=row[2],
                departure_at=_str_to_dt(row[3]),
                arrival_at=_str_to_dt(row[4]),
            )
            for row in rows
        ]

    def replace_all(self, records: list[TrainRecord]) -> None:
        with self._connect() as conn:
            conn.execute("DELETE FROM train_records")
            conn.executemany(
                """
                INSERT INTO train_records (
                    train_number, departure_station, arrival_station, departure_at, arrival_at
                ) VALUES (?, ?, ?, ?, ?)
                """,
                [
                    (
                        r.train_number,
                        r.departure_station,
                        r.arrival_station,
                        _dt_to_str(r.departure_at),
                        _dt_to_str(r.arrival_at),
                    )
                    for r in records
                ],
            )
            conn.commit()

    def find(self, criteria: SearchCriteria) -> list[TrainRecord]:
        return [record for record in self.list_all() if _matches(record, criteria)]

    def delete(self, criteria: SearchCriteria) -> int:
        all_records = self.list_all()
        to_keep = [record for record in all_records if not _matches(record, criteria)]
        deleted = len(all_records) - len(to_keep)
        if deleted > 0:
            self.replace_all(to_keep)
        return deleted


def _matches(record: TrainRecord, criteria: SearchCriteria) -> bool:
    if criteria.train_number and criteria.train_number.lower() not in record.train_number.lower():
        return False
    if criteria.departure_date and record.departure_at.date() != criteria.departure_date.date():
        return False
    if criteria.departure_station and criteria.departure_station.lower() not in record.departure_station.lower():
        return False
    if criteria.arrival_station and criteria.arrival_station.lower() not in record.arrival_station.lower():
        return False
    if criteria.departure_time_from and record.departure_at < criteria.departure_time_from:
        return False
    if criteria.departure_time_to and record.departure_at > criteria.departure_time_to:
        return False
    if criteria.arrival_time_from and record.arrival_at < criteria.arrival_time_from:
        return False
    if criteria.arrival_time_to and record.arrival_at > criteria.arrival_time_to:
        return False
    travel_minutes = int(record.travel_time.total_seconds() // 60)
    if criteria.travel_minutes_from is not None and travel_minutes < criteria.travel_minutes_from:
        return False
    if criteria.travel_minutes_to is not None and travel_minutes > criteria.travel_minutes_to:
        return False
    return True
