#include "../include/Set.h"

bool Set::charExists(char c) const {
    return std::find(chars.begin(), chars.end(), c) != chars.end();
}

bool Set::setExists(const SetPtr& set) const {
    return std::find_if(elements.begin(), elements.end(),
        [&set](const SetPtr& elem) { return *elem == *set; }) != elements.end();
}

void Set::removeChar(char c) {
    auto it = std::find(chars.begin(), chars.end(), c);
    if (it != chars.end()) {
        chars.erase(it);
    }
}

void Set::removeSet(const SetPtr& set) {
    auto it = std::find_if(elements.begin(), elements.end(),
        [&set](const SetPtr& elem) { return *elem == *set; });
    if (it != elements.end()) {
        elements.erase(it);
    }
}

int Set::findCharIndex(char c) const {
    auto it = std::find(chars.begin(), chars.end(), c);
    return (it != chars.end()) ? std::distance(chars.begin(), it) : -1;
}

int Set::findSetIndex(const SetPtr& set) const {
    auto it = std::find_if(elements.begin(), elements.end(),
        [&set](const SetPtr& elem) { return *elem == *set; });
    return (it != elements.end()) ? std::distance(elements.begin(), it) : -1;
}

void Set::skipWhitespace(const std::string& str, size_t& pos) {
    while (pos < str.length() && std::isspace(str[pos])) {
        ++pos;
    }
}

/**
 * Парсинг строковых представлений множества
 * Примеры: "a", "{a, b}", "{a, {b, c}}", "{}"
 */
SetPtr Set::parseString(const std::string& str, size_t& pos) {
    SetPtr result = std::make_shared<Set>();
    skipWhitespace(str, pos);

    if (pos >= str.length()) {
        return result;
    }

    // Пустое множество или начало множества
    if (str[pos] == '{') {
        ++pos; // Пропустить '{'
        skipWhitespace(str, pos);

        while (pos < str.length() && str[pos] != '}') {
            skipWhitespace(str, pos);

            if (str[pos] == '{') {
                // Вложенное множество
                auto innerSet = parseString(str, pos);
                result->addElement(innerSet);
            } else if (std::isalpha(str[pos]) || std::isdigit(str[pos])) {
                // Символьный элемент
                result->addElement(str[pos]);
                ++pos;
            } else if (str[pos] != ',' && str[pos] != '}') {
                throw std::invalid_argument("Неожиданный символ в множестве");
            }

            skipWhitespace(str, pos);

            if (pos < str.length() && str[pos] == ',') {
                ++pos; // Пропустить ','
                skipWhitespace(str, pos);
            }
        }

        if (pos < str.length() && str[pos] == '}') {
            ++pos; // Пропустить '}'
        }
    } else if (std::isalpha(str[pos]) || std::isdigit(str[pos])) {
        // Одиночный символ
        result->addElement(str[pos]);
        ++pos;
    }

    return result;
}


// Конструкторы
Set::Set(const std::string& str) {
    size_t pos = 0;
    auto parsed = parseString(str, pos);
    chars = parsed->chars;
    elements = parsed->elements;
}

Set::Set(const char* str) : Set(std::string(str)) {}

Set::Set(const Set& other) : chars(other.chars), elements(other.elements) {}

Set& Set::operator=(const Set& other) {
    if (this != &other) {
        chars = other.chars;
        elements = other.elements;
    }
    return *this;
}

Set::Set(Set&& other) noexcept 
    : chars(std::move(other.chars)), elements(std::move(other.elements)) {}

Set& Set::operator=(Set&& other) noexcept {
    if (this != &other) {
        chars = std::move(other.chars);
        elements = std::move(other.elements);
    }
    return *this;
}

// Основные операции
bool Set::isEmpty() const {
    return chars.empty() && elements.empty();
}

size_t Set::cardinality() const {
    return chars.size() + elements.size();
}

void Set::addElement(char c) {
    if (!charExists(c)) {
        chars.push_back(c);
    }
}

void Set::addElement(const SetPtr& set) {
    if (!setExists(set)) {
        elements.push_back(set);
    }
}

void Set::removeElement(char c) {
    removeChar(c);
}

void Set::removeElement(const SetPtr& set) {
    removeSet(set);
}

bool Set::contains(char c) const {
    return charExists(c);
}

bool Set::contains(const SetPtr& set) const {
    return setExists(set);
}

// Теоретико-множественные операции
Set Set::unite(const Set& other) const {
    Set result(*this);
    for (char c : other.chars) {
        result.addElement(c);
    }
    for (const auto& elem : other.elements) {
        result.addElement(elem);
    }
    return result;
}

Set Set::intersect(const Set& other) const {
    Set result;
    
    for (char c : chars) {
        if (other.charExists(c)) {
            result.addElement(c);
        }
    }
    
    for (const auto& elem : elements) {
        if (other.setExists(elem)) {
            result.addElement(elem);
        }
    }
    
    return result;
}

Set Set::difference(const Set& other) const {
    Set result(*this);
    
    for (char c : other.chars) {
        result.removeChar(c);
    }
    
    for (const auto& elem : other.elements) {
        result.removeSet(elem);
    }
    
    return result;
}

/**
 * Булеан (powerset) - множество всех подмножеств
 * Возвращает вектор указателей на все подмножества
 */
std::vector<SetPtr> Set::powerset() const {
    std::vector<SetPtr> result;
    size_t n = cardinality();
    
    // 2^n подмножеств
    for (size_t mask = 0; mask < (1u << n); ++mask) {
        SetPtr subset = std::make_shared<Set>();
        
        // Добавляем символьные элементы
        for (size_t i = 0; i < chars.size(); ++i) {
            if (mask & (1u << i)) {
                subset->addElement(chars[i]);
            }
        }
        
        // Добавляем элементы-множества
        for (size_t i = 0; i < elements.size(); ++i) {
            if (mask & (1u << (chars.size() + i))) {
                subset->addElement(elements[i]);
            }
        }
        
        result.push_back(subset);
    }
    
    return result;
}

// Перегруженные операторы
bool Set::operator[](char c) const {
    return contains(c);
}

bool Set::operator[](const SetPtr& set) const {
    return contains(set);
}

Set Set::operator+(const Set& other) const {
    return unite(other);
}

Set& Set::operator+=(const Set& other) {
    *this = unite(other);
    return *this;
}

Set Set::operator*(const Set& other) const {
    return intersect(other);
}

Set& Set::operator*=(const Set& other) {
    *this = intersect(other);
    return *this;
}

Set Set::operator-(const Set& other) const {
    return difference(other);
}

Set& Set::operator-=(const Set& other) {
    *this = difference(other);
    return *this;
}

bool Set::operator==(const Set& other) const {
    if (chars.size() != other.chars.size() || elements.size() != other.elements.size()) {
        return false;
    }
    
    for (char c : chars) {
        if (!other.charExists(c)) {
            return false;
        }
    }
    
    for (const auto& elem : elements) {
        if (!other.setExists(elem)) {
            return false;
        }
    }
    
    return true;
}

bool Set::operator!=(const Set& other) const {
    return !(*this == other);
}

// Вывод
std::string Set::toString() const {
    if (isEmpty()) {
        return "{}";
    }
    
    std::string result = "{";
    
    // Символьные элементы
    for (size_t i = 0; i < chars.size(); ++i) {
        result += chars[i];
        if (i + 1 < chars.size() || !elements.empty()) {
            result += ", ";
        }
    }
    
    // Элементы-множества
    for (size_t i = 0; i < elements.size(); ++i) {
        result += elements[i]->toString();
        if (i + 1 < elements.size()) {
            result += ", ";
        }
    }
    
    result += "}";
    return result;
}

std::ostream& operator<<(std::ostream& os, const Set& set) {
    os << set.toString();
    return os;
}

void Set::buildCantorSetHelper(const std::vector<char>& elements, 
                                int start, int end, Set& result) const {
    // Базовый случай: пустой диапазон
    if (start > end) {
        return;
    }

    int length = end - start + 1;

    // Базовый случай: один элемент
    if (length == 1) {
        result.addElement(elements[start]);
        return;
    }

    // Базовый случай: два элемента (оба включаются)
    if (length == 2) {
        result.addElement(elements[start]);
        result.addElement(elements[end]);
        return;
    }

    // Вычисление индексов элементов для удаления (средняя часть)
    int removeStart, removeEnd;

    if (length % 2 == 1) {
        // Нечетное количество: удаляем центральный элемент
        int middle = start + length / 2;
        removeStart = middle;
        removeEnd = middle;
    }
    else {
        // Четное количество: удаляем два центральных элемента
        int middle1 = start + length / 2 - 1;
        int middle2 = start + length / 2;
        removeStart = middle1;
        removeEnd = middle2;
    }

    // Рекурсивная обработка левой и правой частей
    buildCantorSetHelper(elements, start, removeStart - 1, result);
    buildCantorSetHelper(elements, removeEnd + 1, end, result);
}

Set Set::buildCantorSet() const {
    Set result;
    
    // Проверка на пустое множество
    if (chars.empty()) {
        return result;
    }
    
    // Создаем копию символьных элементов для обработки
    std::vector<char> sortedChars = chars;
    std::sort(sortedChars.begin(), sortedChars.end());
    
    // Запускаем рекурсивное построение множества Кантора
    buildCantorSetHelper(sortedChars, 0, sortedChars.size() - 1, result);
    
    return result;
}


