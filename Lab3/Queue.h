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

    ICollection<T>* Map(std::function<T(const T&)> f) const override;
    ICollection<T>* Where(std::function<bool(const T&)> predicate) const override;
    T Reduce(std::function<T(const T&, const T&)> f, const T& init) const override;

    ICollection<T>* Concat(const ICollection<T>& other) const override;
    ICollection<T>* GetSubsequence(size_t startIndex, size_t endIndex) const override;
    int FindSubsequence(const ICollection<T>& sub) const override;

private:
    MutableListSequence<T> seq_;
};

#include "Queue.tpp"