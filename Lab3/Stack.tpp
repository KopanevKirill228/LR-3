#include <stdexcept>
#include "Stack.h"

template <typename T>
const T& Stack<T>::Get(size_t index) const {
    if (static_cast<int>(index) >= seq_.GetLength())
        throw std::out_of_range("Index out of range");
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
void Stack<T>::PushBack(const T& item) {
    seq_.Prepend(item);
}

template <typename T>
T Stack<T>::PopFront() {
    if (IsEmpty())
        throw std::underflow_error("Stack is empty");
    T top = seq_.GetFirst();
    seq_.RemoveFirst();
    return top;
}

template <typename T>
const T& Stack<T>::PeekFront() const {
    if (IsEmpty())
        throw std::underflow_error("Stack is empty");
    return seq_.GetFirst();
}

template <typename T>
const T& Stack<T>::PeekBack() const {
    return PeekFront();
}

template <typename T>
void Stack<T>::Push(const T& item) { PushBack(item); }

template <typename T>
T Stack<T>::Pop() { return PopFront(); }

template <typename T>
const T& Stack<T>::Peek() const { return PeekFront(); }

template <typename T>
ICollection<T>* Stack<T>::Concat(const ICollection<T>& other) const {
    Stack<T>* result = new Stack<T>(*this);
    for (int i = static_cast<int>(other.GetCount()) - 1; i >= 0; --i)
        result->PushBack(other.Get(static_cast<size_t>(i)));
    return result;
}