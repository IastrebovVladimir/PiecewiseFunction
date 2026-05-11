#ifndef LAB_3_RATIONALFUNCTION_H
#define LAB_3_RATIONALFUNCTION_H

#define EPS 1e-7

#include <cmath>
#include <stdexcept>

#include "Function.h"
#include "PolynomialFunction.h"
#include "Lab_2/ArraySequence.h"
#include "Lab_2/types.h"

template<typename T>
class RationalFunction : public Function<T> {
private:
    PolynomialFunction<T> numerator;
    PolynomialFunction<T> denominator;

public:
    RationalFunction(const PolynomialFunction<T>& numerator, const PolynomialFunction<T>& denominator)
            : numerator(numerator), denominator(denominator) {}

    RationalFunction(const RationalFunction& other)
            : numerator(other.numerator), denominator(other.denominator) {}

    RationalFunction& operator=(const RationalFunction& other) {
        if (this != &other) {
            numerator = other.numerator;
            denominator = other.denominator;
        }
        return *this;
    }

    T Evaluate(double x) const override {
        T denominatorValue = denominator.Evaluate(x);

        if (denominatorValue == T{}) {
            throw std::domain_error("RationalFunction: denominator is zero");
        }

        return numerator.Evaluate(x) / denominatorValue;
    }

    RationalFunction<T>* Derivative() const override {
        PolynomialFunction<T>* numeratorDerivative = numerator.Derivative();
        PolynomialFunction<T>* denominatorDerivative = denominator.Derivative();

        PolynomialFunction<T> newNumerator = (*numeratorDerivative) * denominator - numerator * (*denominatorDerivative);

        PolynomialFunction<T> newDenominator = denominator * denominator;

        delete numeratorDerivative;
        delete denominatorDerivative;

        return new RationalFunction<T>(newNumerator, newDenominator);
    }

    std::string ToString(const TypeInfo* t) const {
        return "(" + numerator.ToString(t) + ") / (" + denominator.ToString(t) + ")";
    }

    Function<T>* Clone() const override {
        return new RationalFunction<T>(*this);
    }

    PolynomialFunction<T> GetNumerator() const {
        return numerator;
    }

    PolynomialFunction<T> GetDenominator() const {
        return denominator;
    }
};

#endif