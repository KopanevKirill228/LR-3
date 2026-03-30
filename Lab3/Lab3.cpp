#include <iostream>
#include "Stack.h"
#include "Queue.h"
#include "lib/MapReduce.h"

int main() {
    // === Stack ===
    std::cout << "=== Stack ===" << std::endl;

    Stack<int> s;
    s.Push(1); s.Push(2); s.Push(3); s.Push(4); s.Push(5);

    std::cout << "Stack: ";
    for (size_t i = 0; i < s.GetCount(); i++) std::cout << s.Get(i) << " ";
    std::cout << std::endl;

    // Map: удвоить
    auto* mapped = s.GetSequence()->Map([](const int& x) { return x * 2; });
    std::cout << "Map (*2): ";
    for (int i = 0; i < mapped->GetLength(); i++) std::cout << mapped->Get(i) << " ";
    std::cout << std::endl;
    delete mapped;

    // Where: только чётные
    auto* filtered = s.GetSequence()->Where([](const int& x) { return x % 2 == 0; });
    std::cout << "Where (even): ";
    for (int i = 0; i < filtered->GetLength(); i++) std::cout << filtered->Get(i) << " ";
    std::cout << std::endl;
    delete filtered;

    // Reduce: сумма
    int sum = s.GetSequence()->Reduce(
        [](const int& acc, const int& x) { return acc + x; }, 0);
    std::cout << "Reduce (sum): " << sum << std::endl;

    // === Queue ===
    std::cout << "\n=== Queue ===" << std::endl;

    Queue<int> q;
    q.Enqueue(10); q.Enqueue(20); q.Enqueue(30); q.Enqueue(40);

    std::cout << "Queue: ";
    for (size_t i = 0; i < q.GetCount(); i++) std::cout << q.Get(i) << " ";
    std::cout << std::endl;

    // Map: x / 10
    auto* qmapped = q.GetSequence()->Map([](const int& x) { return x / 10; });
    std::cout << "Map (/10): ";
    for (int i = 0; i < qmapped->GetLength(); i++) std::cout << qmapped->Get(i) << " ";
    std::cout << std::endl;
    delete qmapped;

    // Where: > 15
    auto* qfiltered = q.GetSequence()->Where([](const int& x) { return x > 15; });
    std::cout << "Where (>15): ";
    for (int i = 0; i < qfiltered->GetLength(); i++) std::cout << qfiltered->Get(i) << " ";
    std::cout << std::endl;
    delete qfiltered;

    // Reduce: произведение
    int product = q.GetSequence()->Reduce(
        [](const int& acc, const int& x) { return acc * x; }, 1);
    std::cout << "Reduce (product): " << product << std::endl;

    return 0;
}