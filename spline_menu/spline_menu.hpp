#ifndef SPLINE_MENU_HPP
#define SPLINE_MENU_HPP

#include <memory>
#include <iostream>
#include <stdexcept>

#include "../Lab_2/types.h"
#include "../Lab_2/ArraySequence.h"
#include "../Functions/PiecewiseFunction.h"
#include "../Functions/SpliteInterpolation.h"
#include "../Functions/PolynomialFunction.h"

inline void print_spline_menu_title() {
    std::cout << "=====================================\n";
    std::cout << "             Spline Menu             \n";
    std::cout << "=====================================\n";
}

inline void print_enter_points_title() {
    std::cout << "=====================================\n";
    std::cout << "            Enter Points             \n";
    std::cout << "=====================================\n";
}

inline void print_points_title() {
    std::cout << "=====================================\n";
    std::cout << "            Print Points             \n";
    std::cout << "=====================================\n";
}

inline void print_clear_points_title() {
    std::cout << "=====================================\n";
    std::cout << "         Clear Points / Spline       \n";
    std::cout << "=====================================\n";
}

inline void print_build_spline_title() {
    std::cout << "=====================================\n";
    std::cout << "       Build Natural Spline          \n";
    std::cout << "=====================================\n";
}

inline void print_evaluate_spline_title() {
    std::cout << "=====================================\n";
    std::cout << "          Evaluate Spline            \n";
    std::cout << "=====================================\n";
}

inline void print_spline_formulas_title() {
    std::cout << "=====================================\n";
    std::cout << "         Print Spline Formulas       \n";
    std::cout << "=====================================\n";
}

template<typename T>
void run_spline_menu(const TypeInfo* t) {
    MutableArraySequence<double> xs;
    MutableArraySequence<T> ys;
    std::unique_ptr<PiecewiseFunction<T>> spline;

    while (true) {
        print_spline_menu_title();

        if (xs.GetLength() == 0)
            std::cout << "Points: not entered\n";
        else
            std::cout << "Points: entered (" << xs.GetLength() << ")\n";

        if (!spline)
            std::cout << "Spline: not built\n";
        else
            std::cout << "Spline: built on [" << spline->GetLeft()
                      << ", " << spline->GetRight() << "]\n";

        std::cout << "\n1. Enter points\n";
        std::cout << "2. Print points\n";
        std::cout << "3. Clear points\n";
        std::cout << "4. Build natural cubic spline\n";
        std::cout << "5. Evaluate spline at x\n";
        std::cout << "6. Print spline formulas\n";
        std::cout << "0. Exit\n";

        int command = 0;
        std::cout << "\nChoose: ";

        while (!get_int_t()->scan(&command) || command < 0 || command > 6) {
            std::cout << "Error! Enter an integer from 0 to 6: ";
        }

        switch (command) {
            case 1:
                case_enter_points(xs, ys, spline, t);
                break;
            case 2:
                case_print_points(xs, ys, t);
                break;
            case 3:
                case_clear_points(xs, ys, spline);
                break;
            case 4:
                case_build_spline(xs, ys, spline);
                break;
            case 5:
                case_evaluate_spline(spline.get(), t);
                break;
            case 6:
                case_print_spline_formulas(spline.get(), t);
                break;
            case 0:
                std::cout << "Exit.\n";
                return;
        }
    }
}

template<typename T>
void case_enter_points(MutableArraySequence<double>& xs, MutableArraySequence<T>& ys,
                       std::unique_ptr<PiecewiseFunction<T>>& spline,
                       const TypeInfo* t) {
    xs = MutableArraySequence<double>();
    ys = MutableArraySequence<T>();
    spline.reset();

    print_enter_points_title();
    int n = 0;
    std::cout << "Enter number of points: ";
    while (!get_int_t()->scan(&n) || n < 2) {
        std::cout << "Error! Enter an integer >= 2: ";
    }

    std::cout << "Enter points:\n";

    for (int index = 0; index < n; index++) {
        double x = 0.0;
        T y;

        std::cout << "Point " << index << '\n';

        std::cout << "Enter x: ";
        while (!get_double_t()->scan(&x)) {
            std::cout << "Error! Enter a double: ";
        }

        if (index > 0 && x <= xs.Get(index - 1)) {
            std::cout << "Error! x values must be strictly increasing\n";
            index--;
            continue;
        }

        std::cout << "Enter y: ";
        while (!t->scan(&y)) {
            std::cout << "Error! Enter value of correct type: ";
        }

        xs.Append(x);
        ys.Append(y);
    }

    std::cout << "Points entered\n";
}

template<typename T>
void case_print_points(const MutableArraySequence<double>& xs, const MutableArraySequence<T>& ys,
                       const TypeInfo* t) {
    print_points_title();
    if (xs.GetLength() == 0) {
        std::cout << "No points entered\n";
        return;
    }

    std::cout << "Points:\n";
    for (int index = 0; index < xs.GetLength(); index++) {
        std::cout << index << ": x = " << xs.Get(index) << ", y = ";
        T value = ys.Get(index);
        t->print(&value);
        std::cout << '\n';
    }
}

template<typename T>
void case_clear_points(MutableArraySequence<double>& xs, MutableArraySequence<T>& ys,
                       std::unique_ptr<PiecewiseFunction<T>>& spline) {
    xs = MutableArraySequence<double>();
    ys = MutableArraySequence<T>();
    spline.reset();

    print_clear_points_title();
    std::cout << "Points and spline cleared\n";
}

template<typename T>
void case_build_spline(const MutableArraySequence<double>& xs, const MutableArraySequence<T>& ys,
                       std::unique_ptr<PiecewiseFunction<T>>& spline) {
    print_build_spline_title();
    if (xs.GetLength() != ys.GetLength()) {
        std::cout << "xs and ys must have the same length\n";
        return;
    }

    if (xs.GetLength() < 2) {
        std::cout << "Not enough points to build spline\n";
        return;
    }

    for (int index = 1; index < xs.GetLength(); index++) {
        if (xs.Get(index) <= xs.Get(index - 1)) {
            std::cout << "x values must be strictly increasing\n";
            return;
        }
    }

    PiecewiseFunction<T> builtSpline = BuildNaturalCubicSpline<T>(xs, ys);
    spline = std::make_unique<PiecewiseFunction<T>>(builtSpline);

    std::cout << "Natural cubic spline has been built\n";
    std::cout << "Domain: [" << xs.GetFirst() << ", " << xs.GetLast() << "]\n";
}

template<typename T>
void case_evaluate_spline(PiecewiseFunction<T>* spline,
                          const TypeInfo* t) {
    print_evaluate_spline_title();
    if (spline == nullptr) {
        std::cout << "Spline is not built\n";
        return;
    }

    double x = 0.0;
    std::cout << "Enter x: ";
    while (!get_double_t()->scan(&x)) {
        std::cout << "Error! Enter a double: ";
    }

    T value = spline->EvaluateExtrapolation(x);
    std::cout << "S(x) = ";
    t->print(&value);
    std::cout << '\n';
}

template<typename T>
void print_polynomial_function(const PolynomialFunction<T>* polyF,
                               const TypeInfo* t) {
    if (polyF == nullptr) {
        std::cout << "Not a polynomial";
        return;
    }

    const MutableArraySequence<T>& coefficients = polyF->GetCoeffs();
    bool first = true;

    for (int power = coefficients.GetLength() - 1; power >= 0; --power) {
        T coefficient = coefficients.Get(power);

        if (coefficient == T{}) {
            continue;
        }

        if constexpr (std::is_same_v<T, Complex>) {
            if (!first) {
                std::cout << " + ";
            }
        } else {
            bool negative = coefficient < T{};

            if (!first) {
                std::cout << (negative ? " - " : " + ");
            } else if (negative) {
                std::cout << "-";
            }

            if (negative) {
                coefficient = T{} - coefficient;
            }
        }

        bool printCoefficient = true;
        if (power > 0 && coefficient == T{1}) {
            printCoefficient = false;
        }

        if (printCoefficient) {
            t->print(&coefficient);
        }

        if (power >= 1) {
            if (printCoefficient) {
                std::cout << "*";
            }
            std::cout << "x";
        }

        if (power >= 2) {
            std::cout << "^" << power;
        }

        first = false;
    }

    if (first) {
        T zero = T{};
        t->print(&zero);
    }
}

template<typename T>
void case_print_spline_formulas(PiecewiseFunction<T>* spline,
                                const TypeInfo* t) {
    print_spline_formulas_title();
    if (spline == nullptr) {
        std::cout << "Spline is not built\n";
        return;
    }


    for (int index = 0; index < spline->GetLength(); index++) {
        const Segment<T>& segment = spline->GetSegment(index);

        std::cout << "S_" << index << "(x), x in ["
                  << segment.left << ", " << segment.right << "] = ";

        const PolynomialFunction<T>* polyF = dynamic_cast<const PolynomialFunction<T>*>(segment.function.get());

        print_polynomial_function(polyF, t);
        std::cout << '\n';
    }
}

#endif