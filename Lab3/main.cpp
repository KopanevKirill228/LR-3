#include <iostream>
#include <limits>
#include <string>
#include <stdexcept>
#include <cmath>
#include <climits>

#include "Stack.h"
#include "Queue.h"
#include "Hanoi.h"
#include "Solvers.h"
#include "SquareMatrix.h"
#include "DiagonalMatrix.h"
#include "SparseMatrix.h"
#include "MatrixOperators.h"

#ifdef _WIN32
#include <windows.h>
#endif

int readInt(const std::string& prompt, int lo = INT_MIN, int hi = INT_MAX) {
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
            std::cout << "  ! Invalid input, enter an integer\n";
        }
    }
}

double readDouble(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);
        try {
            size_t pos;
            double v = std::stod(line, &pos);
            if (pos != line.size()) throw std::invalid_argument("");
            return v;
        }
        catch (...) {
            std::cout << "  ! Invalid input, enter a number\n";
        }
    }
}

void printSeparator(char c = '-', int n = 50) {
    std::cout << "\n" << std::string(n, c) << "\n";
}

void menuStack() {
    Stack<int> st;
    while (true) {
        printSeparator();
        std::cout << "  STACK  (size: " << st.GetCount() << ")\n";
        std::cout << "  1. Push\n";
        std::cout << "  2. Pop\n";
        std::cout << "  3. Peek (top)\n";
        std::cout << "  4. Show all\n";
        std::cout << "  0. Back\n";
        int ch = readInt("  > ", 0, 4);
        if (ch == 0) break;
        if (ch == 1) {
            int v = readInt("  Value: ");
            st.Push(v);
            std::cout << "  Pushed: " << v << "\n";
        }
        else if (ch == 2) {
            try { std::cout << "  Popped: " << st.Pop() << "\n"; }
            catch (const std::exception& e) { std::cout << "  ! " << e.what() << "\n"; }
        }
        else if (ch == 3) {
            try { std::cout << "  Top: " << st.Peek() << "\n"; }
            catch (const std::exception& e) { std::cout << "  ! " << e.what() << "\n"; }
        }
        else if (ch == 4) {
            if (st.IsEmpty()) { std::cout << "  Stack is empty\n"; continue; }
            std::cout << "  [ top -> bottom ]: ";
            for (size_t i = 0; i < st.GetCount(); ++i)
                std::cout << st.Get(i) << " ";
            std::cout << "\n";
        }
    }
}

void menuQueue() {
    Queue<int> q;
    while (true) {
        printSeparator();
        std::cout << "  QUEUE  (size: " << q.GetCount() << ")\n";
        std::cout << "  1. Enqueue (add to back)\n";
        std::cout << "  2. Dequeue (remove from front)\n";
        std::cout << "  3. Front / Back\n";
        std::cout << "  4. Show all\n";
        std::cout << "  0. Back\n";
        int ch = readInt("  > ", 0, 4);
        if (ch == 0) break;
        if (ch == 1) {
            int v = readInt("  Value: ");
            q.Enqueue(v);
            std::cout << "  Enqueued: " << v << "\n";
        }
        else if (ch == 2) {
            try { std::cout << "  Dequeued: " << q.Dequeue() << "\n"; }
            catch (const std::exception& e) { std::cout << "  ! " << e.what() << "\n"; }
        }
        else if (ch == 3) {
            try { std::cout << "  Front: " << q.Front() << "  Back: " << q.Back() << "\n"; }
            catch (const std::exception& e) { std::cout << "  ! " << e.what() << "\n"; }
        }
        else if (ch == 4) {
            if (q.IsEmpty()) { std::cout << "  Queue is empty\n"; continue; }
            std::cout << "  [ front -> back ]: ";
            for (size_t i = 0; i < q.GetCount(); ++i)
                std::cout << q.Get(i) << " ";
            std::cout << "\n";
        }
    }
}

void menuHanoi() {
    while (true) {
        printSeparator();
        std::cout << "  TOWER OF HANOI\n";
        std::cout << "  1. Solve\n";
        std::cout << "  0. Back\n";
        int ch = readInt("  > ", 0, 1);
        if (ch == 0) break;
        int n = readInt("  Number of disks (1-20): ", 1, 20);
        Stack<int> src;
        for (int d = n; d >= 1; d--) src.Push(d);
        Hanoi<int> h(src, "A", "B", "C");
        h.Solve();
        const auto& mv = h.GetMoves();
        int total = mv.GetLength();
        std::cout << "\n  Disks: " << n
            << "  |  Moves: " << total
            << "  (2^" << n << "-1 = " << ((1 << n) - 1) << ")\n\n";
        int limit = 32;
        int show = std::min(total, limit);
        for (int i = 0; i < show; ++i) {
            const Move& m = mv.Get(i);
            std::cout << "  " << (i + 1) << ". Disk " << m.disk
                << ": " << m.from << " -> " << m.to << "\n";
        }
        if (total > limit)
            std::cout << "  ... and " << (total - limit) << " more moves\n";
    }
}

void readSquareMatrix(SquareMatrix<double>& m, int n) {
    std::cout << "  Enter elements row by row (" << n << "x" << n << "):\n";
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
            double v = readDouble("  [" + std::to_string(i) + "][" + std::to_string(j) + "] = ");
            m.Set(i, j, v);
        }
}

void printMatrix(const IMatrix<double>& m) {
    for (int i = 0; i < m.Rows(); ++i) {
        std::cout << "  |";
        for (int j = 0; j < m.Cols(); ++j)
            std::cout << " " << m.Get(i, j);
        std::cout << " |\n";
    }
}

void menuSquareMatrix() {
    int n = 0;
    SquareMatrix<double>* mat = nullptr;
    auto ensureMatrix = [&]() -> bool {
        if (!mat) { std::cout << "  ! Create a matrix first (option 1)\n"; return false; }
        return true;
        };
    while (true) {
        printSeparator();
        std::cout << "  SQUARE MATRIX";
        if (mat) std::cout << "  (" << n << "x" << n << ")";
        std::cout << "\n";
        std::cout << "  1. Create / Enter matrix\n";
        std::cout << "  2. Show matrix\n";
        std::cout << "  3. Determinant\n";
        std::cout << "  4. Trace\n";
        std::cout << "  5. Power\n";
        std::cout << "  6. Transpose\n";
        std::cout << "  7. Multiply by scalar\n";
        std::cout << "  0. Back\n";
        int ch = readInt("  > ", 0, 7);
        if (ch == 0) { delete mat; mat = nullptr; break; }
        if (ch == 1) {
            n = readInt("  Matrix size n (1-10): ", 1, 10);
            delete mat; mat = new SquareMatrix<double>(n);
            readSquareMatrix(*mat, n);
        }
        else if (ch == 2) {
            if (!ensureMatrix()) continue;
            printMatrix(*mat);
        }
        else if (ch == 3) {
            if (!ensureMatrix()) continue;
            std::cout << "  det = " << mat->Determinant() << "\n";
        }
        else if (ch == 4) {
            if (!ensureMatrix()) continue;
            std::cout << "  trace = " << mat->Trace() << "\n";
        }
        else if (ch == 5) {
            if (!ensureMatrix()) continue;
            int p = readInt("  Power p (0-30): ", 0, 30);
            try {
                auto* r = mat->Power(p);
                std::cout << "  Matrix to the power " << p << ":\n";
                printMatrix(*r); delete r;
            }
            catch (const std::exception& e) { std::cout << "  ! " << e.what() << "\n"; }
        }
        else if (ch == 6) {
            if (!ensureMatrix()) continue;
            auto* t = mat->Transpose();
            std::cout << "  Transposed:\n"; printMatrix(*t); delete t;
        }
        else if (ch == 7) {
            if (!ensureMatrix()) continue;
            double s = readDouble("  Scalar: ");
            auto* r = mat->MulScalar(s);
            std::cout << "  Result:\n"; printMatrix(*r); delete r;
        }
    }
}

void menuSolvers() {
    while (true) {
        printSeparator();
        std::cout << "  SOLVE LINEAR SYSTEM\n";
        std::cout << "  1. Gauss no pivot\n";
        std::cout << "  2. Gauss partial pivot\n";
        std::cout << "  3. LU decomposition\n";
        std::cout << "  0. Back\n";
        int ch = readInt("  > ", 0, 3);
        if (ch == 0) break;
        int n = readInt("  System size n (1-8): ", 1, 8);
        SquareMatrix<double> A(n);
        MutableArraySequence<double> b;
        std::cout << "\n  Enter matrix A (" << n << "x" << n << "):\n";
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j) {
                double v = readDouble("  A[" + std::to_string(i) + "][" + std::to_string(j) + "] = ");
                A.Set(i, j, v);
            }
        std::cout << "\n  Enter vector b (" << n << " elements):\n";
        for (int i = 0; i < n; ++i)
            b.Append(readDouble("  b[" + std::to_string(i) + "] = "));
        try {
            MutableArraySequence<double> x;
            if (ch == 1) x = GaussNopivot<double>(A, b);
            else if (ch == 2) x = GaussPartialPivot<double>(A, b);
            else x = LUSolve<double>(A, b);
            std::cout << "\n  Solution x:\n";
            for (int i = 0; i < n; ++i)
                std::cout << "  x[" << i << "] = " << x.Get(i) << "\n";
            std::cout << "  Residual ||Ax-b|| = " << Residual<double>(A, x, b) << "\n";
        }
        catch (const std::exception& e) {
            std::cout << "  ! Error: " << e.what() << "\n";
        }
    }
}

void menuDiagonal() {
    DiagonalMatrix<double>* mat = nullptr;
    int n = 0;
    while (true) {
        printSeparator();
        std::cout << "  DIAGONAL MATRIX";
        if (mat) std::cout << "  (" << n << "x" << n << ")";
        std::cout << "\n";
        std::cout << "  1. Create\n";
        std::cout << "  2. Show\n";
        std::cout << "  3. Inverse\n";
        std::cout << "  4. Multiply two diagonal matrices\n";
        std::cout << "  0. Back\n";
        int ch = readInt("  > ", 0, 4);
        if (ch == 0) { delete mat; mat = nullptr; break; }
        if (ch == 1) {
            n = readInt("  Size (1-10): ", 1, 10);
            delete mat; mat = new DiagonalMatrix<double>(n);
            for (int i = 0; i < n; ++i) {
                double v = readDouble("  diag[" + std::to_string(i) + "] = ");
                mat->SetDiag(i, v);
            }
        }
        else if (ch == 2) {
            if (!mat) { std::cout << "  ! Create a matrix first\n"; continue; }
            printMatrix(*mat);
        }
        else if (ch == 3) {
            if (!mat) { std::cout << "  ! Create a matrix first\n"; continue; }
            try {
                auto* inv = mat->Inverse();
                std::cout << "  Inverse:\n"; printMatrix(*inv); delete inv;
            }
            catch (const std::exception& e) { std::cout << "  ! " << e.what() << "\n"; }
        }
        else if (ch == 4) {
            if (!mat) { std::cout << "  ! Create a matrix first\n"; continue; }
            std::cout << "  Enter second diagonal matrix (same size):\n";
            DiagonalMatrix<double> b(n);
            for (int i = 0; i < n; ++i) {
                double v = readDouble("  diag[" + std::to_string(i) + "] = ");
                b.SetDiag(i, v);
            }
            auto* r = mat->MulDiag(b);
            std::cout << "  Result:\n"; printMatrix(*r); delete r;
        }
    }
}

void menuSparse() {
    SparseMatrix<double>* mat = nullptr;
    int rows = 0, cols = 0;
    while (true) {
        printSeparator();
        std::cout << "  SPARSE MATRIX";
        if (mat) std::cout << "  (" << rows << "x" << cols
            << ", nonzero: " << mat->NonZeroCount() << ")";
        std::cout << "\n";
        std::cout << "  1. Create\n";
        std::cout << "  2. Set element\n";
        std::cout << "  3. Get element\n";
        std::cout << "  4. Show\n";
        std::cout << "  5. Transpose\n";
        std::cout << "  6. Frobenius norm\n";
        std::cout << "  0. Back\n";
        int ch = readInt("  > ", 0, 6);
        if (ch == 0) { delete mat; mat = nullptr; break; }
        if (ch == 1) {
            rows = readInt("  Rows (1-10): ", 1, 10);
            cols = readInt("  Cols (1-10): ", 1, 10);
            delete mat; mat = new SparseMatrix<double>(rows, cols);
        }
        else if (ch == 2) {
            if (!mat) { std::cout << "  ! Create a matrix first\n"; continue; }
            int r = readInt("  Row (0-" + std::to_string(rows - 1) + "): ", 0, rows - 1);
            int c = readInt("  Col (0-" + std::to_string(cols - 1) + "): ", 0, cols - 1);
            double v = readDouble("  Value: ");
            mat->Set(r, c, v);
        }
        else if (ch == 3) {
            if (!mat) { std::cout << "  ! Create a matrix first\n"; continue; }
            int r = readInt("  Row: ", 0, rows - 1);
            int c = readInt("  Col: ", 0, cols - 1);
            std::cout << "  [" << r << "][" << c << "] = " << mat->Get(r, c) << "\n";
        }
        else if (ch == 4) {
            if (!mat) { std::cout << "  ! Create a matrix first\n"; continue; }
            printMatrix(*mat);
        }
        else if (ch == 5) {
            if (!mat) { std::cout << "  ! Create a matrix first\n"; continue; }
            auto* t = mat->Transpose();
            std::cout << "  Transposed (" << t->Rows() << "x" << t->Cols() << "):\n";
            printMatrix(*t); delete t;
        }
        else if (ch == 6) {
            if (!mat) { std::cout << "  ! Create a matrix first\n"; continue; }
            std::cout << "  Frobenius norm = " << mat->Norm() << "\n";
        }
    }
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
    while (true) {
        printSeparator('=');
        std::cout << "  Laboratory Work #3\n";
        printSeparator();
        std::cout << "  1. Stack\n";
        std::cout << "  2. Queue\n";
        std::cout << "  3. Tower of Hanoi\n";
        std::cout << "  4. Square Matrix\n";
        std::cout << "  5. Solve Linear System\n";
        std::cout << "  6. Diagonal Matrix\n";
        std::cout << "  7. Sparse Matrix\n";
        std::cout << "  0. Exit\n";
        int ch = readInt("  > ", 0, 7);
        if (ch == 0) { std::cout << "  Goodbye!\n"; break; }
        if (ch == 1) menuStack();
        else if (ch == 2) menuQueue();
        else if (ch == 3) menuHanoi();
        else if (ch == 4) menuSquareMatrix();
        else if (ch == 5) menuSolvers();
        else if (ch == 6) menuDiagonal();
        else if (ch == 7) menuSparse();
    }
    return 0;
}