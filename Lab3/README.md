# Лабораторная работа №3 — Структуры данных и алгоритмы на C++

Полиморфные абстрактные типы данных, построенные на базе иерархии `Sequence` (`ArraySequence` / `ListSequence` с вариантами Mutable/Immutable), с консольным UI и модульными тестами.

---

## Возможности

### Коллекции

| Тип | Внутренняя структура | Основные операции |
|-----|---------------------|-------------------|
| `Stack<T>` | `MutableArraySequence` | Push, Pop, Peek, Concat |
| `Queue<T>` | `MutableListSequence` | Enqueue, Dequeue, PeekFront, PeekBack, Concat |

Оба типа реализуют `ICollection<T>` и поддерживают операторы `==`, `!=`, `<<`.

### Последовательности

- **Mutable / Immutable** варианты для `ArraySequence` и `ListSequence`
- **Builder** паттерн для обоих типов
- Поддержка **IEnumerator**
- Функциональные операции: **Map, Where, Reduce, Zip, Unzip**
- Вспомогательные функции: **GetPrefixes, GetSuffixes, GetMedian, CountInversions, GetMovingAverage, GetMirrorSum, GetMinMaxAvg**

### Ханойская башня

- Решение для произвольного числа дисков (1–12)
- История ходов хранится в `MutableArraySequence<Move>`
- Анимированный рендерер в терминале с ANSI-цветами
- Проверка корректности входного стека при создании

### Матрицы

Все типы матриц реализуют `IMatrix<T>` и поддерживают:

- `Add`, `MultiplyByScalar`, `MultiplyByMatrix`
- `Transpose`, `FrobeniusNorm`
- Элементарные преобразования строк и столбцов (`SwapRows`, `ScaleRow`, `AddScaledRow` и аналоги для столбцов)

| Тип | Дополнительные операции |
|-----|------------------------|
| `SquareMatrix` | `Determinant`, `Trace` |
| `DiagonalMatrix` | `Inverse`, `MultiplyDiagonal` |
| `SparseMatrix` | `NonZeroCount`, разреженное COO-хранилище |

Перегрузка операторов: `+`, `*` (скаляр и матрица), `==`, `!=`, `<<`

### Решатели СЛАУ (`Solvers.h`)

- **Метод Гаусса без выбора ведущего** — `GaussNopivot`
- **Метод Гаусса с частичным выбором** — `GaussPartialPivot`
- **LU-разложение** — `LUDecompose`
- **Решение через LU** — `LUSolve`, `LUSolveDecomposed`
- **Вспомогательные функции** — `VectorNorm`, `Residual`, `ForwardSubstitution`, `BackSubstitution`

---

## Сборка программы и тестов

```bash
1. Очистить старые файлы
make clean

2. Запустить тесты
make run_tests
make run_solvers

3. Запустить основную программу
make run
```

---

## Тесты

Тесты написаны с использованием легковесного макро-фреймворка (`CHECK`, `CHECK_THROWS`, `SUITE`).

Покрытие включает: Stack, Queue, Ханойскую башню, все типы матриц, перегрузки операторов, граничные случаи и обработку исключений.

---

## Требования

- C++17 и выше
- Внешние зависимости отсутствуют
