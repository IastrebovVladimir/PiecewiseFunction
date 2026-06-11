#ifndef PIECEWISE_MENU_HPP
#define PIECEWISE_MENU_HPP

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>

#include "../Lab_2/types.h"
#include "../Lab_2/ArraySequence.h"
#include "../Functions/PiecewiseFunction.h"
#include "../Functions/PolynomialFunction.h"
#include "../Functions/Read.h"
#include "../Complex.h"
#include "menu_plot.hpp"


inline void print_piecewise_menu_title() {
    std::cout << "=====================================\n";
    std::cout << "       Piecewise Function Menu       \n";
    std::cout << "=====================================\n";
}

inline void print_create_piecewise_title() {
    std::cout << "=====================================\n";
    std::cout << "       Create PiecewiseFunction      \n";
    std::cout << "=====================================\n";
}

inline void print_show_piecewise_title() {
    std::cout << "=====================================\n";
    std::cout << "       Print PiecewiseFunction       \n";
    std::cout << "=====================================\n";
}

inline void print_evaluate_piecewise_title() {
    std::cout << "=====================================\n";
    std::cout << "      Evaluate PiecewiseFunction     \n";
    std::cout << "=====================================\n";
}

inline void print_redefine_piecewise_title() {
    std::cout << "=====================================\n";
    std::cout << "          Redefine Interval          \n";
    std::cout << "=====================================\n";
}

inline void print_continuity_piecewise_title() {
    std::cout << "=====================================\n";
    std::cout << "          Check Continuity           \n";
    std::cout << "=====================================\n";
}

inline void print_monotonicity_piecewise_title() {
    std::cout << "=====================================\n";
    std::cout << "         Check Monotonicity          \n";
    std::cout << "=====================================\n";
}

inline void print_clear_piecewise_title() {
    std::cout << "=====================================\n";
    std::cout << "       Clear PiecewiseFunction       \n";
    std::cout << "=====================================\n";
}

template<typename T>
inline void print_piecewise_status(const std::unique_ptr<PiecewiseFunction<T>>& function) {
    if (!function) {
        std::cout << "Status: function is not created\n";
    } else {
        std::cout << "Status: function is created\n";
        std::cout << "Domain: [" << function->GetLeft()
                  << ", " << function->GetRight() << "]\n";
    }
    std::cout << '\n';
}

template<typename T>
inline int read_piecewise_command(const std::unique_ptr<PiecewiseFunction<T>>& function) {
    int command = 0;
    std::cout << "Choose: ";

    if (!function) {
        while (!get_int_t()->scan(&command) || (command != 0 && command != 1)) {
            std::cout << "Error! Enter 0 or 1: ";
        }
    } else {
        while (!get_int_t()->scan(&command) || command < 0 || command > 8) {
            std::cout << "Error! Enter an integer from 0 to 7: ";
        }
    }

    return command;
}

template<typename T>
void run_piecewise_menu(const TypeInfo* t) {
    std::unique_ptr<PiecewiseFunction<T>> function;

    while (true) {
        print_piecewise_menu_title();
        print_piecewise_status(function);

        if (!function) {
            std::cout << "1. Create PiecewiseFunction\n";
            std::cout << "0. Exit\n";
        } else {
            std::cout << "1. Recreate PiecewiseFunction\n";
            std::cout << "2. Print PiecewiseFunction\n";
            std::cout << "3. Evaluate PiecewiseFunction\n";
            std::cout << "4. Redefine Interval\n";
            std::cout << "5. Check Continuity\n";
            std::cout << "6. Check Monotonicity\n";
            std::cout << "7. Clear PiecewiseFunction\n";
            std::cout << "8. Plot Menu\n";
            std::cout << "0. Exit\n";
        }

        std::cout << '\n';
        int command = read_piecewise_command(function);

        if (!function) {
            switch (command) {
                case 1:
                    create_piecewise_function(function, t);
                    break;
                case 0:
                    std::cout << "Exit.\n";
                    return;
                default:
                    break;
            }
        } else {
            switch (command) {
                case 1:
                    create_piecewise_function(function, t);
                    break;
                case 2:
                    print_piecewise_function(function.get(), t);
                    break;
                case 3:
                    evaluate_piecewise_function(function.get(), t);
                    break;
                case 4:
                    redefine_piecewise_function(function.get(), t);
                    break;
                case 5:
                    check_piecewise_continuity(function.get());
                    break;
                case 6:
                    check_piecewise_monotonicity(function.get());
                    break;
                case 7:
                    clear_piecewise_function(function);
                    break;
                case 8:
                    show_piecewise_graph(function.get());
                    break;
                case 0:
                    std::cout << "Exit.\n";
                    return;
                default:
                    break;
            }
        }
    }
}

template<typename T>
void create_piecewise_function(std::unique_ptr<PiecewiseFunction<T>>& function, const TypeInfo* t)
{
    print_create_piecewise_title();

    function.reset();

    int count = 0;
    std::cout << "Enter number of segments: ";
    while (!get_int_t()->scan(&count) || count <= 0) {
        std::cout << "Error! Enter integer > 0: ";
    }

    MutableArraySequence<Segment<T>> segments;

    for (int index = 0; index < count; ++index) {
        double left = 0.0;
        double right = 0.0;

        std::cout << "\nSegment " << index + 1 << '\n';

        std::cout << "Enter left border: ";
        while (!get_double_t()->scan(&left)) {
            std::cout << "Error! Enter a double: ";
        }

        std::cout << "Enter right border: ";
        while (!get_double_t()->scan(&right) || right <= left) {
            std::cout << "Error! Enter right > left: ";
        }

        if (index > 0 &&
            std::abs(left - segments.Get(index - 1).right) > 1e-9) {
            std::cout << "Error! Segments must be contiguous.\n";
            std::cout << "Left border must be equal to previous right border.\n";
            --index;
            continue;
        }

        std::cout << "Enter function on segment:\n";
        auto func_on_segment = ReadFunction<T>(t);
        segments.Append(Segment<T>(left, right, *func_on_segment));
    }

    function = std::make_unique<PiecewiseFunction<T>>(segments);

    std::cout << "\nPiecewise function created.\n";
    std::cout << "Domain: [" << function->GetLeft()
              << ", " << function->GetRight() << "]\n";
}

template<typename T>
void print_piecewise_function(PiecewiseFunction<T>* function, const TypeInfo* t) {
    print_show_piecewise_title();

    if (!function) {
        std::cout << "Piecewise function is not created.\n";
        return;
    }

    std::cout << function->ToString(t) << '\n';
}

template<typename T>
void evaluate_piecewise_function(PiecewiseFunction<T>* function, const TypeInfo* t) {
    print_evaluate_piecewise_title();

    if (!function) {
        std::cout << "Piecewise function is not created.\n";
        return;
    }

    double x = 0.0;
    std::cout << "Enter x: ";
    while (!get_double_t()->scan(&x)) {
        std::cout << "Error! Enter a double: ";
    }

    try {
        T value = function->Evaluate(x);
        std::cout << "f(" << x << ") = ";
        t->print(&value);
        std::cout << '\n';
    } catch (const std::exception& error) {
        std::cout << "Error: " << error.what() << '\n';
    }
}

template<typename T>
void redefine_piecewise_function(PiecewiseFunction<T>* function, const TypeInfo* t) {
    print_redefine_piecewise_title();

    if (!function) {
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

    try {
        auto replacement = ReadFunction<T>(t);
        function->Redefine(a, b, *replacement);
        std::cout << "Function redefined on [" << a << ", " << b << "].\n";
    } catch (const std::exception& error) {
        std::cout << "Error: " << error.what() << '\n';
    }
}

template<typename T>
void check_piecewise_continuity(PiecewiseFunction<T>* function) {
    print_continuity_piecewise_title();

    if (!function) {
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
void check_piecewise_monotonicity(PiecewiseFunction<T>* function) {
    print_monotonicity_piecewise_title();

    if (!function) {
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
inline void check_piecewise_monotonicity<Complex>(PiecewiseFunction<Complex>* function) {
    print_monotonicity_piecewise_title();

    if (!function) {
        std::cout << "Piecewise function is not created.\n";
        return;
    }

    std::cout << "Monotonicity is not defined for complex-valued functions.\n";
}

template<typename T>
void clear_piecewise_function(std::unique_ptr<PiecewiseFunction<T>>& function) {
    print_clear_piecewise_title();

    if (!function) {
        std::cout << "Piecewise function is not created.\n";
        return;
    }

    function.reset();
    std::cout << "Piecewise function cleared.\n";
}


#endif
