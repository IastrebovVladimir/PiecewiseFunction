#ifndef LAB_3_COMPLEX_H
#define LAB_3_COMPLEX_H

#include <stdexcept>
#include <cmath>

struct Complex {
    double re;
    double im;

    Complex(double re = 0.0, double im = 0.0)
            : re(re), im(im) {}
};

inline bool operator==(const Complex& a, const Complex& b) {
    return a.re == b.re and a.im == b.im;
}

inline bool operator!=(const Complex& a, const Complex& b) {
    return !(a == b);
}

inline double abs(const Complex& z) {
    return std::sqrt(z.re * z.re + z.im * z.im);
}

inline Complex operator+(const Complex& a, const Complex& b) {
    return Complex(a.re + b.re, a.im + b.im);
}

inline Complex operator-(const Complex& a, const Complex& b) {
    return Complex(a.re - b.re, a.im - b.im);
}

inline Complex operator*(const Complex& a, const Complex& b) {
    return Complex(
            a.re * b.re - a.im * b.im,
            a.re * b.im + a.im * b.re
    );
}

inline Complex operator*(const Complex& a, double k) {
    return Complex(a.re * k, a.im * k);
}

inline Complex operator*(double k, const Complex& a) {
    return Complex(a.re * k, a.im * k);
}

inline Complex operator/(const Complex& a, double k) {
    if (k == 0.0) {
        throw std::invalid_argument("Division by zero");
    }
    return Complex(a.re / k, a.im / k);
}

inline Complex operator/(const Complex& a, const Complex& b) {
    double denom = b.re * b.re + b.im * b.im;

    if (denom == 0.0) {
        throw std::runtime_error("Complex division by zero");
    }

    return Complex((a.re * b.re + a.im * b.im) / denom,(a.im * b.re - a.re * b.im) / denom);
}

#endif