/*===--- or.h - header for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines Or operator in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_OR_H
#define DARK_LOGIC_OR_H
#include "Operator/operator.h"
#include "Formula/Boolean/asubrule.h"

namespace N_DarkLogic{

struct OrToStr
{
    static const std::string s_symbol;
};

/**
 * Or Function operator
 * return prop1 || prop2
 */
template<Proposition SubPropertyType>
struct OrFun
{
    using ValueType = bool;
    using SubFormulaType = SubPropertyType;

    OrFun() = default;
    OrFun(const ptr<SubPropertyType>& prop1, const ptr<SubPropertyType>& prop2);


    static constexpr Name name()
    {
        return Name::OR;
    }

    constexpr Arity arity() const
    {
        return 2;
    }

    static constexpr unsigned short priority();

    static constexpr Associativity associativity()
    {
        return Associativity::LEFT;
    }

    static std::string symbol();
    static constexpr NbArgBefore nbArgBefore()
    {
        return NbArgBefore::E_ONE;
    }

    bool operator()() const;

    std::string toString(unsigned short priorityParent=1000) const;

    template<Proposition SubPropertyType2>
    bool operator==(const OrFun<SubPropertyType2>& ope) const;

    const ptr<SubPropertyType>& operator[](const size_t &k) const;

    ~OrFun() = default;

    std::tuple<const ptr<SubPropertyType>,const ptr<SubPropertyType>> m_sonProps;
};

template<size_t N, Proposition SubPropertyType>
const ptr<SubPropertyType>&
get(const OrFun<SubPropertyType>& ope);

template<Proposition SubPropertyType>
class Or : public Operator<OrFun<SubPropertyType>>
{
public:
    using RuleOpe = Or<typename Operator<OrFun<SubPropertyType>>::SubRuleFormulaType>;
    using TheoremOpe = Or<typename Operator<OrFun<SubPropertyType>>::SubTheoremFormulaType>;

    Or() = default;
    Or(const ptr<SubPropertyType> &prop1, const ptr<SubPropertyType> &prop2);

    template<Proposition SubPropertyType2>
    bool operator==(const Or<SubPropertyType2>& ope) const;
    const ptr<SubPropertyType>& operator[](const size_t &k) const;
};

template<size_t N, Proposition SubPropertyType>
const ptr<SubPropertyType>&
get(const Or<SubPropertyType>& ope);


#include "or.hxx"
}


#endif // DARK_LOGIC_OR_H
