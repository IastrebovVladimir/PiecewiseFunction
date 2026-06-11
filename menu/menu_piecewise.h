#ifndef PIECEWISE_MENU_H
#define PIECEWISE_MENU_H

#include "../Lab_2/types.h"
#include "../Functions/PiecewiseFunction.h"

template<typename T>
void run_piecewise_menu(const TypeInfo* t);

template<typename T>
inline void print_piecewise_status(const std::unique_ptr<PiecewiseFunction<T>>& function);

template<typename T>
inline int read_piecewise_command(const std::unique_ptr<PiecewiseFunction<T>>& function);

template<typename T>
void create_piecewise_function(std::unique_ptr<PiecewiseFunction<T>>& function, const TypeInfo* t);

template<typename T>
void print_piecewise_function(PiecewiseFunction<T>* function, const TypeInfo* t);

template<typename T>
void evaluate_piecewise_function(PiecewiseFunction<T>* function, const TypeInfo* t);

template<typename T>
void redefine_piecewise_function(PiecewiseFunction<T>* function, const TypeInfo* t);

template<typename T>
void check_piecewise_continuity(PiecewiseFunction<T>* function);

template<typename T>
void check_piecewise_monotonicity(PiecewiseFunction<T>* function);

template<typename T>
void clear_piecewise_function(std::unique_ptr<PiecewiseFunction<T>>& function);

template<typename T>
void show_piecewise_graph(PiecewiseFunction<T>* function);

#include "menu_piecewise.hpp"
#include "menu_plot.hpp"

#endif