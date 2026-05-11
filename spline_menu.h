#ifndef SPLINE_MENU_H
#define SPLINE_MENU_H

#include "Lab_2/types.h"
#include "Lab_2/ArraySequence.h"
#include "PiecewiseFunction.h"

template<typename T>
void run_spline_menu(const TypeInfo* t);

template<typename T>
void case_enter_points(MutableArraySequence<double>& xs,
                       MutableArraySequence<T>& ys,
                       PiecewiseFunction<T>*& spline,
                       bool& splineBuilt,
                       const TypeInfo* t);

template<typename T>
void case_print_points(const MutableArraySequence<double>& xs,
                       const MutableArraySequence<T>& ys,
                       const TypeInfo* t);

template<typename T>
void case_clear_points(MutableArraySequence<double>& xs,
                       MutableArraySequence<T>& ys,
                       PiecewiseFunction<T>*& spline,
                       bool& splineBuilt);

template<typename T>
void case_build_spline(const MutableArraySequence<double>& xs,
                       const MutableArraySequence<T>& ys,
                       PiecewiseFunction<T>*& spline,
                       bool& splineBuilt);

template<typename T>
void case_evaluate_spline(PiecewiseFunction<T>* spline,
                          bool splineBuilt,
                          const TypeInfo* t);

template<typename T>
void case_print_spline_formulas(PiecewiseFunction<T>* spline, bool splineBuilt,const TypeInfo* t);

#include "spline_menu.hpp"

#endif