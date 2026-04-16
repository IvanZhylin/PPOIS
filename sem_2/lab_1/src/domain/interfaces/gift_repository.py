from abc import abstractmethod
from typing import List, Optional
from domain.entities.gift import Gift
from .repository import Repository

class GiftRepository(Reopsitory[Gift]):
    
    @abstractmethod
    def find_by_owner(self, owner_id: int) -> List[Gift]:
        pass

    @abstractmethod
    def find_by_wedding(self, wedding_id: int) -> List[Gift]:
        pass