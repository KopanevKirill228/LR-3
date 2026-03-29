#include <stdexcept>
#include "Queue.h"

template <typename T>
const T& Queue<T>::Get(size_t index) const {
    if (static_cast<int>(index) >= seq_.GetLength())
        throw std::out_of_range("Index out of range");
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
void Queue<T>::PushBack(const T& item) {
    seq_.Append(item);
}

template <typename T>
T Queue<T>::PopFront() {
    if (IsEmpty())
        throw std::underflow_error("Queue is empty");
    T front = seq_.GetFirst();
    seq_.RemoveFirst();
    return front;
}

template <typename T>
const T& Queue<T>::PeekFront() const {
    if (IsEmpty())
        throw std::underflow_error("Queue is empty");
    return seq_.GetFirst();
}

template <typename T>
const T& Queue<T>::PeekBack() const {
    if (IsEmpty())
        throw std::underflow_error("Queue is empty");
    return seq_.GetLast();
}

template <typename T>
void Queue<T>::Enqueue(const T& item) { PushBack(item); }

template <typename T>
T Queue<T>::Dequeue() { return PopFront(); }

template <typename T>
const T& Queue<T>::Front() const { return PeekFront(); }

template <typename T>
const T& Queue<T>::Back() const { return PeekBack(); }

template <typename T>
ICollection<T>* Queue<T>::Map(std::function<T(const T&)> f) const {
    Queue<T>* result = new Queue<T>();
    for (int i = 0; i < seq_.GetLength(); ++i)
        result->PushBack(f(seq_.Get(i)));
    return result;
}

template <typename T>
ICollection<T>* Queue<T>::Where(std::function<bool(const T&)> pred) const {
    Queue<T>* result = new Queue<T>();
    for (int i = 0; i < seq_.GetLength(); ++i)
        if (pred(seq_.Get(i)))
            result->PushBack(seq_.Get(i));
    return result;
}

template <typename T>
T Queue<T>::Reduce(std::function<T(const T&, const T&)> f, const T& init) const {
    T acc = init;
    for (int i = 0; i < seq_.GetLength(); ++i)
        acc = f(seq_.Get(i), acc);
    return acc;
}

template <typename T>
ICollection<T>* Queue<T>::Concat(const ICollection<T>& other) const {
    Queue<T>* result = new Queue<T>(*this);
    for (size_t i = 0; i < other.GetCount(); ++i)
        result->PushBack(other.Get(i));
    return result;
}

template <typename T>
ICollection<T>* Queue<T>::GetSubsequence(size_t start, size_t end) const {
    if (start > end || static_cast<int>(end) >= seq_.GetLength())
        throw std::out_of_range("Queue::GetSubsequence — invalid indices");
    Queue<T>* result = new Queue<T>();
    for (size_t i = start; i <= end; ++i)
        result->PushBack(seq_.Get(static_cast<int>(i)));
    return result;
}

template <typename T>
int Queue<T>::FindSubsequence(const ICollection<T>& sub) const {
    int n = seq_.GetLength(), m = static_cast<int>(sub.GetCount());
    if (m == 0) return 0;
    if (m > n)  return -1;
    for (int i = 0; i <= n - m; ++i) {
        bool match = true;
        for (int j = 0; j < m; ++j)
            if (!(seq_.Get(i + j) == sub.Get(static_cast<size_t>(j))))
            {
                match = false; break;
            }
        if (match) return i;
    }
    return -1;
}