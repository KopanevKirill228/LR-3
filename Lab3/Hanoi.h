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
    Hanoi(Stack<T> source, const std::string& nameA, const std::string& nameB, const std::string& nameC);

    void Solve();

    const MutableArraySequence<Move>& GetMoves() const;
    const Stack<T>& GetPegA() const;
    const Stack<T>& GetPegB() const;
    const Stack<T>& GetPegC() const;
    int GetMaxDisk() const;
    int GetDiskCount() const;

private:
    Stack<T> pegA_, pegB_, pegC_;
    std::string nameA_, nameB_, nameC_;
    MutableArraySequence<Move> moves_;
    int maxDisk_ = 0;

    void MoveDisk(Stack<T>& from, Stack<T>& to,
        const std::string& fromName, const std::string& toName);

    void Solve(int n, Stack<T>& from, Stack<T>& aux, Stack<T>& to,
        const std::string& fromName, const std::string& auxName,
        const std::string& toName);
};

#include "Hanoi.tpp"