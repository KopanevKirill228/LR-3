#pragma once

#include "ICollection.h"
#include "lib/ListSequence.h"
#include <functional>
#include <stdexcept>

template <typename T>
class Queue : public ICollection<T> {
public:
    Queue() = default;
    Queue(const Queue<T>& other) = default;
    Queue& operator=(const Queue<T>& other) = default;
    ~Queue() override = default;

    const T& Get(size_t index) const override;
    size_t GetCount() const override;
    bool IsEmpty() const override;

    void PushBack(const T& item) override;
    T PopFront() override;

    const T& PeekFront() const override;
    const T& PeekBack()  const override;

    void Enqueue(const T& item);
    T Dequeue();
    const T& Front() const;
    const T& Back()  const;

    ICollection<T>* Concat(const ICollection<T>& other) const override;

    const MutableListSequence<T>* GetSequence() const { return &seq_; }

private:
    MutableListSequence<T> seq_;
};

#include "Queue.tpp"