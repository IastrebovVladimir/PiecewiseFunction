#ifndef LAB_3_POLYNOMIALFUNCTION_H
#define LAB_3_POLYNOMIALFUNCTION_H

#define EPS 1e-7

#include <stdexcept>
#include "Function.h"
#include "Lab_2/ArraySequence.h"
#include "Lab_2/types.h"

template<typename T>
class PolynomialFunction : public Function<T> {
private:
    MutableArraySequence<T> coeffs;

public:
    PolynomialFunction() {
        coeffs.Append(T{});
    }

    explicit PolynomialFunction(const MutableArraySequence<T>& coefficients)
            : coeffs(coefficients) {
        if (coeffs.GetLength() == 0) {
            throw std::invalid_argument("PolynomialFunction: empty coefficients");
        }
    }

    PolynomialFunction(const PolynomialFunction& other) = default;
    PolynomialFunction& operator=(const PolynomialFunction& other) = default;
    ~PolynomialFunction() override = default;

    T Evaluate(double x) const override {
        T result = T{};
        double power = 1.0;

        for (int index = 0; index < coeffs.GetLength(); index++) {
            result = result + coeffs.Get(index) * power;
            power *= x;
        }

        return result;
    }

    PolynomialFunction<T>* Derivative() const override {
        MutableArraySequence<T> resultCoeffs;

        if (coeffs.GetLength() == 1) {
            resultCoeffs.Append(T{});
            return new PolynomialFunction<T>(resultCoeffs);
        }

        for (int index = 1; index < coeffs.GetLength(); index++) {
            resultCoeffs.Append(coeffs.Get(index) * static_cast<double>(index));
        }

        return new PolynomialFunction<T>(resultCoeffs);
    }

    Function<T>* Clone() const override {
        return new PolynomialFunction<T>(*this);
    }

    PolynomialFunction<T> operator+(const PolynomialFunction<T>& other) const {
        int maxLength = coeffs.GetLength() > other.coeffs.GetLength()
                        ? coeffs.GetLength()
                        : other.coeffs.GetLength();

        MutableArraySequence<T> resultCoeffs;

        for (int index = 0; index < maxLength; index++) {
            T first  = index < coeffs.GetLength() ? coeffs.Get(index) : T{};
            T second = index < other.coeffs.GetLength() ? other.coeffs.Get(index) : T{};
            resultCoeffs.Append(first + second);
        }

        return PolynomialFunction<T>(resultCoeffs);
    }

    PolynomialFunction<T> operator-(const PolynomialFunction<T>& other) const {
        int maxLength = coeffs.GetLength() > other.coeffs.GetLength()
                        ? coeffs.GetLength()
                        : other.coeffs.GetLength();

        MutableArraySequence<T> resultCoeffs;

        for (int index = 0; index < maxLength; index++) {
            T first  = index < coeffs.GetLength()       ? coeffs.Get(index)       : T{};
            T second = index < other.coeffs.GetLength() ? other.coeffs.Get(index) : T{};
            resultCoeffs.Append(first - second);
        }

        return PolynomialFunction<T>(resultCoeffs);
    }

    PolynomialFunction<T> operator*(const PolynomialFunction<T>& other) const {
        int firstLength  = coeffs.GetLength();
        int secondLength = other.coeffs.GetLength();

        MutableArraySequence<T> resultCoeffs;
        for (int index = 0; index < firstLength + secondLength - 1; index++) {
            resultCoeffs.Append(T{});
        }

        for (int i = 0; i < firstLength; i++) {
            for (int j = 0; j < secondLength; j++) {
                int pos = i + j;
                T oldValue = resultCoeffs.Get(pos);
                T newValue = oldValue + coeffs.Get(i) * other.coeffs.Get(j);
                resultCoeffs.Set(pos, newValue);
            }
        }

        return PolynomialFunction<T>(resultCoeffs);
    }

    std::string ToString(const TypeInfo* t) const override {
        std::string result;

        for (int index = 0; index < coeffs.GetLength(); ++index) {
            T coefficient = coeffs.Get(index);

            if (index > 0) {
                result += " + ";
            }

            result += t->to_string(&coefficient);

            if (index >= 1) {
                result += "*x";
            }
            if (index >= 2) {
                result += "^" + std::to_string(index);
            }
        }

        return result;
    }

    const MutableArraySequence<T>& GetCoeffs() const {
        return coeffs;
    }
};


#endif