/*===--- allvariable - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines constant class in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_CONSTANTE_H
#define DARK_LOGIC_CONSTANTE_H
#include "term.h"

namespace N_DarkLogic {
template<SetDerived SetType_>
class Constante: public AbstractTerm
{
public:
    typedef SetType_ SetType;
    typedef typename SetType::Type ValueType;

    Constante(const std::string& name_);
    Constante(const ValueType& val);
    Constante(const Constante& cste);

    //get operators
    constexpr ValueType evaluate() const;
    bool isConstant() const override;
    static constexpr bool isConstantExpr()
    {
        return true;
    }

    //comparison operators
    /*template<typename T>
    constexpr bool operator==(const T&) const;*/
    constexpr bool operator==(const ValueType& val2) const;
    template<SetDerived SetType2>
    constexpr bool operator==(const Constante<SetType2>&) const;
    constexpr bool operator==(const Constante<SetType>& var) const;

    static std::string valToString(const ValueType& val);
    static ValueType stringToVal(const std::string& str);

    virtual ~Constante() override = default;
protected:
    const ValueType m_val;
};

#include "constante.hxx"
}
#endif // DARK_LOGIC_CONSTANTE_H
