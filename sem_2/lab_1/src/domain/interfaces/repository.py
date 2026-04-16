from abc import ABC, abstractmethod
from typing import Generic, TypeVar, Optional, List

T = TypeVar('T')

class Repository(ABC, Generic[T]):
    
    @abstractmethod
    def save(self, entity: T) -> None:
        pass

    @abstractmethod
    def find(self, id: int) -> Optional[T]:
        pass

    @abstractmethod
    def delete(self, id: int) -> None:
        pass

    @abstractmethod
    def find_all(self, id: int) -> List[T]:
        pass