#pragma once
#include "HanoiRenderer.h"

template <typename T>
HanoiRenderer<T>::HanoiRenderer(int maxDisk) : maxDisk_(maxDisk) {}

template <typename T>
void HanoiRenderer<T>::enableAnsi() const {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

template <typename T>
void HanoiRenderer<T>::clearScreen() const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

template <typename T>
MutableArraySequence<int> HanoiRenderer<T>::collectDisks(const Stack<T>& peg) const {
    int count = (int)peg.GetCount();
    MutableArraySequence<int> result;
    for (int i = 0; i < count; ++i)
        result.Append((int)peg.Get(i));
    return result;
}

template <typename T>
void HanoiRenderer<T>::Render(
    const Stack<T>& a, const Stack<T>& b, const Stack<T>& c,
    const std::string& na, const std::string& nb, const std::string& nc,
    int movesDone, const std::string& lastMove) const
{
    using namespace HanoiColors;
    const int rows = maxDisk_ + 1;
    const int width = maxDisk_ * 2 + 3;

    std::cout << BOLD << YELLOW << "  Tower of Hanoi" << RESET << "\n";
    std::cout << GRAY << std::string((width + 2) * 3, '-') << RESET << "\n";

    const Stack<T>* pegs[3] = { &a, &b, &c };
    const std::string names[3] = { na, nb, nc };

    MutableArraySequence<int> diskArrays[3];
    for (int p = 0; p < 3; ++p)
        diskArrays[p] = collectDisks(*pegs[p]);

    for (int row = rows - 1; row >= 0; --row) {
        std::cout << "  ";
        for (int p = 0; p < 3; ++p) {
            const MutableArraySequence<int>& disks = diskArrays[p];
            int dv = (row < disks.GetLength()) ? disks.Get(row) : 0;

            if (dv == 0) {
                int pad = width / 2;
                std::cout << std::string(pad, ' ')
                    << GRAY << "|" << RESET
                    << std::string(width - pad - 1, ' ');
            }
            else {
                int dw = dv * 2 + 1;
                int pad = (width - dw) / 2;
                std::cout << std::string(pad, ' ')
                    << disk(dv) << BOLD << WHITE << "[";
                for (int d = 0; d < dv * 2 - 1; ++d) std::cout << " ";
                std::cout << "]" << RESET
                    << std::string(width - pad - dw, ' ');
            }
            std::cout << "  ";
        }
        std::cout << "\n";
    }

    std::cout << "  ";
    for (int p = 0; p < 3; ++p)
        std::cout << GRAY << std::string(width, '=') << RESET << "  ";
    std::cout << "\n";

    std::cout << "  ";
    for (int p = 0; p < 3; ++p) {
        int pad = (width - (int)names[p].size()) / 2;
        std::cout << std::string(pad, ' ')
            << BOLD << YELLOW << names[p] << RESET
            << std::string(width - pad - (int)names[p].size(), ' ')
            << "  ";
    }
    std::cout << "\n";

    if (!lastMove.empty())
        std::cout << "\n  " << BOLD << "Last move: " << RESET
        << YELLOW << lastMove << RESET << "\n";

    std::cout << "  " << GRAY
        << "Moves: " << movesDone
        << "  |  Total: " << ((1 << maxDisk_) - 1)
        << RESET << "\n";
}

template <typename T>
void HanoiRenderer<T>::Render(const Hanoi<T>& h, const std::string& lastMove) const {
    enableAnsi();
    Render(h.GetPegA(), h.GetPegB(), h.GetPegC(),
        "A", "B", "C",
        h.GetMoves().GetLength(), lastMove);
}

template <typename T>
void HanoiRenderer<T>::PrintMoves(const Hanoi<T>& h, std::ostream& out) const {
    const auto& moves = h.GetMoves();
    for (int i = 0; i < moves.GetLength(); ++i) {
        const Move& m = moves.Get(i);
        out << "  " << (i + 1) << ". Disk " << m.disk
            << ": " << m.from << " -> " << m.to << "\n";
    }
}

template <typename T>
void HanoiRenderer<T>::SolveAnimated(Hanoi<T>& h, int delayMs) const {
    using namespace HanoiColors;
    enableAnsi();

    Stack<T> pegA = h.GetPegA();
    Stack<T> pegB = h.GetPegB();
    Stack<T> pegC = h.GetPegC();

    int movesDone = 0;
    int n = (int)pegA.GetCount();

    clearScreen();
    Render(pegA, pegB, pegC, "A", "B", "C", 0, "Starting...");
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));

    std::function<void(int, Stack<T>&, Stack<T>&, Stack<T>&,
        const std::string&, const std::string&, const std::string&)> solve =
        [&](int k, Stack<T>& from, Stack<T>& aux, Stack<T>& to,
            const std::string& fName, const std::string& aName, const std::string& tName)
        {
            if (k == 0) return;
            solve(k - 1, from, to, aux, fName, tName, aName);

            T d = from.Pop();
            to.Push(d);
            ++movesDone;

            std::string desc = fName + " -> " + tName +
                "  (disk " + std::to_string((int)d) + ")";

            clearScreen();
            Render(pegA, pegB, pegC, "A", "B", "C", movesDone, desc);
            std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));

            solve(k - 1, aux, from, to, aName, fName, tName);
        };

    solve(n, pegA, pegB, pegC, "A", "B", "C");

    std::cout << "\n  " << BOLD << GREEN
        << "Solved! " << ((1 << n) - 1) << " moves."
        << RESET << "\n";
}