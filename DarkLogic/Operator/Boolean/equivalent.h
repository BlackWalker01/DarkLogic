/*===--- equivalent.h - header for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines equivalent operator in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_EQUIVALENT_H
#define DARK_LOGIC_EQUIVALENT_H
#include "Operator/operator.h"
#include "Formula/Boolean/asubrule.h"

namespace N_DarkLogic{

struct EquivalentToStr
{
    static const std::string s_symbol;
};

/**
 * Eq Function operator
 * return prop1 <=> prop2
 */
template<Proposition SubPropertyType>
struct EqFun
{
    using ValueType = bool;
    using SubFormulaType = SubPropertyType;

    EqFun() = default;
    EqFun(const ptr<SubPropertyType>& prop1, const ptr<SubPropertyType>& prop2);


    static constexpr Name name()
    {
        return Name::EQUIVALENT;
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

    std::string toString(unsigned short priorityParent) const;

    template<Proposition SubPropertyType2>
    bool operator==(const EqFun<SubPropertyType2>& ope) const;
    const ptr<SubPropertyType>& operator[](const size_t &k) const;
    bool operator()() const;

    ~EqFun() = default;

    std::tuple<const ptr<SubPropertyType>,const ptr<SubPropertyType>> m_sonProps;
};

template<size_t N, Proposition SubPropertyType>
const ptr<SubPropertyType>&
get(const EqFun<SubPropertyType>& ope);

template<Proposition SubPropertyType>
class Equivalent : public Operator<EqFun<SubPropertyType>>
{
public:
    using RuleOpe = Equivalent<typename Operator<EqFun<SubPropertyType>>::SubRuleFormulaType>;
    using TheoremOpe = Equivalent<typename Operator<EqFun<SubPropertyType>>::SubTheoremFormulaType>;

    Equivalent() = default;
    Equivalent(const ptr<SubPropertyType>& prop1, const ptr<SubPropertyType>& prop2);

    template<Proposition SubPropertyType2>
    bool operator==(const Equivalent<SubPropertyType2>& ope) const;
    const ptr<SubPropertyType>& operator[](const size_t &k) const;
};

template<size_t N, Proposition SubPropertyType>
const ptr<SubPropertyType>&
get(const Equivalent<SubPropertyType>& ope);

#include "equivalent.hxx"
}

#endif // DARK_LOGIC_EQUIVALENT_H
