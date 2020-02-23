#ifndef CONSTNATURALINTEGER_H
#define CONSTNATURALINTEGER_H
#include "Variable/constante.h"
#include "Set/naturalintegerset.h"

namespace N_Logic {
class ConstNaturalInteger: public Constante<NaturalIntegerSet>
{
public:
    ConstNaturalInteger(const typename NaturalIntegerSet::Type& val);

    VALUE_TYPE valueType() const override final;
    std::string toString(unsigned short =1000) const;

    ~ConstNaturalInteger() override = default;
};
}

#endif // CONSTNATURALINTEGER_H
