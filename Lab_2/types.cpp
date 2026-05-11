#include "types.h"
#include "../Complex.h"

#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <limits>

// Int и Double
void sumInt(const void* arg1, const void* arg2, void* result) {
    *static_cast<int*>(result) =
            *static_cast<const int*>(arg1) + *static_cast<const int*>(arg2);
}

void sumDouble(const void* arg1, const void* arg2, void* result) {
    *static_cast<double*>(result) =
            *static_cast<const double*>(arg1) + *static_cast<const double*>(arg2);
}

void subInt(const void* arg1, const void* arg2, void* result) {
    *static_cast<int*>(result) =
            *static_cast<const int*>(arg1) - *static_cast<const int*>(arg2);
}

void subDouble(const void* arg1, const void* arg2, void* result) {
    *static_cast<double*>(result) =
            *static_cast<const double*>(arg1) - *static_cast<const double*>(arg2);
}

void mulInt(const void* arg1, const void* arg2, void* result) {
    *static_cast<int*>(result) =
            *static_cast<const int*>(arg1) * *static_cast<const int*>(arg2);
}

void mulDouble(const void* arg1, const void* arg2, void* result) {
    *static_cast<double*>(result) =
            *static_cast<const double*>(arg1) * *static_cast<const double*>(arg2);
}

void negInt(void* arg) {
    *static_cast<int*>(arg) = -*static_cast<int*>(arg);
}

void negDouble(void* arg) {
    *static_cast<double*>(arg) = -*static_cast<double*>(arg);
}

void zeroInt(void* arg) {
    *static_cast<int*>(arg) = 0;
}

void zeroDouble(void* arg) {
    *static_cast<double*>(arg) = 0.0;
}

void printInt(const void* arg) {
    std::cout << *static_cast<const int*>(arg);
}

void printDouble(const void* arg) {
    std::cout << *static_cast<const double*>(arg);
}

int scanInt(void* arg) {
    int* valuePtr = static_cast<int*>(arg);
    std::string line;

    if (!std::getline(std::cin, line)) {
        return 0;
    }

    auto first = line.find_first_not_of(" \t");
    if (first == std::string::npos) {
        return 0;
    }
    auto last = line.find_last_not_of(" \t");
    line = line.substr(first, last - first + 1);

    std::stringstream ss(line);
    int value;

    if (!(ss >> value)) {
        return 0;
    }
    ss >> std::ws;
    if (!ss.eof()) {
        return 0;
    }

    *valuePtr = value;
    return 1;
}

int scanDouble(void* arg) {
    double* valuePtr = static_cast<double*>(arg);
    std::string line;

    if (!std::getline(std::cin, line)) {
        return 0;
    }

    auto first = line.find_first_not_of(" \t");
    if (first == std::string::npos) {
        return 0;
    }
    auto last = line.find_last_not_of(" \t");
    line = line.substr(first, last - first + 1);

    std::stringstream ss(line);
    double value;


    if (!(ss >> value)) {
        return 0;
    }
    ss >> std::ws;
    if (!ss.eof()) {
        return 0;
    }

    *valuePtr = value;
    return 1;
}

// комплексные числа

void sumComplex(const void* arg1, const void* arg2, void* result) {
    const Complex* a = static_cast<const Complex*>(arg1);
    const Complex* b = static_cast<const Complex*>(arg2);
    Complex* r = static_cast<Complex*>(result);

    r->re = a->re + b->re;
    r->im = a->im + b->im;
}

void subComplex(const void* arg1, const void* arg2, void* result) {
    const Complex* a = static_cast<const Complex*>(arg1);
    const Complex* b = static_cast<const Complex*>(arg2);
    Complex* r = static_cast<Complex*>(result);

    r->re = a->re - b->re;
    r->im = a->im - b->im;
}

void mulComplex(const void* arg1, const void* arg2, void* result) {
    const Complex* a = static_cast<const Complex*>(arg1);
    const Complex* b = static_cast<const Complex*>(arg2);
    Complex* r = static_cast<Complex*>(result);

    r->re = a->re * b->re - a->im * b->im;
    r->im = a->re * b->im + a->im * b->re;
}

void negComplex(void* arg) {
    Complex* z = static_cast<Complex*>(arg);
    z->re = -z->re;
    z->im = -z->im;
}

void zeroComplex(void* arg) {
    Complex* z = static_cast<Complex*>(arg);
    z->re = 0.0;
    z->im = 0.0;
}

void printComplex(const void* arg) {
    const Complex* z = static_cast<const Complex*>(arg);
    std::cout << "(" << z->re << ", " << z->im << ")";
}

int scanComplex(void* arg) {
    Complex* z = static_cast<Complex*>(arg);
    std::string line;

    if (!std::getline(std::cin, line)) {
        return 0;
    }

    std::stringstream ss(line);
    double re, im;

    if (!(ss >> re >> im)) {
        return 0;
    }

    ss >> std::ws;
    if (!ss.eof()) {
        return 0;
    }

    z->re = re;
    z->im = im;
    return 1;
}

std::string int_to_string(const void* arg) {
    return std::to_string(*static_cast<const int*>(arg));
}

std::string double_to_string(const void* arg) {
    return std::to_string(*static_cast<const double*>(arg));
}

std::string complex_to_string(const void* arg) {
    const Complex* z = static_cast<const Complex*>(arg);
    return "(" + std::to_string(z->re) + ", " + std::to_string(z->im) + ")";
}

const TypeInfo *get_int_t(void) {
    static TypeInfo int_t;
    static int inited = 0;

    if (!inited) {
        int_t.size = sizeof(int);
        int_t.sum = sumInt;
        int_t.sub = subInt;
        int_t.neg = negInt;
        int_t.mul = mulInt;
        int_t.zero = zeroInt;
        int_t.print = printInt;
        int_t.scan = scanInt;
        int_t.to_string = int_to_string;
        inited = 1;
    }
    return &int_t;
}

const TypeInfo *get_double_t(void) {
    static TypeInfo double_t;
    static int inited = 0;

    if (!inited) {
        double_t.size = sizeof(double);
        double_t.sum = sumDouble;
        double_t.sub = subDouble;
        double_t.neg = negDouble;
        double_t.mul = mulDouble;
        double_t.zero = zeroDouble;
        double_t.print = printDouble;
        double_t.scan = scanDouble;
        double_t.to_string = double_to_string;
        inited = 1;
    }
    return &double_t;
}

const TypeInfo *get_complex_t(void) {
    static TypeInfo complex_t;
    static int inited = 0;

    if (!inited) {
        complex_t.size = sizeof(Complex);
        complex_t.sum = sumComplex;
        complex_t.sub = subComplex;
        complex_t.neg = negComplex;
        complex_t.mul = mulComplex;
        complex_t.zero = zeroComplex;
        complex_t.print = printComplex;
        complex_t.scan = scanComplex;
        complex_t.to_string = complex_to_string;
        inited = 1;
    }
    return &complex_t;
}