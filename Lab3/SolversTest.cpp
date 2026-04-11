#include "Solvers.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <string>
#include "SquareMatrix.h"
#include "lib/ArraySequence.h"

using Clock = std::chrono::high_resolution_clock;
using Ms = std::chrono::duration<double, std::milli>;

// Генерация случайной матрицы [?1, 1] с фиксированным seed
SquareMatrix<double> RandomMatrix(int n, unsigned seed = 42) {
    srand(seed);
    SquareMatrix<double> A(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            A.Set(i, j, -1.0 + 2.0 * rand() / RAND_MAX);
    return A;
}

// Генерация случайного вектора [?1, 1]
MutableArraySequence<double> RandomVector(int n, unsigned seed = 123) {
    srand(seed);
    MutableArraySequence<double> b;
    for (int i = 0; i < n; i++)
        b.Append(-1.0 + 2.0 * rand() / RAND_MAX);
    return b;
}

// Матрица Гильберта: H[i][j] = 1/(i+j+1)
SquareMatrix<double> HilbertMatrix(int n) {
    SquareMatrix<double> H(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            H.Set(i, j, 1.0 / (i + j + 1));
    return H;
}

// 4.1 Сравнение времени решения одной системы
void Experiment_4_1() {
    std::cout << "\n========================================\n";
    std::cout << "4.1 Сравнение времени (одна система)\n";
    std::cout << "========================================\n\n";

    std::cout << std::left
        << std::setw(6) << "n"
        << std::setw(18) << "Гаусс (без выб.)"
        << std::setw(18) << "Гаусс (с выб.)"
        << std::setw(18) << "LU (разложение)"
        << std::setw(18) << "LU (подстановка)"
        << std::setw(18) << "LU (итого)"
        << "\n";
    std::cout << std::string(96, '-') << "\n";

    int sizes[] = { 100, 200, 500, 1000 };

    for (int n : sizes) {
        auto A = RandomMatrix(n, 42);
        auto b = RandomVector(n, 123);

        // Гаусс без выбора ведущего
        double t_gauss_nopivot = 0;
        try {
            auto t0 = Clock::now();
            auto x = GaussNopivot<double>(A, b);
            t_gauss_nopivot = Ms(Clock::now() - t0).count();
        }
        catch (...) {
            t_gauss_nopivot = -1;
        }

        // Гаусс с выбором ведущего
        auto t0 = Clock::now();
        auto x_pivot = GaussPartialPivot<double>(A, b);
        double t_gauss_pivot = Ms(Clock::now() - t0).count();

        // LU разложение
        t0 = Clock::now();
        auto lu = LUDecompose<double>(A);
        double t_lu_decomp = Ms(Clock::now() - t0).count();

        // LU подстановка
        t0 = Clock::now();
        auto x_lu = LUSolveDecomposed<double>(lu, b);
        double t_lu_solve = Ms(Clock::now() - t0).count();

        std::cout << std::left
            << std::setw(6) << n
            << std::setw(18) << (t_gauss_nopivot < 0 ? std::string("сингул.") : std::to_string(t_gauss_nopivot).substr(0, 6) + " мс")
            << std::setw(18) << (std::to_string(t_gauss_pivot).substr(0, 6) + " мс")
            << std::setw(18) << (std::to_string(t_lu_decomp).substr(0, 6) + " мс")
            << std::setw(18) << (std::to_string(t_lu_solve).substr(0, 6) + " мс")
            << std::setw(18) << (std::to_string(t_lu_decomp + t_lu_solve).substr(0, 6) + " мс")
            << "\n";
    }
}

// 4.2 Экономия при множественных правых частях
void Experiment_4_2() {
    std::cout << "\n========================================\n";
    std::cout << "4.2 Множественные правые части (n=500)\n";
    std::cout << "========================================\n\n";

    int n = 500;
    auto A = RandomMatrix(n, 42);

    // LU разложение — один раз
    auto t0 = Clock::now();
    auto lu = LUDecompose<double>(A);
    double t_decomp = Ms(Clock::now() - t0).count();

    std::cout << "LU разложение (один раз): " << t_decomp << " мс\n\n";

    std::cout << std::left
        << std::setw(6) << "k"
        << std::setw(25) << "Гаусс (k систем)"
        << std::setw(25) << "LU (k подстановок)"
        << std::setw(25) << "LU всего (разл.+подст.)"
        << "\n";
    std::cout << std::string(81, '-') << "\n";

    int ks[] = { 1, 10, 100 };

    for (int k : ks) {
        MutableArraySequence<MutableArraySequence<double>> bs;
        for (int q = 0; q < k; q++)
            bs.Append(RandomVector(n, 100 + q));

        // Гаусс — k раз полное решение
        t0 = Clock::now();
        try {
            for (int q = 0; q < k; q++)
                GaussPartialPivot<double>(A, bs.Get(q));
        }
        catch (const std::exception& e) {
            std::cout << "  [ERR] " << e.what() << "\n";
        }

        double t_gauss = Ms(Clock::now() - t0).count();

        // LU — только k подстановок
        t0 = Clock::now();
        for (int q = 0; q < k; q++)
            LUSolveDecomposed<double>(lu, bs.Get(q));
        double t_lu_solve = Ms(Clock::now() - t0).count();

        std::cout << std::left
            << std::setw(6) << k
            << std::setw(25) << (std::to_string(t_gauss).substr(0, 6) + " мс")
            << std::setw(25) << (std::to_string(t_lu_solve).substr(0, 6) + " мс")
            << std::setw(25) << (std::to_string(t_decomp + t_lu_solve).substr(0, 6) + " мс")
            << "\n";
    }
}

// 4.3 Точность на матрице Гильберта
void Experiment_4_3() {
    std::cout << "\n========================================\n";
    std::cout << "4.3 Точность на матрице Гильберта\n";
    std::cout << "========================================\n\n";

    std::cout << std::left
        << std::setw(5) << "n"
        << std::setw(22) << "Метод"
        << std::setw(22) << "Отн. погрешность"
        << std::setw(22) << "Невязка"
        << "\n";
    std::cout << std::string(71, '-') << "\n";

    int sizes[] = { 5, 10, 15 };

    for (int n : sizes) {
        auto H = HilbertMatrix(n);

        MutableArraySequence<double> xExact;
        for (int i = 0; i < n; i++)
            xExact.Append(1.0);

        MutableArraySequence<double> b;
        for (int i = 0; i < n; i++) {
            double sum = 0.0;
            for (int j = 0; j < n; j++)
                sum += H.Get(i, j) * xExact.Get(j);
            b.Append(sum);
        }

        double xNorm = VectorNorm(xExact);

        auto calcRelError = [&](const MutableArraySequence<double>& xApprox) {
            MutableArraySequence<double> diff;
            for (int i = 0; i < n; i++)
                diff.Append(xApprox.Get(i) - xExact.Get(i));
            return VectorNorm(diff) / xNorm;
            };

        try {
            auto x1 = GaussNopivot<double>(H, b);
            double err = calcRelError(x1);
            double res = Residual(H, x1, b);
            std::cout << std::setw(5) << n
                << std::setw(22) << "Гаусс (без выб.)"
                << std::setw(22) << std::scientific << std::setprecision(2) << err
                << std::setw(22) << std::fixed << std::setprecision(8) << res
                << "\n";
        }
        catch (const std::exception& e) {
            std::cout << std::setw(5) << n
                << std::setw(22) << "Гаусс (без выб.)"
                << std::setw(22) << "ОШИБКА"
                << std::setw(22) << e.what()
                << "\n";
        }

        try {
            auto x2 = GaussPartialPivot<double>(H, b);
            double err = calcRelError(x2);
            double res = Residual(H, x2, b);
            std::cout << std::setw(5) << n
                << std::setw(22) << "Гаусс (с выб.)"
                << std::setw(22) << std::scientific << std::setprecision(2) << err
                << std::setw(22) << std::fixed << std::setprecision(8) << res
                << "\n";
        }
        catch (const std::exception& e) {
            std::cout << std::setw(5) << n
                << std::setw(22) << "Гаусс (с выб.)"
                << std::setw(22) << "ОШИБКА"
                << std::setw(22) << e.what()
                << "\n";
        }

        try {
            auto x3 = LUSolve<double>(H, b);
            double err = calcRelError(x3);
            double res = Residual(H, x3, b);
            std::cout << std::setw(5) << n
                << std::setw(22) << "LU"
                << std::setw(22) << std::scientific << std::setprecision(2) << err
                << std::setw(22) << std::fixed << std::setprecision(8) << res
                << "\n";
        }
        catch (const std::exception& e) {
            std::cout << std::setw(5) << n
                << std::setw(22) << "LU"
                << std::setw(22) << "ОШИБКА"
                << std::setw(22) << e.what()
                << "\n";
        }

        std::cout << std::string(71, '-') << "\n";
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    std::cout << "========================================\n";
    std::cout << "Лабораторная: Сравнение методов СЛАУ\n";
    std::cout << "========================================\n";

    Experiment_4_1();
    Experiment_4_2();
    Experiment_4_3();

    std::cout << "\n========================================\n";
    std::cout << "Все эксперименты завершены\n";
    std::cout << "========================================\n";

    return 0;
}