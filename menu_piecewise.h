#ifndef PIECEWISE_MENU_H
#define PIECEWISE_MENU_H

#include "Lab_2/types.h"
#include "PiecewiseFunction.h"

template<typename T>
void run_piecewise_menu(const TypeInfo* t);

template<typename T>
void case_enter_piecewise(PiecewiseFunction<T>*& function, bool& functionBuilt, const TypeInfo* t);

template<typename T>
void case_print_piecewise(PiecewiseFunction<T>* function, bool functionBuilt, const TypeInfo* t);

template<typename T>
void case_evaluate_piecewise(PiecewiseFunction<T>* function, bool functionBuilt, const TypeInfo* t);

template<typename T>
void case_redefine_piecewise(PiecewiseFunction<T>* function, bool functionBuilt, const TypeInfo* t);

template<typename T>
void case_check_continuity(PiecewiseFunction<T>* function, bool functionBuilt);

template<typename T>
void case_check_monotonicity(PiecewiseFunction<T>* function, bool functionBuilt);

template<typename T>
void case_clear_piecewise(PiecewiseFunction<T>*& function, bool& functionBuilt);

#include "menu_piecewise.hpp"

#endif
