from typing import List, Optional
from datetime import date
from domain.entities.wedding import Wedding
from domain.interfaces.wedding_repository import WeddingRepository


class InMemoryWeddingRepository(WeddingRepository):
    def __init__(self):
        self._storage = {}
        self._id_counter = 1
