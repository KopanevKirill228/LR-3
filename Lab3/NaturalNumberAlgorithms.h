#pragma once

#include "lib/Sequence.h"
#include "lib/ArraySequence.h"
#include "lib/MapReduce.h"

#include <stdexcept>


template <class T>
static void AppendToNaturalResult(Sequence<T>*& seq, const T& item) {
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


// A-3. Range: [low, low + 1, ..., high]
inline Sequence<int>* Range(int low, int high) {
    if (low > high) {
        throw std::invalid_argument("Range: low is greater than high");
    }

    Sequence<int>* result = new MutableArraySequence<int>();

    try {
        for (int i = low; i <= high; ++i) {
            AppendToNaturalResult(result, i);
        }

        return result;
    }
    catch (...) {
        delete result;
        throw;
    }
}


// Проверка простоты числа
inline bool IsPrime(int n) {
    if (n < 2) {
        return false;
    }

    if (n == 2) {
        return true;
    }

    if (n % 2 == 0) {
        return false;
    }

    for (int d = 3; d * d <= n; d += 2) {
        if (n % d == 0) {
            return false;
        }
    }

    return true;
}


// A-3.1. Разложение заданного числа на множители
inline Sequence<int>* GetDivisors(int n) {
    if (n <= 0) {
        throw std::invalid_argument("GetDivisors: n must be positive");
    }

    Sequence<int>* result = new MutableArraySequence<int>();

    try {
        for (int d = 1; d <= n; ++d) {
            if (n % d == 0) {
                AppendToNaturalResult(result, d);
            }
        }

        return result;
    }
    catch (...) {
        delete result;
        throw;
    }
}


// A-3.2. Определение простых чисел
// Cначала Range, потом фильтрация через Where
inline Sequence<int>* GetPrimes(int low, int high) {
    Sequence<int>* numbers = nullptr;
    Sequence<int>* primes = nullptr;

    try {
        numbers = Range(low, high);

        primes = Where<int>(
            numbers,
            [](const int& x) {
                return IsPrime(x);
            },
            []() -> Sequence<int>*{
                return new MutableArraySequence<int>();
            }
        );

        delete numbers;
        return primes;
    }
    catch (...) {
        delete numbers;
        delete primes;
        throw;
    }
}


// A-3.3. Разложение числа на простые множители
inline Sequence<int>* GetPrimeFactors(int n) {
    if (n < 2) {
        throw std::invalid_argument("GetPrimeFactors: n must be >= 2");
    }

    Sequence<int>* result = new MutableArraySequence<int>();

    try {
        int value = n;

        while (value % 2 == 0) {
            AppendToNaturalResult(result, 2);
            value /= 2;
        }

        for (int d = 3; d * d <= value; d += 2) {
            while (value % d == 0) {
                AppendToNaturalResult(result, d);
                value /= d;
            }
        }

        if (value > 1) {
            AppendToNaturalResult(result, value);
        }

        return result;
    }
    catch (...) {
        delete result;
        throw;
    }
}


// A-3.4. Решето Эратосфена
// Возвращает простые числа на отрезке [low, high]
inline Sequence<int>* SieveOfEratosthenes(int low, int high) {
    if (low > high) {
        throw std::invalid_argument("SieveOfEratosthenes: low is greater than high");
    }

    if (high < 2) {
        return new MutableArraySequence<int>();
    }

    bool* isPrime = nullptr;
    Sequence<int>* result = nullptr;

    try {
        isPrime = new bool[high + 1];

        for (int i = 0; i <= high; ++i) {
            isPrime[i] = true;
        }

        isPrime[0] = false;
        isPrime[1] = false;

        for (int p = 2; p * p <= high; ++p) {
            if (isPrime[p]) {
                for (int multiple = p * p; multiple <= high; multiple += p) {
                    isPrime[multiple] = false;
                }
            }
        }

        result = new MutableArraySequence<int>();

        int start = low;

        if (start < 2) {
            start = 2;
        }

        for (int i = start; i <= high; ++i) {
            if (isPrime[i]) {
                AppendToNaturalResult(result, i);
            }
        }

        delete[] isPrime;
        return result;
    }
    catch (...) {
        delete[] isPrime;
        delete result;
        throw;
    }
}