#ifndef OPTION_H
#define OPTION_H

template <class T>
class Option {
private:
    T value;
    bool hasValue;
public:
    Option() : hasValue(false) {}
    Option(const T& val) : value(val), hasValue(true) {}

    bool IsSome() const {
        return hasValue;
    }
    bool IsNone() const {
        return !hasValue;
    }
    const T &GetValue() const {
        return value;
    }
};

#endif
