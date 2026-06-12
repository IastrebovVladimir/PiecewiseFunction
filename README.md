# PiecewiseFunction

Учебный проект по кусочно-заданным функциям и интерполяции на C++.

## Описание

Проект реализует класс PiecewiseFunction<T>, который хранит функцию как набор отрезков (Segment<T>) с собственными функциями на каждом интервале.


## Установка

```bash
cd ~
git clone https://github.com/IastrebovVladimir/PiecewiseFunction.git
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
g++ -g -O0 -std=c++20 menu/main_piecewise.cpp Lab_2/types.cpp -o Lab3
./Lab3
```

## Запуск сплайн-интерполяции

```bash
g++ -g -O0 -std=c++20 spline_menu/main_spline.cpp Lab_2/types.cpp -o SplineInterpolation
./SplineInterpolation
```


```text
PiecewiseFunction/
├─ Functions/
│  ├─ Function.h             # Интерфейс Function<T>(double x) → T
│  ├─ PiecewiseFunction.h    # Класс PiecewiseFunction<T> и операции с кусочно-заданной функцией
│  ├─ PolynomialFunction.h   # Полиномная функция
│  ├─ RationalFunction.h     # Рациональная функция
│  ├─ SpliteInterpolation.h  # Кубический сплайн/интерполяция
│  ├─ Complex.h              # Тип комплексного числа
│  ├─ Segment.h              # Один отрезок кусочно-заданной функции [left, right]  указатель на Function<T>
│  └─ Read.h                 # Функции ввода
│
├─ Lab_2                     # Последовательность (из ЛР2, ArraySequence.h, types.h, types.cpp)
│                    
├─ menu/
│  ├─ main_piecewise.cpp     # Консольный интерфейс (кусочная функция)
│  ├─ menu_piecewise.h       # Объявления меню для кусочно-заданных функций
│  ├─ menu_piecewise.hpp     # Реализация меню для кусочно-заданных функций
│  └─ menu_plot.hpp
├─ spline_menu/
│  ├─ main_spline.cpp        # Консольный интерфейс (сплайн-интерполяция)
│  ├─ spline_menu.h          # Объявления меню для сплайн-интерполяции
│  └─ spline_menu.hpp        # Реализация меню для сплайн-интерполяции
├─ tests/
│  ├─ testMain.cpp           # Запуск тестов 
│  ├─ testSegment.cpp        # Тесты Segment<T>
│  ├─ testPiecewise.cpp      # Тесты PiecewiseFunction<T>
│  └─ testSpline.cpp         # Тесты SplineInterpolation
├─ CMakeLists.txt            # Конфигурация сборки CMake
└─ .github/
   └─ workflows/
      └─ ci.yml              # Установка gnuplot
```
