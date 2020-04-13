/*===--- varset.h - source for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines set variables in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_VARSET_H
#define DARK_LOGIC_VARSET_H
#include "Variable/variable.h"
#include "Set/omniset.h"
#include "Set/booleanset.h"
#include "Set/naturalintegerset.h"

namespace N_DarkLogic
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
#endif // DARK_LOGIC_VARSET_H
