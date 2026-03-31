#pragma once
#include "Stack.h"
#include "lib/ArraySequence.h"
#include <string>

struct Move {
    std::string from;
    std::string to;
    int disk;
};

template <typename T>
class Hanoi {
public:
    Hanoi(Stack<T> source, std::string nameA, std::string nameB, std::string nameC);

    void Solve();
    const MutableArraySequence<Move>& GetMoves() const;
    void PrintMoves() const;

    const Stack<T>& GetPegA() const;
    const Stack<T>& GetPegB() const;
    const Stack<T>& GetPegC() const;

private:
    Stack<T> pegA_, pegB_, pegC_;
    std::string nameA_, nameB_, nameC_;
    MutableArraySequence<Move> moves_;

    void MoveDisk(Stack<T>& from, Stack<T>& to,
        const std::string& fromName, const std::string& toName);

    void Solve(int n, Stack<T>& from, Stack<T>& aux, Stack<T>& to,
        const std::string& fromName, const std::string& auxName,
        const std::string& toName);
};

#include "Hanoi.tpp"