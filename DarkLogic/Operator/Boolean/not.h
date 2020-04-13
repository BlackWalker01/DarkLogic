/*===--- not.h - header for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines not operator in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_NOT_H
#define DARK_LOGIC_NOT_H
#include "Operator/operator.h"
#include "Formula/Boolean/asubrule.h"

namespace N_DarkLogic{

struct NotToStr
{
    static const std::string s_symbol;
};

/**
 * Not Function operator
 * return not prop1
 */
template<Proposition SubPropertyType>
struct NotFun
{
    using ValueType = bool;
    using SubFormulaType = SubPropertyType;

    NotFun() = default;
    NotFun(const ptr<SubPropertyType>& prop1);


    static constexpr Name name()
    {
        return Name::NOT;
    }

    constexpr Arity arity() const
    {
        return 1;
    }

    static constexpr unsigned short priority();

    static constexpr Associativity associativity()
    {
        return Associativity::LEFT;
    }

    static std::string symbol();
    static constexpr NbArgBefore nbArgBefore()
    {
        return NbArgBefore::E_ZERO;
    }


    bool operator()() const;

    std::string toString(unsigned short priorityParent=1000) const;

    template<Proposition SubPropertyType2>
    bool operator==(const NotFun<SubPropertyType2>& ope) const;

    const ptr<SubPropertyType>& operator[](const size_t &k) const;

    ~NotFun() = default;

    const ptr<SubPropertyType> m_sonProp;
};

template<Proposition SubPropertyType>
const ptr<SubPropertyType>&
get(const NotFun<SubPropertyType>& ope);

template<Proposition SubPropertyType>
class Not : public Operator<NotFun<SubPropertyType>>
{
public:
    using RuleOpe = Not<typename Operator<NotFun<SubPropertyType>>::SubRuleFormulaType>;
    using TheoremOpe = Not<typename Operator<NotFun<SubPropertyType>>::SubTheoremFormulaType>;

    Not() = default;
    Not(const ptr<SubPropertyType> &prop1);

    template<Proposition SubPropertyType2>
    bool operator==(const Not<SubPropertyType2>& ope) const;
    const ptr<SubPropertyType>& operator[](const size_t &k) const;
};

template<Proposition SubPropertyType>
const ptr<SubPropertyType>&
get(const Not<SubPropertyType>& ope);

#include "not.hxx"
}

#endif // DARK_LOGIC_NOT_H
