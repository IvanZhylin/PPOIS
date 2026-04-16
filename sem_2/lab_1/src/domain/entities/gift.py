from dataclasses import dataclass

@dataclass
class Gift:
    id: int
    owner_id: int
    wedding_id: int
    name: str
    info: str
    price: float
