#ifndef LAB_3_FUNCTION_H
#define LAB_3_FUNCTION_H

#include <string>
#include "Lab_2/types.h"

#define EPS 1e-7
template<typename T>
class Function {
public:
    virtual ~Function() = default;
    virtual T Evaluate(double x) const = 0;
    virtual Function<T>* Derivative() const = 0;
    virtual std::string ToString(const TypeInfo* t) const = 0;
    virtual Function<T>* Clone() const = 0;
};

#endif
