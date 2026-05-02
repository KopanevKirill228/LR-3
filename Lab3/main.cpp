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
#include "NaturalNumberAlgorithms.h"
#include "RecurrenceAlgorithms.h"

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

static bool readIntWithPrompt(int& val, const std::string& prompt) {
    std::cout << prompt;
    return readInt(val);
}

static bool readIntInRange(int& val, const std::string& prompt, int lo, int hi) {
    while (true) {
        std::cout << prompt;
        if (!readInt(val)) {
            std::cout << "  [ERR] Not a number. Try again.\n";
            continue;
        }
        if (val == 0) return false;
        if (val < lo || val > hi) {
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

static void printIntSequence(const Sequence<int>* seq) {
    if (seq == nullptr) {
        std::cout << "  null\n";
        return;
    }

    std::cout << "  [";

    for (int i = 0; i < seq->GetLength(); ++i) {
        if (i > 0) {
            std::cout << ", ";
        }

        std::cout << seq->Get(i);
    }

    std::cout << "]\n";
}

static void printAndDelete(IMatrix<int>* res) {
    try {
        if (res) {
            printMatrix(*res);
        }

        delete res;
    }
    catch (...) {
        delete res;
        throw;
    }
}

static void printError(const std::string& msg) {
    std::cout << "  [ERR] " << msg << "\n";
}

static bool handleInputError() {
    printError("Not a number");
    return false;
}


static bool readMatrixDims(int& rows, int& cols) {
    if (!readIntWithPrompt(rows, "  Rows: ") || rows <= 0) {
        printError("Rows must be positive");
        return false;
    }
    if (!readIntWithPrompt(cols, "  Cols: ") || cols <= 0) {
        printError("Cols must be positive");
        return false;
    }
    return true;
}

static bool readSquareDim(int& n) {
    if (!readIntWithPrompt(n, "  Size (n): ") || n <= 0) {
        printError("Size must be positive");
        return false;
    }
    return true;
}

static void fillMatrix(IMatrix<int>& m) {
    std::cout << "  Enter elements row by row:\n";
    for (int i = 0; i < m.Rows(); i++) {
        for (int j = 0; j < m.Cols(); j++) {
            int val;
            std::string prompt = "  [" + std::to_string(i) + "][" + std::to_string(j) + "]: ";
            while (!readIntWithPrompt(val, prompt)) {
                printError("Not a number. Try again");
                prompt = "  [" + std::to_string(i) + "][" + std::to_string(j) + "]: ";
            }
            try {
                m.Set(i, j, val);
            }
            catch (const std::exception& e) {
                printError(std::string(e.what()) + " (skipped)");
            }
        }
    }
}

static bool readTwoInts(int& a, int& b, const std::string& promptA, const std::string& promptB) {
    if (!readIntWithPrompt(a, promptA)) return handleInputError();
    if (!readIntWithPrompt(b, promptB)) return handleInputError();
    return true;
}

static bool readThreeInts(int& a, int& b, int& c,
    const std::string& promptA, const std::string& promptB, const std::string& promptC) {
    if (!readIntWithPrompt(a, promptA)) return handleInputError();
    if (!readIntWithPrompt(b, promptB)) return handleInputError();
    if (!readIntWithPrompt(c, promptC)) return handleInputError();
    return true;
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

        if (!readIntInRange(choice, "Choice: ", 1, 5)) return;

        switch (choice) {
        case 1: {
            int val;
            if (!readIntWithPrompt(val, "  Value: ")) {
                handleInputError();
                continue;
            }
            s.Push(val);
            std::cout << "  Pushed " << val << "\n";
            break;
        }
        case 2: {
            try {
                std::cout << "  Popped: " << s.Pop() << "\n";
            }
            catch (const std::exception& e) {
                printError(e.what());
            }
            break;
        }
        case 3: {
            try {
                std::cout << "  Top: " << s.Peek() << "\n";
            }
            catch (const std::exception& e) {
                printError(e.what());
            }
            break;
        }
        case 4: {
            Stack<int> other;
            int n;
            if (!readIntWithPrompt(n, "  How many elements to add: ") || n < 0) {
                printError("Invalid count");
                continue;
            }
            for (int i = 0; i < n; i++) {
                int val;
                std::string prompt = "  Element " + std::to_string(i + 1) + ": ";
                while (!readIntWithPrompt(val, prompt)) {
                    handleInputError();
                    prompt = "  Element " + std::to_string(i + 1) + ": ";
                }
                other.Push(val);
            }
            s = s.Concat(other);
            std::cout << "  Concatenated. New size: " << s.GetCount() << "\n";
            break;
        }
        case 5: {
            if (s.IsEmpty()) {
                std::cout << "  (empty)\n";
                continue;
            }
            std::cout << "  Bottom -> Top: ";
            for (size_t i = 0; i < s.GetCount(); i++)
                std::cout << s.Get(i) << " ";
            std::cout << "\n";
            break;
        }
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

        if (!readIntInRange(choice, "Choice: ", 1, 6)) return;

        switch (choice) {
        case 1: {
            int val;
            if (!readIntWithPrompt(val, "  Value: ")) {
                handleInputError();
                continue;
            }
            q.Enqueue(val);
            std::cout << "  Enqueued " << val << "\n";
            break;
        }
        case 2: {
            try {
                std::cout << "  Dequeued: " << q.Dequeue() << "\n";
            }
            catch (const std::exception& e) {
                printError(e.what());
            }
            break;
        }
        case 3: {
            try {
                std::cout << "  Front: " << q.PeekFront() << "\n";
            }
            catch (const std::exception& e) {
                printError(e.what());
            }
            break;
        }
        case 4: {
            try {
                std::cout << "  Back: " << q.PeekBack() << "\n";
            }
            catch (const std::exception& e) {
                printError(e.what());
            }
            break;
        }
        case 5: {
            Queue<int> other;
            int n;
            if (!readIntWithPrompt(n, "  How many elements to add: ") || n < 0) {
                printError("Invalid count");
                continue;
            }
            for (int i = 0; i < n; i++) {
                int val;
                std::string prompt = "  Element " + std::to_string(i + 1) + ": ";
                while (!readIntWithPrompt(val, prompt)) {
                    handleInputError();
                    prompt = "  Element " + std::to_string(i + 1) + ": ";
                }
                other.Enqueue(val);
            }
            q = q.Concat(other);
            std::cout << "  Concatenated. New size: " << q.GetCount() << "\n";
            break;
        }
        case 6: {
            if (q.IsEmpty()) {
                std::cout << "  (empty)\n";
                continue;
            }
            std::cout << "  Front -> Back: ";
            for (size_t i = 0; i < q.GetCount(); i++)
                std::cout << q.Get(i) << " ";
            std::cout << "\n";
            break;
        }
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

        if (!readIntInRange(choice, "Choice: ", 1, 2)) return;

        int n;
        if (!readIntWithPrompt(n, "  Number of disks (1-12): ") || n < 1 || n > 12) {
            printError("Enter a number from 1 to 12");
            continue;
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
            if (!readIntWithPrompt(delay, "  Delay ms (100-2000): ") || delay < 100 || delay > 2000) {
                printError("Enter 100-2000");
                continue;
            }
            HanoiRenderer<int> renderer(n);
            renderer.SolveAnimated(h, delay);
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

        if (!readIntInRange(choice, "Choice: ", 1, 13)) { delete m; return; }

        if (choice == 1) {
            int r, c;
            if (!readMatrixDims(r, c)) continue;
            delete m;
            m = new RectangularMatrix<int>(r, c);
            fillMatrix(*m);
            std::cout << "  Created.\n";
        }
        else if (!m) {
            printError("Create a matrix first");
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
                printError(e.what());
            }
        }
        else if (choice == 4) {
            int s;
            if (!readIntWithPrompt(s, "  Scalar: ")) {
                handleInputError();
                continue;
            }
            auto* res = m->MultiplyByScalar(s);
            std::cout << "  Result:\n";
            printAndDelete(res);
        }
        else if (choice == 5) {
            int r, c;
            std::cout << "  Second matrix dims (must be " << m->Cols() << " x ?):\n";
            if (!readMatrixDims(r, c)) continue;
            if (r != m->Cols()) {
                printError("Rows must equal " + std::to_string(m->Cols()));
                continue;
            }
            RectangularMatrix<int> other(r, c);
            fillMatrix(other);
            try {
                auto* res = m->MultiplyByMatrix(other);
                std::cout << "  Result:\n";
                printAndDelete(res);
            }
            catch (const std::exception& e) {
                printError(e.what());
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
            if (!readTwoInts(i, j, "  Row 1 (0-based): ", "  Row 2 (0-based): ")) continue;
            try {
                m->SwapRows(i, j);
                printMatrix(*m);
            }
            catch (const std::exception& e) {
                printError(e.what());
            }
        }
        else if (choice == 9) {
            int row, s;
            if (!readTwoInts(row, s, "  Row (0-based): ", "  Scalar: ")) continue;
            try {
                m->ScaleRow(row, s);
                printMatrix(*m);
            }
            catch (const std::exception& e) {
                printError(e.what());
            }
        }
        else if (choice == 10) {
            int target, source, scalar;
            if (!readThreeInts(target, source, scalar,
                "  Target row: ", "  Source row: ", "  Scalar: ")) continue;
            try {
                m->AddScaledRow(target, source, scalar);
                printMatrix(*m);
            }
            catch (const std::exception& e) {
                printError(e.what());
            }
        }
        else if (choice == 11) {
            int i, j;
            if (!readTwoInts(i, j, "  Col 1 (0-based): ", "  Col 2 (0-based): ")) continue;
            try {
                m->SwapCols(i, j);
                printMatrix(*m);
            }
            catch (const std::exception& e) {
                printError(e.what());
            }
        }
        else if (choice == 12) {
            int col, s;
            if (!readTwoInts(col, s, "  Col (0-based): ", "  Scalar: ")) continue;
            try {
                m->ScaleCol(col, s);
                printMatrix(*m);
            }
            catch (const std::exception& e) {
                printError(e.what());
            }
        }
        else if (choice == 13) {
            int target, source, scalar;
            if (!readThreeInts(target, source, scalar,
                "  Target col: ", "  Source col: ", "  Scalar: ")) continue;
            try {
                m->AddScaledCol(target, source, scalar);
                printMatrix(*m);
            }
            catch (const std::exception& e) {
                printError(e.what());
            }
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

        if (!readIntInRange(choice, "Choice: ", 1, 15)) { delete m; return; }

        if (choice == 1) {
            int n;
            if (!readSquareDim(n)) continue;
            delete m;
            m = new SquareMatrix<int>(n);
            fillMatrix(*m);
            std::cout << "  Created.\n";
        }
        else if (!m) {
            printError("Create a matrix first");
        }
        else if (choice == 2) {
            printMatrix(*m);
        }
        else if (choice == 3) {
            SquareMatrix<int> other(m->Rows());
            fillMatrix(other);
            try {
                auto* res = m->Add(other);
                std::cout << "  Result:\n";
                printAndDelete(res);
            }
            catch (const std::exception& e) {
                printError(e.what());
            }
        }
        else if (choice == 4) {
            int s;
            if (!readIntWithPrompt(s, "  Scalar: ")) {
                handleInputError();
                continue;
            }
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
                printError(e.what());
            }
        }
        else if (choice == 6) {
            auto* res = m->Transpose();
            std::cout << "  Transposed:\n";
            printAndDelete(res);
        }
        else if (choice == 7) {
            std::cout << "  Frobenius: " << m->FrobeniusNorm() << "\n";
        }
        else if (choice == 8) {
            std::cout << "  Trace: " << m->Trace() << "\n";
        }
        else if (choice == 9) {
            std::cout << "  Determinant: " << m->Determinant() << "\n";
        }
        else if (choice == 10) {
            int i, j;
            if (!readTwoInts(i, j, "  Row 1: ", "  Row 2: ")) continue;
            try {
                m->SwapRows(i, j);
                printMatrix(*m);
            }
            catch (const std::exception& e) {
                printError(e.what());
            }
        }
        else if (choice == 11) {
            int row, s;
            if (!readTwoInts(row, s, "  Row: ", "  Scalar: ")) continue;
            try {
                m->ScaleRow(row, s);
                printMatrix(*m);
            }
            catch (const std::exception& e) {
                printError(e.what());
            }
        }
        else if (choice == 12) {
            int target, source, scalar;
            if (!readThreeInts(target, source, scalar,
                "  Target row: ", "  Source row: ", "  Scalar: ")) continue;
            try {
                m->AddScaledRow(target, source, scalar);
                printMatrix(*m);
            }
            catch (const std::exception& e) {
                printError(e.what());
            }
        }
        else if (choice == 13) {
            int i, j;
            if (!readTwoInts(i, j, "  Col 1: ", "  Col 2: ")) continue;
            try {
                m->SwapCols(i, j);
                printMatrix(*m);
            }
            catch (const std::exception& e) {
                printError(e.what());
            }
        }
        else if (choice == 14) {
            int col, s;
            if (!readTwoInts(col, s, "  Col: ", "  Scalar: ")) continue;
            try {
                m->ScaleCol(col, s);
                printMatrix(*m);
            }
            catch (const std::exception& e) {
                printError(e.what());
            }
        }
        else if (choice == 15) {
            int target, source, scalar;
            if (!readThreeInts(target, source, scalar,
                "  Target col: ", "  Source col: ", "  Scalar: ")) continue;
            try {
                m->AddScaledCol(target, source, scalar);
                printMatrix(*m);
            }
            catch (const std::exception& e) {
                printError(e.what());
            }
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

        if (!readIntInRange(choice, "Choice: ", 1, 10)) { delete m; return; }

        if (choice == 1) {
            int n;
            if (!readSquareDim(n)) continue;
            delete m;
            m = new DiagonalMatrix<int>(n);
            std::cout << "  Enter diagonal elements:\n";
            for (int i = 0; i < n; i++) {
                int val;
                std::string prompt = "  diag[" + std::to_string(i) + "]: ";
                while (!readIntWithPrompt(val, prompt)) {
                    printError("Not a number");
                    prompt = "  diag[" + std::to_string(i) + "]: ";
                }
                m->SetDiag(i, val);
            }
            std::cout << "  Created.\n";
        }
        else if (!m) {
            printError("Create a matrix first");
        }
        else if (choice == 2) {
            printMatrix(*m);
        }
        else if (choice == 3) {
            DiagonalMatrix<int> other(m->Rows());
            std::cout << "  Enter diagonal elements of second matrix:\n";
            for (int i = 0; i < m->Rows(); i++) {
                int val;
                std::string prompt = "  diag[" + std::to_string(i) + "]: ";
                while (!readIntWithPrompt(val, prompt)) {
                    printError("Not a number");
                    prompt = "  diag[" + std::to_string(i) + "]: ";
                }
                other.SetDiag(i, val);
            }
            try {
                auto* res = m->Add(other);
                std::cout << "  Result:\n";
                printAndDelete(res);
            }
            catch (const std::exception& e) {
                printError(e.what());
            }
        }
        else if (choice == 4) {
            int s;
            if (!readIntWithPrompt(s, "  Scalar: ")) {
                handleInputError();
                continue;
            }
            auto* res = m->MultiplyByScalar(s);
            std::cout << "  Result:\n";
            printAndDelete(res);
        }
        else if (choice == 5) {
            DiagonalMatrix<int> other(m->Rows());
            std::cout << "  Enter diagonal elements of second matrix:\n";
            for (int i = 0; i < m->Rows(); i++) {
                int val;
                std::string prompt = "  diag[" + std::to_string(i) + "]: ";
                while (!readIntWithPrompt(val, prompt)) {
                    printError("Not a number");
                    prompt = "  diag[" + std::to_string(i) + "]: ";
                }
                other.SetDiag(i, val);
            }
            try {
                auto* res = m->MultiplyDiagonal(other);
                std::cout << "  Result:\n";
                printAndDelete(res);
            }
            catch (const std::exception& e) {
                printError(e.what());
            }
        }
        else if (choice == 6) {
            auto* res = m->Transpose();
            std::cout << "  Transposed:\n";
            printAndDelete(res);
        }
        else if (choice == 7) {
            std::cout << "  Frobenius: " << m->FrobeniusNorm() << "\n";
        }
        else if (choice == 8) {
            try {
                auto* inv = m->Inverse();
                std::cout << "  Inverse:\n";
                printAndDelete(inv);
            }
            catch (const std::exception& e) {
                printError(e.what());
            }
        }
        else if (choice == 9) {
            int i, j;
            if (!readTwoInts(i, j, "  Row 1: ", "  Row 2: ")) continue;
            try {
                m->SwapRows(i, j);
                printMatrix(*m);
            }
            catch (const std::exception& e) {
                printError(e.what());
            }
        }
        else if (choice == 10) {
            int row, s;
            if (!readTwoInts(row, s, "  Row: ", "  Scalar: ")) continue;
            try {
                m->ScaleRow(row, s);
                printMatrix(*m);
            }
            catch (const std::exception& e) {
                printError(e.what());
            }
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

        if (!readIntInRange(choice, "Choice: ", 1, 15)) { delete m; return; }

        if (choice == 1) {
            int r, c;
            if (!readMatrixDims(r, c)) continue;
            delete m;
            m = new SparseMatrix<int>(r, c);
            std::cout << "  Created empty " << r << "x" << c << " sparse matrix.\n";
            std::cout << "  Enter non-zero elements, row=-1 to stop:\n";
            while (true) {
                int row, col, val;
                if (!readIntWithPrompt(row, "  row: ")) {
                    handleInputError();
                    continue;
                }
                if (row == -1) break;
                if (!readIntWithPrompt(col, "  col: ")) {
                    handleInputError();
                    continue;
                }
                if (!readIntWithPrompt(val, "  val: ")) {
                    handleInputError();
                    continue;
                }
                try {
                    m->Set(row, col, val);
                }
                catch (const std::exception& e) {
                    printError(e.what());
                }
            }
        }
        else if (!m) {
            printError("Create a matrix first");
        }
        else if (choice == 2) {
            int row, col, val;
            if (!readThreeInts(row, col, val, "  Row: ", "  Col: ", "  Value: ")) continue;
            try {
                m->Set(row, col, val);
                std::cout << "  Set.\n";
            }
            catch (const std::exception& e) {
                printError(e.what());
            }
        }
        else if (choice == 3) {
            int row, col;
            if (!readTwoInts(row, col, "  Row: ", "  Col: ")) continue;
            try {
                std::cout << "  Value: " << m->Get(row, col) << "\n";
            }
            catch (const std::exception& e) {
                printError(e.what());
            }
        }
        else if (choice == 4) {
            printMatrix(*m);
        }
        else if (choice == 5) {
            SparseMatrix<int> other(m->Rows(), m->Cols());
            std::cout << "  Enter non-zero elements, row=-1 to stop:\n";
            while (true) {
                int row, col, val;
                if (!readIntWithPrompt(row, "  row: ")) {
                    handleInputError();
                    continue;
                }
                if (row == -1) break;
                if (!readIntWithPrompt(col, "  col: ")) {
                    handleInputError();
                    continue;
                }
                if (!readIntWithPrompt(val, "  val: ")) {
                    handleInputError();
                    continue;
                }
                try {
                    other.Set(row, col, val);
                }
                catch (const std::exception& e) {
                    printError(e.what());
                }
            }
            try {
                auto* res = m->Add(other);
                std::cout << "  Result:\n";
                printAndDelete(res);
            }
            catch (const std::exception& e) {
                printError(e.what());
            }
        }
        else if (choice == 6) {
            int s;
            if (!readIntWithPrompt(s, "  Scalar: ")) {
                handleInputError();
                continue;
            }
            auto* res = m->MultiplyByScalar(s);
            std::cout << "  Result:\n";
            printAndDelete(res);
        }
        else if (choice == 7) {
            int r, c;
            std::cout << "  Second matrix dims (must be " << m->Cols() << " x ?):\n";
            if (!readMatrixDims(r, c)) continue;
            if (r != m->Cols()) {
                printError("Rows must equal " + std::to_string(m->Cols()));
                continue;
            }
            SparseMatrix<int> other(r, c);
            std::cout << "  Enter non-zero elements, row=-1 to stop:\n";
            while (true) {
                int row, col, val;
                if (!readIntWithPrompt(row, "  row: ")) {
                    handleInputError();
                    continue;
                }
                if (row == -1) break;
                if (!readIntWithPrompt(col, "  col: ")) {
                    handleInputError();
                    continue;
                }
                if (!readIntWithPrompt(val, "  val: ")) {
                    handleInputError();
                    continue;
                }
                try {
                    other.Set(row, col, val);
                }
                catch (const std::exception& e) {
                    printError(e.what());
                }
            }
            try {
                auto* res = m->MultiplyByMatrix(other);
                std::cout << "  Result:\n";
                printAndDelete(res);
            }
            catch (const std::exception& e) {
                printError(e.what());
            }
        }
        else if (choice == 8) {
            auto* res = m->Transpose();
            std::cout << "  Transposed:\n";
            printAndDelete(res);
        }
        else if (choice == 9) {
            std::cout << "  Frobenius: " << m->FrobeniusNorm() << "\n";
        }
        else if (choice == 10) {
            int i, j;
            if (!readTwoInts(i, j, "  Row 1: ", "  Row 2: ")) continue;
            try {
                m->SwapRows(i, j);
                printMatrix(*m);
            }
            catch (const std::exception& e) {
                printError(e.what());
            }
        }
        else if (choice == 11) {
            int row, s;
            if (!readTwoInts(row, s, "  Row: ", "  Scalar: ")) continue;
            try {
                m->ScaleRow(row, s);
                printMatrix(*m);
            }
            catch (const std::exception& e) {
                printError(e.what());
            }
        }
        else if (choice == 12) {
            int target, source, scalar;
            if (!readThreeInts(target, source, scalar,
                "  Target row: ", "  Source row: ", "  Scalar: ")) continue;
            try {
                m->AddScaledRow(target, source, scalar);
                printMatrix(*m);
            }
            catch (const std::exception& e) {
                printError(e.what());
            }
        }
        else if (choice == 13) {
            int i, j;
            if (!readTwoInts(i, j, "  Col 1: ", "  Col 2: ")) continue;
            try {
                m->SwapCols(i, j);
                printMatrix(*m);
            }
            catch (const std::exception& e) {
                printError(e.what());
            }
        }
        else if (choice == 14) {
            int col, s;
            if (!readTwoInts(col, s, "  Col: ", "  Scalar: ")) continue;
            try {
                m->ScaleCol(col, s);
                printMatrix(*m);
            }
            catch (const std::exception& e) {
                printError(e.what());
            }
        }
        else if (choice == 15) {
            int target, source, scalar;
            if (!readThreeInts(target, source, scalar,
                "  Target col: ", "  Source col: ", "  Scalar: ")) continue;
            try {
                m->AddScaledCol(target, source, scalar);
                printMatrix(*m);
            }
            catch (const std::exception& e) {
                printError(e.what());
            }
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

        if (!readIntInRange(choice, "Choice: ", 1, 4)) return;

        switch (choice) {
        case 1: rectangularMatrixMenu(); break;
        case 2: squareMatrixMenu(); break;
        case 3: diagonalMatrixMenu(); break;
        case 4: sparseMatrixMenu(); break;
        }
    }
}

static void naturalNumberAlgorithmsMenu() {
    int choice;

    while (true) {
        printSeparator();
        std::cout << "  Natural Number Algorithms\n";
        std::cout << "  1. Range [low, high]\n";
        std::cout << "  2. Check prime\n";
        std::cout << "  3. Get divisors\n";
        std::cout << "  4. Get primes in range\n";
        std::cout << "  5. Get prime factors\n";
        std::cout << "  6. Sieve of Eratosthenes\n";
        std::cout << "  0. Back\n";

        if (!readIntInRange(choice, "Choice: ", 1, 6)) {
            return;
        }

        try {
            if (choice == 1) {
                int low, high;

                if (!readTwoInts(low, high, "  Low: ", "  High: ")) {
                    continue;
                }

                Sequence<int>* result = nullptr;

                try {
                    result = Range(low, high);
                    printIntSequence(result);
                    delete result;
                }
                catch (...) {
                    delete result;
                    throw;
                }
            }
            else if (choice == 2) {
                int n;

                if (!readIntWithPrompt(n, "  Number: ")) {
                    handleInputError();
                    continue;
                }

                if (IsPrime(n)) {
                    std::cout << "  Prime\n";
                }
                else {
                    std::cout << "  Not prime\n";
                }
            }
            else if (choice == 3) {
                int n;

                if (!readIntWithPrompt(n, "  Number: ")) {
                    handleInputError();
                    continue;
                }

                Sequence<int>* result = nullptr;

                try {
                    result = GetDivisors(n);
                    printIntSequence(result);
                    delete result;
                }
                catch (...) {
                    delete result;
                    throw;
                }
            }
            else if (choice == 4) {
                int low, high;

                if (!readTwoInts(low, high, "  Low: ", "  High: ")) {
                    continue;
                }

                Sequence<int>* result = nullptr;

                try {
                    result = GetPrimes(low, high);
                    printIntSequence(result);
                    delete result;
                }
                catch (...) {
                    delete result;
                    throw;
                }
            }
            else if (choice == 5) {
                int n;

                if (!readIntWithPrompt(n, "  Number: ")) {
                    handleInputError();
                    continue;
                }

                Sequence<int>* result = nullptr;

                try {
                    result = GetPrimeFactors(n);
                    printIntSequence(result);
                    delete result;
                }
                catch (...) {
                    delete result;
                    throw;
                }
            }
            else if (choice == 6) {
                int low, high;

                if (!readTwoInts(low, high, "  Low: ", "  High: ")) {
                    continue;
                }

                Sequence<int>* result = nullptr;

                try {
                    result = SieveOfEratosthenes(low, high);
                    printIntSequence(result);
                    delete result;
                }
                catch (...) {
                    delete result;
                    throw;
                }
            }
        }
        catch (const std::exception& e) {
            printError(e.what());
        }
    }
}

static void recurrenceAlgorithmsMenu() {
    int choice;

    while (true) {
        printSeparator();
        std::cout << "  Recurrence Algorithms\n";
        std::cout << "  1. Factorials\n";
        std::cout << "  2. Fibonacci\n";
        std::cout << "  3. Linear recurrence\n";
        std::cout << "  0. Back\n";

        if (!readIntInRange(choice, "Choice: ", 1, 3)) {
            return;
        }

        try {
            if (choice == 1) {
                int count;

                if (!readIntWithPrompt(count, "  Count: ")) {
                    handleInputError();
                    continue;
                }

                Sequence<int>* result = nullptr;

                try {
                    result = Factorials(count);
                    printIntSequence(result);
                    delete result;
                }
                catch (...) {
                    delete result;
                    throw;
                }
            }
            else if (choice == 2) {
                int count;

                if (!readIntWithPrompt(count, "  Count: ")) {
                    handleInputError();
                    continue;
                }

                Sequence<int>* result = nullptr;

                try {
                    result = Fibonacci(count);
                    printIntSequence(result);
                    delete result;
                }
                catch (...) {
                    delete result;
                    throw;
                }
            }
            else if (choice == 3) {
                int order;
                int count;

                if (!readIntWithPrompt(order, "  Order: ")) {
                    handleInputError();
                    continue;
                }

                if (order <= 0) {
                    printError("Order must be positive");
                    continue;
                }

                if (!readIntWithPrompt(count, "  Count: ")) {
                    handleInputError();
                    continue;
                }

                int* coeffData = nullptr;
                int* initData = nullptr;
                MutableArraySequence<int>* coefficients = nullptr;
                MutableArraySequence<int>* initialValues = nullptr;
                Sequence<int>* result = nullptr;

                try {
                    coeffData = new int[order];
                    initData = new int[order];

                    std::cout << "  Enter coefficients [a1, a2, ..., ak]:\n";
                    for (int i = 0; i < order; ++i) {
                        std::string prompt = "  a" + std::to_string(i + 1) + ": ";

                        while (!readIntWithPrompt(coeffData[i], prompt)) {
                            handleInputError();
                        }
                    }

                    std::cout << "  Enter initial values [x0, x1, ..., x(k-1)]:\n";
                    for (int i = 0; i < order; ++i) {
                        std::string prompt = "  x" + std::to_string(i) + ": ";

                        while (!readIntWithPrompt(initData[i], prompt)) {
                            handleInputError();
                        }
                    }

                    coefficients = new MutableArraySequence<int>(coeffData, order);
                    initialValues = new MutableArraySequence<int>(initData, order);

                    result = LinearRecurrence(coefficients, initialValues, count);

                    printIntSequence(result);

                    delete[] coeffData;
                    delete[] initData;
                    delete coefficients;
                    delete initialValues;
                    delete result;
                }
                catch (...) {
                    delete[] coeffData;
                    delete[] initData;
                    delete coefficients;
                    delete initialValues;
                    delete result;
                    throw;
                }
            }
        }
        catch (const std::exception& e) {
            printError(e.what());
        }
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
        std::cout << "  5. Natural Number Algorithms\n";
        std::cout << "  6. Recurrence Algorithms\n";
        std::cout << "  0. Exit\n";

        if (!readIntInRange(choice, "Choice: ", 1, 6)) {
            std::cout << "  Goodbye.\n";
            return 0;
        }

        switch (choice) {
        case 1: stackMenu(); break;
        case 2: queueMenu(); break;
        case 3: hanoiMenu(); break;
        case 4: matrixMenu(); break;
        case 5: naturalNumberAlgorithmsMenu(); break;
        case 6: recurrenceAlgorithmsMenu(); break;
        }
    }
}