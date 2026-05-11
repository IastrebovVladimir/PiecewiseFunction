#ifndef LAB_3_PIECEWISEFUNCTION_H
#define LAB_3_PIECEWISEFUNCTION_H

#define EPS 1e-7

#include "Lab_2/ArraySequence.h"
#include "Segment.h"
#include <stdexcept>
#include <fstream>
#include <cmath>
#include <type_traits>
#include "Complex.h"

template<typename T>
class PiecewiseFunction : public MutableArraySequence<Segment<T>> {
public:
    explicit PiecewiseFunction(const MutableArraySequence<Segment<T>>& segments) {
        if (segments.GetLength() == 0) {
            throw std::invalid_argument("Segments is empty");
        }

        auto iterator = segments.GetEnumerator();
        while (iterator.MoveNext()) {
            this->Append(iterator.Current());
        }

        auto enumerator = this->GetEnumerator();
        if (!enumerator.MoveNext()) {
            throw std::invalid_argument("Segments is empty");
        }

        Segment<T> prev = enumerator.Current();

        while (enumerator.MoveNext()) {
            Segment<T> current = enumerator.Current();
            if (prev.right != current.left) {
                throw std::invalid_argument("Segments not contiguous");
            }
            prev = current;
        }
    }

    void Redefine(double a, double b, Function<T>* function) {
        if (a >= b) {
            throw std::invalid_argument("a >= b");
        }
        if (a < GetLeft() || b > GetRight()) {
            throw std::out_of_range("PiecewiseFunction: [a, b] is outside the function domain");
        }
        if (!function) {
            throw std::invalid_argument("PiecewiseFunction: function is nullptr");
        }

        MutableArraySequence<Segment<T>> newSegments;
        auto iterator = this->GetEnumerator();
        bool inserted = false;

        while (iterator.MoveNext()) {
            Segment<T> current = iterator.Current();

            if (current.right <= a || current.left >= b) {
                newSegments.Append(current);
            } else if (!inserted) {
                if (current.left < a) {
                    newSegments.Append(Segment<T>(current.left, a, current.function->Clone()));
                }
                newSegments.Append(Segment<T>(a, b, function));
                inserted = true;
                if (current.right > b) {
                    newSegments.Append(Segment<T>(b, current.right, current.function->Clone()));
                }
            } else {
                if (current.right > b) {
                    newSegments.Append(Segment<T>(b, current.right, current.function->Clone()));
                }
            }
        }

        MutableArraySequence<Segment<T>>& base = *this;
        base = newSegments;
    }

    bool IsContinuous(double eps = EPS) const {
        auto iterator = this->GetEnumerator();
        if (!iterator.MoveNext()) {
            return true;
        }

        Segment<T> prev = iterator.Current();

        while (iterator.MoveNext()) {
            Segment<T> current = iterator.Current();

            auto v1 = prev(prev.right);
            auto v2 = current(current.left);

            if (v1 != v2) {
                return false;
            }

            prev = current;
        }

        return true;
    }

    T Evaluate(double x) const {
        if (this->GetLength() == 0) {
            throw std::invalid_argument("Segments is empty");
        }

        Segment<T> first = this->GetFirst();
        Segment<T> last = this->GetLast();

        if (x < first.left) {
            return first(x);
        }

        if (x > last.right) {
            return last(x);
        }

        auto iterator = this->GetEnumerator();
        while (iterator.MoveNext()) {
            Segment<T> current = iterator.Current();
            if (current.Contains(x)) {
                return current(x);
            }
        }

        return last(x);
    }

    T EvaluateExtrapolation(double x) {
        if (this->GetLength() == 0) {
            throw std::invalid_argument("Segments is empty");
        }

        if (x < this->GetLeft()) {
            Segment<T> segment = this->GetFirst();
            return segment(x);
        }

        if (x > this->GetRight()) {
            Segment<T> segment = this->GetLast();
            return segment(x);
        }

        return this->Evaluate(x);
    }


    int CheckMonotonicity() const {
        auto iterator = this->GetEnumerator();

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

    double GetLeft() const {
        return this->GetFirst().left;
    }

    double GetRight() const {
        return this->GetLast().right;
    }
};

#endif
