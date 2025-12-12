#ifndef SET_H
#define SET_H

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>

/**
 * @file Set.h
 * @brief Определение класса Set для работы с математическими множествами
 */

class Set;
/// Умный указатель на объект Set
using SetPtr = std::shared_ptr<Set>;

/**
 * @class Set
 * @brief Класс для представления математических множеств
 * 
 * Класс поддерживает работу с множествами, содержащими символьные элементы
 * и вложенные множества. Реализует основные операции теории множеств:
 * объединение, пересечение, разность, а также вычисление булеана.
 */
class Set {
private:
    std::vector<SetPtr> elements;  ///< Вложенные множества
    std::vector<char> chars;       ///< Символьные элементы

    /**
     * @brief Проверяет наличие символа в множестве
     * @param c Проверяемый символ
     * @return true если символ существует, иначе false
     */
    bool charExists(char c) const;
    
    /**
     * @brief Проверяет наличие вложенного множества
     * @param set Указатель на проверяемое множество
     * @return true если множество существует, иначе false
     */
    bool setExists(const SetPtr& set) const;
    
    /**
     * @brief Удаляет символ из множества
     * @param c Удаляемый символ
     */
    void removeChar(char c);
    
    /**
     * @brief Удаляет вложенное множество
     * @param set Указатель на удаляемое множество
     */
    void removeSet(const SetPtr& set);
    
    /**
     * @brief Находит индекс символа в векторе chars
     * @param c Искомый символ
     * @return Индекс символа или -1 если не найден
     */
    int findCharIndex(char c) const;
    
    /**
     * @brief Находит индекс множества в векторе elements
     * @param set Указатель на искомое множество
     * @return Индекс множества или -1 если не найдено
     */
    int findSetIndex(const SetPtr& set) const;
    
    /**
     * @brief Парсит строковое представление множества
     * @param str Входная строка
     * @param pos Текущая позиция в строке (изменяется при парсинге)
     * @return Указатель на созданное множество
     * @throw std::invalid_argument При некорректном формате строки
     */
    static SetPtr parseString(const std::string& str, size_t& pos);
    
    /**
     * @brief Пропускает пробельные символы в строке
     * @param str Входная строка
     * @param pos Текущая позиция (изменяется)
     */
    static void skipWhitespace(const std::string& str, size_t& pos);

       /**
     * @brief Вспомогательная рекурсивная функция для построения множества Кантора
     * @param elements Массив символьных элементов
     * @param start Начальный индекс
     * @param end Конечный индекс
     * @param result Результирующее множество
     */
    void buildCantorSetHelper(const std::vector<char>& elements, 
                              int start, int end, Set& result) const;

public:
    /**
     * @brief Конструктор по умолчанию
     * @details Создает пустое множество
     */
    Set() = default;
    
    /**
     * @brief Конструктор из строки
     * @param str Строковое представление множества (например, "{a, b, {c, d}}")
     * @throw std::invalid_argument При некорректном формате
     */
    explicit Set(const std::string& str);
    
    /**
     * @brief Конструктор из C-строки
     * @param str C-строка с представлением множества
     * @throw std::invalid_argument При некорректном формате
     */
    explicit Set(const char* str);
    
    /**
     * @brief Конструктор копирования
     * @param other Копируемое множество
     */
    Set(const Set& other);
    
    /**
     * @brief Оператор присваивания копированием
     * @param other Копируемое множество
     * @return Ссылка на текущий объект
     */
    Set& operator=(const Set& other);
    
    /**
     * @brief Конструктор перемещения
     * @param other Перемещаемое множество
     */
    Set(Set&& other) noexcept;

        /**
     * @brief Создает множество Кантора из символьных элементов
     * @details Множество Кантора строится путем рекурсивного удаления средних элементов.
     *          Для нечетного количества элементов удаляется центральный элемент.
     *          Для четного количества удаляются два центральных элемента.
     * @return Новое множество Кантора
     * @note Работает только с символьными элементами, вложенные множества игнорируются
     */
    Set buildCantorSet() const;
    
    /**
     * @brief Оператор присваивания перемещением
     * @param other Перемещаемое множество
     * @return Ссылка на текущий объект
     */
    Set& operator=(Set&& other) noexcept;
    
    /**
     * @brief Проверяет, пусто ли множество
     * @return true если множество пустое, иначе false
     */
    bool isEmpty() const;
    
    /**
     * @brief Возвращает мощность множества (количество элементов)
     * @return Количество элементов в множестве
     */
    size_t cardinality() const;
    
    /**
     * @brief Добавляет символьный элемент
     * @param c Добавляемый символ
     */
    void addElement(char c);
    
    /**
     * @brief Добавляет вложенное множество
     * @param set Указатель на добавляемое множество
     */
    void addElement(const SetPtr& set);
    
    /**
     * @brief Удаляет символьный элемент
     * @param c Удаляемый символ
     */
    void removeElement(char c);
    
    /**
     * @brief Удаляет вложенное множество
     * @param set Указатель на удаляемое множество
     */
    void removeElement(const SetPtr& set);
    
    /**
     * @brief Проверяет принадлежность символа множеству
     * @param c Проверяемый символ
     * @return true если символ принадлежит множеству
     */
    bool contains(char c) const;
    
    /**
     * @brief Проверяет принадлежность вложенного множества
     * @param set Указатель на проверяемое множество
     * @return true если множество принадлежит данному множеству
     */
    bool contains(const SetPtr& set) const;
    
    /**
     * @brief Вычисляет объединение двух множеств
     * @param other Второе множество
     * @return Новое множество - объединение
     */
    Set unite(const Set& other) const;
    
    /**
     * @brief Вычисляет пересечение двух множеств
     * @param other Второе множество
     * @return Новое множество - пересечение
     */
    Set intersect(const Set& other) const;
    
    /**
     * @brief Вычисляет разность множеств (this \ other)
     * @param other Вычитаемое множество
     * @return Новое множество - разность
     */
    Set difference(const Set& other) const;
    
    /**
     * @brief Вычисляет булеан (множество всех подмножеств)
     * @return Вектор указателей на все подмножества
     */
    std::vector<SetPtr> powerset() const;
    
    /**
     * @brief Оператор проверки принадлежности символа
     * @param c Проверяемый символ
     * @return true если символ принадлежит множеству
     */
    bool operator[](char c) const;
    
    /**
     * @brief Оператор проверки принадлежности множества
     * @param set Указатель на проверяемое множество
     * @return true если множество принадлежит данному множеству
     */
    bool operator[](const SetPtr& set) const;
    
    /**
     * @brief Оператор объединения множеств
     * @param other Второе множество
     * @return Новое множество - объединение
     */
    Set operator+(const Set& other) const;
    
    /**
     * @brief Оператор объединения с присваиванием
     * @param other Второе множество
     * @return Ссылка на текущий объект
     */
    Set& operator+=(const Set& other);
    
    /**
     * @brief Оператор пересечения множеств
     * @param other Второе множество
     * @return Новое множество - пересечение
     */
    Set operator*(const Set& other) const;
    
    /**
     * @brief Оператор пересечения с присваиванием
     * @param other Второе множество
     * @return Ссылка на текущий объект
     */
    Set& operator*=(const Set& other);
    
    /**
     * @brief Оператор разности множеств
     * @param other Вычитаемое множество
     * @return Новое множество - разность
     */
    Set operator-(const Set& other) const;
    
    /**
     * @brief Оператор разности с присваиванием
     * @param other Вычитаемое множество
     * @return Ссылка на текущий объект
     */
    Set& operator-=(const Set& other);
    
    /**
     * @brief Оператор сравнения на равенство
     * @param other Сравниваемое множество
     * @return true если множества равны
     */
    bool operator==(const Set& other) const;
    
    /**
     * @brief Оператор сравнения на неравенство
     * @param other Сравниваемое множество
     * @return true если множества не равны
     */
    bool operator!=(const Set& other) const;
    
    /**
     * @brief Оператор вывода в поток
     * @param os Выходной поток
     * @param set Выводимое множество
     * @return Ссылка на поток
     */
    friend std::ostream& operator<<(std::ostream& os, const Set& set);
    
    /**
     * @brief Преобразует множество в строковое представление
     * @return Строка в формате "{element1, element2, ...}"
     */
    std::string toString() const;
};

#endif
