#ifndef LAB_3_PIECEWISEFUNCTION_H
#define LAB_3_PIECEWISEFUNCTION_H

#include <cmath>
#include <memory>
#include <stdexcept>
#include <string>
#include "Function.h"
#include "../Segment.h"
#include "../Lab_2/ArraySequence.h"

template<typename T>
class PiecewiseFunction : public Function<T> {
private:
    MutableArraySequence<Segment<T>> segments;

    void ValidateRedefineInterval(double a, double b) const {
        if (a >= b) {
            throw std::invalid_argument("PiecewiseFunction: a >= b");
        }

        if (a < GetLeft() || b > GetRight()) {
            throw std::out_of_range("PiecewiseFunction: [a, b] is outside the domain");
        }
    }

public:
    explicit PiecewiseFunction(const MutableArraySequence<Segment<T>>& inputSegments) {
        if (inputSegments.GetLength() == 0) {
            throw std::invalid_argument("PiecewiseFunction: segments are empty");
        }

        auto iterator = inputSegments.GetEnumerator();
        while (iterator.MoveNext()) {
            segments.Append(iterator.Current());
        }

        auto check = segments.GetEnumerator();
        if (!check.MoveNext()) {
            throw std::invalid_argument("PiecewiseFunction: segments are empty");
        }

        Segment<T> previous = check.Current();

        if (previous.left > previous.right) {
            throw std::invalid_argument("PiecewiseFunction: invalid segment bounds");
        }

        while (check.MoveNext()) {
            Segment<T> current = check.Current();

            if (previous.right != current.left) {
                throw std::invalid_argument("PiecewiseFunction: segments are not contiguous");
            }

            if (current.left > current.right) {
                throw std::invalid_argument("PiecewiseFunction: invalid segment bounds");
            }

            previous = current;
        }
    }

    void Redefine(double a, double b, const Function<T>& function) {
        ValidateRedefineInterval(a, b);

        MutableArraySequence<Segment<T>> result;
        auto iterator = segments.GetEnumerator();
        bool replacementInserted = false;

        while (iterator.MoveNext()) {
            Segment<T> currentSegment = iterator.Current();

            if (currentSegment.right <= a || currentSegment.left >= b) {
                result.Append(currentSegment);
                continue;
            }

            if (currentSegment.left < a) {
                result.Append(Segment<T>(currentSegment.left, a, *currentSegment.function));
            }

            if (!replacementInserted) {
                result.Append(Segment<T>(a, b, function));
                replacementInserted = true;
            }

            if (currentSegment.right > b) {
                result.Append(Segment<T>(b, currentSegment.right, *currentSegment.function));
            }
        }

        segments = result;
    }

    bool IsContinuous(double eps = 1e-7) const {
        if (segments.GetLength() <= 1) {
            return true;
        }

        auto iterator = segments.GetEnumerator();
        iterator.MoveNext();
        Segment<T> prev = iterator.Current();

        using std::abs;

        while (iterator.MoveNext()) {
            Segment<T> current = iterator.Current();

            T v1 = prev(prev.right);
            T v2 = current(current.left);

            if (abs(v1 - v2) > eps) {
                return false;
            }

            prev = current;
        }

        return true;
    }

    T Evaluate(double x) const override {
        if (segments.GetLength() == 0) {
            throw std::logic_error("PiecewiseFunction: no segments");
        }

        Segment<T> last = segments.GetLast();

        if (x == last.right) {
            return last(x);
        }

        auto iterator = segments.GetEnumerator();
        while (iterator.MoveNext()) {
            Segment<T> current = iterator.Current();

            if (x >= current.left && x < current.right) {
                return current(x);
            }
        }

        throw std::out_of_range("PiecewiseFunction: x is outside the domain");
    }

    T EvaluateExtrapolation(double x) const {
        if (segments.GetLength() == 0) {
            throw std::logic_error("PiecewiseFunction: no segments");
        }

        Segment<T> first = segments.GetFirst();
        Segment<T> last = segments.GetLast();

        if (x < first.left) {
            return first(x);
        }

        if (x > last.right) {
            return last(x);
        }

        return Evaluate(x);
    }

    std::unique_ptr<Function<T>> Derivative() const override {
        MutableArraySequence<Segment<T>> derivativeSegments;

        auto iterator = segments.GetEnumerator();
        while (iterator.MoveNext()) {
            const Segment<T>& segment = iterator.Current();
            auto derivativeFunction = segment.function->Derivative();
            derivativeSegments.Append(Segment<T>(segment.left, segment.right, *derivativeFunction));
        }

        return std::make_unique<PiecewiseFunction<T>>(derivativeSegments);
    }


    int CheckMonotonicity() const {
        if (segments.GetLength() == 0) {
            return 0;
        }

        auto iterator = segments.GetEnumerator();

        bool allIncreasing = true;
        bool allDecreasing = true;

        while (iterator.MoveNext()) {
            Segment<T> current = iterator.Current();

            int code = current.GetMonotonicity();

            if (code == 0) {
                return 0;
            }

            if (code != 2) {
                allIncreasing = false;
            }
            if (code != 1) {
                allDecreasing = false;
            }
        }

        if (allDecreasing) {
            return 1;
        }
        if (allIncreasing) {
            return 2;
        }
        return 0;
    }

    std::unique_ptr<Function<T>> Clone() const override {
        return std::make_unique<PiecewiseFunction<T>>(*this);
    }

    std::string ToString(const TypeInfo* t) const override {
        std::string result = "Piecewise[" + std::to_string(GetLeft()) + ", " + std::to_string(GetRight()) + "]:\n{\n";

        auto iterator = segments.GetEnumerator();

        while (iterator.MoveNext()) {
            const Segment<T>& segment = iterator.Current();

            result += "     [" + std::to_string(segment.left) + ", " + std::to_string(segment.right) + "): ";
            result += segment.function->ToString(t);
            result += ";\n";
        }

        result += "}";
        return result;
    }

    const Segment<T>& GetSegment(int index) const {
        return segments.Get(index);
    }

    double GetLeft() const {
        return segments.GetFirst().left;
    }

    double GetRight() const {
        return segments.GetLast().right;
    }

    int GetLength() {
        return segments.GetLength();
    }
};

#endif
