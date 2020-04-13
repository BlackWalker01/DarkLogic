/*===--- implication.h - header for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines implication operator in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_IMPLICATION_H
#define DARK_LOGIC_IMPLICATION_H
#include "Operator/operator.h"
#include "Formula/Boolean/asubrule.h"

namespace N_DarkLogic{

struct ImplToStr
{
    static const std::string s_symbol;
};

/**
 * Impl Function operator
 * return prop1 => prop2
 */
template<Proposition SubPropertyType>
struct ImplFun
{
    using ValueType = bool;
    using SubFormulaType = SubPropertyType;

    ImplFun() = default;
    ImplFun(const ptr<SubPropertyType>& prop1, const ptr<SubPropertyType>& prop2);


    static constexpr Name name()
    {
        return Name::IMPLICATION;
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

    std::string toString(unsigned short priorityParent=1000) const;

    bool operator()() const;

    template<Proposition SubPropertyType2>
    bool operator==(const ImplFun<SubPropertyType2>& ope) const;

    const ptr<const SubPropertyType>& operator[](const size_t &k) const;

    ~ImplFun() = default;

    std::tuple<const ptr<const SubPropertyType>,const ptr<const SubPropertyType>> m_sonProps;
};

template<size_t N, Proposition SubPropertyType>
const ptr<SubPropertyType>&
get(const ImplFun<SubPropertyType>& ope);

template<Proposition SubPropertyType>
class Implication : public Operator<ImplFun<SubPropertyType>>
{
public:
    using RuleOpe = Implication<typename Operator<ImplFun<SubPropertyType>>::SubRuleFormulaType>;
    using TheoremOpe = Implication<typename Operator<ImplFun<SubPropertyType>>::SubTheoremFormulaType>;

    Implication() = default;
    Implication(const ptr<SubPropertyType> &prop1, const ptr<SubPropertyType> &prop2);

    template<Proposition SubPropertyType2>
    bool operator==(const Implication<SubPropertyType2>& ope) const;
    const ptr<const SubPropertyType>& operator[](const size_t &k) const;
};

template<size_t N, Proposition SubPropertyType>
const ptr<SubPropertyType>&
get(const Implication<SubPropertyType>& ope);

#include "implication.hxx"
}


#endif // DARK_LOGIC_IMPLICATION_H
