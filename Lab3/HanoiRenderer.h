#pragma once
#include "Hanoi.h"
#include <iostream>
#include <string>
#include <functional>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#endif

namespace HanoiColors {
    inline const char* disk(int d) {
        static const char* c[] = {
            "\033[41m", "\033[43m", "\033[42m", "\033[46m", "\033[44m",
            "\033[45m", "\033[101m", "\033[103m", "\033[102m", "\033[104m",
        };
        return c[(d - 1) % 10];
    }

    static const char* RESET = "\033[0m";
    static const char* BOLD = "\033[1m";
    static const char* GRAY = "\033[90m";
    static const char* WHITE = "\033[97m";
    static const char* YELLOW = "\033[93m";
    static const char* GREEN = "\033[92m";
}

template <typename T>
class HanoiRenderer {
public:
    explicit HanoiRenderer(int maxDisk);

    void Render(const Stack<T>& a, const Stack<T>& b, const Stack<T>& c,
        const std::string& na, const std::string& nb, const std::string& nc,
        int movesDone, const std::string& lastMove = "") const;

    void Render(const Hanoi<T>& h, const std::string& lastMove = "") const;

    void PrintMoves(const Hanoi<T>& h, std::ostream& out = std::cout) const;

    void SolveAnimated(Hanoi<T>& h, int delayMs = 400) const;

private:
    int maxDisk_;

    void enableAnsi() const;
    void clearScreen() const;
    MutableArraySequence<int> collectDisks(const Stack<T>& peg) const;
};

#include "HanoiRenderer.tpp"