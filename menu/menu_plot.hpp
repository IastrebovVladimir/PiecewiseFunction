#ifndef PIECEWISE_PLOT_HPP
#define PIECEWISE_PLOT_HPP

#include <cstdlib>
#include <fstream>
#include <iostream>

#include "../Complex.h"
#include "../Functions/PiecewiseFunction.h"

template<typename T>
void show_piecewise_graph(PiecewiseFunction<T>* function) {
    if (!function) {
        std::cout << "Piecewise function is not created.\n";
        return;
    }

    const double left = function->GetLeft();
    const double right = function->GetRight();
    const int samples = 2000;

    std::ofstream data("piecewise_plot.dat");
    if (!data.is_open()) {
        std::cout << "Error: cannot create data file.\n";
        return;
    }

    for (int index = 0; index <= samples; index++) {
        double x = left + (right - left) * index / samples;
        double y = static_cast<double>(function->Evaluate(x));
        data << x << ' ' << y << '\n';
    }
    data.close();

    std::ofstream script("piecewise_plot.gp");
    if (!script.is_open()) {
        std::cout << "Error: cannot create gnuplot script.\n";
        return;
    }

    script << "set title 'Piecewise Function'\n";
    script << "set grid\n";
    script << "set xlabel 'x'\n";
    script << "set ylabel 'y'\n";
    script << "plot 'piecewise_plot.dat' using 1:2 with lines linewidth 2 title 'f(x)'\n";
    script.close();

    int result = std::system("gnuplot -persist piecewise_plot.gp");

    if (result != 0) {
        std::cout << "Error: failed to run gnuplot.\n";
        std::cout << "Make sure gnuplot is installed and added to PATH.\n";
    }
}

template<>
inline void show_piecewise_graph<Complex>(PiecewiseFunction<Complex>* function) {
    (void)function;
    std::cout << "Graph is not supported for complex-valued functions.\n";
}

#endif