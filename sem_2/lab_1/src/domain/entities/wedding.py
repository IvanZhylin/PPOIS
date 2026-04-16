from dataclasses import dataclass
from datetime import date, time, datetime

@dataclass
class Wedding:
    id: int
    date: date
    time: time
    venue: str
    budget: float
    bride_id: int
    groom_id: int
