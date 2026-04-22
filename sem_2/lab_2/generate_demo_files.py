from __future__ import annotations

import random
from datetime import datetime, timedelta
from pathlib import Path

from train_app.models import TrainRecord
from train_app.xml_storage import DomSaxXmlStorage


def generate_records(seed: int, count: int = 50) -> list[TrainRecord]:
    random.seed(seed)
    stations = [
        "Минск", "Гродно", "Брест", "Витебск", "Гомель", "Могилев",
        "Полоцк", "Бобруйск", "Орша", "Барановичи", "Пинск", "Лида",
    ]
    base = datetime(2026, 5, 1, 6, 0)
    records: list[TrainRecord] = []
    for i in range(count):
        dep = random.choice(stations)
        arr = random.choice([s for s in stations if s != dep])
        dep_dt = base + timedelta(hours=i * 3 + random.randint(0, 2), minutes=random.randint(0, 59))
        travel = timedelta(hours=random.randint(1, 8), minutes=random.choice([0, 10, 20, 30, 40, 50]))
        records.append(
            TrainRecord(
                train_number=f"{1000 + seed * 100 + i}",
                departure_station=dep,
                arrival_station=arr,
                departure_at=dep_dt,
                arrival_at=dep_dt + travel,
            )
        )
    return records


def main() -> None:
    storage = DomSaxXmlStorage()
    out_dir = Path("demo_files")
    out_dir.mkdir(exist_ok=True)
    for idx, seed in enumerate([11, 22, 33], start=1):
        records = generate_records(seed, count=50)
        storage.save(str(out_dir / f"train_records_{idx}.xml"), records)
    print("Generated 3 demo XML files with 50 unique records each.")


if __name__ == "__main__":
    main()
