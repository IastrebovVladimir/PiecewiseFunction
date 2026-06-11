#ifndef LAB_3_SPLINEINTERPOLATION_H
#define LAB_3_SPLINEINTERPOLATION_H

#include <stdexcept>
#include "PiecewiseFunction.h"
#include "PolynomialFunction.h"

template<typename T>
void ValidateSplineInput(const MutableArraySequence<double>& xs, const MutableArraySequence<T>& ys)
{
    int n = xs.GetLength();
    if (n != ys.GetLength() || n < 2) {
        throw std::invalid_argument("Invalid spline data");
    }

    for (int index = 1; index < n; index++) {
        if (xs.Get(index) <= xs.Get(index - 1)) {
            throw std::invalid_argument("x values must be strictly increasing");
        }
    }
}

template<typename T>
PiecewiseFunction<T> BuildNaturalCubicSpline(const MutableArraySequence<double>& xs, const MutableArraySequence<T>& ys)
{
    ValidateSplineInput(xs, ys);
    int n = xs.GetLength();

    MutableArraySequence<double> h;
    for (int index = 0; index < n - 1; index++) {
        h.Append(xs.Get(index + 1) - xs.Get(index));
    }

    MutableArraySequence<T> alpha;
    alpha.Append(0);
    for (int index = 1; index < n - 1; index++) {
        alpha.Append(
                3.0 * (ys.Get(index + 1) - ys.Get(index)) / h.Get(index) -
                3.0 * (ys.Get(index) - ys.Get(index - 1)) / h.Get(index - 1)
        );
    }
    alpha.Append(0);

    MutableArraySequence<double> mu;
    MutableArraySequence<T> z;

    mu.Append(0.0);
    z.Append(0.0);

    for (int i = 1; i < n - 1; ++i) {
        double ai = h.Get(i - 1);
        double bi = 2.0 * (h.Get(i - 1) + h.Get(i));
        double ci = h.Get(i);

        double Li = bi - ai * mu.Get(i - 1);
        mu.Append(ci / Li);
        z.Append((alpha.Get(i) - ai * z.Get(i - 1)) / Li);
    }

    z.Append(0.0);

    MutableArraySequence<T> c;
    for (int i = 0; i < n; ++i) {
        c.Append(0.0);
    }

    MutableArraySequence<T> b;
    MutableArraySequence<T> d;
    MutableArraySequence<T> a;

    for (int i = 0; i < n - 1; ++i) {
        a.Append(ys.Get(i));
        b.Append(0.0);
        d.Append(0.0);
    }

    for (int i = n - 2; i >= 0; --i) {
        c.Set(i, z.Get(i) - mu.Get(i) * c.Get(i + 1));
        b.Set(i,
              (ys.Get(i + 1) - ys.Get(i)) / h.Get(i) -
              h.Get(i) * (c.Get(i + 1) + 2.0 * c.Get(i)) / 3.0);
        d.Set(i, (c.Get(i + 1) - c.Get(i)) / (3.0 * h.Get(i)));
    }

    MutableArraySequence<Segment<T>> segments;

    for (int i = 0; i < n - 1; ++i) {
        double x0 = xs.Get(i);

        T A0 = a.Get(i) - b.Get(i) * x0 + c.Get(i) * x0 * x0 - d.Get(i) * x0 * x0 * x0;
        T A1 = b.Get(i) - 2.0 * c.Get(i) * x0 + 3.0 * d.Get(i) * x0 * x0;
        T A2 = c.Get(i) - 3.0 * d.Get(i) * x0;
        T A3 = d.Get(i);

        MutableArraySequence<T> coeffs;
        coeffs.Append(A0);
        coeffs.Append(A1);
        coeffs.Append(A2);
        coeffs.Append(A3);

        PolynomialFunction<T> function(coeffs);
        segments.Append(Segment<T>(xs.Get(i), xs.Get(i + 1), function));
    }

    return PiecewiseFunction<T>(segments);
}

#endif
