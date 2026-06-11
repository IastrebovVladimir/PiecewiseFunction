#ifndef LAB_3_READ_H
#define LAB_3_READ_H

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include "PolynomialFunction.h"
#include "RationalFunction.h"
#include "../Lab_2/types.h"

template<typename T>
std::unique_ptr<Function<T>> ReadFunction(const TypeInfo* t) {
    int choice = 0;

    std::cout << "Choose function type:\n";
    std::cout << "1. Polynomial function\n";
    std::cout << "2. Rational function\n";
    std::cout << "Enter choice: ";

    while (!get_int_t()->scan(&choice) || choice < 1 || choice > 2) {
        std::cout << "Error! Enter an integer from 1 to 2: ";
    }

    switch (choice) {
        case 1:
            return ReadPolynomial<T>(t);

        case 2:
            return ReadRationalFunction<T>(t);

        default:
            throw std::logic_error("Unknown function type");
    }
}

template<typename T>
std::unique_ptr<PolynomialFunction<T>> ReadPolynomial(const TypeInfo* t) {
    int degree = 0;

    std::cout << "Enter polynomial degree: ";
    while (!get_int_t()->scan(&degree) || degree < 0) {
        std::cout << "Error! Enter a non-negative integer: ";
    }

    MutableArraySequence<T> coeffsRead;

    std::cout << "Enter coefficients from c0 to c" << degree << ":\n";

    for (int index = 0; index <= degree; ++index) {
        T value{};

        std::cout << "Enter coefficient for x^" << index << ": ";
        while (!t->scan(&value)) {
            std::cout << "Error! Enter value of correct type: ";
        }

        coeffsRead.Append(value);
    }

    return std::make_unique<PolynomialFunction<T>>(coeffsRead);
}

template<typename T>
std::unique_ptr<RationalFunction<T>> ReadRationalFunction(const TypeInfo* t) {
    std::cout << "Enter numerator polynomial:\n";
    auto numerator = ReadPolynomial<T>(t);

    std::cout << "Enter denominator polynomial:\n";
    auto denominator = ReadPolynomial<T>(t);

    return std::make_unique<RationalFunction<T>>(*numerator, *denominator);
}

#endif