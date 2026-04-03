#include "Hanoi.h"
#include "HanoiRenderer.h"
#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

static int readInt(const std::string& prompt, int lo, int hi) {
    while (true) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);
        try {
            size_t pos;
            int v = std::stoi(line, &pos);
            if (pos != line.size()) throw std::invalid_argument("");
            if (v < lo || v > hi) {
                std::cout << "  ! Enter a number from " << lo << " to " << hi << "\n";
                continue;
            }
            return v;
        }
        catch (...) {
            std::cout << "  ! Invalid input\n";
        }
    }
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    std::cout << "\033[1m\033[93m=== Tower of Hanoi ===\033[0m\n\n";

    int n = readInt("  Disks (1-10): ", 1, 10);
    int delay = readInt("  Delay ms (50-1000): ", 50, 1000);

    Stack<int> src;
    for (int d = n; d >= 1; d--) src.Push(d);

    Hanoi<int> h(src, "A", "B", "C");
    HanoiRenderer<int> renderer(n);

    std::cout << "\n";
    renderer.Render(h, "Initial state");

    std::cout << "\n  Press Enter to start...";
    std::string dummy;
    std::getline(std::cin, dummy);

    renderer.SolveAnimated(h, delay);

    Hanoi<int> solved(src, "A", "B", "C");
    solved.Solve();

    int total = solved.GetMoves().GetLength();
    int expected = (1 << n) - 1;
    int show = total > 20 ? 20 : total;

    std::cout << "\n  Move list (first " << show << "):\n";
    for (int i = 0; i < show; ++i) {
        const Move& m = solved.GetMoves().Get(i);
        std::cout << "  " << (i + 1) << ". Disk " << m.disk
            << ": " << m.from << " -> " << m.to << "\n";
    }

    if (total > show)
        std::cout << "  ... and " << (total - show) << " more\n";

    std::cout << "\n  Total: " << total
        << "  expected: " << expected
        << "  " << (total == expected ? "\033[92mPASSED\033[0m" : "\033[91mFAILED\033[0m")
        << "\n";

    return 0;
}