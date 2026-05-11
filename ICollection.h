#ifndef LAB_3_ICOLLECTION_H
#define LAB_3_ICOLLECTION_H

template<typename T>
class ICollection {
public:
    virtual ~ICollection() = default;
    virtual const T& GetByIndex(int index) const = 0;
    virtual int GetLength() const = 0;
};

#endif
