#include <iostream>
#include "../Lab_2/types.h"
#include "../Complex.h"
#include "spline_menu.h"

int main() {
    int choice = 0;

    print_spline_menu_title();
    std::cout << "Choose point type for spline:\n";
    std::cout << "1. int\n";
    std::cout << "2. double\n";
    std::cout << "3. complex\n";
    std::cout << "Enter choice: ";

    while (!get_int_t()->scan(&choice) || choice < 1 || choice > 3) {
        std::cout << "Error! Enter an integer from 1 to 3: ";
    }

    switch (choice) {
        case 1:
            run_spline_menu<int>(get_int_t());
            break;

        case 2:
            run_spline_menu<double>(get_double_t());
            break;
        case 3:
            run_spline_menu<Complex>(get_complex_t());
            break;
    }

    return 0;
}