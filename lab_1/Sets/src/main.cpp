#include "../include/Set.h"

#include <iostream>

int main() {
    std::cout << "=== Пример 1: Простое множество ===" << std::endl;
    Set s1("{a, b, c}");
    std::cout << "s1 = " << s1 << std::endl;
    std::cout << "Мощность s1: " << s1.cardinality() << std::endl;
    std::cout << "Содержит 'a'? " << s1['a'] << std::endl;
    
    std::cout << "\n=== Пример 2: Множество с вложенными множествами ===" << std::endl;
    Set s2("{a, b, {a, b}, {}, {a, {c}}}");
    std::cout << "s2 = " << s2 << std::endl;
    std::cout << "Мощность s2: " << s2.cardinality() << std::endl;
    std::cout << "Пусто ли s2? " << s2.isEmpty() << std::endl;
    
    std::cout << "\n=== Пример 3: Добавление и удаление ===" << std::endl;
    Set s3;
    s3.addElement('x');
    s3.addElement('y');
    s3.addElement('z');
    std::cout << "После добавления: " << s3 << std::endl;
    s3.removeElement('y');
    std::cout << "После удаления 'y': " << s3 << std::endl;
    
    std::cout << "\n=== Пример 4: Объединение (A ∪ B) ===" << std::endl;
    Set a("{a, b, c}");
    Set b("{b, c, d}");
    std::cout << "A = " << a << std::endl;
    std::cout << "B = " << b << std::endl;
    std::cout << "A ∪ B = " << (a + b) << std::endl;
    
    std::cout << "\n=== Пример 5: Пересечение (A ∩ B) ===" << std::endl;
    std::cout << "A ∩ B = " << (a * b) << std::endl;
    
    std::cout << "\n=== Пример 6: Разность (A \\ B) ===" << std::endl;
    std::cout << "A \\ B = " << (a - b) << std::endl;
    
    std::cout << "\n=== Пример 7: Булеан P(A) ===" << std::endl;
    Set simple("{a, b}");
    auto power = simple.powerset();
    std::cout << "P({a, b}) содержит " << power.size() << " подмножеств:" << std::endl;
    for (const auto& subset : power) {
        std::cout << "  " << *subset << std::endl;
    }
    
    std::cout << "\n=== Пример 8: Операторные присваивания ===" << std::endl;
    Set x("{1, 2, 3}");
    Set y("{2, 3, 4}");
    std::cout << "x = " << x << std::endl;
    std::cout << "y = " << y << std::endl;
    x += y;
    std::cout << "После x += y: " << x << std::endl;
    x *= Set("{2, 3}");
    std::cout << "После x *= {2, 3}: " << x << std::endl;
    
    std::cout << "\n=== Пример 9: Множество Кантора ===" << std::endl;
    std::cout << "\n--- 9.1: Множество из 9 элементов ---" << std::endl;
    Set cantor1;
    for (char c = 'a'; c <= 'i'; ++c) {
        cantor1.addElement(c);
    }
    std::cout << "Исходное множество: " << cantor1 << std::endl;
    std::cout << "Мощность: " << cantor1.cardinality() << std::endl;
    
    Set result1 = cantor1.buildCantorSet();
    std::cout << "Множество Кантора: " << result1 << std::endl;
    std::cout << "Мощность после применения: " << result1.cardinality() << std::endl;
    std::cout << "Удалено элементов: " << (cantor1.cardinality() - result1.cardinality()) << std::endl;
    
    std::cout << "\n--- 9.2: Множество из 8 элементов ---" << std::endl;
    Set cantor2("{1, 2, 3, 4, 5, 6, 7, 8}");
    std::cout << "Исходное множество: " << cantor2 << std::endl;
    std::cout << "Мощность: " << cantor2.cardinality() << std::endl;
    
    Set result2 = cantor2.buildCantorSet();
    std::cout << "Множество Кантора: " << result2 << std::endl;
    std::cout << "Мощность после применения: " << result2.cardinality() << std::endl;
    
    std::cout << "\n--- 9.3: Множество из 3 элементов ---" << std::endl;
    Set cantor3("{x, y, z}");
    std::cout << "Исходное множество: " << cantor3 << std::endl;
    Set result3 = cantor3.buildCantorSet();
    std::cout << "Множество Кантора: " << result3 << std::endl;
    std::cout << "Примечание: центральный элемент 'y' удален" << std::endl;
    
    std::cout << "\n--- 9.4: Множество из 2 элементов ---" << std::endl;
    Set cantor4("{A, B}");
    std::cout << "Исходное множество: " << cantor4 << std::endl;
    Set result4 = cantor4.buildCantorSet();
    std::cout << "Множество Кантора: " << result4 << std::endl;
    std::cout << "Примечание: оба элемента сохранены (базовый случай)" << std::endl;
    
    std::cout << "\n--- 9.5: Пустое множество ---" << std::endl;
    Set emptySet;
    std::cout << "Исходное множество: " << emptySet << std::endl;
    Set result5 = emptySet.buildCantorSet();
    std::cout << "Множество Кантора: " << result5 << std::endl;
    
    std::cout << "\n--- 9.6: Большое множество (27 элементов) ---" << std::endl;
    Set cantor6;
    for (char c = 'A'; c <= 'Z'; ++c) {
        cantor6.addElement(c);
    }
    cantor6.addElement('0');
    std::cout << "Исходное множество: " << cantor6 << std::endl;
    std::cout << "Мощность: " << cantor6.cardinality() << std::endl;
    
    Set result6 = cantor6.buildCantorSet();
    std::cout << "Множество Кантора: " << result6 << std::endl;
    std::cout << "Мощность: " << result6.cardinality() << std::endl;
    std::cout << "Коэффициент сжатия: " 
              << static_cast<double>(result6.cardinality()) / cantor6.cardinality() * 100 
              << "%" << std::endl;
    
    return 0;
}
