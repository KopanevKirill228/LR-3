#pragma once
#include "Hanoi.h"

template <typename T>
Hanoi<T>::Hanoi(Stack<T> source, std::string nameA, std::string nameB, std::string nameC)
    : pegA_(source), pegB_(), pegC_(),
    nameA_(nameA), nameB_(nameB), nameC_(nameC) {
}

template <typename T>
void Hanoi<T>::MoveDisk(Stack<T>& from, Stack<T>& to,
    const std::string& fromName, const std::string& toName) {
    T disk = from.Pop();
    to.Push(disk);
    Move m;
    m.from = fromName;
    m.to = toName;
    m.disk = static_cast<int>(disk);
    moves_.Append(m);
}

template <typename T>
void Hanoi<T>::Solve(int n, Stack<T>& from, Stack<T>& aux, Stack<T>& to,
    const std::string& fromName, const std::string& auxName,
    const std::string& toName) {
    if (n <= 0) return;
    Solve(n - 1, from, to, aux, fromName, toName, auxName);
    MoveDisk(from, to, fromName, toName);
    Solve(n - 1, aux, from, to, auxName, fromName, toName);
}

template <typename T>
void Hanoi<T>::Solve() {
    moves_ = MutableArraySequence<Move>();
    int n = static_cast<int>(pegA_.GetCount());
    Solve(n, pegA_, pegB_, pegC_, nameA_, nameB_, nameC_);
}

template <typename T>
const MutableArraySequence<Move>& Hanoi<T>::GetMoves() const {
    return moves_;
}

template <typename T>
void Hanoi<T>::PrintMoves() const {
    for (int i = 0; i < moves_.GetLength(); ++i) {
        const Move& m = moves_.Get(i);
        std::cout << "Диск " << m.disk
            << ": " << m.from << " -> " << m.to << "\n";
    }
}

template <typename T>
const Stack<T>& Hanoi<T>::GetPegA() const { return pegA_; }

template <typename T>
const Stack<T>& Hanoi<T>::GetPegB() const { return pegB_; }

template <typename T>
const Stack<T>& Hanoi<T>::GetPegC() const { return pegC_; }