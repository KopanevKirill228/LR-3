#pragma once

#include "ICollection.h"
#include "lib/ListSequence.h"
#include <functional>
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

    void PushBack(const T& item) override;
    T PopFront() override;

    const T& PeekFront() const override;
    const T& PeekBack()  const override;

    void Push(const T& item);
    T Pop();
    const T& Peek() const;

    ICollection<T>* Concat(const ICollection<T>& other) const override;

    const MutableListSequence<T>* GetSequence() const { return &seq_; }

private:
    MutableListSequence<T> seq_;
};

#include "Stack.tpp"