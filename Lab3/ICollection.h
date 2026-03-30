#pragma once

#include <cstddef>
#include <functional>

template <typename T>
class ICollection {
public:
    virtual ~ICollection() = default;
    virtual const T& Get(size_t index) const = 0;
    virtual size_t GetCount() const = 0;

    virtual bool IsEmpty() const = 0;

    virtual void PushBack(const T& item) = 0;
    virtual T PopFront() = 0;
    virtual const T& PeekFront() const = 0;
    virtual const T& PeekBack() const = 0;


    virtual ICollection<T>* Concat(const ICollection<T>& other) const = 0;
};