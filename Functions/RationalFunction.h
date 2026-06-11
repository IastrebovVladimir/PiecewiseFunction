#ifndef LAB_3_RATIONALFUNCTION_H
#define LAB_3_RATIONALFUNCTION_H

#include <memory>
#include <stdexcept>
#include <string>

#include "Function.h"
#include "PolynomialFunction.h"

template<typename T>
class RationalFunction : public Function<T> {
private:
    PolynomialFunction<T> numerator;
    PolynomialFunction<T> denominator;

public:
    RationalFunction(const PolynomialFunction<T>& numerator,
                     const PolynomialFunction<T>& denominator)
            : numerator(numerator), denominator(denominator) {}

    RationalFunction(const RationalFunction& other) = default;
    RationalFunction& operator=(const RationalFunction& other) = default;
    ~RationalFunction() override = default;

    T Evaluate(double x) const override {
        T denominatorValue = denominator.Evaluate(x);

        if (denominatorValue == T{}) {
            throw std::domain_error("RationalFunction: denominator is zero");
        }

        return numerator.Evaluate(x) / denominatorValue;
    }

    std::unique_ptr<Function<T>> Derivative() const override {
        std::unique_ptr<Function<T>> numeratorDerivativeBase = numerator.Derivative();
        std::unique_ptr<Function<T>> denominatorDerivativeBase = denominator.Derivative();

        auto* numeratorDerivative = static_cast<PolynomialFunction<T>*>(numeratorDerivativeBase.get());
        auto* denominatorDerivative = static_cast<PolynomialFunction<T>*>(denominatorDerivativeBase.get());

        if (!numeratorDerivative || !denominatorDerivative) {
            throw std::logic_error("RationalFunction: derivative cast failed");
        }

        PolynomialFunction<T> newNumerator = (*numeratorDerivative) * denominator - numerator * (*denominatorDerivative);

        PolynomialFunction<T> newDenominator = denominator * denominator;

        return std::make_unique<RationalFunction<T>>(newNumerator, newDenominator);
    }

    std::string ToString(const TypeInfo* t) const override {
        return "(" + numerator.ToString(t) + ") / (" + denominator.ToString(t) + ")";
    }

    std::unique_ptr<Function<T>> Clone() const override {
        return std::make_unique<RationalFunction<T>>(*this);
    }

    const PolynomialFunction<T>& GetNumerator() const {
        return numerator;
    }

    const PolynomialFunction<T>& GetDenominator() const {
        return denominator;
    }
};

#endif