#ifndef LAB_3_SEGMENT_H
#define LAB_3_SEGMENT_H

#include <memory>
#include <stdexcept>
#include <cmath>
#include "Functions/Function.h"

template<typename T>
struct Segment {
    double left;
    double right;
    std::unique_ptr<Function<T>> function;

    Segment() : left(0), right(0), function(nullptr) {}

    Segment(double left, double right, const Function<T>& function)
            : left(left), right(right), function(function.Clone()) {
        if (left > right) {
            throw std::invalid_argument("Segment: left > right");
        }
    }

    Segment(const Segment& other) : left(other.left), right(other.right), function(other.function ? other.function->Clone() : nullptr) {}

    Segment& operator=(const Segment& other) {
        if (this != &other) {
            left = other.left;
            right = other.right;
            function = other.function ? other.function->Clone() : nullptr;
        }
        return *this;
    }

    Segment(Segment&&) noexcept = default;
    Segment& operator=(Segment&&) noexcept = default;
    ~Segment() = default;

    bool Contains(double x) const {
        return x >= left && x <= right;
    }

    T operator()(double x) const {
        if (!function) {
            throw std::logic_error("Segment: function is nullptr");
        }
        return function->Evaluate(x);
    }

    int GetMonotonicity(int steps = 5000, double eps = 1e-7) const {
        if (!function) {
            throw std::logic_error("Segment: function is nullptr");
        }

        if (right <= left) {
            return 0;
        }

        bool hasPositive = false;
        bool hasNegative = false;

        auto diff = function->Derivative();

        double h = (right - left) / steps;

        for (int index = 0; index <= steps; index++) {
            double x = left + index * h;
            T value = diff->Evaluate(x);

            using std::abs;
            if (abs(value) <= eps) {
                continue;
            }

            if (value > 0) {
                hasPositive = true;
            } else if (value < 0) {
                hasNegative = true;
            }

            if (hasPositive && hasNegative) {
                return 0;
            }
        }

        if (hasNegative && !hasPositive) {
            return 1;
        }

        if (hasPositive && !hasNegative) {
            return 2;
        }

        return 0;
    }
};

#endif
