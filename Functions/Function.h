#ifndef LAB_3_FUNCTION_H
#define LAB_3_FUNCTION_H

#include <memory>
#include <string>
#include "../Lab_2/types.h"

template<typename T>
class Function {
public:
    virtual ~Function() = default;

    virtual T Evaluate(double x) const = 0;
    virtual std::unique_ptr<Function<T>> Clone() const = 0;
    virtual std::unique_ptr<Function<T>> Derivative() const = 0;
    virtual std::string ToString(const TypeInfo* t) const = 0;
};

#endif
