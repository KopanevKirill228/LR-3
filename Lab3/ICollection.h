#pragma once
#include <cstddef>

template <typename T>
class ICollection {
public:
    virtual ~ICollection() = default;

    virtual const T& Get(size_t index) const = 0;
    virtual size_t GetCount() const = 0;
    virtual bool IsEmpty() const = 0;
};