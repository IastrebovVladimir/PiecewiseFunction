#ifndef SPLINE_MENU_H
#define SPLINE_MENU_H

#include <memory>

#include "../Lab_2/types.h"
#include "../Lab_2/ArraySequence.h"
#include "../Functions/PiecewiseFunction.h"
#include "../Functions/PolynomialFunction.h"

template<typename T>
void run_spline_menu(const TypeInfo* t);

template<typename T>
void case_enter_points(MutableArraySequence<double>& xs, MutableArraySequence<T>& ys,
                       std::unique_ptr<PiecewiseFunction<T>>& spline,
                       const TypeInfo* t);

template<typename T>
void case_print_points(const MutableArraySequence<double>& xs, const MutableArraySequence<T>& ys,
                       const TypeInfo* t);

template<typename T>
void case_clear_points(MutableArraySequence<double>& xs, MutableArraySequence<T>& ys,
                       std::unique_ptr<PiecewiseFunction<T>>& spline);

template<typename T>
void case_build_spline(const MutableArraySequence<double>& xs, const MutableArraySequence<T>& ys,
                       std::unique_ptr<PiecewiseFunction<T>>& spline);

template<typename T>
void case_evaluate_spline(PiecewiseFunction<T>* spline,
                          const TypeInfo* t);

template<typename T>
void print_polynomial_function(const PolynomialFunction<T>* polyF,
                               const TypeInfo* t);

template<typename T>
void case_print_spline_formulas(PiecewiseFunction<T>* spline,
                                const TypeInfo* t);

#include "spline_menu.hpp"

#endif