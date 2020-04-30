/*===--- iproposition.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines IProposition class in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_IPROPERTY_H
#define DARK_LOGIC_IPROPERTY_H
#include "Formula/formula.h"

namespace N_DarkLogic {

class DbVarProp;
class DbVar;

class ASubRule;
class ASubTheorem;
class AbstractTerm;
class BooleanSet;

class IProposition: public AbstractFormula<bool>
{
public:
    enum PropType
    {
        AND_PROP,
        BELONGSTO_PROP,
        EQUIV_PROP,
        EQUAL_PROP,
        HYP_PROP,
        IMPL_PROP,
        LET_PROP,
        NOT_PROP,
        OR_PROP,
        VAR_PROP,
        CONST_PROP
    };

    IProposition() = default;

    virtual constexpr PropType type() const=0;

    //method to get sub properties
    virtual size_t arity() const =0;

    virtual bool isEqual(const ASubRule& prop) const =0;
    virtual bool isEqual(const ASubTheorem& prop) const =0;
    
    //terminal subproposition methods
    virtual inline bool isHypProp() const
    {
        return false;
    }
    virtual IDVar variableId() const;


    virtual ~IProposition() = default;
};

inline bool operator==(const ASubRule& rule, const ASubTheorem& thm);
inline bool operator==(const ASubTheorem& th, const ASubTheorem& th2);
inline bool operator==(const ASubRule& th, const ASubRule& th2);
}

#endif // DARK_LOGIC_IPROPERTY_H
