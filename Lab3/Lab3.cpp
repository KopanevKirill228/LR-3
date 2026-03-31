#include <iostream>
#include "Stack.h"
#include "Queue.h"
#include "lib/MapReduce.h"
#include "Hanoi.h"

int main() {
    Stack<int> source;
    source.Push(4);  // кладём снизу вверх: 4 — самый большой
    source.Push(3);
    source.Push(2);
    source.Push(1);  // 1 — на вершине (самый маленький)

    Hanoi<int> hanoi(source, "A", "B", "C");
    hanoi.Solve();
    hanoi.PrintMoves();

    std::cout << "Всего ходов: " << hanoi.GetMoves().size() << "\n";

    return 0;
}