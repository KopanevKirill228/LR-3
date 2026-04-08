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

// Queue sequence
void test_Queue_Sequence_Map() {
    SUITE("Queue: GetSequence()->Map");
    {
        Queue<int> q;
        q.Enqueue(1); q.Enqueue(2); q.Enqueue(3);
        auto* seq = q.GetSequence()->Map([](const int& x) { return x * 2; });
        CHECK("map count", seq->GetLength() == 3);
        CHECK("map Get(0)", seq->Get(0) == 2);
        CHECK("map Get(1)", seq->Get(1) == 4);
        CHECK("map Get(2)", seq->Get(2) == 6);
        delete seq;
    }
    {
        Queue<int> q; // пустая очередь
        auto* seq = q.GetSequence()->Map([](const int& x) { return x * 2; });
        CHECK("map on empty queue", seq->GetLength() == 0);
        delete seq;
    }
}

void test_Queue_Sequence_Where() {
    SUITE("Queue: GetSequence()->Where");
    {
        Queue<int> q;
        q.Enqueue(1); q.Enqueue(2); q.Enqueue(3); q.Enqueue(4);
        auto* seq = q.GetSequence()->Where([](const int& x) { return x % 2 == 0; });
        CHECK("where count", seq->GetLength() == 2);
        CHECK("where Get(0)", seq->Get(0) == 2);
        CHECK("where Get(1)", seq->Get(1) == 4);
        delete seq;
    }
    {
        Queue<int> q;
        q.Enqueue(1); q.Enqueue(3);
        auto* seq = q.GetSequence()->Where([](const int& x) { return x % 2 == 0; });
        CHECK("where none match returns empty", seq->GetLength() == 0);
        delete seq;
    }
}

void test_Queue_Sequence_Reduce() {
    SUITE("Queue: GetSequence()->Reduce");
    {
        Queue<int> q;
        q.Enqueue(1); q.Enqueue(2); q.Enqueue(3); q.Enqueue(4);
        int sum = q.GetSequence()->Reduce(
            [](const int& acc, const int& x) { return acc + x; }, 0);
        CHECK("reduce sum", sum == 10);
    }
    {
        Queue<int> q;
        q.Enqueue(5);
        int sum = q.GetSequence()->Reduce(
            [](const int& acc, const int& x) { return acc + x; }, 0);
        CHECK("reduce single element", sum == 5);
    }
    {
        Queue<int> q;
        int sum = q.GetSequence()->Reduce(
            [](const int& acc, const int& x) { return acc + x; }, 0);
        CHECK("reduce empty returns initial", sum == 0);
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
        CHECK("sparse swaprows (0,1) now 0", m.Get(0, 1) == 2);
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

    // Queue sequence
    test_Queue_Sequence_Map();
    test_Queue_Sequence_Where();
    test_Queue_Sequence_Reduce();

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