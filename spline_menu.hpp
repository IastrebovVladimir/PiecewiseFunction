#ifndef SPLINE_MENU_HPP
#define SPLINE_MENU_HPP

#include <iostream>
#include <stdexcept>
#include <complex>

#include "Lab_2/types.h"
#include "Lab_2/ArraySequence.h"
#include "PiecewiseFunction.h"
#include "SpliteInterpolation.h"

template<typename T>
void run_spline_menu(const TypeInfo* t) {
    MutableArraySequence<double> xs;
    MutableArraySequence<T> ys;

    PiecewiseFunction<T>* spline = nullptr;
    bool splineBuilt = false;

    std::cout << "\n1. Enter points\n";
    std::cout << "2. Print points\n";
    std::cout << "3. Clear points\n";
    std::cout << "4. Build natural cubic spline\n";
    std::cout << "5. Evaluate spline at x\n";
    std::cout << "6. Print spline formulas\n";
    std::cout << "0. Exit\n";

    while (true) {
        int command = 0;
        std::cout << "Choose: ";

        while (!get_int_t()->scan(&command) || command < 0 || command > 6) {
            std::cout << "Error! Enter an integer from 0 to 6: ";
        }

        switch (command) {
            case 1:
                case_enter_points<T>(xs, ys, spline, splineBuilt, t);
                break;

            case 2:
                case_print_points<T>(xs, ys, t);
                break;

            case 3:
                case_clear_points<T>(xs, ys, spline, splineBuilt);
                break;

            case 4:
                case_build_spline<T>(xs, ys, spline, splineBuilt);
                break;

            case 5:
                case_evaluate_spline<T>(spline, splineBuilt, t);
                break;

            case 6:
                case_print_spline_formulas(spline, splineBuilt, t);
                break;

            case 0:
                delete spline;
                std::cout << "Exit.\n";
                return;

            default:
                std::cout << "Unknown command.\n";
        }
    }
}

template<typename T>
void case_enter_points(MutableArraySequence<double>& xs,
                       MutableArraySequence<T>& ys,
                       PiecewiseFunction<T>*& spline,
                       bool& splineBuilt,
                       const TypeInfo* t) {
    xs = MutableArraySequence<double>();
    ys = MutableArraySequence<T>();

    if (spline != nullptr) {
        delete spline;
        spline = nullptr;
    }

    splineBuilt = false;

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
void case_print_points(const MutableArraySequence<double>& xs,
                       const MutableArraySequence<T>& ys,
                       const TypeInfo* t) {
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
void case_clear_points(MutableArraySequence<double>& xs,
                       MutableArraySequence<T>& ys,
                       PiecewiseFunction<T>*& spline,
                       bool& splineBuilt) {
    xs = MutableArraySequence<double>();
    ys = MutableArraySequence<T>();

    if (spline != nullptr) {
        delete spline;
        spline = nullptr;
    }

    splineBuilt = false;
    std::cout << "Points and spline cleared\n";
}

template<typename T>
void case_build_spline(const MutableArraySequence<double>& xs,
                       const MutableArraySequence<T>& ys,
                       PiecewiseFunction<T>*& spline,
                       bool& splineBuilt) {
    if (xs.GetLength() != ys.GetLength()) {
        std::cout << "xs and ys must have the same length\n";
        splineBuilt = false;
        return;
    }

    if (xs.GetLength() < 2) {
        std::cout << "Not enough points to build spline\n";
        splineBuilt = false;
        return;
    }

    for (int index = 1; index < xs.GetLength(); index++) {
        if (xs.Get(index) <= xs.Get(index - 1)) {
            std::cout << "x values must be strictly increasing\n";
            splineBuilt = false;
            return;
        }
    }

    if (spline != nullptr) {
        delete spline;
        spline = nullptr;
    }

    PiecewiseFunction<T> builtSpline = BuildNaturalCubicSpline<T>(xs, ys);
    spline = new PiecewiseFunction<T>(builtSpline);

    splineBuilt = true;
    std::cout << "Natural cubic spline has been built\n";
    std::cout << "Domain: [" << xs.GetFirst() << ", " << xs.GetLast() << "]\n";
}

template<typename T>
void case_evaluate_spline(PiecewiseFunction<T>* spline,
                          bool splineBuilt,
                          const TypeInfo* t) {
    if (!splineBuilt || spline == nullptr) {
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
void case_print_spline_formulas(PiecewiseFunction<T>* spline,
                                bool splineBuilt,
                                const TypeInfo* t) {
    if (!splineBuilt || spline == nullptr) {
        std::cout << "Spline is not built\n";
        return;
    }

    for (int index = 0; index < spline->GetLength(); index++) {
        const Segment<T>& segment = spline->Get(index);

        std::cout << "S_" << index << "(x), x in [" << segment.left << ", " << segment.right << "] = ";

        const PolynomialFunction<T>* polyF = dynamic_cast<const PolynomialFunction<T>*>(segment.function);

        if (polyF == nullptr) {
            std::cout << "Not a polynomial\n";
            continue;
        }

        const MutableArraySequence<T>& coefficients = polyF->GetCoeffs();

        for (int power = 0; power < coefficients.GetLength(); power++) {
            if (power > 0) {
                std::cout << " + ";
            }

            T coefficient = coefficients.Get(power);
            t->print(&coefficient);

            if (power >= 1) {
                std::cout << "*x";
            }
            if (power >= 2) {
                std::cout << "^" << power;
            }
        }

        std::cout << '\n';
    }
}

#endif