from __future__ import annotations

from datetime import datetime
from tkinter import BOTH, END, LEFT, RIGHT, TOP, Button, Frame, Label, Menu, StringVar, Tk, Toplevel, messagebox
from tkinter import filedialog
from tkinter import ttk

from train_app.controller import MainController
from train_app.models import Page, SearchCriteria, TrainRecord

DATETIME_FMT = "%Y-%m-%d %H:%M"
DATE_FMT = "%Y-%m-%d"

try:
    from tkcalendar import DateEntry  # type: ignore
except ImportError:
    DateEntry = None


def _duration_to_hhmm(record: TrainRecord) -> str:
    minutes = int(record.travel_time.total_seconds() // 60)
    return f"{minutes // 60:02d}:{minutes % 60:02d}"


class MainWindow:
    def __init__(self, root: Tk, controller: MainController) -> None:
        self._root = root
        self._controller = controller
        self._is_tree_mode = False

        self._root.title("Вариант 13 - Поезда")
        self._root.geometry("1200x700")

        self._build_menu()
        self._build_toolbar()
        self._build_content()
        self._build_pagination()

        self.refresh()

    def _build_menu(self) -> None:
        menu = Menu(self._root)
        self._root.config(menu=menu)

        records_menu = Menu(menu, tearoff=0)
        records_menu.add_command(label="Добавить запись", command=self.open_add_dialog)
        records_menu.add_command(label="Поиск", command=self.open_search_dialog)
        records_menu.add_command(label="Удаление", command=self.open_delete_dialog)
        menu.add_cascade(label="Записи", menu=records_menu)

        io_menu = Menu(menu, tearoff=0)
        io_menu.add_command(label="Сохранить в XML", command=self.save_xml)
        io_menu.add_command(label="Загрузить из XML", command=self.load_xml)
        io_menu.add_separator()
        io_menu.add_command(label="Сохранить в БД", command=self.save_db)
        io_menu.add_command(label="Загрузить из БД", command=self.load_db)
        menu.add_cascade(label="Файл", menu=io_menu)

        view_menu = Menu(menu, tearoff=0)
        view_menu.add_command(label="Переключить таблица/дерево", command=self.toggle_view_mode)
        menu.add_cascade(label="Вид", menu=view_menu)

    def _build_toolbar(self) -> None:
        toolbar = Frame(self._root)
        toolbar.pack(side=TOP, fill="x")

        Button(toolbar, text="Добавить", command=self.open_add_dialog).pack(side=LEFT, padx=3, pady=3)
        Button(toolbar, text="Поиск", command=self.open_search_dialog).pack(side=LEFT, padx=3, pady=3)
        Button(toolbar, text="Удалить", command=self.open_delete_dialog).pack(side=LEFT, padx=3, pady=3)
        Button(toolbar, text="Сохр.XML", command=self.save_xml).pack(side=LEFT, padx=3, pady=3)
        Button(toolbar, text="Загр.XML", command=self.load_xml).pack(side=LEFT, padx=3, pady=3)
        Button(toolbar, text="Сохр.БД", command=self.save_db).pack(side=LEFT, padx=3, pady=3)
        Button(toolbar, text="Загр.БД", command=self.load_db).pack(side=LEFT, padx=3, pady=3)
        Button(toolbar, text="Таблица/Дерево", command=self.toggle_view_mode).pack(side=LEFT, padx=3, pady=3)

    def _build_content(self) -> None:
        content = Frame(self._root)
        content.pack(fill=BOTH, expand=True)

        self._table = ttk.Treeview(
            content,
            columns=("num", "dep_s", "arr_s", "dep_t", "arr_t", "travel"),
            show="headings",
        )
        for col, text, width in [
            ("num", "№ Поезда", 100),
            ("dep_s", "Станция отправления", 180),
            ("arr_s", "Станция прибытия", 180),
            ("dep_t", "Дата и время отправления", 190),
            ("arr_t", "Дата и время прибытия", 190),
            ("travel", "Время в пути", 120),
        ]:
            self._table.heading(col, text=text)
            self._table.column(col, width=width, anchor="center")
        self._table.pack(fill=BOTH, expand=True)

        self._tree = ttk.Treeview(content, show="tree")
        self._tree.pack_forget()

    def _build_pagination(self) -> None:
        panel = Frame(self._root)
        panel.pack(fill="x")

        Button(panel, text="<<", command=lambda: self._page_action("first")).pack(side=LEFT, padx=2, pady=3)
        Button(panel, text="<", command=lambda: self._page_action("prev")).pack(side=LEFT, padx=2, pady=3)
        Button(panel, text=">", command=lambda: self._page_action("next")).pack(side=LEFT, padx=2, pady=3)
        Button(panel, text=">>", command=lambda: self._page_action("last")).pack(side=LEFT, padx=2, pady=3)

        Label(panel, text="Записей на странице:").pack(side=LEFT, padx=8)
        self._page_size = ttk.Combobox(panel, values=["5", "10", "20", "50"], width=5, state="readonly")
        self._page_size.set("10")
        self._page_size.pack(side=LEFT)
        self._page_size.bind("<<ComboboxSelected>>", self._on_page_size_change)

        self._pagination_label = Label(panel, text="")
        self._pagination_label.pack(side=RIGHT, padx=8)

    def _on_page_size_change(self, _event) -> None:
        self._controller.set_page_size(int(self._page_size.get()))
        self.refresh()

    def _page_action(self, action: str) -> None:
        if action == "first":
            self._controller.first_page()
        elif action == "prev":
            self._controller.prev_page()
        elif action == "next":
            self._controller.next_page()
        elif action == "last":
            self._controller.last_page()
        self.refresh()

    def refresh(self) -> None:
        page = self._controller.get_current_page()
        self._render_page(page)

    def _render_page(self, page: Page) -> None:
        if self._is_tree_mode:
            self._render_tree(page)
        else:
            self._render_table(page)
        current_count = len(page.items)
        self._pagination_label.config(
            text=(
                f"Страница {page.page_index + 1}/{page.total_pages}, "
                f"на странице {current_count}, всего {page.total_items}"
            )
        )

    def _render_table(self, page: Page) -> None:
        self._tree.pack_forget()
        self._table.pack(fill=BOTH, expand=True)
        self._table.delete(*self._table.get_children())
        for record in page.items:
            self._table.insert(
                "",
                END,
                values=(
                    record.train_number,
                    record.departure_station,
                    record.arrival_station,
                    record.departure_at.strftime(DATETIME_FMT),
                    record.arrival_at.strftime(DATETIME_FMT),
                    _duration_to_hhmm(record),
                ),
            )

    def _render_tree(self, page: Page) -> None:
        self._table.pack_forget()
        self._tree.pack(fill=BOTH, expand=True)
        self._tree.delete(*self._tree.get_children())

        for index, record in enumerate(page.items, 1):
            parent = self._tree.insert("", END, text=f"Запись {index}")
            self._tree.insert(parent, END, text=f"Номер поезда: {record.train_number}")
            self._tree.insert(parent, END, text=f"Станция отправления: {record.departure_station}")
            self._tree.insert(parent, END, text=f"Станция прибытия: {record.arrival_station}")
            self._tree.insert(parent, END, text=f"Отправление: {record.departure_at.strftime(DATETIME_FMT)}")
            self._tree.insert(parent, END, text=f"Прибытие: {record.arrival_at.strftime(DATETIME_FMT)}")
            self._tree.insert(parent, END, text=f"В пути: {_duration_to_hhmm(record)}")

    def toggle_view_mode(self) -> None:
        self._is_tree_mode = not self._is_tree_mode
        self.refresh()

    def open_add_dialog(self) -> None:
        dialog = AddRecordDialog(self._root)
        if dialog.result:
            self._controller.add_record(dialog.result)
            self.refresh()

    def open_search_dialog(self) -> None:
        SearchDialog(self._root, self._controller)

    def open_delete_dialog(self) -> None:
        dialog = DeleteDialog(self._root)
        if not dialog.criteria:
            return
        deleted = self._controller.delete(dialog.criteria)
        if deleted:
            messagebox.showinfo("Удаление", f"Удалено записей: {deleted}")
        else:
            messagebox.showinfo("Удаление", "По заданным условиям записи не найдены")
        self.refresh()

    def save_xml(self) -> None:
        path = filedialog.asksaveasfilename(defaultextension=".xml", filetypes=[("XML", "*.xml")])
        if not path:
            return
        self._controller.save_xml(path)
        messagebox.showinfo("XML", "Данные сохранены")

    def load_xml(self) -> None:
        path = filedialog.askopenfilename(filetypes=[("XML", "*.xml")])
        if not path:
            return
        count = self._controller.load_xml(path)
        self.refresh()
        messagebox.showinfo("XML", f"Загружено записей: {count}")

    def save_db(self) -> None:
        path = filedialog.asksaveasfilename(defaultextension=".db", filetypes=[("SQLite DB", "*.db")])
        if not path:
            return
        count = self._controller.save_to_database_file(path)
        messagebox.showinfo("БД", f"Сохранено записей: {count}")

    def load_db(self) -> None:
        path = filedialog.askopenfilename(filetypes=[("SQLite DB", "*.db")])
        if not path:
            return
        count = self._controller.load_from_database_file(path)
        self.refresh()
        messagebox.showinfo("БД", f"Загружено записей: {count}")


class AddRecordDialog:
    def __init__(self, parent: Tk) -> None:
        self.result: TrainRecord | None = None
        self._win = Toplevel(parent)
        self._win.title("Добавить запись")
        self._win.grab_set()

        self._entries: dict[str, ttk.Entry] = {}
        self._date_controls: dict[str, object] = {}
        fields = [
            ("train_number", "Номер поезда"),
            ("departure_station", "Станция отправления"),
            ("arrival_station", "Станция прибытия"),
            ("departure_at", "Отправление (YYYY-MM-DD HH:MM)"),
            ("arrival_at", "Прибытие (YYYY-MM-DD HH:MM)"),
        ]

        for row, (key, label) in enumerate(fields):
            ttk.Label(self._win, text=label).grid(row=row, column=0, padx=5, pady=5, sticky="w")
            if key in {"departure_at", "arrival_at"}:
                date_widget, time_entry = self._build_datetime_input(row)
                self._date_controls[key] = (date_widget, time_entry)
            else:
                entry = ttk.Entry(self._win, width=32)
                entry.grid(row=row, column=1, padx=5, pady=5)
                self._entries[key] = entry

        ttk.Button(self._win, text="Сохранить", command=self._on_save).grid(row=len(fields), column=0, columnspan=2, pady=8)
        self._win.wait_window()

    def _on_save(self) -> None:
        try:
            departure_at = self._read_datetime_value("departure_at")
            arrival_at = self._read_datetime_value("arrival_at")
            if arrival_at <= departure_at:
                raise ValueError("Время прибытия должно быть позже отправления")
            self.result = TrainRecord(
                train_number=self._entries["train_number"].get().strip(),
                departure_station=self._entries["departure_station"].get().strip(),
                arrival_station=self._entries["arrival_station"].get().strip(),
                departure_at=departure_at,
                arrival_at=arrival_at,
            )
            self._win.destroy()
        except ValueError as exc:
            messagebox.showerror("Ошибка", str(exc))

    def _build_datetime_input(self, row: int) -> tuple[object, ttk.Entry]:
        holder = Frame(self._win)
        holder.grid(row=row, column=1, padx=5, pady=5, sticky="w")
        if DateEntry:
            date_widget = DateEntry(holder, date_pattern="yyyy-mm-dd", width=12)
        else:
            date_widget = ttk.Entry(holder, width=14)
            date_widget.insert(0, "YYYY-MM-DD")
        date_widget.pack(side=LEFT)
        ttk.Label(holder, text=" ").pack(side=LEFT)
        time_entry = ttk.Entry(holder, width=8)
        time_entry.insert(0, "HH:MM")
        time_entry.pack(side=LEFT)
        return date_widget, time_entry

    def _read_datetime_value(self, key: str) -> datetime:
        date_widget, time_entry = self._date_controls[key]
        if DateEntry:
            date_part = date_widget.get_date().strftime(DATE_FMT)  # type: ignore[attr-defined]
        else:
            date_part = date_widget.get().strip()  # type: ignore[attr-defined]
        time_part = time_entry.get().strip()
        return datetime.strptime(f"{date_part} {time_part}", DATETIME_FMT)


class _CriteriaDialogBase:
    def __init__(self, parent: Tk, title: str) -> None:
        self.criteria: SearchCriteria | None = None
        self._win = Toplevel(parent)
        self._win.title(title)
        self._win.grab_set()
        self._fields: dict[str, ttk.Entry] = {}
        self._build_criteria_form()

    def _build_criteria_form(self) -> None:
        fields = [
            ("train_number", "Номер поезда"),
            ("departure_date", "Дата отправления (YYYY-MM-DD)"),
            ("departure_station", "Станция отправления"),
            ("arrival_station", "Станция прибытия"),
            ("departure_time_from", "Отправление от (YYYY-MM-DD HH:MM)"),
            ("departure_time_to", "Отправление до (YYYY-MM-DD HH:MM)"),
            ("arrival_time_from", "Прибытие от (YYYY-MM-DD HH:MM)"),
            ("arrival_time_to", "Прибытие до (YYYY-MM-DD HH:MM)"),
            ("travel_minutes_from", "В пути от (мин)"),
            ("travel_minutes_to", "В пути до (мин)"),
        ]
        for row, (key, label) in enumerate(fields):
            ttk.Label(self._win, text=label).grid(row=row, column=0, padx=5, pady=3, sticky="w")
            entry = ttk.Entry(self._win, width=32)
            entry.grid(row=row, column=1, padx=5, pady=3)
            self._fields[key] = entry

    def _parse_criteria(self) -> SearchCriteria:
        get = lambda key: self._fields[key].get().strip()
        dep_date = get("departure_date")
        return SearchCriteria(
            train_number=get("train_number") or None,
            departure_date=datetime.strptime(dep_date, DATE_FMT) if dep_date else None,
            departure_station=get("departure_station") or None,
            arrival_station=get("arrival_station") or None,
            departure_time_from=datetime.strptime(get("departure_time_from"), DATETIME_FMT) if get("departure_time_from") else None,
            departure_time_to=datetime.strptime(get("departure_time_to"), DATETIME_FMT) if get("departure_time_to") else None,
            arrival_time_from=datetime.strptime(get("arrival_time_from"), DATETIME_FMT) if get("arrival_time_from") else None,
            arrival_time_to=datetime.strptime(get("arrival_time_to"), DATETIME_FMT) if get("arrival_time_to") else None,
            travel_minutes_from=int(get("travel_minutes_from")) if get("travel_minutes_from") else None,
            travel_minutes_to=int(get("travel_minutes_to")) if get("travel_minutes_to") else None,
        )


class DeleteDialog(_CriteriaDialogBase):
    def __init__(self, parent: Tk) -> None:
        super().__init__(parent, "Удаление")
        ttk.Button(self._win, text="Удалить", command=self._on_delete).grid(row=11, column=0, columnspan=2, pady=8)
        self._win.wait_window()

    def _on_delete(self) -> None:
        try:
            self.criteria = self._parse_criteria()
            self._win.destroy()
        except ValueError as exc:
            messagebox.showerror("Ошибка", str(exc))


class SearchDialog(_CriteriaDialogBase):
    def __init__(self, parent: Tk, controller: MainController) -> None:
        self._controller = controller
        self._search_page_index = 0
        self._search_page_size = 10
        self._active_criteria = SearchCriteria()
        self._pagination_text = StringVar(value="")
        super().__init__(parent, "Поиск")
        self._build_results_ui()
        self._win.wait_window()

    def _build_results_ui(self) -> None:
        ttk.Button(self._win, text="Найти", command=self._on_search).grid(row=11, column=0, columnspan=2, pady=8)

        self._result_table = ttk.Treeview(
            self._win,
            columns=("num", "dep_s", "arr_s", "dep_t", "arr_t", "travel"),
            show="headings",
            height=10,
        )
        for col, text, width in [
            ("num", "№", 70),
            ("dep_s", "Откуда", 120),
            ("arr_s", "Куда", 120),
            ("dep_t", "Отправление", 150),
            ("arr_t", "Прибытие", 150),
            ("travel", "В пути", 80),
        ]:
            self._result_table.heading(col, text=text)
            self._result_table.column(col, width=width, anchor="center")
        self._result_table.grid(row=12, column=0, columnspan=2, padx=5, pady=8)

        control = Frame(self._win)
        control.grid(row=13, column=0, columnspan=2, pady=4)
        Button(control, text="<<", command=lambda: self._page("first")).pack(side=LEFT, padx=2)
        Button(control, text="<", command=lambda: self._page("prev")).pack(side=LEFT, padx=2)
        Button(control, text=">", command=lambda: self._page("next")).pack(side=LEFT, padx=2)
        Button(control, text=">>", command=lambda: self._page("last")).pack(side=LEFT, padx=2)

        size_box = ttk.Combobox(control, values=["5", "10", "20", "50"], width=5, state="readonly")
        size_box.set("10")
        size_box.pack(side=LEFT, padx=8)
        size_box.bind("<<ComboboxSelected>>", lambda _e: self._change_page_size(size_box.get()))

        Label(control, textvariable=self._pagination_text).pack(side=LEFT, padx=8)

    def _change_page_size(self, value: str) -> None:
        self._search_page_size = int(value)
        self._search_page_index = 0
        self._render_page()

    def _on_search(self) -> None:
        try:
            self._active_criteria = self._parse_criteria()
            self._search_page_index = 0
            self._render_page()
        except ValueError as exc:
            messagebox.showerror("Ошибка", str(exc))

    def _page(self, action: str) -> None:
        page = self._controller.search(self._active_criteria, self._search_page_index, self._search_page_size)
        if action == "first":
            self._search_page_index = 0
        elif action == "prev":
            self._search_page_index = max(0, self._search_page_index - 1)
        elif action == "next":
            self._search_page_index = min(page.total_pages - 1, self._search_page_index + 1)
        elif action == "last":
            self._search_page_index = page.total_pages - 1
        self._render_page()

    def _render_page(self) -> None:
        page = self._controller.search(self._active_criteria, self._search_page_index, self._search_page_size)
        self._search_page_index = page.page_index
        self._result_table.delete(*self._result_table.get_children())
        for record in page.items:
            self._result_table.insert(
                "",
                END,
                values=(
                    record.train_number,
                    record.departure_station,
                    record.arrival_station,
                    record.departure_at.strftime(DATETIME_FMT),
                    record.arrival_at.strftime(DATETIME_FMT),
                    _duration_to_hhmm(record),
                ),
            )
        self._pagination_text.set(
            f"Стр. {page.page_index + 1}/{page.total_pages}, на стр. {len(page.items)}, всего {page.total_items}"
        )
