#pragma once

#include "ICollection.h"
#include "lib/ArraySequence.h"
#include <stdexcept>

template <typename T>
class Stack : public ICollection<T> {
public:
    Stack() = default;
    Stack(const Stack<T>& other) = default;
    Stack& operator=(const Stack<T>& other) = default;
    ~Stack() override = default;

    const T& Get(size_t index) const override;
    size_t GetCount() const override;
    bool IsEmpty() const override;

    void Push(const T& item);
    T Pop();
    const T& Peek() const;

    Stack<T> Concat(const ICollection<T>& other) const;

    const MutableArraySequence<T>* GetSequence() const { return &seq_; }

private:
    MutableArraySequence<T> seq_;
};

#include "Stack.tpp"