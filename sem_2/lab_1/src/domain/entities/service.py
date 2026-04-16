from dataclasses import dataclass

@dataclass
class Service:
    id: str
    name: str
    info: str
    price: float