#include <iostream>
#include <string>
#include <limits>
#include "Stack.h"
#include "Queue.h"
#include "Hanoi.h"
#include "HanoiRenderer.h"
#include "RectangularMatrix.h"
#include "SquareMatrix.h"
#include "DiagonalMatrix.h"
#include "SparseMatrix.h"

static void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static bool readInt(int& val) {
    if (!(std::cin >> val)) {
        clearInput();
        return false;
    }
    return true;
}

static bool menu(const std::string& prompt, int lo, int hi, int& choice) {
    while (true) {
        std::cout << prompt;
        if (!readInt(choice)) {
            std::cout << "  [ERR] Not a number. Try again.\n";
            continue;
        }
        if (choice == 0) return false;
        if (choice < lo || choice > hi) {
            std::cout << "  [ERR] Enter a number from " << lo
                << " to " << hi << " (0 = back).\n";
            continue;
        }
        return true;
    }
}

static void printSeparator() {
    std::cout << "----------------------------------------\n";
}

static void printMatrix(const IMatrix<int>& m) {
    for (int i = 0; i < m.Rows(); i++) {
        std::cout << "  ";
        for (int j = 0; j < m.Cols(); j++)
            std::cout << m.Get(i, j) << "\t";
        std::cout << "\n";
    }
}

// печатает результат операции и удаляет его
static void printAndDelete(IMatrix<int>* res) {
    if (res) {
        printMatrix(*res);
        delete res;
    }
}

static void stackMenu() {
    Stack<int> s;
    int choice;
    while (true) {
        printSeparator();
        std::cout << "  Stack  (size=" << s.GetCount() << ")\n";
        std::cout << "  1. Push\n";
        std::cout << "  2. Pop\n";
        std::cout << "  3. Peek\n";
        std::cout << "  4. Concat with another stack\n";
        std::cout << "  5. Print all\n";
        std::cout << "  0. Back\n";
        if (!menu("Choice: ", 1, 5, choice)) return;

        if (choice == 1) {
            int val;
            std::cout << "  Value: ";
            if (!readInt(val)) { std::cout << "  [ERR] Not a number.\n"; continue; }
            s.Push(val);
            std::cout << "  Pushed " << val << "\n";
        }
        else if (choice == 2) {
            try {
                std::cout << "  Popped: " << s.Pop() << "\n";
            }
            catch (const std::exception& e) {
                std::cout << "  [ERR] " << e.what() << "\n";
            }
        }
        else if (choice == 3) {
            try {
                std::cout << "  Top: " << s.Peek() << "\n";
            }
            catch (const std::exception& e) {
                std::cout << "  [ERR] " << e.what() << "\n";
            }
        }
        else if (choice == 4) {
            Stack<int> other;
            int n;
            std::cout << "  How many elements to add: ";
            if (!readInt(n) || n < 0) {
                std::cout << "  [ERR] Invalid count.\n"; continue;
            }
            for (int i = 0; i < n; i++) {
                int val;
                std::cout << "  Element " << (i + 1) << ": ";
                if (!readInt(val)) { std::cout << "  [ERR] Not a number.\n"; i--; continue; }
                other.Push(val);
            }
            s = s.Concat(other);
            std::cout << "  Concatenated. New size: " << s.GetCount() << "\n";
        }
        else if (choice == 5) {
            if (s.IsEmpty()) { std::cout << "  (empty)\n"; continue; }
            std::cout << "  Bottom -> Top: ";
            for (size_t i = 0; i < s.GetCount(); i++)
                std::cout << s.Get(i) << " ";
            std::cout << "\n";
        }
    }
}

static void queueMenu() {
    Queue<int> q;
    int choice;
    while (true) {
        printSeparator();
        std::cout << "  Queue  (size=" << q.GetCount() << ")\n";
        std::cout << "  1. Enqueue\n";
        std::cout << "  2. Dequeue\n";
        std::cout << "  3. Peek front\n";
        std::cout << "  4. Peek back\n";
        std::cout << "  5. Concat with another queue\n";
        std::cout << "  6. Print all\n";
        std::cout << "  0. Back\n";
        if (!menu("Choice: ", 1, 6, choice)) return;

        if (choice == 1) {
            int val;
            std::cout << "  Value: ";
            if (!readInt(val)) { std::cout << "  [ERR] Not a number.\n"; continue; }
            q.Enqueue(val);
            std::cout << "  Enqueued " << val << "\n";
        }
        else if (choice == 2) {
            try {
                std::cout << "  Dequeued: " << q.Dequeue() << "\n";
            }
            catch (const std::exception& e) {
                std::cout << "  [ERR] " << e.what() << "\n";
            }
        }
        else if (choice == 3) {
            try {
                std::cout << "  Front: " << q.PeekFront() << "\n";
            }
            catch (const std::exception& e) {
                std::cout << "  [ERR] " << e.what() << "\n";
            }
        }
        else if (choice == 4) {
            try {
                std::cout << "  Back: " << q.PeekBack() << "\n";
            }
            catch (const std::exception& e) {
                std::cout << "  [ERR] " << e.what() << "\n";
            }
        }
        else if (choice == 5) {
            Queue<int> other;
            int n;
            std::cout << "  How many elements to add: ";
            if (!readInt(n) || n < 0) {
                std::cout << "  [ERR] Invalid count.\n"; continue;
            }
            for (int i = 0; i < n; i++) {
                int val;
                std::cout << "  Element " << (i + 1) << ": ";
                if (!readInt(val)) { std::cout << "  [ERR] Not a number.\n"; i--; continue; }
                other.Enqueue(val);
            }
            q = q.Concat(other);
            std::cout << "  Concatenated. New size: " << q.GetCount() << "\n";
        }
        else if (choice == 6) {
            if (q.IsEmpty()) { std::cout << "  (empty)\n"; continue; }
            std::cout << "  Front -> Back: ";
            for (size_t i = 0; i < q.GetCount(); i++)
                std::cout << q.Get(i) << " ";
            std::cout << "\n";
        }
    }
}

static void hanoiMenu() {
    int choice;
    while (true) {
        printSeparator();
        std::cout << "  Hanoi Tower\n";
        std::cout << "  1. Solve (print moves)\n";
        std::cout << "  2. Solve (animated)\n";
        std::cout << "  0. Back\n";
        if (!menu("Choice: ", 1, 2, choice)) return;

        int n;
        std::cout << "  Number of disks (1-12): ";
        if (!readInt(n) || n < 1 || n > 12) {
            std::cout << "  [ERR] Enter a number from 1 to 12.\n"; continue;
        }

        Stack<int> src;
        for (int i = n; i >= 1; i--)
            src.Push(i);

        Hanoi<int> h(src, "A", "B", "C");

        if (choice == 1) {
            h.Solve();
            const auto& moves = h.GetMoves();
            std::cout << "  Total moves: " << moves.GetLength() << "\n";
            for (int i = 0; i < moves.GetLength(); i++) {
                const Move& m = moves.Get(i);
                std::cout << "  " << (i + 1) << ". Disk " << m.disk
                    << ": " << m.from << " -> " << m.to << "\n";
            }
        }
        else if (choice == 2) {
            int delay;
            std::cout << "  Delay ms (100-2000): ";
            if (!readInt(delay) || delay < 100 || delay > 2000) {
                std::cout << "  [ERR] Enter 100-2000.\n"; continue;
            }
            HanoiRenderer<int> renderer(n);
            renderer.SolveAnimated(h, delay);
        }
    }
}

static bool readMatrixDims(int& rows, int& cols) {
    std::cout << "  Rows: ";
    if (!readInt(rows) || rows <= 0) {
        std::cout << "  [ERR] Rows must be positive.\n"; return false;
    }
    std::cout << "  Cols: ";
    if (!readInt(cols) || cols <= 0) {
        std::cout << "  [ERR] Cols must be positive.\n"; return false;
    }
    return true;
}

static bool readSquareDim(int& n) {
    std::cout << "  Size (n): ";
    if (!readInt(n) || n <= 0) {
        std::cout << "  [ERR] Size must be positive.\n"; return false;
    }
    return true;
}

static void fillMatrix(IMatrix<int>& m) {
    std::cout << "  Enter elements row by row:\n";
    for (int i = 0; i < m.Rows(); i++) {
        for (int j = 0; j < m.Cols(); j++) {
            int val;
            std::cout << "  [" << i << "][" << j << "]: ";
            while (!readInt(val)) {
                std::cout << "  [ERR] Not a number. Try again.\n";
                std::cout << "  [" << i << "][" << j << "]: ";
            }
            try { m.Set(i, j, val); }
            catch (const std::exception& e) {
                std::cout << "  [ERR] " << e.what() << " (skipped)\n";
            }
        }
    }
}

static void rectangularMatrixMenu() {
    RectangularMatrix<int>* m = nullptr;
    int choice;

    while (true) {
        printSeparator();
        std::cout << "  Rectangular Matrix";
        if (m) std::cout << " [" << m->Rows() << "x" << m->Cols() << "]";
        std::cout << "\n";
        std::cout << "  1. Create\n";
        std::cout << "  2. Print\n";
        std::cout << "  3. Add another matrix\n";
        std::cout << "  4. Multiply by scalar\n";
        std::cout << "  5. Multiply by matrix\n";
        std::cout << "  6. Transpose\n";
        std::cout << "  7. Frobenius norm\n";
        std::cout << "  8. Swap rows\n";
        std::cout << "  9. Scale row\n";
        std::cout << "  10. Add scaled row\n";
        std::cout << "  11. Swap cols\n";
        std::cout << "  12. Scale col\n";
        std::cout << "  13. Add scaled col\n";
        std::cout << "  0. Back\n";
        if (!menu("Choice: ", 1, 13, choice)) { delete m; return; }

        if (choice == 1) {
            int r, c;
            if (!readMatrixDims(r, c)) continue;
            delete m;
            m = new RectangularMatrix<int>(r, c);
            fillMatrix(*m);
            std::cout << "  Created.\n";
        }
        else if (!m) {
            std::cout << "  [ERR] Create a matrix first.\n";
        }
        else if (choice == 2) {
            printMatrix(*m);
        }
        else if (choice == 3) {
            RectangularMatrix<int> other(m->Rows(), m->Cols());
            fillMatrix(other);
            try {
                auto* res = m->Add(other);
                std::cout << "  Result:\n";
                printAndDelete(res);
            }
            catch (const std::exception& e) {
                std::cout << "  [ERR] " << e.what() << "\n";
            }
        }
        else if (choice == 4) {
            int s;
            std::cout << "  Scalar: ";
            if (!readInt(s)) { std::cout << "  [ERR] Not a number.\n"; continue; }
            auto* res = m->MultiplyByScalar(s);
            std::cout << "  Result:\n";
            printAndDelete(res);
        }
        else if (choice == 5) {
            int r, c;
            std::cout << "  Second matrix dims (must be " << m->Cols() << " x ?):\n";
            if (!readMatrixDims(r, c)) continue;
            if (r != m->Cols()) {
                std::cout << "  [ERR] Rows must equal " << m->Cols() << "\n"; continue;
            }
            RectangularMatrix<int> other(r, c);
            fillMatrix(other);
            try {
                auto* res = m->MultiplyByMatrix(other);
                std::cout << "  Result:\n";
                printAndDelete(res);
            }
            catch (const std::exception& e) {
                std::cout << "  [ERR] " << e.what() << "\n";
            }
        }
        else if (choice == 6) {
            auto* res = m->Transpose();
            std::cout << "  Transposed:\n";
            printAndDelete(res);
        }
        else if (choice == 7) {
            std::cout << "  Frobenius norm: " << m->FrobeniusNorm() << "\n";
        }
        else if (choice == 8) {
            int i, j;
            std::cout << "  Row 1 (0-based): "; if (!readInt(i)) { std::cout << "  [ERR]\n"; continue; }
            std::cout << "  Row 2 (0-based): "; if (!readInt(j)) { std::cout << "  [ERR]\n"; continue; }
            try { m->SwapRows(i, j); printMatrix(*m); }
            catch (const std::exception& e) { std::cout << "  [ERR] " << e.what() << "\n"; }
        }
        else if (choice == 9) {
            int row, s;
            std::cout << "  Row (0-based): "; if (!readInt(row)) { std::cout << "  [ERR]\n"; continue; }
            std::cout << "  Scalar: ";        if (!readInt(s)) { std::cout << "  [ERR]\n"; continue; }
            try { m->ScaleRow(row, s); printMatrix(*m); }
            catch (const std::exception& e) { std::cout << "  [ERR] " << e.what() << "\n"; }
        }
        else if (choice == 10) {
            int target, source, s;
            std::cout << "  Target row: "; if (!readInt(target)) { std::cout << "  [ERR]\n"; continue; }
            std::cout << "  Source row: "; if (!readInt(source)) { std::cout << "  [ERR]\n"; continue; }
            std::cout << "  Scalar: ";     if (!readInt(s)) { std::cout << "  [ERR]\n"; continue; }
            try { m->AddScaledRow(target, source, s); printMatrix(*m); }
            catch (const std::exception& e) { std::cout << "  [ERR] " << e.what() << "\n"; }
        }
        else if (choice == 11) {
            int i, j;
            std::cout << "  Col 1 (0-based): "; if (!readInt(i)) { std::cout << "  [ERR]\n"; continue; }
            std::cout << "  Col 2 (0-based): "; if (!readInt(j)) { std::cout << "  [ERR]\n"; continue; }
            try { m->SwapCols(i, j); printMatrix(*m); }
            catch (const std::exception& e) { std::cout << "  [ERR] " << e.what() << "\n"; }
        }
        else if (choice == 12) {
            int col, s;
            std::cout << "  Col (0-based): "; if (!readInt(col)) { std::cout << "  [ERR]\n"; continue; }
            std::cout << "  Scalar: ";        if (!readInt(s)) { std::cout << "  [ERR]\n"; continue; }
            try { m->ScaleCol(col, s); printMatrix(*m); }
            catch (const std::exception& e) { std::cout << "  [ERR] " << e.what() << "\n"; }
        }
        else if (choice == 13) {
            int target, source, s;
            std::cout << "  Target col: "; if (!readInt(target)) { std::cout << "  [ERR]\n"; continue; }
            std::cout << "  Source col: "; if (!readInt(source)) { std::cout << "  [ERR]\n"; continue; }
            std::cout << "  Scalar: ";     if (!readInt(s)) { std::cout << "  [ERR]\n"; continue; }
            try { m->AddScaledCol(target, source, s); printMatrix(*m); }
            catch (const std::exception& e) { std::cout << "  [ERR] " << e.what() << "\n"; }
        }
    }
}

static void squareMatrixMenu() {
    SquareMatrix<int>* m = nullptr;
    int choice;

    while (true) {
        printSeparator();
        std::cout << "  Square Matrix";
        if (m) std::cout << " [" << m->Rows() << "x" << m->Cols() << "]";
        std::cout << "\n";
        std::cout << "  1. Create\n";
        std::cout << "  2. Print\n";
        std::cout << "  3. Add another matrix\n";
        std::cout << "  4. Multiply by scalar\n";
        std::cout << "  5. Multiply by matrix\n";
        std::cout << "  6. Transpose\n";
        std::cout << "  7. Frobenius norm\n";
        std::cout << "  8. Trace\n";
        std::cout << "  9. Determinant\n";
        std::cout << "  10. Swap rows\n";
        std::cout << "  11. Scale row\n";
        std::cout << "  12. Add scaled row\n";
        std::cout << "  13. Swap cols\n";
        std::cout << "  14. Scale col\n";
        std::cout << "  15. Add scaled col\n";
        std::cout << "  0. Back\n";
        if (!menu("Choice: ", 1, 15, choice)) { delete m; return; }

        if (choice == 1) {
            int n;
            if (!readSquareDim(n)) continue;
            delete m;
            m = new SquareMatrix<int>(n);
            fillMatrix(*m);
            std::cout << "  Created.\n";
        }
        else if (!m) {
            std::cout << "  [ERR] Create a matrix first.\n";
        }
        else if (choice == 2) { printMatrix(*m); }
        else if (choice == 3) {
            SquareMatrix<int> other(m->Rows());
            fillMatrix(other);
            try {
                auto* res = m->Add(other);
                std::cout << "  Result:\n";
                printAndDelete(res);
            }
            catch (const std::exception& e) {
                std::cout << "  [ERR] " << e.what() << "\n";
            }
        }
        else if (choice == 4) {
            int s;
            std::cout << "  Scalar: "; if (!readInt(s)) { std::cout << "  [ERR]\n"; continue; }
            auto* res = m->MultiplyByScalar(s);
            std::cout << "  Result:\n";
            printAndDelete(res);
        }
        else if (choice == 5) {
            SquareMatrix<int> other(m->Rows());
            fillMatrix(other);
            try {
                auto* res = m->MultiplyByMatrix(other);
                std::cout << "  Result:\n";
                printAndDelete(res);
            }
            catch (const std::exception& e) {
                std::cout << "  [ERR] " << e.what() << "\n";
            }
        }
        else if (choice == 6) {
            auto* res = m->Transpose();
            std::cout << "  Transposed:\n";
            printAndDelete(res);
        }
        else if (choice == 7) { std::cout << "  Frobenius: " << m->FrobeniusNorm() << "\n"; }
        else if (choice == 8) { std::cout << "  Trace: " << m->Trace() << "\n"; }
        else if (choice == 9) { std::cout << "  Determinant: " << m->Determinant() << "\n"; }
        else if (choice == 10) {
            int i, j;
            std::cout << "  Row 1: "; if (!readInt(i)) { std::cout << "  [ERR]\n"; continue; }
            std::cout << "  Row 2: "; if (!readInt(j)) { std::cout << "  [ERR]\n"; continue; }
            try { m->SwapRows(i, j); printMatrix(*m); }
            catch (const std::exception& e) { std::cout << "  [ERR] " << e.what() << "\n"; }
        }
        else if (choice == 11) {
            int row, s;
            std::cout << "  Row: ";    if (!readInt(row)) { std::cout << "  [ERR]\n"; continue; }
            std::cout << "  Scalar: "; if (!readInt(s)) { std::cout << "  [ERR]\n"; continue; }
            try { m->ScaleRow(row, s); printMatrix(*m); }
            catch (const std::exception& e) { std::cout << "  [ERR] " << e.what() << "\n"; }
        }
        else if (choice == 12) {
            int target, source, s;
            std::cout << "  Target row: "; if (!readInt(target)) { std::cout << "  [ERR]\n"; continue; }
            std::cout << "  Source row: "; if (!readInt(source)) { std::cout << "  [ERR]\n"; continue; }
            std::cout << "  Scalar: ";     if (!readInt(s)) { std::cout << "  [ERR]\n"; continue; }
            try { m->AddScaledRow(target, source, s); printMatrix(*m); }
            catch (const std::exception& e) { std::cout << "  [ERR] " << e.what() << "\n"; }
        }
        else if (choice == 13) {
            int i, j;
            std::cout << "  Col 1: "; if (!readInt(i)) { std::cout << "  [ERR]\n"; continue; }
            std::cout << "  Col 2: "; if (!readInt(j)) { std::cout << "  [ERR]\n"; continue; }
            try { m->SwapCols(i, j); printMatrix(*m); }
            catch (const std::exception& e) { std::cout << "  [ERR] " << e.what() << "\n"; }
        }
        else if (choice == 14) {
            int col, s;
            std::cout << "  Col: ";    if (!readInt(col)) { std::cout << "  [ERR]\n"; continue; }
            std::cout << "  Scalar: "; if (!readInt(s)) { std::cout << "  [ERR]\n"; continue; }
            try { m->ScaleCol(col, s); printMatrix(*m); }
            catch (const std::exception& e) { std::cout << "  [ERR] " << e.what() << "\n"; }
        }
        else if (choice == 15) {
            int target, source, s;
            std::cout << "  Target col: "; if (!readInt(target)) { std::cout << "  [ERR]\n"; continue; }
            std::cout << "  Source col: "; if (!readInt(source)) { std::cout << "  [ERR]\n"; continue; }
            std::cout << "  Scalar: ";     if (!readInt(s)) { std::cout << "  [ERR]\n"; continue; }
            try { m->AddScaledCol(target, source, s); printMatrix(*m); }
            catch (const std::exception& e) { std::cout << "  [ERR] " << e.what() << "\n"; }
        }
    }
}

static void diagonalMatrixMenu() {
    DiagonalMatrix<int>* m = nullptr;
    int choice;

    while (true) {
        printSeparator();
        std::cout << "  Diagonal Matrix";
        if (m) std::cout << " [" << m->Rows() << "x" << m->Cols() << "]";
        std::cout << "\n";
        std::cout << "  1. Create\n";
        std::cout << "  2. Print\n";
        std::cout << "  3. Add another diagonal matrix\n";
        std::cout << "  4. Multiply by scalar\n";
        std::cout << "  5. Multiply diagonal matrices\n";
        std::cout << "  6. Transpose\n";
        std::cout << "  7. Frobenius norm\n";
        std::cout << "  8. Inverse\n";
        std::cout << "  9. Swap rows\n";
        std::cout << "  10. Scale row\n";
        std::cout << "  0. Back\n";
        if (!menu("Choice: ", 1, 10, choice)) { delete m; return; }

        if (choice == 1) {
            int n;
            if (!readSquareDim(n)) continue;
            delete m;
            m = new DiagonalMatrix<int>(n);
            std::cout << "  Enter diagonal elements:\n";
            for (int i = 0; i < n; i++) {
                int val;
                std::cout << "  diag[" << i << "]: ";
                while (!readInt(val)) {
                    std::cout << "  [ERR] Not a number.\n  diag[" << i << "]: ";
                }
                m->SetDiag(i, val);
            }
            std::cout << "  Created.\n";
        }
        else if (!m) {
            std::cout << "  [ERR] Create a matrix first.\n";
        }
        else if (choice == 2) { printMatrix(*m); }
        else if (choice == 3) {
            DiagonalMatrix<int> other(m->Rows());
            std::cout << "  Enter diagonal elements of second matrix:\n";
            for (int i = 0; i < m->Rows(); i++) {
                int val;
                std::cout << "  diag[" << i << "]: ";
                while (!readInt(val)) { std::cout << "  [ERR]\n  diag[" << i << "]: "; }
                other.SetDiag(i, val);
            }
            try {
                auto* res = m->Add(other);
                std::cout << "  Result:\n";
                printAndDelete(res);
            }
            catch (const std::exception& e) {
                std::cout << "  [ERR] " << e.what() << "\n";
            }
        }
        else if (choice == 4) {
            int s;
            std::cout << "  Scalar: "; if (!readInt(s)) { std::cout << "  [ERR]\n"; continue; }
            auto* res = m->MultiplyByScalar(s);
            std::cout << "  Result:\n";
            printAndDelete(res);
        }
        else if (choice == 5) {
            DiagonalMatrix<int> other(m->Rows());
            std::cout << "  Enter diagonal elements of second matrix:\n";
            for (int i = 0; i < m->Rows(); i++) {
                int val;
                std::cout << "  diag[" << i << "]: ";
                while (!readInt(val)) { std::cout << "  [ERR]\n  diag[" << i << "]: "; }
                other.SetDiag(i, val);
            }
            try {
                auto* res = m->MultiplyDiagonal(other);
                std::cout << "  Result:\n";
                printAndDelete(res);
            }
            catch (const std::exception& e) {
                std::cout << "  [ERR] " << e.what() << "\n";
            }
        }
        else if (choice == 6) {
            auto* res = m->Transpose();
            std::cout << "  Transposed:\n";
            printAndDelete(res);
        }
        else if (choice == 7) { std::cout << "  Frobenius: " << m->FrobeniusNorm() << "\n"; }
        else if (choice == 8) {
            try {
                auto* inv = m->Inverse();
                std::cout << "  Inverse:\n";
                printAndDelete(inv);
            }
            catch (const std::exception& e) {
                std::cout << "  [ERR] " << e.what() << "\n";
            }
        }
        else if (choice == 9) {
            int i, j;
            std::cout << "  Row 1: "; if (!readInt(i)) { std::cout << "  [ERR]\n"; continue; }
            std::cout << "  Row 2: "; if (!readInt(j)) { std::cout << "  [ERR]\n"; continue; }
            try { m->SwapRows(i, j); printMatrix(*m); }
            catch (const std::exception& e) { std::cout << "  [ERR] " << e.what() << "\n"; }
        }
        else if (choice == 10) {
            int row, s;
            std::cout << "  Row: ";    if (!readInt(row)) { std::cout << "  [ERR]\n"; continue; }
            std::cout << "  Scalar: "; if (!readInt(s)) { std::cout << "  [ERR]\n"; continue; }
            try { m->ScaleRow(row, s); printMatrix(*m); }
            catch (const std::exception& e) { std::cout << "  [ERR] " << e.what() << "\n"; }
        }
    }
}

static void sparseMatrixMenu() {
    SparseMatrix<int>* m = nullptr;
    int choice;

    while (true) {
        printSeparator();
        std::cout << "  Sparse Matrix";
        if (m) std::cout << " [" << m->Rows() << "x" << m->Cols()
            << ", nonzero=" << m->NonZeroCount() << "]";
        std::cout << "\n";
        std::cout << "  1. Create\n";
        std::cout << "  2. Set element\n";
        std::cout << "  3. Get element\n";
        std::cout << "  4. Print\n";
        std::cout << "  5. Add another sparse matrix\n";
        std::cout << "  6. Multiply by scalar\n";
        std::cout << "  7. Multiply by matrix\n";
        std::cout << "  8. Transpose\n";
        std::cout << "  9. Frobenius norm\n";
        std::cout << "  10. Swap rows\n";
        std::cout << "  11. Scale row\n";
        std::cout << "  12. Add scaled row\n";
        std::cout << "  13. Swap cols\n";
        std::cout << "  14. Scale col\n";
        std::cout << "  15. Add scaled col\n";
        std::cout << "  0. Back\n";
        if (!menu("Choice: ", 1, 15, choice)) { delete m; return; }

        if (choice == 1) {
            int r, c;
            if (!readMatrixDims(r, c)) continue;
            delete m;
            m = new SparseMatrix<int>(r, c);
            std::cout << "  Created empty " << r << "x" << c << " sparse matrix.\n";
            std::cout << "  Enter non-zero elements, row=-1 to stop:\n";
            while (true) {
                int row, col, val;
                std::cout << "  row: "; if (!readInt(row)) { std::cout << "  [ERR]\n"; continue; }
                if (row == -1) break;
                std::cout << "  col: "; if (!readInt(col)) { std::cout << "  [ERR]\n"; continue; }
                std::cout << "  val: "; if (!readInt(val)) { std::cout << "  [ERR]\n"; continue; }
                try { m->Set(row, col, val); }
                catch (const std::exception& e) { std::cout << "  [ERR] " << e.what() << "\n"; }
            }
        }
        else if (!m) {
            std::cout << "  [ERR] Create a matrix first.\n";
        }
        else if (choice == 2) {
            int row, col, val;
            std::cout << "  Row: ";   if (!readInt(row)) { std::cout << "  [ERR]\n"; continue; }
            std::cout << "  Col: ";   if (!readInt(col)) { std::cout << "  [ERR]\n"; continue; }
            std::cout << "  Value: "; if (!readInt(val)) { std::cout << "  [ERR]\n"; continue; }
            try { m->Set(row, col, val); std::cout << "  Set.\n"; }
            catch (const std::exception& e) { std::cout << "  [ERR] " << e.what() << "\n"; }
        }
        else if (choice == 3) {
            int row, col;
            std::cout << "  Row: "; if (!readInt(row)) { std::cout << "  [ERR]\n"; continue; }
            std::cout << "  Col: "; if (!readInt(col)) { std::cout << "  [ERR]\n"; continue; }
            try { std::cout << "  Value: " << m->Get(row, col) << "\n"; }
            catch (const std::exception& e) { std::cout << "  [ERR] " << e.what() << "\n"; }
        }
        else if (choice == 4) { printMatrix(*m); }
        else if (choice == 5) {
            SparseMatrix<int> other(m->Rows(), m->Cols());
            std::cout << "  Enter non-zero elements, row=-1 to stop:\n";
            while (true) {
                int row, col, val;
                std::cout << "  row: "; if (!readInt(row)) { std::cout << "  [ERR]\n"; continue; }
                if (row == -1) break;
                std::cout << "  col: "; if (!readInt(col)) { std::cout << "  [ERR]\n"; continue; }
                std::cout << "  val: "; if (!readInt(val)) { std::cout << "  [ERR]\n"; continue; }
                try { other.Set(row, col, val); }
                catch (const std::exception& e) { std::cout << "  [ERR] " << e.what() << "\n"; }
            }
            try {
                auto* res = m->Add(other);
                std::cout << "  Result:\n";
                printAndDelete(res);
            }
            catch (const std::exception& e) {
                std::cout << "  [ERR] " << e.what() << "\n";
            }
        }
        else if (choice == 6) {
            int s;
            std::cout << "  Scalar: "; if (!readInt(s)) { std::cout << "  [ERR]\n"; continue; }
            auto* res = m->MultiplyByScalar(s);
            std::cout << "  Result:\n";
            printAndDelete(res);
        }
        else if (choice == 7) {
            int r, c;
            std::cout << "  Second matrix dims (must be " << m->Cols() << " x ?):\n";
            if (!readMatrixDims(r, c)) continue;
            if (r != m->Cols()) {
                std::cout << "  [ERR] Rows must equal " << m->Cols() << "\n"; continue;
            }
            SparseMatrix<int> other(r, c);
            std::cout << "  Enter non-zero elements, row=-1 to stop:\n";
            while (true) {
                int row, col, val;
                std::cout << "  row: "; if (!readInt(row)) { std::cout << "  [ERR]\n"; continue; }
                if (row == -1) break;
                std::cout << "  col: "; if (!readInt(col)) { std::cout << "  [ERR]\n"; continue; }
                std::cout << "  val: "; if (!readInt(val)) { std::cout << "  [ERR]\n"; continue; }
                try { other.Set(row, col, val); }
                catch (const std::exception& e) { std::cout << "  [ERR] " << e.what() << "\n"; }
            }
            try {
                auto* res = m->MultiplyByMatrix(other);
                std::cout << "  Result:\n";
                printAndDelete(res);
            }
            catch (const std::exception& e) {
                std::cout << "  [ERR] " << e.what() << "\n";
            }
        }
        else if (choice == 8) {
            auto* res = m->Transpose();
            std::cout << "  Transposed:\n";
            printAndDelete(res);
        }
        else if (choice == 9) { std::cout << "  Frobenius: " << m->FrobeniusNorm() << "\n"; }
        else if (choice == 10) {
            int i, j;
            std::cout << "  Row 1: "; if (!readInt(i)) { std::cout << "  [ERR]\n"; continue; }
            std::cout << "  Row 2: "; if (!readInt(j)) { std::cout << "  [ERR]\n"; continue; }
            try { m->SwapRows(i, j); printMatrix(*m); }
            catch (const std::exception& e) { std::cout << "  [ERR] " << e.what() << "\n"; }
        }
        else if (choice == 11) {
            int row, s;
            std::cout << "  Row: ";    if (!readInt(row)) { std::cout << "  [ERR]\n"; continue; }
            std::cout << "  Scalar: "; if (!readInt(s)) { std::cout << "  [ERR]\n"; continue; }
            try { m->ScaleRow(row, s); printMatrix(*m); }
            catch (const std::exception& e) { std::cout << "  [ERR] " << e.what() << "\n"; }
        }
        else if (choice == 12) {
            int target, source, s;
            std::cout << "  Target row: "; if (!readInt(target)) { std::cout << "  [ERR]\n"; continue; }
            std::cout << "  Source row: "; if (!readInt(source)) { std::cout << "  [ERR]\n"; continue; }
            std::cout << "  Scalar: ";     if (!readInt(s)) { std::cout << "  [ERR]\n"; continue; }
            try { m->AddScaledRow(target, source, s); printMatrix(*m); }
            catch (const std::exception& e) { std::cout << "  [ERR] " << e.what() << "\n"; }
        }
        else if (choice == 13) {
            int i, j;
            std::cout << "  Col 1: "; if (!readInt(i)) { std::cout << "  [ERR]\n"; continue; }
            std::cout << "  Col 2: "; if (!readInt(j)) { std::cout << "  [ERR]\n"; continue; }
            try { m->SwapCols(i, j); printMatrix(*m); }
            catch (const std::exception& e) { std::cout << "  [ERR] " << e.what() << "\n"; }
        }
        else if (choice == 14) {
            int col, s;
            std::cout << "  Col: ";    if (!readInt(col)) { std::cout << "  [ERR]\n"; continue; }
            std::cout << "  Scalar: "; if (!readInt(s)) { std::cout << "  [ERR]\n"; continue; }
            try { m->ScaleCol(col, s); printMatrix(*m); }
            catch (const std::exception& e) { std::cout << "  [ERR] " << e.what() << "\n"; }
        }
        else if (choice == 15) {
            int target, source, s;
            std::cout << "  Target col: "; if (!readInt(target)) { std::cout << "  [ERR]\n"; continue; }
            std::cout << "  Source col: "; if (!readInt(source)) { std::cout << "  [ERR]\n"; continue; }
            std::cout << "  Scalar: ";     if (!readInt(s)) { std::cout << "  [ERR]\n"; continue; }
            try { m->AddScaledCol(target, source, s); printMatrix(*m); }
            catch (const std::exception& e) { std::cout << "  [ERR] " << e.what() << "\n"; }
        }
    }
}

static void matrixMenu() {
    int choice;
    while (true) {
        printSeparator();
        std::cout << "  Matrices\n";
        std::cout << "  1. Rectangular Matrix\n";
        std::cout << "  2. Square Matrix\n";
        std::cout << "  3. Diagonal Matrix\n";
        std::cout << "  4. Sparse Matrix\n";
        std::cout << "  0. Back\n";
        if (!menu("Choice: ", 1, 4, choice)) return;
        if (choice == 1) rectangularMatrixMenu();
        else if (choice == 2) squareMatrixMenu();
        else if (choice == 3) diagonalMatrixMenu();
        else if (choice == 4) sparseMatrixMenu();
    }
}

int main() {
    int choice;
    while (true) {
        printSeparator();
        std::cout << "  Main Menu\n";
        std::cout << "  1. Stack\n";
        std::cout << "  2. Queue\n";
        std::cout << "  3. Hanoi Tower\n";
        std::cout << "  4. Matrices\n";
        std::cout << "  0. Exit\n";
        if (!menu("Choice: ", 1, 4, choice)) {
            std::cout << "  Goodbye.\n";
            return 0;
        }
        if (choice == 1) stackMenu();
        else if (choice == 2) queueMenu();
        else if (choice == 3) hanoiMenu();
        else if (choice == 4) matrixMenu();
    }
}