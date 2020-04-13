/*===--- constset.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines set constants in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_CONSTSET_H
#define DARK_LOGIC_CONSTSET_H
#include "Variable/constante.h"
#include "Set/omniset.h"
#include "Set/booleanset.h"
#include "Set/naturalintegerset.h"

namespace N_DarkLogic
{
template<SetDerived SetType_>
class ConstSet: public Constante<OmniSet<SetType_>>
{
public:
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
#endif // DARK_LOGIC_CONSTSET_H
