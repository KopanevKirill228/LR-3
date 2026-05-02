#include <iostream>
#include <cmath>
#include "Stack.h"
#include "Queue.h"
#include "Hanoi.h"
#include "CollectionOperators.h"
#include "RectangularMatrix.h"
#include "SquareMatrix.h"
#include "DiagonalMatrix.h"
#include "SparseMatrix.h"
#include "MatrixOperators.h"
#include "NaturalNumberAlgorithms.h"
#include "RecurrenceAlgorithms.h"

static int total = 0;
static int failed = 0;
static const char* current_suite = "";

static void ok(const char* desc) {
    ++total;
    std::cout << "  [PASS] " << desc << "\n";
}
static void fail(const char* desc, const char* file, int line, const char* expr) {
    ++total; ++failed;
    std::cout << "  [FAIL] " << desc << "\n"
        << "         " << file << ":" << line << " — " << expr << "\n";
}
#define CHECK(desc, expr) \
    do { if (expr) { ok(desc); } else { fail(desc, __FILE__, __LINE__, #expr); } } while(0)
#define CHECK_THROWS(desc, expr) \
    do { \
        bool _threw = false; \
        try { expr; } catch (...) { _threw = true; } \
        if (_threw) { ok(desc); } \
        else { fail(desc, __FILE__, __LINE__, "expected exception: " #expr); } \
    } while(0)
#define SUITE(name) \
    do { current_suite = name; \
         std::cout << "\n=== " << name << " ===\n"; } while(0)


// Stack
void test_Stack_PushPeekPop() {
    SUITE("Stack: Push / Peek / Pop");
    {
        Stack<int> s;
        s.Push(1); s.Push(2); s.Push(3);
        CHECK("peek returns top", s.Peek() == 3);
        CHECK("count after 3 pushes", s.GetCount() == 3);
        CHECK("pop returns top", s.Pop() == 3);
        CHECK("count after pop", s.GetCount() == 2);
        CHECK("pop returns next", s.Pop() == 2);
        CHECK("pop returns last", s.Pop() == 1);
        CHECK("empty after all pops", s.IsEmpty());
    }
}

void test_Stack_EmptyThrows() {
    SUITE("Stack: empty boundary");
    {
        Stack<int> s;
        CHECK("empty on new stack", s.IsEmpty());
        CHECK_THROWS("pop from empty throws", s.Pop());
        CHECK_THROWS("peek from empty throws", s.Peek());
    }
}

void test_Stack_Get() {
    SUITE("Stack: Get by index");
    {
        Stack<int> s;
        s.Push(10); s.Push(20); s.Push(30);
        CHECK("Get(0) is bottom", s.Get(0) == 10);
        CHECK("Get(2) is top", s.Get(2) == 30);
        CHECK_THROWS("Get out of range throws", s.Get(3));
    }
}

void test_Stack_Concat() {
    SUITE("Stack: Concat");
    {
        Stack<int> a, b;
        a.Push(1); a.Push(2);
        b.Push(3); b.Push(4);
        Stack<int> c = a.Concat(b);
        CHECK("concat count", c.GetCount() == 4);
        CHECK("concat preserves order", c.Get(0) == 1 && c.Get(3) == 4);
        CHECK("original unchanged", a.GetCount() == 2);
    }
    {
        Stack<int> a, b;
        a.Push(1);
        Stack<int> c = a.Concat(b); // concat с пустым
        CHECK("concat with empty", c.GetCount() == 1);
    }
    {
        Stack<int> a, b;
        b.Push(1);
        Stack<int> c = a.Concat(b); // пустой concat с непустым
        CHECK("empty concat with nonempty", c.GetCount() == 1);
    }
}

void test_Stack_SingleElement() {
    SUITE("Stack: single element boundary");
    {
        Stack<int> s;
        s.Push(42);
        CHECK("peek single", s.Peek() == 42);
        CHECK("pop single", s.Pop() == 42);
        CHECK("empty after pop single", s.IsEmpty());
    }
}

// Queue
void test_Queue_EnqueueDequeue() {
    SUITE("Queue: Enqueue / Dequeue");
    {
        Queue<int> q;
        q.Enqueue(1); q.Enqueue(2); q.Enqueue(3);
        CHECK("count after 3 enqueues", q.GetCount() == 3);
        CHECK("dequeue returns front", q.Dequeue() == 1);
        CHECK("dequeue returns next", q.Dequeue() == 2);
        CHECK("dequeue returns last", q.Dequeue() == 3);
        CHECK("empty after all dequeues", q.IsEmpty());
    }
}

void test_Queue_EmptyThrows() {
    SUITE("Queue: empty boundary");
    {
        Queue<int> q;
        CHECK("empty on new queue", q.IsEmpty());
        CHECK_THROWS("dequeue from empty throws", q.Dequeue());
        CHECK_THROWS("peekfront from empty throws", q.PeekFront());
        CHECK_THROWS("peekback from empty throws", q.PeekBack());
    }
}

void test_Queue_Peek() {
    SUITE("Queue: PeekFront / PeekBack");
    {
        Queue<int> q;
        q.Enqueue(10); q.Enqueue(20); q.Enqueue(30);
        CHECK("peekfront is first", q.PeekFront() == 10);
        CHECK("peekback is last", q.PeekBack() == 30);
        CHECK("peek does not remove", q.GetCount() == 3);
    }
}

void test_Queue_Get() {
    SUITE("Queue: Get by index");
    {
        Queue<int> q;
        q.Enqueue(5); q.Enqueue(10); q.Enqueue(15);
        CHECK("Get(0) is front", q.Get(0) == 5);
        CHECK("Get(2) is back", q.Get(2) == 15);
        CHECK_THROWS("Get out of range throws", q.Get(3));
    }
}

void test_Queue_Concat() {
    SUITE("Queue: Concat");
    {
        Queue<int> a, b;
        a.Enqueue(1); a.Enqueue(2);
        b.Enqueue(3); b.Enqueue(4);
        Queue<int> c = a.Concat(b);
        CHECK("concat count", c.GetCount() == 4);
        CHECK("concat front", c.PeekFront() == 1);
        CHECK("concat back", c.PeekBack() == 4);
        CHECK("original unchanged", a.GetCount() == 2);
    }
    {
        Queue<int> a, b;
        Queue<int> c = a.Concat(b);
        CHECK("empty concat empty", c.IsEmpty());
    }
}

void test_Queue_SingleElement() {
    SUITE("Queue: single element boundary");
    {
        Queue<int> q;
        q.Enqueue(99);
        CHECK("peekfront == peekback for single", q.PeekFront() == q.PeekBack());
        CHECK("dequeue single", q.Dequeue() == 99);
        CHECK("empty after dequeue single", q.IsEmpty());
    }
}

// Collection operators
void test_CollectionOperators_Equality() {
    SUITE("CollectionOperators: == and !=");
    {
        Stack<int> a, b;
        a.Push(1); a.Push(2);
        b.Push(1); b.Push(2);
        CHECK("equal stacks", a == b);
        CHECK("not not-equal", !(a != b));
    }
    {
        Stack<int> a, b;
        a.Push(1); a.Push(2);
        b.Push(1); b.Push(3);
        CHECK("different stacks not equal", !(a == b));
        CHECK("different stacks !=", a != b);
    }
    {
        Stack<int> a, b;
        CHECK("two empty stacks equal", a == b);
    }
    {
        Stack<int> a, b;
        a.Push(1);
        CHECK("different sizes not equal", !(a == b));
    }
}

// Hanoi
void test_Hanoi_OneDisk() {
    SUITE("Hanoi: 1 disk");
    {
        Stack<int> src;
        src.Push(1);
        Hanoi<int> h(src, "A", "B", "C");
        h.Solve();
        CHECK("1 disk: 1 move", h.GetMoves().GetLength() == 1);
        CHECK("1 disk: pegC has disk", h.GetPegC().GetCount() == 1);
        CHECK("1 disk: pegA empty", h.GetPegA().IsEmpty());
        CHECK("1 disk: move from A", h.GetMoves().Get(0).from == "A");
        CHECK("1 disk: move to C", h.GetMoves().Get(0).to == "C");
    }
}

void test_Hanoi_ThreeDisks() {
    SUITE("Hanoi: 3 disks");
    {
        Stack<int> src;
        src.Push(3); src.Push(2); src.Push(1);
        Hanoi<int> h(src, "A", "B", "C");
        h.Solve();
        CHECK("3 disks: 7 moves", h.GetMoves().GetLength() == 7);
        CHECK("3 disks: pegC has 3", h.GetPegC().GetCount() == 3);
        CHECK("3 disks: pegA empty", h.GetPegA().IsEmpty());
        CHECK("3 disks: pegB empty", h.GetPegB().IsEmpty());
        // проверяем порядок на pegC: снизу 3, сверху 1
        CHECK("3 disks: bottom of C is 3", h.GetPegC().Get(0) == 3);
        CHECK("3 disks: top of C is 1", h.GetPegC().Get(2) == 1);
    }
}

void test_Hanoi_TwoDisks() {
    SUITE("Hanoi: 2 disks");
    {
        Stack<int> src;
        src.Push(2); src.Push(1);
        Hanoi<int> h(src, "A", "B", "C");
        h.Solve();
        CHECK("2 disks: 3 moves", h.GetMoves().GetLength() == 3);
        CHECK("2 disks: pegC has 2", h.GetPegC().GetCount() == 2);
    }
}

void test_Hanoi_MaxDisk() {
    SUITE("Hanoi: GetMaxDisk");
    {
        Stack<int> src;
        src.Push(4); src.Push(3); src.Push(2); src.Push(1);
        Hanoi<int> h(src, "A", "B", "C");
        CHECK("maxDisk is 4", h.GetMaxDisk() == 4);
    }
}

void test_Hanoi_SolveIdempotent() {
    SUITE("Hanoi: Solve twice resets moves");
    {
        Stack<int> src;
        src.Push(2); src.Push(1);
        Hanoi<int> h(src, "A", "B", "C");
        h.Solve();
        int first = h.GetMoves().GetLength();
        // повторный вызов — но стек уже пуст, поэтому 0 ходов
        h.Solve();
        CHECK("second solve on empty gives 0 moves", h.GetMoves().GetLength() == 0);
        CHECK("first solve gave 3 moves", first == 3);
    }
}

// Rectangular Matrix
void test_RectangularMatrix_GetSet() {
    SUITE("RectangularMatrix: Get / Set");
    {
        RectangularMatrix<int> m(2, 3);
        m.Set(0, 0, 1); m.Set(0, 1, 2); m.Set(0, 2, 3);
        m.Set(1, 0, 4); m.Set(1, 1, 5); m.Set(1, 2, 6);
        CHECK("Get(0,0)", m.Get(0, 0) == 1);
        CHECK("Get(1,2)", m.Get(1, 2) == 6);
        CHECK("Rows", m.Rows() == 2);
        CHECK("Cols", m.Cols() == 3);
    }
    {
        RectangularMatrix<int> m(2, 2);
        CHECK_THROWS("Get out of range row", m.Get(2, 0));
        CHECK_THROWS("Get out of range col", m.Get(0, 2));
        CHECK_THROWS("Get negative row", m.Get(-1, 0));
    }
    {
        CHECK_THROWS("zero rows throws", RectangularMatrix<int>(0, 2));
        CHECK_THROWS("zero cols throws", RectangularMatrix<int>(2, 0));
    }
}

void test_RectangularMatrix_Add() {
    SUITE("RectangularMatrix: Add");
    {
        int d1[] = { 1,2,3,4 }; int d2[] = { 5,6,7,8 };
        RectangularMatrix<int> a(2, 2, d1), b(2, 2, d2);
        auto* c = a.Add(b);
        CHECK("add (0,0)", c->Get(0, 0) == 6);
        CHECK("add (0,1)", c->Get(0, 1) == 8);
        CHECK("add (1,0)", c->Get(1, 0) == 10);
        CHECK("add (1,1)", c->Get(1, 1) == 12);
        delete c;
    }
    {
        RectangularMatrix<int> a(2, 2), b(2, 3);
        CHECK_THROWS("add different sizes throws", a.Add(b));
    }
}

void test_RectangularMatrix_MultiplyByScalar() {
    SUITE("RectangularMatrix: MultiplyByScalar");
    {
        int d[] = { 1,2,3,4 };
        RectangularMatrix<int> m(2, 2, d);
        auto* r = m.MultiplyByScalar(3);
        CHECK("scalar (0,0)", r->Get(0, 0) == 3);
        CHECK("scalar (1,1)", r->Get(1, 1) == 12);
        delete r;
    }
    {
        int d[] = { 1,2,3,4 };
        RectangularMatrix<int> m(2, 2, d);
        auto* r = m.MultiplyByScalar(0);
        CHECK("multiply by zero", r->Get(0, 0) == 0 && r->Get(1, 1) == 0);
        delete r;
    }
}

void test_RectangularMatrix_MultiplyByMatrix() {
    SUITE("RectangularMatrix: MultiplyByMatrix");
    {
        int d1[] = { 1,2,3,4 };
        int d2[] = { 5,6,7,8 };
        RectangularMatrix<int> a(2, 2, d1), b(2, 2, d2);
        auto* c = a.MultiplyByMatrix(b);
        CHECK("mul (0,0)", c->Get(0, 0) == 19);
        CHECK("mul (0,1)", c->Get(0, 1) == 22);
        CHECK("mul (1,0)", c->Get(1, 0) == 43);
        CHECK("mul (1,1)", c->Get(1, 1) == 50);
        delete c;
    }
    {
        RectangularMatrix<int> a(2, 3), b(2, 2);
        CHECK_THROWS("incompatible multiply throws", a.MultiplyByMatrix(b));
    }
}

void test_RectangularMatrix_Transpose() {
    SUITE("RectangularMatrix: Transpose");
    {
        int d[] = { 1,2,3,4,5,6 };
        RectangularMatrix<int> m(2, 3, d);
        auto* t = m.Transpose();
        CHECK("transpose rows", t->Rows() == 3);
        CHECK("transpose cols", t->Cols() == 2);
        CHECK("transpose (0,1)", t->Get(0, 1) == 4);
        CHECK("transpose (2,0)", t->Get(2, 0) == 3);
        delete t;
    }
}

void test_RectangularMatrix_FrobeniusNorm() {
    SUITE("RectangularMatrix: FrobeniusNorm");
    {
        int d[] = { 3,4 };
        RectangularMatrix<int> m(1, 2, d);
        CHECK("frobenius norm 3-4-5", std::abs(m.FrobeniusNorm() - 5.0) < 1e-9);
    }
    {
        int d[] = { 0,0,0,0 };
        RectangularMatrix<int> m(2, 2, d);
        CHECK("frobenius norm zero matrix", m.FrobeniusNorm() == 0.0);
    }
}

void test_RectangularMatrix_RowColOps() {
    SUITE("RectangularMatrix: row/col operations");
    {
        int d[] = { 1,2,3,4 };
        RectangularMatrix<int> m(2, 2, d);
        m.SwapRows(0, 1);
        CHECK("swaprows (0,0)", m.Get(0, 0) == 3);
        CHECK("swaprows (1,0)", m.Get(1, 0) == 1);
    }
    {
        int d[] = { 1,2,3,4 };
        RectangularMatrix<int> m(2, 2, d);
        m.SwapCols(0, 1);
        CHECK("swapcols (0,0)", m.Get(0, 0) == 2);
        CHECK("swapcols (0,1)", m.Get(0, 1) == 1);
    }
    {
        int d[] = { 1,2,3,4 };
        RectangularMatrix<int> m(2, 2, d);
        m.ScaleRow(0, 2);
        CHECK("scalerow (0,0)", m.Get(0, 0) == 2);
        CHECK("scalerow (0,1)", m.Get(0, 1) == 4);
    }
    {
        int d[] = { 1,2,3,4 };
        RectangularMatrix<int> m(2, 2, d);
        m.ScaleCol(1, 3);
        CHECK("scalecol (0,1)", m.Get(0, 1) == 6);
        CHECK("scalecol (1,1)", m.Get(1, 1) == 12);
    }
    {
        int d[] = { 1,2,3,4 };
        RectangularMatrix<int> m(2, 2, d);
        m.AddScaledRow(1, 0, 2); // row1 += 2*row0
        CHECK("addscaledrow (1,0)", m.Get(1, 0) == 5); // 3+2*1
        CHECK("addscaledrow (1,1)", m.Get(1, 1) == 8); // 4+2*2
    }
    {
        RectangularMatrix<int> m(2, 2);
        CHECK_THROWS("swaprows out of range", m.SwapRows(0, 2));
        CHECK_THROWS("swapcols out of range", m.SwapCols(0, 2));
    }
}

void test_RectangularMatrix_Operators() {
    SUITE("RectangularMatrix: Operators");
    {
        int d1[] = { 1,2,3,4 };
        int d2[] = { 5,6,7,8 };
        RectangularMatrix<int> a(2, 2, d1), b(2, 2, d2);
        auto* c = a + b;
        CHECK("operator+ (0,0)", c->Get(0, 0) == 6);
        delete c;
    }
    {
        int d[] = { 1,2,3,4 };
        RectangularMatrix<int> m(2, 2, d);
        auto* r = m * 2;
        CHECK("operator* scalar", r->Get(0, 0) == 2);
        delete r;
    }
    {
        int d1[] = { 1,2,3,4 };
        int d2[] = { 1,2,3,4 };
        RectangularMatrix<int> a(2, 2, d1), b(2, 2, d2);
        CHECK("operator==", a == b);
    }
}

// Square Matrix
void test_SquareMatrix_Trace() {
    SUITE("SquareMatrix: Trace");
    {
        int d[] = { 1,2,3,4 };
        SquareMatrix<int> m(2, d);
        CHECK("trace 2x2", m.Trace() == 5);
    }
    {
        int d[] = { 5 };
        SquareMatrix<int> m(1, d);
        CHECK("trace 1x1", m.Trace() == 5);
    }
}

void test_SquareMatrix_Determinant() {
    SUITE("SquareMatrix: Determinant");
    {
        int d[] = { 1,2,3,4 };
        SquareMatrix<int> m(2, d);
        CHECK("det 2x2", m.Determinant() == -2);
    }
    {
        int d[] = { 1,0,0, 0,1,0, 0,0,1 };
        SquareMatrix<int> m(3, d);
        CHECK("det identity 3x3", m.Determinant() == 1);
    }
    {
        int d[] = { 1,2, 2,4 }; // singular
        SquareMatrix<int> m(2, d);
        CHECK("det singular is 0", m.Determinant() == 0);
    }
    {
        int d[] = { 5 };
        SquareMatrix<int> m(1, d);
        CHECK("det 1x1", m.Determinant() == 5);
    }
}

// Diagonal Matrix
void test_DiagonalMatrix_GetSet() {
    SUITE("DiagonalMatrix: Get / Set");
    {
        int d[] = { 1,2,3 };
        DiagonalMatrix<int> m(3, d);
        CHECK("diag Get(0,0)", m.Get(0, 0) == 1);
        CHECK("diag Get(1,1)", m.Get(1, 1) == 2);
        CHECK("diag Get(2,2)", m.Get(2, 2) == 3);
        CHECK("off-diag is zero", m.Get(0, 1) == 0);
        CHECK("off-diag is zero 2", m.Get(1, 0) == 0);
    }
    {
        DiagonalMatrix<int> m(2);
        CHECK_THROWS("set non-zero off-diag throws", m.Set(0, 1, 5));
        CHECK("set zero off-diag ok", (m.Set(0, 1, 0), true));
    }
    {
        CHECK_THROWS("zero dimension throws", DiagonalMatrix<int>(0));
    }
}

void test_DiagonalMatrix_Add() {
    SUITE("DiagonalMatrix: Add");
    {
        int d1[] = { 1,2,3 }, d2[] = { 4,5,6 };
        DiagonalMatrix<int> a(3, d1), b(3, d2);
        auto* c = a.Add(b);
        CHECK("diag add (0,0)", c->Get(0, 0) == 5);
        CHECK("diag add (1,1)", c->Get(1, 1) == 7);
        CHECK("diag add (2,2)", c->Get(2, 2) == 9);
        delete c;
    }
}

void test_DiagonalMatrix_Inverse() {
    SUITE("DiagonalMatrix: Inverse");
    {
        double d[] = { 2.0, 4.0 };
        DiagonalMatrix<double> m(2, d);
        auto* inv = m.Inverse();
        CHECK("inverse (0,0)", std::abs(inv->Get(0, 0) - 0.5) < 1e-9);
        CHECK("inverse (1,1)", std::abs(inv->Get(1, 1) - 0.25) < 1e-9);
        delete inv;
    }
    {
        int d[] = { 0, 1 };
        DiagonalMatrix<int> m(2, d);
        CHECK_THROWS("inverse singular throws", m.Inverse());
    }
}

void test_DiagonalMatrix_Transpose() {
    SUITE("DiagonalMatrix: Transpose is copy");
    {
        int d[] = { 1,2,3 };
        DiagonalMatrix<int> m(3, d);
        auto* t = m.Transpose();
        CHECK("transpose diag (0,0)", t->Get(0, 0) == 1);
        CHECK("transpose diag (2,2)", t->Get(2, 2) == 3);
        CHECK("transpose off-diag zero", t->Get(0, 1) == 0);
        delete t;
    }
}

void test_DiagonalMatrix_MultiplyDiagonal() {
    SUITE("DiagonalMatrix: MultiplyDiagonal");
    {
        int d1[] = { 2,3 }, d2[] = { 4,5 };
        DiagonalMatrix<int> a(2, d1), b(2, d2);
        auto* c = a.MultiplyDiagonal(b);
        CHECK("multidiag (0,0)", c->Get(0, 0) == 8);
        CHECK("multidiag (1,1)", c->Get(1, 1) == 15);
        delete c;
    }
}

// Sparse Matrix
void test_SparseMatrix_GetSet() {
    SUITE("SparseMatrix: Get / Set");
    {
        SparseMatrix<int> m(3, 3);
        CHECK("default is zero", m.Get(0, 0) == 0);
        m.Set(0, 0, 5); m.Set(1, 2, 7);
        CHECK("set and get (0,0)", m.Get(0, 0) == 5);
        CHECK("set and get (1,2)", m.Get(1, 2) == 7);
        CHECK("unset is zero", m.Get(2, 2) == 0);
        CHECK("nonzero count", m.NonZeroCount() == 2);
    }
    {
        SparseMatrix<int> m(2, 2);
        m.Set(0, 0, 5);
        m.Set(0, 0, 0); // обнуляем
        CHECK("set to zero removes element", m.NonZeroCount() == 0);
        CHECK("get after zero is 0", m.Get(0, 0) == 0);
    }
    {
        CHECK_THROWS("zero rows throws", SparseMatrix<int>(0, 2));
        CHECK_THROWS("zero cols throws", SparseMatrix<int>(2, 0));
        SparseMatrix<int> m(2, 2);
        CHECK_THROWS("get out of range", m.Get(2, 0));
    }
}

void test_SparseMatrix_Add() {
    SUITE("SparseMatrix: Add");
    {
        SparseMatrix<int> a(2, 2), b(2, 2);
        a.Set(0, 0, 1); a.Set(1, 1, 2);
        b.Set(0, 0, 3); b.Set(0, 1, 4);
        auto* c = a.Add(b);
        CHECK("sparse add (0,0)", c->Get(0, 0) == 4);
        CHECK("sparse add (0,1)", c->Get(0, 1) == 4);
        CHECK("sparse add (1,1)", c->Get(1, 1) == 2);
        delete c;
    }
    {
        SparseMatrix<int> a(2, 2), b(3, 3);
        CHECK_THROWS("sparse add different sizes throws", a.Add(b));
    }
}

void test_SparseMatrix_MultiplyByScalar() {
    SUITE("SparseMatrix: MultiplyByScalar");
    {
        SparseMatrix<int> m(2, 2);
        m.Set(0, 0, 3); m.Set(1, 1, 4);
        auto* r = m.MultiplyByScalar(2);
        CHECK("sparse scalar (0,0)", r->Get(0, 0) == 6);
        CHECK("sparse scalar (1,1)", r->Get(1, 1) == 8);
        CHECK("sparse scalar zero stays zero", r->Get(0, 1) == 0);
        delete r;
    }
}

void test_SparseMatrix_Transpose() {
    SUITE("SparseMatrix: Transpose");
    {
        SparseMatrix<int> m(2, 3);
        m.Set(0, 2, 7); m.Set(1, 0, 3);
        auto* t = m.Transpose();
        CHECK("sparse transpose rows", t->Rows() == 3);
        CHECK("sparse transpose cols", t->Cols() == 2);
        CHECK("sparse transpose (2,0)", t->Get(2, 0) == 7);
        CHECK("sparse transpose (0,1)", t->Get(0, 1) == 3);
        delete t;
    }
}

void test_SparseMatrix_FrobeniusNorm() {
    SUITE("SparseMatrix: FrobeniusNorm");
    {
        SparseMatrix<int> m(2, 2);
        m.Set(0, 0, 3); m.Set(0, 1, 4);
        CHECK("sparse frobenius", std::abs(m.FrobeniusNorm() - 5.0) < 1e-9);
    }
    {
        SparseMatrix<int> m(3, 3);
        CHECK("sparse frobenius zero matrix", m.FrobeniusNorm() == 0.0);
    }
}

void test_SparseMatrix_RowColOps() {
    SUITE("SparseMatrix: row/col operations");
    {
        SparseMatrix<int> m(2, 2);
        m.Set(0, 0, 1); m.Set(1, 1, 2);
        m.SwapRows(0, 1);
        CHECK("sparse swaprows (0,0) now 0", m.Get(0, 0) == 0);
        CHECK("sparse swaprows (0,1) now 2", m.Get(0, 1) == 2);
        CHECK("sparse swaprows (1,1) now 0", m.Get(1, 1) == 0);
        CHECK("sparse swaprows (1,0) now 1", m.Get(1, 0) == 1);
    }
    {
        SparseMatrix<int> m(2, 2);
        m.Set(0, 0, 5);
        m.ScaleRow(0, 0);
        CHECK("sparse scalerow by zero removes", m.NonZeroCount() == 0);
    }
    {
        SparseMatrix<int> m(2, 2);
        CHECK_THROWS("sparse swaprows out of range", m.SwapRows(0, 2));
        CHECK_THROWS("sparse swapcols out of range", m.SwapCols(0, 2));
    }
}

// Exception tests
void test_ExceptionTests() {
    SUITE("Exception tests");

    // Stack
    {
        Stack<int> s;

        CHECK_THROWS("Stack Pop from empty throws", s.Pop());
        CHECK_THROWS("Stack Peek from empty throws", s.Peek());
        CHECK_THROWS("Stack Get from empty throws", s.Get(0));
    }

    {
        Stack<int> s;
        s.Push(1);

        CHECK_THROWS("Stack Get negative index throws", s.Get(-1));
        CHECK_THROWS("Stack Get high index throws", s.Get(1));
    }

    // Queue
    {
        Queue<int> q;

        CHECK_THROWS("Queue Dequeue from empty throws", q.Dequeue());
        CHECK_THROWS("Queue PeekFront from empty throws", q.PeekFront());
        CHECK_THROWS("Queue PeekBack from empty throws", q.PeekBack());
        CHECK_THROWS("Queue Get from empty throws", q.Get(0));
    }

    {
        Queue<int> q;
        q.Enqueue(10);

        CHECK_THROWS("Queue Get negative index throws", q.Get(-1));
        CHECK_THROWS("Queue Get high index throws", q.Get(1));
    }

    // RectangularMatrix
    {
        CHECK_THROWS("RectangularMatrix zero rows throws", RectangularMatrix<int>(0, 2));
        CHECK_THROWS("RectangularMatrix zero cols throws", RectangularMatrix<int>(2, 0));
        CHECK_THROWS("RectangularMatrix negative rows throws", RectangularMatrix<int>(-1, 2));
        CHECK_THROWS("RectangularMatrix negative cols throws", RectangularMatrix<int>(2, -1));
        CHECK_THROWS("RectangularMatrix null data throws", RectangularMatrix<int>(2, 2, nullptr));
    }

    {
        RectangularMatrix<int> m(2, 3);

        CHECK_THROWS("RectangularMatrix Get negative row throws", m.Get(-1, 0));
        CHECK_THROWS("RectangularMatrix Get negative col throws", m.Get(0, -1));
        CHECK_THROWS("RectangularMatrix Get bad row throws", m.Get(2, 0));
        CHECK_THROWS("RectangularMatrix Get bad col throws", m.Get(0, 3));

        CHECK_THROWS("RectangularMatrix Set negative row throws", m.Set(-1, 0, 5));
        CHECK_THROWS("RectangularMatrix Set negative col throws", m.Set(0, -1, 5));
        CHECK_THROWS("RectangularMatrix Set bad row throws", m.Set(2, 0, 5));
        CHECK_THROWS("RectangularMatrix Set bad col throws", m.Set(0, 3, 5));

        CHECK_THROWS("RectangularMatrix SwapRows bad index throws", m.SwapRows(0, 2));
        CHECK_THROWS("RectangularMatrix SwapCols bad index throws", m.SwapCols(0, 3));

        CHECK_THROWS("RectangularMatrix ScaleRow bad index throws", m.ScaleRow(2, 3));
        CHECK_THROWS("RectangularMatrix ScaleCol bad index throws", m.ScaleCol(3, 3));

        CHECK_THROWS("RectangularMatrix AddScaledRow bad target throws", m.AddScaledRow(2, 0, 3));
        CHECK_THROWS("RectangularMatrix AddScaledRow bad source throws", m.AddScaledRow(0, 2, 3));

        CHECK_THROWS("RectangularMatrix AddScaledCol bad target throws", m.AddScaledCol(3, 0, 3));
        CHECK_THROWS("RectangularMatrix AddScaledCol bad source throws", m.AddScaledCol(0, 3, 3));
    }

    {
        RectangularMatrix<int> a(2, 3);
        RectangularMatrix<int> b(3, 2);
        RectangularMatrix<int> c(4, 4);

        CHECK_THROWS("RectangularMatrix Add incompatible sizes throws", a.Add(b));
        CHECK_THROWS("RectangularMatrix Multiply incompatible sizes throws", a.MultiplyByMatrix(c));
    }

    // SquareMatrix
    {
        CHECK_THROWS("SquareMatrix zero size throws", SquareMatrix<int>(0));
        CHECK_THROWS("SquareMatrix negative size throws", SquareMatrix<int>(-1));
        CHECK_THROWS("SquareMatrix null data throws", SquareMatrix<int>(2, nullptr));
    }

    // DiagonalMatrix
    {
        CHECK_THROWS("DiagonalMatrix zero size throws", DiagonalMatrix<int>(0));
        CHECK_THROWS("DiagonalMatrix negative size throws", DiagonalMatrix<int>(-1));
        CHECK_THROWS("DiagonalMatrix null data throws", DiagonalMatrix<int>(3, nullptr));
    }

    {
        DiagonalMatrix<int> m(3);

        CHECK_THROWS("DiagonalMatrix Get negative row throws", m.Get(-1, 0));
        CHECK_THROWS("DiagonalMatrix Get negative col throws", m.Get(0, -1));
        CHECK_THROWS("DiagonalMatrix Get bad row throws", m.Get(3, 0));
        CHECK_THROWS("DiagonalMatrix Get bad col throws", m.Get(0, 3));

        CHECK_THROWS("DiagonalMatrix Set negative row throws", m.Set(-1, 0, 5));
        CHECK_THROWS("DiagonalMatrix Set negative col throws", m.Set(0, -1, 5));
        CHECK_THROWS("DiagonalMatrix Set bad row throws", m.Set(3, 0, 5));
        CHECK_THROWS("DiagonalMatrix Set bad col throws", m.Set(0, 3, 5));

        CHECK_THROWS("DiagonalMatrix Set non-zero off diagonal throws", m.Set(0, 1, 5));

        CHECK_THROWS("DiagonalMatrix GetDiag negative index throws", m.GetDiag(-1));
        CHECK_THROWS("DiagonalMatrix GetDiag high index throws", m.GetDiag(3));

        CHECK_THROWS("DiagonalMatrix SetDiag negative index throws", m.SetDiag(-1, 5));
        CHECK_THROWS("DiagonalMatrix SetDiag high index throws", m.SetDiag(3, 5));

        CHECK_THROWS("DiagonalMatrix SwapRows bad index throws", m.SwapRows(0, 3));
        CHECK_THROWS("DiagonalMatrix SwapCols bad index throws", m.SwapCols(0, 3));

        CHECK_THROWS("DiagonalMatrix ScaleRow bad index throws", m.ScaleRow(3, 2));
        CHECK_THROWS("DiagonalMatrix ScaleCol bad index throws", m.ScaleCol(3, 2));

        CHECK_THROWS("DiagonalMatrix AddScaledRow bad target throws", m.AddScaledRow(3, 0, 2));
        CHECK_THROWS("DiagonalMatrix AddScaledRow bad source throws", m.AddScaledRow(0, 3, 2));

        CHECK_THROWS("DiagonalMatrix AddScaledCol bad target throws", m.AddScaledCol(3, 0, 2));
        CHECK_THROWS("DiagonalMatrix AddScaledCol bad source throws", m.AddScaledCol(0, 3, 2));
    }

    {
        DiagonalMatrix<int> a(2);
        DiagonalMatrix<int> b(3);

        CHECK_THROWS("DiagonalMatrix Add incompatible sizes throws", a.Add(b));
        CHECK_THROWS("DiagonalMatrix MultiplyDiagonal incompatible sizes throws", a.MultiplyDiagonal(b));
    }

    {
        int d[] = { 0, 1 };
        DiagonalMatrix<int> m(2, d);

        CHECK_THROWS("DiagonalMatrix inverse with zero diagonal throws", m.Inverse());
    }

    // SparseMatrix
    {
        CHECK_THROWS("SparseMatrix zero rows throws", SparseMatrix<int>(0, 2));
        CHECK_THROWS("SparseMatrix zero cols throws", SparseMatrix<int>(2, 0));
        CHECK_THROWS("SparseMatrix negative rows throws", SparseMatrix<int>(-1, 2));
        CHECK_THROWS("SparseMatrix negative cols throws", SparseMatrix<int>(2, -1));
    }

    {
        SparseMatrix<int> m(2, 3);

        CHECK_THROWS("SparseMatrix Get negative row throws", m.Get(-1, 0));
        CHECK_THROWS("SparseMatrix Get negative col throws", m.Get(0, -1));
        CHECK_THROWS("SparseMatrix Get bad row throws", m.Get(2, 0));
        CHECK_THROWS("SparseMatrix Get bad col throws", m.Get(0, 3));

        CHECK_THROWS("SparseMatrix Set negative row throws", m.Set(-1, 0, 5));
        CHECK_THROWS("SparseMatrix Set negative col throws", m.Set(0, -1, 5));
        CHECK_THROWS("SparseMatrix Set bad row throws", m.Set(2, 0, 5));
        CHECK_THROWS("SparseMatrix Set bad col throws", m.Set(0, 3, 5));

        CHECK_THROWS("SparseMatrix SwapRows bad index throws", m.SwapRows(0, 2));
        CHECK_THROWS("SparseMatrix SwapCols bad index throws", m.SwapCols(0, 3));

        CHECK_THROWS("SparseMatrix ScaleRow bad index throws", m.ScaleRow(2, 3));
        CHECK_THROWS("SparseMatrix ScaleCol bad index throws", m.ScaleCol(3, 3));

        CHECK_THROWS("SparseMatrix AddScaledRow bad target throws", m.AddScaledRow(2, 0, 3));
        CHECK_THROWS("SparseMatrix AddScaledRow bad source throws", m.AddScaledRow(0, 2, 3));

        CHECK_THROWS("SparseMatrix AddScaledCol bad target throws", m.AddScaledCol(3, 0, 3));
        CHECK_THROWS("SparseMatrix AddScaledCol bad source throws", m.AddScaledCol(0, 3, 3));
    }

    {
        SparseMatrix<int> a(2, 3);
        SparseMatrix<int> b(3, 2);
        SparseMatrix<int> c(4, 4);

        CHECK_THROWS("SparseMatrix Add incompatible sizes throws", a.Add(b));
        CHECK_THROWS("SparseMatrix Multiply incompatible sizes throws", a.MultiplyByMatrix(c));
    }
}

// Natural number algorithms
void test_NaturalNumberAlgorithms() {
    SUITE("NaturalNumberAlgorithms: Range");

    {
        Sequence<int>* seq = Range(1, 5);

        CHECK("Range length", seq->GetLength() == 5);
        CHECK("Range first", seq->Get(0) == 1);
        CHECK("Range middle", seq->Get(2) == 3);
        CHECK("Range last", seq->Get(4) == 5);

        delete seq;
    }

    {
        Sequence<int>* seq = Range(3, 3);

        CHECK("Range single length", seq->GetLength() == 1);
        CHECK("Range single value", seq->Get(0) == 3);

        delete seq;
    }

    {
        Sequence<int>* seq = Range(-2, 2);

        CHECK("Range negative length", seq->GetLength() == 5);
        CHECK("Range negative first", seq->Get(0) == -2);
        CHECK("Range negative last", seq->Get(4) == 2);

        delete seq;
    }

    CHECK_THROWS("Range low greater than high throws", Range(5, 1));


    SUITE("NaturalNumberAlgorithms: IsPrime");

    CHECK("IsPrime 0 false", !IsPrime(0));
    CHECK("IsPrime 1 false", !IsPrime(1));
    CHECK("IsPrime 2 true", IsPrime(2));
    CHECK("IsPrime 3 true", IsPrime(3));
    CHECK("IsPrime 4 false", !IsPrime(4));
    CHECK("IsPrime 17 true", IsPrime(17));
    CHECK("IsPrime 25 false", !IsPrime(25));
    CHECK("IsPrime negative false", !IsPrime(-7));


    SUITE("NaturalNumberAlgorithms: GetDivisors");

    {
        Sequence<int>* seq = GetDivisors(12);

        CHECK("GetDivisors length", seq->GetLength() == 6);
        CHECK("GetDivisors 1", seq->Get(0) == 1);
        CHECK("GetDivisors 2", seq->Get(1) == 2);
        CHECK("GetDivisors 3", seq->Get(2) == 3);
        CHECK("GetDivisors 4", seq->Get(3) == 4);
        CHECK("GetDivisors 6", seq->Get(4) == 6);
        CHECK("GetDivisors 12", seq->Get(5) == 12);

        delete seq;
    }

    {
        Sequence<int>* seq = GetDivisors(1);

        CHECK("GetDivisors one length", seq->GetLength() == 1);
        CHECK("GetDivisors one value", seq->Get(0) == 1);

        delete seq;
    }

    CHECK_THROWS("GetDivisors zero throws", GetDivisors(0));
    CHECK_THROWS("GetDivisors negative throws", GetDivisors(-10));


    SUITE("NaturalNumberAlgorithms: GetPrimes");

    {
        Sequence<int>* seq = GetPrimes(1, 10);

        CHECK("GetPrimes length", seq->GetLength() == 4);
        CHECK("GetPrimes 2", seq->Get(0) == 2);
        CHECK("GetPrimes 3", seq->Get(1) == 3);
        CHECK("GetPrimes 5", seq->Get(2) == 5);
        CHECK("GetPrimes 7", seq->Get(3) == 7);

        delete seq;
    }

    {
        Sequence<int>* seq = GetPrimes(14, 16);

        CHECK("GetPrimes empty range length", seq->GetLength() == 0);

        delete seq;
    }

    CHECK_THROWS("GetPrimes invalid range throws", GetPrimes(10, 1));


    SUITE("NaturalNumberAlgorithms: GetPrimeFactors");

    {
        Sequence<int>* seq = GetPrimeFactors(60);

        CHECK("GetPrimeFactors 60 length", seq->GetLength() == 4);
        CHECK("GetPrimeFactors 60 factor 0", seq->Get(0) == 2);
        CHECK("GetPrimeFactors 60 factor 1", seq->Get(1) == 2);
        CHECK("GetPrimeFactors 60 factor 2", seq->Get(2) == 3);
        CHECK("GetPrimeFactors 60 factor 3", seq->Get(3) == 5);

        delete seq;
    }

    {
        Sequence<int>* seq = GetPrimeFactors(13);

        CHECK("GetPrimeFactors prime length", seq->GetLength() == 1);
        CHECK("GetPrimeFactors prime value", seq->Get(0) == 13);

        delete seq;
    }

    {
        Sequence<int>* seq = GetPrimeFactors(64);

        CHECK("GetPrimeFactors power length", seq->GetLength() == 6);
        for (int i = 0; i < seq->GetLength(); ++i) {
            CHECK("GetPrimeFactors power all twos", seq->Get(i) == 2);
        }

        delete seq;
    }

    CHECK_THROWS("GetPrimeFactors one throws", GetPrimeFactors(1));
    CHECK_THROWS("GetPrimeFactors negative throws", GetPrimeFactors(-20));


    SUITE("NaturalNumberAlgorithms: SieveOfEratosthenes");

    {
        Sequence<int>* seq = SieveOfEratosthenes(1, 20);

        CHECK("Sieve length", seq->GetLength() == 8);
        CHECK("Sieve 2", seq->Get(0) == 2);
        CHECK("Sieve 3", seq->Get(1) == 3);
        CHECK("Sieve 5", seq->Get(2) == 5);
        CHECK("Sieve 7", seq->Get(3) == 7);
        CHECK("Sieve 11", seq->Get(4) == 11);
        CHECK("Sieve 13", seq->Get(5) == 13);
        CHECK("Sieve 17", seq->Get(6) == 17);
        CHECK("Sieve 19", seq->Get(7) == 19);

        delete seq;
    }

    {
        Sequence<int>* seq = SieveOfEratosthenes(14, 16);

        CHECK("Sieve empty range length", seq->GetLength() == 0);

        delete seq;
    }

    {
        Sequence<int>* seq = SieveOfEratosthenes(-5, 3);

        CHECK("Sieve negative low length", seq->GetLength() == 2);
        CHECK("Sieve negative low first", seq->Get(0) == 2);
        CHECK("Sieve negative low second", seq->Get(1) == 3);

        delete seq;
    }

    CHECK_THROWS("Sieve invalid range throws", SieveOfEratosthenes(10, 1));
}

void test_RecurrenceAlgorithms() {
    SUITE("RecurrenceAlgorithms: Factorials");

    {
        Sequence<int>* seq = Factorials(6);

        CHECK("Factorials length", seq->GetLength() == 6);
        CHECK("0!", seq->Get(0) == 1);
        CHECK("1!", seq->Get(1) == 1);
        CHECK("2!", seq->Get(2) == 2);
        CHECK("3!", seq->Get(3) == 6);
        CHECK("4!", seq->Get(4) == 24);
        CHECK("5!", seq->Get(5) == 120);

        delete seq;
    }

    CHECK_THROWS("Factorials negative count throws", Factorials(-1));


    SUITE("RecurrenceAlgorithms: Fibonacci");

    {
        Sequence<int>* seq = Fibonacci(7);

        CHECK("Fibonacci length", seq->GetLength() == 7);
        CHECK("fib 0", seq->Get(0) == 0);
        CHECK("fib 1", seq->Get(1) == 1);
        CHECK("fib 2", seq->Get(2) == 1);
        CHECK("fib 3", seq->Get(3) == 2);
        CHECK("fib 4", seq->Get(4) == 3);
        CHECK("fib 5", seq->Get(5) == 5);
        CHECK("fib 6", seq->Get(6) == 8);

        delete seq;
    }

    CHECK_THROWS("Fibonacci negative count throws", Fibonacci(-1));


    SUITE("RecurrenceAlgorithms: LinearRecurrence");

    {
        int cData[] = { 1, 1 };
        int initData[] = { 0, 1 };

        MutableArraySequence<int> coefficients(cData, 2);
        MutableArraySequence<int> initialValues(initData, 2);

        Sequence<int>* seq = LinearRecurrence(&coefficients, &initialValues, 7);

        CHECK("LinearRecurrence Fibonacci length", seq->GetLength() == 7);
        CHECK("LinearRecurrence fib 0", seq->Get(0) == 0);
        CHECK("LinearRecurrence fib 1", seq->Get(1) == 1);
        CHECK("LinearRecurrence fib 2", seq->Get(2) == 1);
        CHECK("LinearRecurrence fib 3", seq->Get(3) == 2);
        CHECK("LinearRecurrence fib 4", seq->Get(4) == 3);
        CHECK("LinearRecurrence fib 5", seq->Get(5) == 5);
        CHECK("LinearRecurrence fib 6", seq->Get(6) == 8);

        delete seq;
    }

    {
        int cData[] = { 2 };
        int initData[] = { 1 };

        MutableArraySequence<int> coefficients(cData, 1);
        MutableArraySequence<int> initialValues(initData, 1);

        Sequence<int>* seq = LinearRecurrence(&coefficients, &initialValues, 5);

        CHECK("LinearRecurrence order 1 length", seq->GetLength() == 5);
        CHECK("LinearRecurrence order 1 x0", seq->Get(0) == 1);
        CHECK("LinearRecurrence order 1 x1", seq->Get(1) == 2);
        CHECK("LinearRecurrence order 1 x2", seq->Get(2) == 4);
        CHECK("LinearRecurrence order 1 x3", seq->Get(3) == 8);
        CHECK("LinearRecurrence order 1 x4", seq->Get(4) == 16);

        delete seq;
    }

    CHECK_THROWS("LinearRecurrence null coefficients throws",
        LinearRecurrence(nullptr, nullptr, 5));
}

void run_all_tests() {
    // Stack
    test_Stack_PushPeekPop();
    test_Stack_EmptyThrows();
    test_Stack_Get();
    test_Stack_Concat();
    test_Stack_SingleElement();

    // Queue
    test_Queue_EnqueueDequeue();
    test_Queue_EmptyThrows();
    test_Queue_Peek();
    test_Queue_Get();
    test_Queue_Concat();
    test_Queue_SingleElement();

    // Collection operators
    test_CollectionOperators_Equality();

    // Hanoi
    test_Hanoi_OneDisk();
    test_Hanoi_TwoDisks();
    test_Hanoi_ThreeDisks();
    test_Hanoi_MaxDisk();
    test_Hanoi_SolveIdempotent();

    // Rectangular Matrix
    test_RectangularMatrix_GetSet();
    test_RectangularMatrix_Add();
    test_RectangularMatrix_MultiplyByScalar();
    test_RectangularMatrix_MultiplyByMatrix();
    test_RectangularMatrix_Transpose();
    test_RectangularMatrix_FrobeniusNorm();
    test_RectangularMatrix_RowColOps();
    test_RectangularMatrix_Operators();

    // Square Matrix
    test_SquareMatrix_Trace();
    test_SquareMatrix_Determinant();

    // Diagonal Matrix
    test_DiagonalMatrix_GetSet();
    test_DiagonalMatrix_Add();
    test_DiagonalMatrix_Inverse();
    test_DiagonalMatrix_Transpose();
    test_DiagonalMatrix_MultiplyDiagonal();

    // Sparse Matrix
    test_SparseMatrix_GetSet();
    test_SparseMatrix_Add();
    test_SparseMatrix_MultiplyByScalar();
    test_SparseMatrix_Transpose();
    test_SparseMatrix_FrobeniusNorm();
    test_SparseMatrix_RowColOps();

    // Exception tests
    test_ExceptionTests();

    // Natural number algorithms
    test_NaturalNumberAlgorithms();

    // Recurrence algorithms
    test_RecurrenceAlgorithms();

    std::cout << "\n=== RESULTS: "
        << (total - failed) << "/" << total << " passed";
    if (failed) std::cout << "  (" << failed << " FAILED)";
    std::cout << " ===\n";
}

int main() {
    setlocale(LC_ALL, "Russian");
    run_all_tests();
    return failed == 0 ? 0 : 1;
}