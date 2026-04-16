from dataclasses import dataclass
from person_info.py import PersonInfo

@dataclass
class Guest:
    id: int
    wedding_id: int
    info: PersonInfo
    status: str
