#ifndef INONTERMARITHMETIC_H
#define INONTERMARITHMETIC_H
#include "Utils/utils.h"

namespace N_Logic
{
template<typename SubPropertyType>
class INonTermArithmetic
{
public:
    INonTermArithmetic() = default;

    virtual const ptr<SubPropertyType>& operator[](const size_t& k) const =0;

    virtual ~INonTermArithmetic() = default;
};
}

#endif // INONTERMARITHMETIC_H
