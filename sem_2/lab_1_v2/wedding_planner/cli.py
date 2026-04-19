"""Интерфейс командной строки для системы планирования свадьбы."""

from __future__ import annotations

from wedding_planner.exceptions import PersistenceError, ValidationError, WeddingPlannerError
from wedding_planner.planner import WeddingPlannerService


class WeddingPlannerCLI:
    """Простой интерактивный CLI для работы с сервисом планирования."""

    def __init__(self, service: WeddingPlannerService) -> None:
        self._service = service

    def run(self) -> None:
        """Запускает основной цикл CLI."""
        print("CLI планировщика свадьбы запущен.")
        while True:
            self._print_menu()
            choice = input("Выберите пункт меню: ").strip()
            try:
                if choice == "1":
                    self._set_newlyweds()
                elif choice == "2":
                    self._add_guest()
                elif choice == "3":
                    self._choose_date_place()
                elif choice == "4":
                    self._choose_outfit()
                elif choice == "5":
                    self._prepare_menu()
                elif choice == "6":
                    self._organize_ceremony()
                elif choice == "7":
                    self._prepare_photo_session()
                elif choice == "8":
                    self._show_plan()
                elif choice == "9":
                    print("До свидания!")
                    break
                else:
                    print("Неизвестная команда. Выберите число от 1 до 9.")
            except (WeddingPlannerError, ValueError) as error:
                print(f"Ошибка: {error}")

    @staticmethod
    def _print_menu() -> None:
        print("\n--- Планировщик свадьбы ---")
        print("1. Указать молодоженов")
        print("2. Добавить гостя")
        print("3. Выбрать дату и места")
        print("4. Выбрать свадебный наряд")
        print("5. Подготовить свадебное меню")
        print("6. Организовать церемонию")
        print("7. Подготовиться к фотосессии")
        print("8. Показать текущий план")
        print("9. Выход")

    def _set_newlyweds(self) -> None:
        partner_one = input("Имя первого молодожена: ")
        partner_two = input("Имя второго молодожена: ")
        self._service.set_newlyweds(partner_one, partner_two)
        print("Молодожены сохранены.")

    def _add_guest(self) -> None:
        guest = input("Имя гостя: ")
        self._service.add_guest(guest)
        print("Гость добавлен.")

    def _choose_date_place(self) -> None:
        date = input("Дата свадьбы: ")
        ceremony_place = input("Место церемонии: ")
        banquet_venue = input("Место банкета: ")
        self._service.apply_choose_date_place(date, ceremony_place, banquet_venue)
        print("Дата и места сохранены.")

    def _choose_outfit(self) -> None:
        dress = input("Описание платья невесты: ")
        rings = input("Описание колец: ")
        self._service.apply_choose_outfit(dress, rings)
        print("Свадебный наряд сохранен.")

    def _prepare_menu(self) -> None:
        raw_items = input("Пункты меню (через запятую): ")
        menu = [item.strip() for item in raw_items.split(",")]
        self._service.apply_prepare_menu(menu)
        print("Меню сохранено.")

    def _organize_ceremony(self) -> None:
        script = input("Сценарий церемонии: ")
        self._service.apply_organize_ceremony(script)
        print("Организация церемонии сохранена.")

    def _prepare_photo_session(self) -> None:
        photo_plan = input("План фотосессии: ")
        self._service.apply_prepare_photo_session(photo_plan)
        print("План фотосессии сохранен.")

    def _show_plan(self) -> None:
        plan = self._service.plan
        print("\nТекущий план свадьбы:")
        print(f"Молодожены: {', '.join(plan.newlyweds) if plan.newlyweds else '-'}")
        print(f"Гости ({len(plan.guests)}): {', '.join(plan.guests) if plan.guests else '-'}")
        print(f"Платье невесты: {plan.bride_dress or '-'}")
        print(f"Кольца: {plan.rings or '-'}")
        print(f"Дата церемонии: {plan.ceremony.date or '-'}")
        print(f"Место церемонии: {plan.ceremony.place or '-'}")
        print(f"Место банкета: {plan.banquet.venue or '-'}")
        menu = ", ".join(plan.banquet.menu) if plan.banquet.menu else "-"
        print(f"Меню: {menu}")
        print(f"Сценарий церемонии: {plan.ceremony.script or '-'}")
        print(f"План фотосессии: {plan.ceremony.photo_plan or '-'}")


def run_cli(data_file: str) -> None:
    """Создает CLI с JSON-хранилищем и запускает его."""
    from wedding_planner.storage import JsonPlanRepository

    try:
        repository = JsonPlanRepository(data_file)
        service = WeddingPlannerService(repository)
        cli = WeddingPlannerCLI(service)
        cli.run()
    except PersistenceError as error:
        print(f"Критическая ошибка: {error}")
