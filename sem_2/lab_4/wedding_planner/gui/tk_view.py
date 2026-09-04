"""Реализация View на tkinter."""

from __future__ import annotations

import tkinter as tk
from tkinter import messagebox, ttk

from wedding_planner.gui.presenter import WeddingPlannerPresenter
from wedding_planner.presentation.plan_formatter import format_snapshot_text
from wedding_planner.presentation.plan_snapshot import PlanSnapshot


class TkWeddingPlannerView:
    """Графическое представление планировщика свадьбы (пассивный View в MVP)."""

    def __init__(self) -> None:
        self._presenter: WeddingPlannerPresenter | None = None
        self._root = tk.Tk()
        self._root.title("Планировщик свадьбы — лабораторная №4")
        self._root.minsize(820, 560)
        self._build_layout()

    def attach_presenter(self, presenter: WeddingPlannerPresenter) -> None:
        self._presenter = presenter

    def show_plan(self, snapshot: PlanSnapshot) -> None:
        """Обновляет текстовую панель состояния."""
        text = format_snapshot_text(snapshot)
        self._plan_text.configure(state=tk.NORMAL)
        self._plan_text.delete("1.0", tk.END)
        self._plan_text.insert(tk.END, text)
        self._plan_text.configure(state=tk.DISABLED)

    def show_error(self, message: str) -> None:
        messagebox.showerror("Ошибка", message, parent=self._root)

    def show_info(self, message: str) -> None:
        messagebox.showinfo("Готово", message, parent=self._root)

    def run(self) -> None:
        self._root.mainloop()

    def _build_layout(self) -> None:
        main = ttk.Frame(self._root, padding=10)
        main.pack(fill=tk.BOTH, expand=True)

        paned = ttk.PanedWindow(main, orient=tk.HORIZONTAL)
        paned.pack(fill=tk.BOTH, expand=True)

        left = ttk.LabelFrame(paned, text="Текущее состояние модели", padding=8)
        paned.add(left, weight=2)

        self._plan_text = tk.Text(left, wrap=tk.WORD, height=24, font=("DejaVu Sans Mono", 10))
        scroll = ttk.Scrollbar(left, orient=tk.VERTICAL, command=self._plan_text.yview)
        self._plan_text.configure(yscrollcommand=scroll.set)
        self._plan_text.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        scroll.pack(side=tk.RIGHT, fill=tk.Y)

        right = ttk.LabelFrame(paned, text="Управление планом", padding=8)
        paned.add(right, weight=1)

        actions = [
            ("Указать молодоженов", self._dialog_newlyweds),
            ("Добавить гостя", self._dialog_guest),
            ("Выбрать дату и места", self._dialog_date_place),
            ("Выбрать свадебный наряд", self._dialog_outfit),
            ("Подготовить меню", self._dialog_menu),
            ("Организовать церемонию", self._dialog_ceremony),
            ("Подготовить фотосессию", self._dialog_photo),
            ("Обновить отображение", self._on_refresh),
        ]
        for index, (label, handler) in enumerate(actions):
            btn = ttk.Button(right, text=label, command=handler)
            btn.grid(row=index, column=0, sticky="ew", pady=4)
        right.columnconfigure(0, weight=1)

        status = ttk.Label(
            main,
            text="Все изменения сохраняются в data/wedding_plan.json",
            foreground="#444",
        )
        status.pack(anchor=tk.W, pady=(8, 0))

    def _require_presenter(self) -> WeddingPlannerPresenter:
        if self._presenter is None:
            raise RuntimeError("Презентер не подключён к View.")
        return self._presenter

    def _on_refresh(self) -> None:
        self._require_presenter().refresh_plan()

    def _dialog_newlyweds(self) -> None:
        fields = [
            ("Имя первого молодожена", "partner_one"),
            ("Имя второго молодожена", "partner_two"),
        ]
        values = self._open_form_dialog("Молодожены", fields)
        if values:
            self._require_presenter().set_newlyweds(
                values["partner_one"],
                values["partner_two"],
            )

    def _dialog_guest(self) -> None:
        values = self._open_form_dialog("Новый гость", [("Имя гостя", "guest")])
        if values:
            self._require_presenter().add_guest(values["guest"])

    def _dialog_date_place(self) -> None:
        fields = [
            ("Дата свадьбы", "date"),
            ("Место церемонии", "ceremony_place"),
            ("Место банкета", "banquet_venue"),
        ]
        values = self._open_form_dialog("Дата и места", fields)
        if values:
            self._require_presenter().choose_date_place(
                values["date"],
                values["ceremony_place"],
                values["banquet_venue"],
            )

    def _dialog_outfit(self) -> None:
        fields = [
            ("Описание платья невесты", "dress"),
            ("Описание колец", "rings"),
        ]
        values = self._open_form_dialog("Свадебный наряд", fields)
        if values:
            self._require_presenter().choose_outfit(values["dress"], values["rings"])

    def _dialog_menu(self) -> None:
        values = self._open_form_dialog(
            "Свадебное меню",
            [("Пункты меню (через запятую)", "menu")],
            width=420,
        )
        if values:
            self._require_presenter().prepare_menu(values["menu"])

    def _dialog_ceremony(self) -> None:
        values = self._open_form_dialog(
            "Церемония",
            [("Сценарий церемонии", "script")],
            width=420,
            multiline_keys={"script"},
        )
        if values:
            self._require_presenter().organize_ceremony(values["script"])

    def _dialog_photo(self) -> None:
        values = self._open_form_dialog(
            "Фотосессия",
            [("План фотосессии", "photo_plan")],
            width=420,
            multiline_keys={"photo_plan"},
        )
        if values:
            self._require_presenter().prepare_photo_session(values["photo_plan"])

    def _open_form_dialog(
        self,
        title: str,
        fields: list[tuple[str, str]],
        *,
        width: int = 360,
        multiline_keys: set[str] | None = None,
    ) -> dict[str, str] | None:
        """Модальное окно ввода; возвращает словарь полей или None при отмене."""
        multiline_keys = multiline_keys or set()
        dialog = tk.Toplevel(self._root)
        dialog.title(title)
        dialog.transient(self._root)
        dialog.grab_set()

        frame = ttk.Frame(dialog, padding=12)
        frame.pack(fill=tk.BOTH, expand=True)

        entries: dict[str, tk.Widget] = {}
        for row, (label, key) in enumerate(fields):
            ttk.Label(frame, text=label).grid(row=row * 2, column=0, sticky=tk.W, pady=(0, 2))
            if key in multiline_keys:
                widget: tk.Widget = tk.Text(frame, width=48, height=4, wrap=tk.WORD)
            else:
                widget = ttk.Entry(frame, width=42)
            widget.grid(row=row * 2 + 1, column=0, sticky=tk.EW, pady=(0, 8))
            entries[key] = widget

        result: dict[str, str] | None = None

        def on_ok() -> None:
            nonlocal result
            collected: dict[str, str] = {}
            for _, key in fields:
                widget = entries[key]
                if isinstance(widget, tk.Text):
                    collected[key] = widget.get("1.0", tk.END).strip()
                else:
                    collected[key] = widget.get().strip()
            result = collected
            dialog.destroy()

        def on_cancel() -> None:
            dialog.destroy()

        buttons = ttk.Frame(frame)
        buttons.grid(row=len(fields) * 2, column=0, pady=(4, 0))
        ttk.Button(buttons, text="Сохранить", command=on_ok).pack(side=tk.LEFT, padx=4)
        ttk.Button(buttons, text="Отмена", command=on_cancel).pack(side=tk.LEFT, padx=4)

        dialog.update_idletasks()
        x = self._root.winfo_x() + (self._root.winfo_width() - width) // 2
        y = self._root.winfo_y() + 80
        dialog.geometry(f"{width}x{dialog.winfo_reqheight()}+{x}+{y}")
        dialog.wait_window()
        return result
