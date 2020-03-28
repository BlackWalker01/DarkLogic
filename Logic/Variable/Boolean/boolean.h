#ifndef BOOLEAN_H
#define BOOLEAN_H
#include "Variable/variable.h"
#include "Set/booleanset.h"

namespace N_Logic {
class Boolean : public N_Logic::Variable<N_Logic::BooleanSet>
{
public:
    Boolean(const std::string& name_);

    VALUE_TYPE valueType() const override final;
    void operator=(const ValueType& val) const;

    ~Boolean() override;
};
}
#endif // BOOLEAN_H
