/*===--- hyp.h - header for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines hypothesis operator in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_HYP_H
#define DARK_LOGIC_HYP_H
#include "Operator/operator.h"
#include "Variable/dbvar.h"
#include "Set/booleanset.h"
#include "Formula/Boolean/asubrule.h"
#include <sstream>

namespace N_DarkLogic{

struct HypToStr
{
    static const std::string s_symbol;
};

/**
 * HypFun Function operator
 * {prop1;... propn} implProp
 * return (prop1 && ... && propn) => implProp
 */
template<Proposition SubPropertyType>
struct HypFun
{
    using ValueType = bool;
    using SubFormulaType = SubPropertyType;

    HypFun(const std::vector<ptr<SubPropertyType>>& props);


    Arity arity() const;
    static constexpr unsigned short priority();
    static constexpr Name name()
    {
        return Name::HYP;
    }

    static constexpr Associativity associativity()
    {
        return Associativity::LEFT;
    }

    static std::string symbol();
    static constexpr NbArgBefore nbArgBefore()
    {
        return NbArgBefore::E_ZERO;
    }

    std::string toString(unsigned short priorityParent =1000) const;


    bool operator()() const;

    template<Proposition SubPropertyType2>
    bool operator==(const HypFun<SubPropertyType2>& ope) const;

    const ptr<SubPropertyType>& operator[](const size_t &k) const;

    ~HypFun() = default;

    const std::vector<ptr<SubPropertyType>> m_sonProps;
};

template<Proposition SubPropertyType>
class Hyp: public Operator<HypFun<SubPropertyType>>
{
public:
    using RuleOpe = Hyp<typename Operator<HypFun<SubPropertyType>>::SubRuleFormulaType>;
    using TheoremOpe = Hyp<typename Operator<HypFun<SubPropertyType>>::SubTheoremFormulaType>;

    Hyp(const std::vector<ptr<SubPropertyType>>& props);

    template<Proposition SubPropertyType2>
    bool operator==(const Hyp<SubPropertyType2>& ope) const;
    const ptr<SubPropertyType>& operator[](const size_t &k) const;
};

#include "hyp.hxx"
}

#endif // DARK_LOGIC_HYP_H
