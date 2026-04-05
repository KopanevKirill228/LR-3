#pragma once
#include "Queue.h"

template <typename T>
const T& Queue<T>::Get(size_t index) const {
    if (index >= static_cast<size_t>(seq_.GetLength()))
        throw std::out_of_range("Queue index out of range");
    return seq_.Get(static_cast<int>(index));
}

template <typename T>
size_t Queue<T>::GetCount() const {
    return static_cast<size_t>(seq_.GetLength());
}

template <typename T>
bool Queue<T>::IsEmpty() const {
    return seq_.GetLength() == 0;
}

template <typename T>
void Queue<T>::Enqueue(const T& item) {
    seq_.Append(item);
}

template <typename T>
T Queue<T>::Dequeue() {
    if (IsEmpty())
        throw std::out_of_range("Dequeue from empty queue");
    T value = std::move(seq_.Get(0));
    seq_.RemoveFirst();
    return value;
}

template <typename T>
const T& Queue<T>::PeekFront() const {
    if (IsEmpty())
        throw std::out_of_range("PeekFront from empty queue");
    return seq_.Get(0);
}

template <typename T>
const T& Queue<T>::PeekBack() const {
    if (IsEmpty())
        throw std::out_of_range("PeekBack from empty queue");
    return seq_.Get(seq_.GetLength() - 1);
}

template <typename T>
Queue<T> Queue<T>::Concat(const ICollection<T>& other) const {
    Queue<T> result = *this;
    for (size_t i = 0; i < other.GetCount(); i++)
        result.Enqueue(other.Get(i));
    return result;
}