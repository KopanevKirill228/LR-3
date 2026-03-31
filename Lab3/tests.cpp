#include <gtest/gtest.h>
#include <cmath>
#include <stdexcept>

#include "RectangularMatrix.h"
#include "SquareMatrix.h"
#include "DiagonalMatrix.h"
#include "SparseMatrix.h"
#include "Stack.h"
#include "Queue.h"
#include "Hanoi.h"
#include "Solvers.h"
#include "MatrixOperators.h"


// HELPERS
static SquareMatrix<double> MakeSq(int n, std::initializer_list<double> vals) {
    SquareMatrix<double> m(n);
    int i = 0;
    for (double v : vals) { m.Set(i / n, i % n, v); ++i; }
    return m;
}

static MutableArraySequence<double> MakeVec(std::initializer_list<double> vals) {
    MutableArraySequence<double> v;
    for (double x : vals) v.Append(x);
    return v;
}

static bool NearVec(const MutableArraySequence<double>& a,
    const MutableArraySequence<double>& b,
    double eps = 1e-9) {
    if (a.GetLength() != b.GetLength()) return false;
    for (int i = 0; i < a.GetLength(); ++i)
        if (std::abs(a.Get(i) - b.Get(i)) > eps) return false;
    return true;
}

// RECTANGULAR MATRIX
TEST(RectangularMatrix, BasicGetSet) {
    RectangularMatrix<int> m(2, 3);
    m.Set(0, 0, 1); m.Set(1, 2, 7);
    EXPECT_EQ(m.Get(0, 0), 1);
    EXPECT_EQ(m.Get(1, 2), 7);
    EXPECT_EQ(m.Get(0, 1), 0);
}

TEST(RectangularMatrix, Dimensions) {
    RectangularMatrix<int> m(3, 5);
    EXPECT_EQ(m.Rows(), 3);
    EXPECT_EQ(m.Cols(), 5);
}

TEST(RectangularMatrix, Add) {
    RectangularMatrix<int> a(2, 2), b(2, 2);
    a.Set(0, 0, 1); a.Set(0, 1, 2); a.Set(1, 0, 3); a.Set(1, 1, 4);
    b.Set(0, 0, 5); b.Set(0, 1, 6); b.Set(1, 0, 7); b.Set(1, 1, 8);
    auto* r = a.Add(b);
    EXPECT_EQ(r->Get(0, 0), 6);
    EXPECT_EQ(r->Get(1, 1), 12);
    delete r;
}

TEST(RectangularMatrix, MulScalar) {
    RectangularMatrix<int> m(2, 2);
    m.Set(0, 0, 3); m.Set(1, 1, 4);
    auto* r = m.MulScalar(2);
    EXPECT_EQ(r->Get(0, 0), 6);
    EXPECT_EQ(r->Get(1, 1), 8);
    delete r;
}

TEST(RectangularMatrix, MulMatrix) {
    RectangularMatrix<int> a(2, 2), b(2, 2);
    a.Set(0, 0, 1); a.Set(0, 1, 2); a.Set(1, 0, 3); a.Set(1, 1, 4);
    b.Set(0, 0, 5); b.Set(0, 1, 6); b.Set(1, 0, 7); b.Set(1, 1, 8);
    auto* r = a.MulMatrix(b);
    EXPECT_EQ(r->Get(0, 0), 19);
    EXPECT_EQ(r->Get(0, 1), 22);
    EXPECT_EQ(r->Get(1, 0), 43);
    EXPECT_EQ(r->Get(1, 1), 50);
    delete r;
}

TEST(RectangularMatrix, Transpose) {
    RectangularMatrix<int> m(2, 3);
    m.Set(0, 0, 1); m.Set(0, 1, 2); m.Set(0, 2, 3);
    m.Set(1, 0, 4); m.Set(1, 1, 5); m.Set(1, 2, 6);
    auto* t = m.Transpose();
    EXPECT_EQ(t->Rows(), 3);
    EXPECT_EQ(t->Cols(), 2);
    EXPECT_EQ(t->Get(0, 0), 1);
    EXPECT_EQ(t->Get(2, 0), 3);
    EXPECT_EQ(t->Get(0, 1), 4);
    delete t;
}

TEST(RectangularMatrix, SwapRows) {
    RectangularMatrix<int> m(2, 2);
    m.Set(0, 0, 1); m.Set(0, 1, 2);
    m.Set(1, 0, 3); m.Set(1, 1, 4);
    m.SwapRows(0, 1);
    EXPECT_EQ(m.Get(0, 0), 3);
    EXPECT_EQ(m.Get(1, 0), 1);
}

TEST(RectangularMatrix, AddRow) {
    RectangularMatrix<int> m(2, 2);
    m.Set(0, 0, 1); m.Set(0, 1, 2);
    m.Set(1, 0, 3); m.Set(1, 1, 4);
    m.AddRow(1, 0, 2);
    EXPECT_EQ(m.Get(1, 0), 5);
    EXPECT_EQ(m.Get(1, 1), 8);
}

TEST(RectangularMatrix, Norm) {
    RectangularMatrix<double> m(1, 2);
    m.Set(0, 0, 3.0); m.Set(0, 1, 4.0);
    EXPECT_NEAR(m.Norm(), 5.0, 1e-9);
}

TEST(RectangularMatrix, Operator_Plus) {
    RectangularMatrix<int> a(1, 1), b(1, 1);
    a.Set(0, 0, 2); b.Set(0, 0, 3);
    auto* r = a + b;
    EXPECT_EQ(r->Get(0, 0), 5);
    delete r;
}

TEST(RectangularMatrix, Operator_MulScalar) {
    RectangularMatrix<int> m(1, 1);
    m.Set(0, 0, 4);
    auto* r = m * 3;
    EXPECT_EQ(r->Get(0, 0), 12);
    delete r;
}

TEST(RectangularMatrix, Operator_Equal) {
    RectangularMatrix<int> a(1, 1), b(1, 1);
    a.Set(0, 0, 5); b.Set(0, 0, 5);
    EXPECT_TRUE(a == b);
    b.Set(0, 0, 6);
    EXPECT_TRUE(a != b);
}

TEST(RectangularMatrix, 1x1) {
    RectangularMatrix<int> m(1, 1);
    m.Set(0, 0, 99);
    EXPECT_EQ(m.Get(0, 0), 99);
    EXPECT_EQ(m.Rows(), 1);
    EXPECT_EQ(m.Cols(), 1);
}

// SQUARE MATRIX
TEST(SquareMatrix, TraceIdentity) {
    auto m = MakeSq(3, { 1,0,0, 0,1,0, 0,0,1 });
    EXPECT_DOUBLE_EQ(m.Trace(), 3.0);
}

TEST(SquareMatrix, TraceGeneral) {
    auto m = MakeSq(2, { 1,2, 3,4 });
    EXPECT_DOUBLE_EQ(m.Trace(), 5.0);
}

TEST(SquareMatrix, Determinant2x2) {
    auto m = MakeSq(2, { 1.0,2.0, 3.0,4.0 });
    EXPECT_NEAR(m.Determinant(), -2.0, 1e-9);
}

TEST(SquareMatrix, Determinant3x3) {
    auto m = MakeSq(3, { 2,1,3, 0,4,1, 0,0,5 });
    EXPECT_NEAR(m.Determinant(), 40.0, 1e-9);
}

TEST(SquareMatrix, DeterminantSingular) {
    auto m = MakeSq(2, { 1.0,2.0, 2.0,4.0 });
    EXPECT_NEAR(m.Determinant(), 0.0, 1e-9);
}

TEST(SquareMatrix, DeterminantIdentity) {
    auto m = MakeSq(3, { 1,0,0, 0,1,0, 0,0,1 });
    EXPECT_NEAR(m.Determinant(), 1.0, 1e-9);
}

TEST(SquareMatrix, Power0IsIdentity) {
    auto m = MakeSq(2, { 2.0,1.0, 0.0,3.0 });
    auto* r = m.Power(0);
    EXPECT_DOUBLE_EQ(r->Get(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(r->Get(0, 1), 0.0);
    EXPECT_DOUBLE_EQ(r->Get(1, 0), 0.0);
    EXPECT_DOUBLE_EQ(r->Get(1, 1), 1.0);
    delete r;
}

TEST(SquareMatrix, Power1) {
    auto m = MakeSq(2, { 2.0,3.0, 1.0,4.0 });
    auto* r = m.Power(1);
    EXPECT_DOUBLE_EQ(r->Get(0, 0), 2.0);
    EXPECT_DOUBLE_EQ(r->Get(0, 1), 3.0);
    delete r;
}

TEST(SquareMatrix, Power2) {
    auto m = MakeSq(2, { 1.0,1.0, 0.0,1.0 });
    auto* r = m.Power(2);
    EXPECT_DOUBLE_EQ(r->Get(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(r->Get(0, 1), 2.0);
    EXPECT_DOUBLE_EQ(r->Get(1, 0), 0.0);
    EXPECT_DOUBLE_EQ(r->Get(1, 1), 1.0);
    delete r;
}

TEST(SquareMatrix, PowerNegativeThrows) {
    auto m = MakeSq(2, { 1.0,0.0, 0.0,1.0 });
    EXPECT_THROW(m.Power(-1), std::invalid_argument);
}

TEST(SquareMatrix, Trace1x1) {
    SquareMatrix<double> m(1);
    m.Set(0, 0, 7.0);
    EXPECT_DOUBLE_EQ(m.Trace(), 7.0);
}

TEST(SquareMatrix, Determinant1x1) {
    SquareMatrix<double> m(1);
    m.Set(0, 0, 5.0);
    EXPECT_NEAR(m.Determinant(), 5.0, 1e-9);
}

// DIAGONAL MATRIX
TEST(DiagonalMatrix, GetSetDiag) {
    DiagonalMatrix<int> m(3);
    m.SetDiag(0, 1); m.SetDiag(1, 2); m.SetDiag(2, 3);
    EXPECT_EQ(m.GetDiag(0), 1);
    EXPECT_EQ(m.GetDiag(1), 2);
    EXPECT_EQ(m.GetDiag(2), 3);
}

TEST(DiagonalMatrix, OffDiagZero) {
    DiagonalMatrix<int> m(3);
    m.SetDiag(0, 5);
    EXPECT_EQ(m.Get(0, 1), 0);
    EXPECT_EQ(m.Get(1, 0), 0);
}

TEST(DiagonalMatrix, SetOffDiagThrows) {
    DiagonalMatrix<int> m(3);
    EXPECT_THROW(m.Set(0, 1, 5), std::invalid_argument);
}

TEST(DiagonalMatrix, TraceFromSquare) {
    double diag[] = { 2.0, 3.0, 4.0 };
    DiagonalMatrix<double> m(3, diag);
    EXPECT_DOUBLE_EQ(m.Trace(), 9.0);
}

TEST(DiagonalMatrix, MulDiag) {
    double a[] = { 2.0, 3.0 }, b[] = { 4.0, 5.0 };
    DiagonalMatrix<double> da(2, a), db(2, b);
    auto* r = da.MulDiag(db);
    EXPECT_DOUBLE_EQ(r->GetDiag(0), 8.0);
    EXPECT_DOUBLE_EQ(r->GetDiag(1), 15.0);
    delete r;
}

TEST(DiagonalMatrix, Inverse) {
    double diag[] = { 2.0, 4.0 };
    DiagonalMatrix<double> m(2, diag);
    auto* inv = m.Inverse();
    EXPECT_NEAR(inv->GetDiag(0), 0.5, 1e-9);
    EXPECT_NEAR(inv->GetDiag(1), 0.25, 1e-9);
    delete inv;
}

TEST(DiagonalMatrix, InverseZeroThrows) {
    double diag[] = { 1.0, 0.0 };
    DiagonalMatrix<double> m(2, diag);
    EXPECT_THROW(m.Inverse(), std::runtime_error);
}

TEST(DiagonalMatrix, 1x1) {
    DiagonalMatrix<int> m(1);
    m.SetDiag(0, 42);
    EXPECT_EQ(m.GetDiag(0), 42);
    EXPECT_EQ(m.Get(0, 0), 42);
}

// SPARSE MATRIX
TEST(SparseMatrix, BasicGetSet) {
    SparseMatrix<int> m(3, 3);
    m.Set(0, 0, 5);
    m.Set(2, 2, 9);
    EXPECT_EQ(m.Get(0, 0), 5);
    EXPECT_EQ(m.Get(2, 2), 9);
    EXPECT_EQ(m.Get(0, 1), 0);
}

TEST(SparseMatrix, NonZeroCount) {
    SparseMatrix<int> m(3, 3);
    EXPECT_EQ(m.NonZeroCount(), 0);
    m.Set(0, 0, 1);
    m.Set(1, 2, 3);
    EXPECT_EQ(m.NonZeroCount(), 2);
}

TEST(SparseMatrix, SetZeroRemovesElement) {
    SparseMatrix<int> m(3, 3);
    m.Set(0, 0, 5);
    EXPECT_EQ(m.NonZeroCount(), 1);
    m.Set(0, 0, 0);
    EXPECT_EQ(m.NonZeroCount(), 0);
}

TEST(SparseMatrix, Add) {
    SparseMatrix<int> a(2, 2), b(2, 2);
    a.Set(0, 0, 1); b.Set(0, 0, 2); b.Set(1, 1, 3);
    auto* r = a.Add(b);
    EXPECT_EQ(r->Get(0, 0), 3);
    EXPECT_EQ(r->Get(1, 1), 3);
    delete r;
}

TEST(SparseMatrix, MulScalar) {
    SparseMatrix<int> m(2, 2);
    m.Set(0, 0, 4);
    auto* r = m.MulScalar(3);
    EXPECT_EQ(r->Get(0, 0), 12);
    delete r;
}

TEST(SparseMatrix, Transpose) {
    SparseMatrix<int> m(2, 3);
    m.Set(0, 2, 7);
    auto* t = m.Transpose();
    EXPECT_EQ(t->Rows(), 3);
    EXPECT_EQ(t->Cols(), 2);
    EXPECT_EQ(t->Get(2, 0), 7);
    delete t;
}

TEST(SparseMatrix, EmptyNorm) {
    SparseMatrix<double> m(3, 3);
    EXPECT_DOUBLE_EQ(m.Norm(), 0.0);
}

TEST(SparseMatrix, Dimensions) {
    SparseMatrix<int> m(4, 7);
    EXPECT_EQ(m.Rows(), 4);
    EXPECT_EQ(m.Cols(), 7);
}

// STACK
TEST(Stack, EmptyOnCreate) {
    Stack<int> s;
    EXPECT_TRUE(s.IsEmpty());
    EXPECT_EQ(s.GetCount(), 0u);
}

TEST(Stack, PushPop) {
    Stack<int> s;
    s.Push(1); s.Push(2); s.Push(3);
    EXPECT_EQ(s.GetCount(), 3u);
    EXPECT_EQ(s.Pop(), 3);
    EXPECT_EQ(s.Pop(), 2);
    EXPECT_EQ(s.Pop(), 1);
    EXPECT_TRUE(s.IsEmpty());
}

TEST(Stack, Peek) {
    Stack<int> s;
    s.Push(10); s.Push(20);
    EXPECT_EQ(s.Peek(), 20);
    EXPECT_EQ(s.GetCount(), 2u);
}

TEST(Stack, PopEmptyThrows) {
    Stack<int> s;
    EXPECT_THROW(s.Pop(), std::runtime_error);
}

TEST(Stack, PeekEmptyThrows) {
    Stack<int> s;
    EXPECT_THROW(s.Peek(), std::runtime_error);
}

TEST(Stack, CopyIndependent) {
    Stack<int> a;
    a.Push(1); a.Push(2);
    Stack<int> b = a;
    b.Pop();
    EXPECT_EQ(a.GetCount(), 2u);
    EXPECT_EQ(b.GetCount(), 1u);
}

TEST(Stack, SingleElement) {
    Stack<int> s;
    s.Push(42);
    EXPECT_EQ(s.Peek(), 42);
    EXPECT_EQ(s.Pop(), 42);
    EXPECT_TRUE(s.IsEmpty());
}

TEST(Stack, Get) {
    Stack<int> s;
    s.Push(1); s.Push(2); s.Push(3);
    EXPECT_EQ(s.Get(0), 3);
}

TEST(Stack, Concat) {
    Stack<int> a, b;
    a.Push(1); a.Push(2);
    b.Push(3); b.Push(4);
    auto* r = a.Concat(b);
    EXPECT_EQ(r->GetCount(), 4u);
    delete r;
}

// QUEUE
TEST(Queue, EmptyOnCreate) {
    Queue<int> q;
    EXPECT_TRUE(q.IsEmpty());
    EXPECT_EQ(q.GetCount(), 0u);
}

TEST(Queue, EnqueueDequeue) {
    Queue<int> q;
    q.Enqueue(1); q.Enqueue(2); q.Enqueue(3);
    EXPECT_EQ(q.GetCount(), 3u);
    EXPECT_EQ(q.Dequeue(), 1);
    EXPECT_EQ(q.Dequeue(), 2);
    EXPECT_EQ(q.Dequeue(), 3);
    EXPECT_TRUE(q.IsEmpty());
}

TEST(Queue, FrontBack) {
    Queue<int> q;
    q.Enqueue(10); q.Enqueue(20);
    EXPECT_EQ(q.Front(), 10);
    EXPECT_EQ(q.Back(), 20);
}

TEST(Queue, DequeueEmptyThrows) {
    Queue<int> q;
    EXPECT_THROW(q.Dequeue(), std::runtime_error);
}

TEST(Queue, FrontEmptyThrows) {
    Queue<int> q;
    EXPECT_THROW(q.Front(), std::runtime_error);
}

TEST(Queue, BackEmptyThrows) {
    Queue<int> q;
    EXPECT_THROW(q.Back(), std::runtime_error);
}

TEST(Queue, CopyIndependent) {
    Queue<int> a;
    a.Enqueue(1); a.Enqueue(2);
    Queue<int> b = a;
    b.Dequeue();
    EXPECT_EQ(a.GetCount(), 2u);
    EXPECT_EQ(b.GetCount(), 1u);
}

TEST(Queue, SingleElement) {
    Queue<int> q;
    q.Enqueue(99);
    EXPECT_EQ(q.Front(), 99);
    EXPECT_EQ(q.Back(), 99);
    EXPECT_EQ(q.Dequeue(), 99);
    EXPECT_TRUE(q.IsEmpty());
}

TEST(Queue, Concat) {
    Queue<int> a, b;
    a.Enqueue(1); a.Enqueue(2);
    b.Enqueue(3); b.Enqueue(4);
    auto* r = a.Concat(b);
    EXPECT_EQ(r->GetCount(), 4u);
    delete r;
}

// HANOI
static Stack<int> MakeStack(int n) {
    Stack<int> s;
    for (int d = n; d >= 1; d--) s.Push(d);
    return s;
}

TEST(Hanoi, MoveCount_1) {
    Hanoi<int> h(MakeStack(1), "A", "B", "C");
    h.Solve();
    EXPECT_EQ(h.GetMoves().GetLength(), 1);
}

TEST(Hanoi, MoveCount_2) {
    Hanoi<int> h(MakeStack(2), "A", "B", "C");
    h.Solve();
    EXPECT_EQ(h.GetMoves().GetLength(), 3);
}

TEST(Hanoi, MoveCount_n) {
    for (int n = 1; n <= 8; ++n) {
        Hanoi<int> h(MakeStack(n), "A", "B", "C");
        h.Solve();
        EXPECT_EQ(h.GetMoves().GetLength(), (1 << n) - 1);
    }
}

TEST(Hanoi, AllDisksOnC) {
    int n = 4;
    Hanoi<int> h(MakeStack(n), "A", "B", "C");
    h.Solve();
    EXPECT_EQ((int)h.GetPegC().GetCount(), n);
    EXPECT_EQ((int)h.GetPegA().GetCount(), 0);
    EXPECT_EQ((int)h.GetPegB().GetCount(), 0);
}

TEST(Hanoi, CorrectOrder_n2) {
    Hanoi<int> h(MakeStack(2), "A", "B", "C");
    h.Solve();
    const auto& mv = h.GetMoves();
    EXPECT_EQ(mv.Get(0).from, "A"); EXPECT_EQ(mv.Get(0).to, "B"); EXPECT_EQ(mv.Get(0).disk, 1);
    EXPECT_EQ(mv.Get(1).from, "A"); EXPECT_EQ(mv.Get(1).to, "C"); EXPECT_EQ(mv.Get(1).disk, 2);
    EXPECT_EQ(mv.Get(2).from, "B"); EXPECT_EQ(mv.Get(2).to, "C"); EXPECT_EQ(mv.Get(2).disk, 1);
}

TEST(Hanoi, StackOrderOnC) {
    int n = 3;
    Hanoi<int> h(MakeStack(n), "A", "B", "C");
    h.Solve();
    for (int i = 0; i < n; ++i)
        EXPECT_EQ(h.GetPegC().Get(i), i + 1);
}

TEST(Hanoi, SolveIdempotent) {
    Hanoi<int> h(MakeStack(3), "A", "B", "C");
    h.Solve();
    EXPECT_NO_THROW(h.Solve());
}

// SOLVERS
TEST(Solvers, GaussNopivot_2x2) {
    auto A = MakeSq(2, { 2.0,1.0, 1.0,3.0 });
    auto b = MakeVec({ 5.0, 10.0 });
    auto x = GaussNopivot<double>(A, b);
    EXPECT_NEAR(x.Get(0), 1.0, 1e-9);
    EXPECT_NEAR(x.Get(1), 3.0, 1e-9);
}

TEST(Solvers, GaussNopivot_1x1) {
    auto A = MakeSq(1, { 4.0 });
    auto b = MakeVec({ 8.0 });
    auto x = GaussNopivot<double>(A, b);
    EXPECT_NEAR(x.Get(0), 2.0, 1e-9);
}

TEST(Solvers, GaussNopivot_SingularThrows) {
    auto A = MakeSq(2, { 1.0,2.0, 2.0,4.0 });
    auto b = MakeVec({ 1.0, 2.0 });
    EXPECT_THROW(GaussNopivot<double>(A, b), std::runtime_error);
}

TEST(Solvers, GaussPartialPivot_2x2) {
    auto A = MakeSq(2, { 0.0,1.0, 2.0,3.0 });
    auto b = MakeVec({ 1.0, 8.0 });
    auto x = GaussPartialPivot<double>(A, b);
    EXPECT_NEAR(x.Get(0), 2.5, 1e-9);
    EXPECT_NEAR(x.Get(1), 1.0, 1e-9);
}

TEST(Solvers, GaussPartialPivot_SingularThrows) {
    auto A = MakeSq(2, { 1.0,2.0, 2.0,4.0 });
    auto b = MakeVec({ 1.0, 2.0 });
    EXPECT_THROW(GaussPartialPivot<double>(A, b), std::runtime_error);
}

TEST(Solvers, GaussPartialPivot_InvalidDimThrows) {
    auto A = MakeSq(2, { 1.0,0.0, 0.0,1.0 });
    auto b = MakeVec({ 1.0, 2.0, 3.0 }); // длина не совпадает
    EXPECT_THROW(GaussPartialPivot<double>(A, b), std::invalid_argument);
}

TEST(Solvers, LUDecompose_LU_eq_A) {
    auto A = MakeSq(3, { 2.0,1.0,1.0, 4.0,3.0,3.0, 8.0,7.0,9.0 });
    auto lu = LUDecompose<double>(A);
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j) {
            double s = 0;
            for (int k = 0; k < 3; ++k) s += lu.L.Get(i, k) * lu.U.Get(k, j);
            EXPECT_NEAR(s, A.Get(i, j), 1e-9);
        }
}

TEST(Solvers, LUDecompose_L_UnitDiag) {
    auto A = MakeSq(2, { 4.0,3.0, 6.0,3.0 });
    auto lu = LUDecompose<double>(A);
    EXPECT_NEAR(lu.L.Get(0, 0), 1.0, 1e-9);
    EXPECT_NEAR(lu.L.Get(1, 1), 1.0, 1e-9);
}

TEST(Solvers, LUSolve_2x2) {
    auto A = MakeSq(2, { 2.0,1.0, 1.0,3.0 });
    auto b = MakeVec({ 5.0, 10.0 });
    auto x = LUSolve<double>(A, b);
    EXPECT_NEAR(x.Get(0), 1.0, 1e-9);
    EXPECT_NEAR(x.Get(1), 3.0, 1e-9);
}

TEST(Solvers, LUSolve_1x1) {
    auto A = MakeSq(1, { 3.0 });
    auto b = MakeVec({ 9.0 });
    auto x = LUSolve<double>(A, b);
    EXPECT_NEAR(x.Get(0), 3.0, 1e-9);
}

TEST(Solvers, LUSolveDecomposed_MultipleRHS) {
    auto A = MakeSq(2, { 2.0,1.0, 1.0,3.0 });
    auto lu = LUDecompose<double>(A);
    auto b1 = MakeVec({ 5.0, 10.0 });
    auto b2 = MakeVec({ 3.0,  4.0 });
    auto x1 = LUSolveDecomposed<double>(lu, b1);
    auto x2 = LUSolveDecomposed<double>(lu, b2);
    EXPECT_NEAR(x1.Get(0), 1.0, 1e-9);
    EXPECT_NEAR(x2.Get(0), 1.0, 1e-9);
    EXPECT_NEAR(x2.Get(1), 1.0, 1e-9);
}

TEST(Solvers, Residual_ZeroForExactSolution) {
    auto A = MakeSq(2, { 2.0,0.0, 0.0,3.0 });
    auto b = MakeVec({ 4.0, 6.0 });
    auto x = LUSolve<double>(A, b);
    EXPECT_NEAR(Residual<double>(A, x, b), 0.0, 1e-9);
}

TEST(Solvers, VectorNorm) {
    auto v = MakeVec({ 3.0, 4.0 });
    EXPECT_NEAR(VectorNorm<double>(v), 5.0, 1e-9);
}

TEST(Solvers, LUDecompose_NonSquareThrows) {
    RectangularMatrix<double> A(2, 3);
    EXPECT_THROW(LUDecompose<double>(A), std::invalid_argument);
}

TEST(Solvers, HilbertMatrix_Residual) {
    int n = 5;
    SquareMatrix<double> H(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            H.Set(i, j, 1.0 / (i + j + 1));
    MutableArraySequence<double> b;
    for (int i = 0; i < n; i++) {
        double s = 0;
        for (int j = 0; j < n; j++) s += H.Get(i, j);
        b.Append(s);
    }
    auto x = LUSolve<double>(H, b);
    EXPECT_LT(Residual<double>(H, x, b), 1e-6);
}

// MAIN
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}