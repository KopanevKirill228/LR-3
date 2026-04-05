#pragma once
#include "Stack.h"

template <typename T>
const T& Stack<T>::Get(size_t index) const {
    if (index >= static_cast<size_t>(seq_.GetLength()))
        throw std::out_of_range("Stack index out of range");
    return seq_.Get(static_cast<int>(index));
}

template <typename T>
size_t Stack<T>::GetCount() const {
    return static_cast<size_t>(seq_.GetLength());
}

template <typename T>
bool Stack<T>::IsEmpty() const {
    return seq_.GetLength() == 0;
}

template <typename T>
void Stack<T>::Push(const T& item) {
    seq_.Append(item);
}

template <typename T>
T Stack<T>::Pop() {
    if (IsEmpty())
        throw std::out_of_range("Pop from empty stack");
    T value = std::move(seq_.Get(seq_.GetLength() - 1));
    seq_.RemoveAt(seq_.GetLength() - 1);
    return value;
}

template <typename T>
const T& Stack<T>::Peek() const {
    if (IsEmpty())
        throw std::out_of_range("Peek from empty stack");
    return seq_.Get(seq_.GetLength() - 1);
}

template <typename T>
Stack<T> Stack<T>::Concat(const ICollection<T>& other) const {
    Stack<T> result = *this;
    for (size_t i = 0; i < other.GetCount(); i++)
        result.Push(other.Get(i));
    return result;
}