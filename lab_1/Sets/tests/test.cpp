#include <gtest/gtest.h>
#include "../include/Set.h"

// ============================================================
// Тесты конструкторов
// ============================================================

class SetConstructorTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(SetConstructorTest, DefaultConstructor) {
    Set s;
    EXPECT_TRUE(s.isEmpty());
    EXPECT_EQ(s.cardinality(), 0);
}

TEST_F(SetConstructorTest, StringConstructorSimple) {
    Set s("{a, b, c}");
    EXPECT_FALSE(s.isEmpty());
    EXPECT_EQ(s.cardinality(), 3);
    EXPECT_TRUE(s.contains('a'));
    EXPECT_TRUE(s.contains('b'));
    EXPECT_TRUE(s.contains('c'));
}

TEST_F(SetConstructorTest, StringConstructorEmpty) {
    Set s("{}");
    EXPECT_TRUE(s.isEmpty());
    EXPECT_EQ(s.cardinality(), 0);
}

TEST_F(SetConstructorTest, StringConstructorNested) {
    Set s("{a, {b, c}}");
    EXPECT_EQ(s.cardinality(), 2);
    EXPECT_TRUE(s.contains('a'));
}

TEST_F(SetConstructorTest, StringConstructorSingleChar) {
    Set s("a");
    EXPECT_EQ(s.cardinality(), 1);
    EXPECT_TRUE(s.contains('a'));
}

TEST_F(SetConstructorTest, CharPointerConstructor) {
    Set s("{x, y}");
    EXPECT_EQ(s.cardinality(), 2);
}

TEST_F(SetConstructorTest, CopyConstructor) {
    Set original("{a, b}");
    Set copy(original);
    EXPECT_EQ(copy.cardinality(), 2);
    EXPECT_TRUE(copy.contains('a'));
    EXPECT_TRUE(copy.contains('b'));
}

TEST_F(SetConstructorTest, MoveConstructor) {
    Set original("{a, b}");
    Set moved(std::move(original));
    EXPECT_EQ(moved.cardinality(), 2);
    EXPECT_TRUE(moved.contains('a'));
}

TEST_F(SetConstructorTest, CopyAssignment) {
    Set original("{a, b}");
    Set assigned;
    assigned = original;
    EXPECT_EQ(assigned.cardinality(), 2);
    EXPECT_TRUE(assigned.contains('a'));
}

TEST_F(SetConstructorTest, MoveAssignment) {
    Set original("{a, b}");
    Set assigned;
    assigned = std::move(original);
    EXPECT_EQ(assigned.cardinality(), 2);
}

TEST_F(SetConstructorTest, SelfAssignment) {
    Set s("{a, b}");
    s = s;
    EXPECT_EQ(s.cardinality(), 2);
}

TEST_F(SetConstructorTest, InvalidString) {
    EXPECT_THROW(Set("{a, @, b}"), std::invalid_argument);
}

// ============================================================
// Тесты добавления и удаления элементов
// ============================================================

class SetModificationTest : public ::testing::Test {
protected:
    Set s;
};

TEST_F(SetModificationTest, AddSingleChar) {
    s.addElement('a');
    EXPECT_TRUE(s.contains('a'));
    EXPECT_EQ(s.cardinality(), 1);
}

TEST_F(SetModificationTest, AddDuplicateChar) {
    s.addElement('a');
    s.addElement('a');
    EXPECT_EQ(s.cardinality(), 1);
}

TEST_F(SetModificationTest, AddMultipleChars) {
    s.addElement('a');
    s.addElement('b');
    s.addElement('c');
    EXPECT_EQ(s.cardinality(), 3);
}

TEST_F(SetModificationTest, RemoveExistingChar) {
    s.addElement('a');
    s.addElement('b');
    s.removeElement('a');
    EXPECT_FALSE(s.contains('a'));
    EXPECT_TRUE(s.contains('b'));
    EXPECT_EQ(s.cardinality(), 1);
}

TEST_F(SetModificationTest, RemoveNonExistingChar) {
    s.addElement('a');
    s.removeElement('b');
    EXPECT_EQ(s.cardinality(), 1);
}

TEST_F(SetModificationTest, AddNestedSet) {
    auto inner = std::make_shared<Set>("{a, b}");
    s.addElement(inner);
    EXPECT_EQ(s.cardinality(), 1);
    EXPECT_TRUE(s.contains(inner));
}

TEST_F(SetModificationTest, AddDuplicateNestedSet) {
    auto inner = std::make_shared<Set>("{a, b}");
    s.addElement(inner);
    s.addElement(inner);
    EXPECT_EQ(s.cardinality(), 1);
}

TEST_F(SetModificationTest, RemoveNestedSet) {
    auto inner = std::make_shared<Set>("{a, b}");
    s.addElement(inner);
    s.removeElement(inner);
    EXPECT_EQ(s.cardinality(), 0);
}

// ============================================================
// Тесты теоретико-множественных операций
// ============================================================

class SetOperationsTest : public ::testing::Test {
protected:
    Set a;
    Set b;
    
    void SetUp() override {
        a.addElement('a');
        a.addElement('b');
        a.addElement('c');
        
        b.addElement('b');
        b.addElement('c');
        b.addElement('d');
    }
};

TEST_F(SetOperationsTest, Union) {
    Set result = a.unite(b);
    EXPECT_EQ(result.cardinality(), 4);
    EXPECT_TRUE(result.contains('a'));
    EXPECT_TRUE(result.contains('b'));
    EXPECT_TRUE(result.contains('c'));
    EXPECT_TRUE(result.contains('d'));
}

TEST_F(SetOperationsTest, UnionWithEmpty) {
    Set empty;
    Set result = a.unite(empty);
    EXPECT_EQ(result.cardinality(), 3);
}

TEST_F(SetOperationsTest, Intersection) {
    Set result = a.intersect(b);
    EXPECT_EQ(result.cardinality(), 2);
    EXPECT_TRUE(result.contains('b'));
    EXPECT_TRUE(result.contains('c'));
    EXPECT_FALSE(result.contains('a'));
    EXPECT_FALSE(result.contains('d'));
}

TEST_F(SetOperationsTest, IntersectionWithEmpty) {
    Set empty;
    Set result = a.intersect(empty);
    EXPECT_TRUE(result.isEmpty());
}

TEST_F(SetOperationsTest, IntersectionDisjoint) {
    Set x;
    x.addElement('x');
    Set y;
    y.addElement('y');
    Set result = x.intersect(y);
    EXPECT_TRUE(result.isEmpty());
}

TEST_F(SetOperationsTest, Difference) {
    Set result = a.difference(b);
    EXPECT_EQ(result.cardinality(), 1);
    EXPECT_TRUE(result.contains('a'));
    EXPECT_FALSE(result.contains('b'));
}

TEST_F(SetOperationsTest, DifferenceWithEmpty) {
    Set empty;
    Set result = a.difference(empty);
    EXPECT_EQ(result.cardinality(), 3);
}

TEST_F(SetOperationsTest, DifferenceResultEmpty) {
    Set result = a.difference(a);
    EXPECT_TRUE(result.isEmpty());
}

// ============================================================
// Тесты булеана (powerset)
// ============================================================

TEST(SetPowersetTest, EmptySet) {
    Set s;
    auto power = s.powerset();
    EXPECT_EQ(power.size(), 1); // Только пустое множество
    EXPECT_TRUE(power[0]->isEmpty());
}

TEST(SetPowersetTest, SingleElement) {
    Set s;
    s.addElement('a');
    auto power = s.powerset();
    EXPECT_EQ(power.size(), 2); // {}, {a}
}

TEST(SetPowersetTest, TwoElements) {
    Set s;
    s.addElement('a');
    s.addElement('b');
    auto power = s.powerset();
    EXPECT_EQ(power.size(), 4); // {}, {a}, {b}, {a,b}
}

TEST(SetPowersetTest, ThreeElements) {
    Set s("{a, b, c}");
    auto power = s.powerset();
    EXPECT_EQ(power.size(), 8); // 2^3 = 8
}

// ============================================================
// Тесты операторов
// ============================================================

class SetOperatorTest : public ::testing::Test {
protected:
    Set a;
    Set b;
    
    void SetUp() override {
        a = Set("{a, b}");
        b = Set("{b, c}");
    }
};

TEST_F(SetOperatorTest, SubscriptOperatorChar) {
    EXPECT_TRUE(a['a']);
    EXPECT_TRUE(a['b']);
    EXPECT_FALSE(a['c']);
}

TEST_F(SetOperatorTest, SubscriptOperatorSet) {
    auto inner = std::make_shared<Set>("{x}");
    Set s;
    s.addElement(inner);
    EXPECT_TRUE(s[inner]);
}

TEST_F(SetOperatorTest, PlusOperator) {
    Set result = a + b;
    EXPECT_EQ(result.cardinality(), 3);
    EXPECT_TRUE(result.contains('a'));
    EXPECT_TRUE(result.contains('b'));
    EXPECT_TRUE(result.contains('c'));
}

TEST_F(SetOperatorTest, PlusEqualOperator) {
    a += b;
    EXPECT_EQ(a.cardinality(), 3);
}

TEST_F(SetOperatorTest, MultiplyOperator) {
    Set result = a * b;
    EXPECT_EQ(result.cardinality(), 1);
    EXPECT_TRUE(result.contains('b'));
}

TEST_F(SetOperatorTest, MultiplyEqualOperator) {
    a *= b;
    EXPECT_EQ(a.cardinality(), 1);
    EXPECT_TRUE(a.contains('b'));
}

TEST_F(SetOperatorTest, MinusOperator) {
    Set result = a - b;
    EXPECT_EQ(result.cardinality(), 1);
    EXPECT_TRUE(result.contains('a'));
}

TEST_F(SetOperatorTest, MinusEqualOperator) {
    a -= b;
    EXPECT_EQ(a.cardinality(), 1);
    EXPECT_TRUE(a.contains('a'));
}

TEST_F(SetOperatorTest, EqualityOperator) {
    Set x("{a, b}");
    Set y("{a, b}");
    Set z("{a, c}");
    
    EXPECT_TRUE(x == y);
    EXPECT_FALSE(x == z);
}

TEST_F(SetOperatorTest, EqualityEmptySets) {
    Set x;
    Set y;
    EXPECT_TRUE(x == y);
}

TEST_F(SetOperatorTest, EqualityDifferentSizes) {
    Set x("{a}");
    Set y("{a, b}");
    EXPECT_FALSE(x == y);
}

TEST_F(SetOperatorTest, InequalityOperator) {
    Set x("{a}");
    Set y("{b}");
    EXPECT_TRUE(x != y);
}

// ============================================================
// Тесты вывода
// ============================================================

TEST(SetOutputTest, EmptySetToString) {
    Set s;
    EXPECT_EQ(s.toString(), "{}");
}

TEST(SetOutputTest, SimpleSetToString) {
    Set s("{a, b}");
    std::string str = s.toString();
    EXPECT_TRUE(str.find('a') != std::string::npos);
    EXPECT_TRUE(str.find('b') != std::string::npos);
}

TEST(SetOutputTest, StreamOutput) {
    Set s("{a, b}");
    std::stringstream ss;
    ss << s;
    std::string str = ss.str();
    EXPECT_TRUE(str.find('a') != std::string::npos);
}

// ============================================================
// Тесты множества Кантора
// ============================================================

class CantorSetTest : public ::testing::Test {
protected:
    Set s;
};

TEST_F(CantorSetTest, EmptySet) {
    Set result = s.buildCantorSet();
    EXPECT_TRUE(result.isEmpty());
}

TEST_F(CantorSetTest, SingleElement) {
    s.addElement('a');
    Set result = s.buildCantorSet();
    EXPECT_EQ(result.cardinality(), 1);
    EXPECT_TRUE(result.contains('a'));
}

TEST_F(CantorSetTest, TwoElements) {
    s.addElement('a');
    s.addElement('b');
    Set result = s.buildCantorSet();
    EXPECT_EQ(result.cardinality(), 2);
    EXPECT_TRUE(result.contains('a'));
    EXPECT_TRUE(result.contains('b'));
}

TEST_F(CantorSetTest, ThreeElements) {
    s.addElement('a');
    s.addElement('b');
    s.addElement('c');
    Set result = s.buildCantorSet();
    EXPECT_EQ(result.cardinality(), 2);
    EXPECT_TRUE(result.contains('a'));
    EXPECT_FALSE(result.contains('b')); // Центральный удален
    EXPECT_TRUE(result.contains('c'));
}

TEST_F(CantorSetTest, FourElements) {
    s.addElement('a');
    s.addElement('b');
    s.addElement('c');
    s.addElement('d');
    Set result = s.buildCantorSet();
    EXPECT_EQ(result.cardinality(), 2);
    EXPECT_TRUE(result.contains('a'));
    EXPECT_FALSE(result.contains('b'));
    EXPECT_FALSE(result.contains('c'));
    EXPECT_TRUE(result.contains('d'));
}

TEST_F(CantorSetTest, NineElements) {
    for (char c = 'a'; c <= 'i'; ++c) {
        s.addElement(c);
    }
    Set result = s.buildCantorSet();
    EXPECT_LT(result.cardinality(), 9);
    EXPECT_TRUE(result.contains('a')); // Крайний левый
    EXPECT_TRUE(result.contains('i')); // Крайний правый
}

TEST_F(CantorSetTest, EightElements) {
    for (char c = '1'; c <= '8'; ++c) {
        s.addElement(c);
    }
    Set result = s.buildCantorSet();
    EXPECT_EQ(result.cardinality(), 2);
    EXPECT_TRUE(result.contains('1'));
    EXPECT_TRUE(result.contains('8'));
}

TEST_F(CantorSetTest, LargeSet) {
    for (char c = 'A'; c <= 'Z'; ++c) {
        s.addElement(c);
    }
    s.addElement('0');
    Set result = s.buildCantorSet();
    EXPECT_LT(result.cardinality(), 27);
    EXPECT_GT(result.cardinality(), 0);
}

// ============================================================
// Тесты граничных случаев
// ============================================================

TEST(SetEdgeCaseTest, VeryLongString) {
    std::string longStr = "{";
    for (int i = 0; i < 100; ++i) {
        longStr += static_cast<char>('a' + (i % 26));
        if (i < 99) longStr += ", ";
    }
    longStr += "}";
    Set s(longStr);
    EXPECT_LE(s.cardinality(), 26); // Только уникальные символы
}

TEST(SetEdgeCaseTest, DeeplyNestedSets) {
    Set s("{a, {b, {c, {d}}}}");
    EXPECT_GE(s.cardinality(), 1);
}

TEST(SetEdgeCaseTest, ManySpaces) {
    Set s("{  a  ,  b  ,  c  }");
    EXPECT_EQ(s.cardinality(), 3);
}

TEST(SetEdgeCaseTest, NumbersAsChars) {
    Set s("{1, 2, 3}");
    EXPECT_EQ(s.cardinality(), 3);
    EXPECT_TRUE(s.contains('1'));
}

// ============================================================
// Main функция для запуска тестов
// ============================================================

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
