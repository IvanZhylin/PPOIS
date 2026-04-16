import json
import os
from typing import Generic, TypeVar, List, Optional, Type
from domain.interfaces.repository import Repository

T = TypeVar('T')

class JsonRepository(Repository[T], Generic[T]):
    def __init__(self, file_path: str, entity_class: Type[T]):
        self.file_path = file_path
        self.entity_class = entity_class
        self._storage = dict[int, T] = {}
        self._id_conuter = 1
        self._load()