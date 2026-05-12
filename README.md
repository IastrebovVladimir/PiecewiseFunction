# PiecewiseFunction

Учебный проект по кусочно-заданным функциям и интерполяции на C++.

## Описание

Проект реализует класс PiecewiseFunction<T>, который хранит функцию как набор отрезков (Segment<T>) с собственными функциями на каждом интервале.


## Установка

```bash
cd ~
git clone https://github.com/IastrebovVladimir/Sequence.git
cd PiecewiseFunction
git clone https://github.com/google/googletest.git
```

## Запуск тестов

```bash
g++ -g -O0 -std=c++20 tests/testMain.cpp tests/testPiecewise.cpp tests/testsInterpolation.cpp -I. -isystem googletest/googletest/include -Igoogletest/googletest googletest/googletest/src/gtest-all.cc -pthread -o PiecewiseTests
./PiecewiseTests
```

## Запуск кусочно-заданной функции

```bash
g++ -g -O0 -std=c++20 main_piecewise.cpp Lab_2/types.cpp -o Lab3
./Lab3
```

## Запуск сплайн-интерполяции

```bash
g++ -g -O0 -std=c++20 main_spline.cpp Lab_2/types.cpp -o SplineInterpolation
./SplineInterpolation
```


```text
PiecewiseFunction/
├─ Lab_2                  # Последовательность (из ЛР2, ArraySequence.h, types.h, types.cpp)
├─ Segment.h              # Один отрезок кусочно-заданной функции [left, right]  указатель на Function<T>
├─ PiecewiseFunction.h    # Класс PiecewiseFunction<T> и операции с кусочно-заданной функцией
├─ PolynomialFunction.h   # Полиномная функция
├─ RationalFunction.h     # Рациональная функция
├─ SpliteInterpolation.h  # Кубический сплайн/интерполяция
├─ Function.h             # Интерфейс Function<T>(double x) → T
├─ Complex.h              # Тип комплексного числа
├─ Read.h                 # Функции ввода
├─ menu_piecewise.h       # Объявления меню для кусочно-заданных функций
├─ menu_piecewise.hpp     # Реализация меню для кусочно-заданных функций
├─ spline_menu.h          # Объявления меню для сплайн-интерполяции
├─ spline_menu.hpp        # Реализация меню для сплайн-интерполяции
├─ main_piecewise.cpp     # Консольный интерфейс (кусочная функция)
├─ main_spline.cpp        # Консольный интерфейс (сплайн-интерполяция)
├─ CMakeLists.txt         # Конфигурация сборки CMake
└─ tests/
   ├─ testMain.cpp        # Запуск тестов 
   ├─ testSegment.cpp     # Тесты Segment<T>
   ├─ testPiecewise.cpp   # Тесты PiecewiseFunction<T>
   └─ testSpline.cpp      # Тесты SplineInterpolation
```
