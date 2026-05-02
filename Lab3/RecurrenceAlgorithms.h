#pragma once

#include "lib/Sequence.h"
#include "lib/ArraySequence.h"

#include <stdexcept>


template <class T>
static void AppendToRecurrenceResult(Sequence<T>*& seq, const T& item) {
    Sequence<T>* old = seq;
    Sequence<T>* next = seq->Append(item);

    if (next == nullptr) {
        throw std::runtime_error("Append returned nullptr");
    }

    if (next != old) {
        delete old;
        seq = next;
    }
}


// A-2.1. Рекуррентная последовательность 1-го порядка
inline Sequence<int>* Factorials(int count) {
    if (count < 0) {
        throw std::invalid_argument("Factorials: count must be non-negative");
    }

    Sequence<int>* result = new MutableArraySequence<int>();

    try {
        int current = 1;

        for (int i = 0; i < count; ++i) {
            if (i == 0) {
                current = 1;
            }
            else {
                current *= i;
            }

            AppendToRecurrenceResult(result, current);
        }

        return result;
    }
    catch (...) {
        delete result;
        throw;
    }
}


// A-2.2. Рекуррентная последовательность 2-го порядка
inline Sequence<int>* Fibonacci(int count) {
    if (count < 0) {
        throw std::invalid_argument("Fibonacci: count must be non-negative");
    }

    Sequence<int>* result = new MutableArraySequence<int>();

    try {
        if (count >= 1) {
            AppendToRecurrenceResult(result, 0);
        }

        if (count >= 2) {
            AppendToRecurrenceResult(result, 1);
        }

        int prev2 = 0;
        int prev1 = 1;

        for (int i = 2; i < count; ++i) {
            int current = prev1 + prev2;

            AppendToRecurrenceResult(result, current);

            prev2 = prev1;
            prev1 = current;
        }

        return result;
    }
    catch (...) {
        delete result;
        throw;
    }
}


// A-2.3. Линейная рекуррентная последовательность произвольного порядка
// coefficients = [a1, a2, ..., ak]
// initialValues = [x0, x1, ..., x(k-1)]
// x_n = a1 * x_(n-1) + a2 * x_(n-2) + ... + ak * x_(n-k)
inline Sequence<int>* LinearRecurrence(
    const Sequence<int>* coefficients,
    const Sequence<int>* initialValues,
    int count)
{
    if (coefficients == nullptr) {
        throw std::invalid_argument("LinearRecurrence: coefficients is nullptr");
    }

    if (initialValues == nullptr) {
        throw std::invalid_argument("LinearRecurrence: initialValues is nullptr");
    }

    if (count < 0) {
        throw std::invalid_argument("LinearRecurrence: count must be non-negative");
    }

    int order = coefficients->GetLength();

    if (order <= 0) {
        throw std::invalid_argument("LinearRecurrence: order must be positive");
    }

    if (initialValues->GetLength() != order) {
        throw std::invalid_argument("LinearRecurrence: initial values count must equal order");
    }

    Sequence<int>* result = new MutableArraySequence<int>();

    try {
        for (int i = 0; i < count && i < order; ++i) {
            AppendToRecurrenceResult(result, initialValues->Get(i));
        }

        for (int n = order; n < count; ++n) {
            int current = 0;

            for (int j = 0; j < order; ++j) {
                int coefficient = coefficients->Get(j);
                int previous = result->Get(n - 1 - j);

                current += coefficient * previous;
            }

            AppendToRecurrenceResult(result, current);
        }

        return result;
    }
    catch (...) {
        delete result;
        throw;
    }
}