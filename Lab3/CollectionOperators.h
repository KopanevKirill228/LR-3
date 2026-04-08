#pragma once

#include "ICollection.h"
#include <ostream>

// Сравнение по элементам
template <typename T>
bool operator==(const ICollection<T>& a, const ICollection<T>& b) {
    if (a.GetCount() != b.GetCount()) return false;
    for (size_t i = 0; i < a.GetCount(); i++)
        if (a.Get(i) != b.Get(i)) return false;
    return true;
}

template <typename T>
bool operator!=(const ICollection<T>& a, const ICollection<T>& b) {
    return !(a == b);
}

// Вывод в поток
template <typename T>
std::ostream& operator<<(std::ostream& os, const ICollection<T>& c) {
    os << "[";
    for (size_t i = 0; i < c.GetCount(); i++) {
        os << c.Get(i);
        if (i + 1 < c.GetCount()) os << ", ";
    }
    os << "]";
    return os;
}