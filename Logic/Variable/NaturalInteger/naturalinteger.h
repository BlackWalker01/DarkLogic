#ifndef NATURALINTEGER_H
#define NATURALINTEGER_H
#include "Variable/variable.h"
#include "Set/naturalintegerset.h"

namespace N_Logic {
class NaturalInteger: public Variable<NaturalIntegerSet>
{
public:
    NaturalInteger(const std::string& name_);

    VALUE_TYPE valueType() const override final;
    void operator=(const ValueType& val);

    ~NaturalInteger() override;
};
}

#endif // NATURALINTEGER_H
