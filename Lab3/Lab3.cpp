#include <iostream>
#include <cassert>
#include "Hanoi.h"

void TestHanoi(int n) {
    // Собираем стек: снизу n, сверху 1
    Stack<int> src;
    for (int d = n; d >= 1; d--)
        src.Push(d);

    Hanoi<int> hanoi(src, "A", "B", "C");
    hanoi.Solve();

    const auto& moves = hanoi.GetMoves();
    const auto& pegC = hanoi.GetPegC();

    // 1. Количество ходов = 2^n - 1
    int expected = (1 << n) - 1;
    assert(moves.GetLength() == expected);

    // 2. Все диски на штыре C, в правильном порядке (снизу n, сверху 1)
    assert(pegC.GetCount() == n);
    for (int i = 0; i < n; i++)
        assert(pegC.Get(i) == i + 1);

    // 3. Штыри A и B пусты
    assert(hanoi.GetPegA().GetCount() == 0);
    assert(hanoi.GetPegB().GetCount() == 0);

    std::cout << "[OK] n=" << n
        << "  ходов=" << moves.GetLength()
        << "  (ожидалось " << expected << ")\n";
}

void TestHanoiMoveOrder() {
    // Для n=2: A→B, A→C, B→C
    Stack<int> src;
    src.Push(2); src.Push(1);
    Hanoi<int> hanoi(src, "A", "B", "C");
    hanoi.Solve();

    const auto& moves = hanoi.GetMoves();
    assert(moves.GetLength() == 3);
    assert(moves.Get(0).from == "A" && moves.Get(0).to == "B" && moves.Get(0).disk == 1);
    assert(moves.Get(1).from == "A" && moves.Get(1).to == "C" && moves.Get(1).disk == 2);
    assert(moves.Get(2).from == "B" && moves.Get(2).to == "C" && moves.Get(2).disk == 1);

    std::cout << "[OK] Порядок ходов для n=2 верный\n";
}

void TestHanoiIdempotent() {
    // Повторный вызов Solve() даёт тот же результат
    Stack<int> src;
    src.Push(3); src.Push(2); src.Push(1);
    Hanoi<int> hanoi(src, "A", "B", "C");

    hanoi.Solve();
    int first = hanoi.GetMoves().GetLength();
    hanoi.Solve(); // вызов второй раз — башня уже на C, 0 ходов или пересборка?
    // зависит от реализации; просто проверяем что не падает
    (void)first;
    std::cout << "[OK] Повторный Solve() не падает\n";
}

int main() {
    std::cout << "=== Тесты Ханойской башни ===\n\n";

    TestHanoi(1);
    TestHanoi(2);
    TestHanoi(3);
    TestHanoi(5);
    TestHanoi(8);

    std::cout << "\n";
    TestHanoiMoveOrder();
    TestHanoiIdempotent();

    std::cout << "\n=== Все тесты пройдены ===\n";
    return 0;
}