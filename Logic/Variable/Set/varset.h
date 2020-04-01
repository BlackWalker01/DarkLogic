#ifndef VARSET_H
#define VARSET_H
#include "Variable/variable.h"
#include "Set/omniset.h"
#include "Set/booleanset.h"
#include "Set/naturalintegerset.h"

namespace N_Logic
{
template<SetDerived SetType_>
class VarSet: public Variable<OmniSet<SetType_>>
{
public:
    using ValueType=typename OmniSet<SetType_>::Type;

    VarSet(const std::string& name_);

    VALUE_TYPE valueType() const override final;
    void operator=(const ValueType& val);

    ~VarSet() override = default;
};

template<SetDerived SetType_>
VarSet<SetType_>::VarSet(const std::string &name_):
    Variable<OmniSet<SetType_> > (name_)
{

}

template<SetDerived SetType_>
void VarSet<SetType_>::operator=(const VarSet::ValueType &val)
{
    delete this->m_val;
    this->m_val=new ValueType(val);
}

}
#endif // VARSET_H
