#ifndef CONSTSET_H
#define CONSTSET_H
#include "Variable/constante.h"
#include "Set/omniset.h"
#include "Set/booleanset.h"
#include "Set/naturalintegerset.h"

namespace N_Logic
{
template<SetDerived SetType_>
class ConstSet: public Constante<OmniSet<SetType_>>
{
public:
    using SetType=SetType_;
    using ValueType=typename OmniSet<SetType_>::Type;

    ConstSet(const ValueType& val);

    VALUE_TYPE valueType() const override final;
    std::string toString(unsigned short =1000) const;
};

template<SetDerived SetType_>
ConstSet<SetType_>::ConstSet(const ValueType &val):
    Constante<OmniSet<SetType_> > (val)
{

}

template<SetDerived SetType_>
std::string ConstSet<SetType_>::toString(unsigned short) const
{
    return this->name();
}

}
#endif // CONSTSET_H
