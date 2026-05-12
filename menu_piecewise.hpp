#ifndef PIECEWISE_MENU_HPP
#define PIECEWISE_MENU_HPP

#include <stdexcept>

#include "Lab_2/types.h"
#include "Lab_2/ArraySequence.h"
#include "PiecewiseFunction.h"
#include "PolynomialFunction.h"
#include "Read.h"

template<typename T>
void run_piecewise_menu(const TypeInfo* t) {
    PiecewiseFunction<T>* function = nullptr;
    bool functionBuilt = false;

    std::cout << "\n1. Enter piecewise function\n";
    std::cout << "2. Print piecewise function\n";
    std::cout << "3. Evaluate at x\n";
    std::cout << "4. Redefine interval [a, b]\n";
    std::cout << "5. Check continuity\n";
    std::cout << "6. Check monotonicity\n";
    std::cout << "7. Clear function\n";
    std::cout << "0. Exit\n";

    while (true) {
        int command = 0;
        std::cout << "Choose: ";

        while (!get_int_t()->scan(&command) || command < 0 || command > 7) {
            std::cout << "Error! Enter an integer from 0 to 7: ";
        }

        switch (command) {
            case 1:
                case_enter_piecewise(function, functionBuilt, t);
                break;

            case 2:
                case_print_piecewise(function, functionBuilt, t);
                break;

            case 3:
                case_evaluate_piecewise(function, functionBuilt, t);
                break;

            case 4:
                case_redefine_piecewise(function, functionBuilt, t);
                break;

            case 5:
                case_check_continuity(function, functionBuilt);
                break;

            case 6:
                case_check_monotonicity(function, functionBuilt);
                break;

            case 7:
                case_clear_piecewise(function, functionBuilt);
                break;
            case 0:
                delete function;
                std::cout << "Exit.\n";
                return;

            default:
                std::cout << "Unknown command.\n";
        }
    }
}

template<typename T>
void case_enter_piecewise(PiecewiseFunction<T>*& function,
                          bool& functionBuilt,
                          const TypeInfo* t) {
    if (function != nullptr) {
        delete function;
        function = nullptr;
    }

    functionBuilt = false;

    int count = 0;
    std::cout << "Enter number of segments: ";
    while (!get_int_t()->scan(&count) || count <= 0) {
        std::cout << "Error! Enter integer > 0: ";
    }

    MutableArraySequence<Segment<T>> segments;

    for (int index = 0; index < count; index++) {
        double left = 0.0;
        double right = 0.0;

        std::cout << "Segment " << index << '\n';

        std::cout << "Enter left border: ";
        while (!get_double_t()->scan(&left)) {
            std::cout << "Error! Enter a double: ";
        }

        std::cout << "Enter right border: ";
        while (!get_double_t()->scan(&right) || right <= left) {
            std::cout << "Error! Enter right > left: ";
        }

        if (index > 0 && left != segments.Get(index - 1).right) {
            std::cout << "Error! Segments must be contiguous.\n";
            std::cout << "Left border must be equal to previous right border.\n";
            index--;
            continue;
        }

        Function<T>* funcOnSegment = ReadFunction<T>(t);
        segments.Append(Segment<T>(left, right, funcOnSegment));
        delete funcOnSegment;
    }

    function = new PiecewiseFunction<T>(segments);
    functionBuilt = true;
    std::cout << "Piecewise function created.\n";
    std::cout << "Domain: [" << function->GetLeft() << ", " << function->GetRight() << "]\n";
}

template<typename T>
void case_print_piecewise(PiecewiseFunction<T>* function, bool functionBuilt, const TypeInfo* t) {
    if (!functionBuilt || function == nullptr) {
        std::cout << "Piecewise function is not created.\n";
        return;
    }

    std::cout << "Piecewise function:\n";

    for (int index = 0; index < function->GetLength(); index++) {
        const Segment<T>& segment = function->Get(index);

        std::cout << index << ": [" << segment.left << ", " << segment.right << "] -> ";
        std::cout << segment.function->ToString(t);
        std::cout << '\n';
    }
}

template<typename T>
void case_evaluate_piecewise(PiecewiseFunction<T>* function,
                             bool functionBuilt,
                             const TypeInfo* t) {
    if (!functionBuilt || function == nullptr) {
        std::cout << "Piecewise function is not created.\n";
        return;
    }

    double x = 0.0;
    std::cout << "Enter x: ";
    while (!get_double_t()->scan(&x)) {
        std::cout << "Error! Enter a double: ";
    }

    T value = function->Evaluate(x);
    std::cout << "f(x) = ";
    t->print(&value);
    std::cout << '\n';
}

template<typename T>
void case_redefine_piecewise(PiecewiseFunction<T>* function,
                             bool functionBuilt,
                             const TypeInfo* t) {
    if (!functionBuilt || function == nullptr) {
        std::cout << "Piecewise function is not created.\n";
        return;
    }

    double a = 0.0;
    double b = 0.0;

    std::cout << "Enter a: ";
    while (!get_double_t()->scan(&a)) {
        std::cout << "Error! Enter a double: ";
    }

    std::cout << "Enter b: ";
    while (!get_double_t()->scan(&b) || b <= a) {
        std::cout << "Error! Enter b > a: ";
    }

    std::cout << "Enter replacement function:\n";
    Function<T>* funcOnSegment = ReadFunction<T>(t);
    function->Redefine(a, b, funcOnSegment);
    delete funcOnSegment;
    std::cout << "Function redefined on [" << a << ", " << b << "].\n";
}

template<typename T>
void case_check_continuity(PiecewiseFunction<T>* function,
                           bool functionBuilt) {
    if (!functionBuilt || function == nullptr) {
        std::cout << "Piecewise function is not created.\n";
        return;
    }

    if (function->IsContinuous()) {
        std::cout << "Function is continuous.\n";
    } else {
        std::cout << "Function is not continuous.\n";
    }
}

template<typename T>
void case_check_monotonicity(PiecewiseFunction<T>* function,
                             bool functionBuilt) {
    if (!functionBuilt || function == nullptr) {
        std::cout << "Piecewise function is not created.\n";
        return;
    }

    int code = function->CheckMonotonicity();

    if (code == 1) {
        std::cout << "Function is decreasing.\n";
    } else if (code == 2) {
        std::cout << "Function is increasing.\n";
    } else {
        std::cout << "Function is not monotonic.\n";
    }
}

template<>
inline void case_check_monotonicity<Complex>(PiecewiseFunction<Complex>* function, bool functionBuilt) {
    if (!functionBuilt || function == nullptr) {
        std::cout << "Piecewise function is not created.\n";
        return;
    }

    std::cout << "Monotonicity is not defined for complex-valued functions.\n";
}

template<typename T>
void case_clear_piecewise(PiecewiseFunction<T>*& function,
                          bool& functionBuilt) {
    if (function != nullptr) {
        delete function;
        function = nullptr;
    }

    functionBuilt = false;
    std::cout << "Piecewise function cleared.\n";
}

#endif
