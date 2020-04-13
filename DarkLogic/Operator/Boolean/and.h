/*===--- and.h - header for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines and operator in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_AND_H
#define DARK_LOGIC_AND_H

#include "Operator/operator.h"
#include "Formula/Boolean/asubrule.h"

namespace N_DarkLogic{

struct AndToStr
{
    static const std::string s_symbol; //&&
};

/**
 * And Function operator
 * return prop1 && prop2
 */
template<Proposition SubPropertyType>
struct AndFun
{
    using ValueType = bool;
    using SubFormulaType = SubPropertyType;

    AndFun() = default;
    AndFun(const ptr<SubPropertyType>& prop1, const ptr<SubPropertyType>& prop2);


    static constexpr Name name()
    {
        return Name::AND;
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
    bool operator==(const AndFun<SubPropertyType2>& ope) const;

    const ptr<SubPropertyType>& operator[](const size_t &k) const;

    ~AndFun() = default;

    std::tuple<const ptr<SubPropertyType>,const ptr<SubPropertyType>> m_sonProps;
};

template<size_t N, Proposition SubPropertyType>
const ptr<SubPropertyType>&
get(const AndFun<SubPropertyType>& ope);

template<Proposition SubPropertyType>
class And : public Operator<AndFun<SubPropertyType>>
{
public:
    using RuleOpe = And<typename Operator<AndFun<SubPropertyType>>::SubRuleFormulaType>;
    using TheoremOpe = And<typename Operator<AndFun<SubPropertyType>>::SubTheoremFormulaType>;

    And() = default;
    And(const ptr<SubPropertyType> &prop1, const ptr<SubPropertyType> &prop2);

    template<Proposition SubPropertyType2>
    bool operator==(const And<SubPropertyType2>& ope) const;
    const ptr<SubPropertyType>& operator[](const size_t &k) const;
};

template<size_t N, Proposition SubPropertyType>
const std::shared_ptr<SubPropertyType>&
get(const And<SubPropertyType>& ope);

#include "and.hxx"
}
#endif // DARK_LOGIC_AND_H
