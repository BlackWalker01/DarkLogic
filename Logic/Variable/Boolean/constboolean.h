#ifndef CONSTBOOLEAN_H
#define CONSTBOOLEAN_H

#include "Variable/constante.h"
#include "Set/booleanset.h"
#include <sstream>

namespace N_Logic {
class ConstBoolean: public Constante<BooleanSet>
{
public:
    ConstBoolean(const bool& val);

    VALUE_TYPE valueType() const override final;
    std::string toString(unsigned short =1000) const;

    ~ConstBoolean() = default;
};
}
#endif // CONSTBOOLEAN_H
