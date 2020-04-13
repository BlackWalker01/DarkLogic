/*===--- let.h - header for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines let operator in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_LETEQUAL_H
#define DARK_LOGIC_LETEQUAL_H
#include "Operator/operator.h"
#include "Formula/Arithmetic/asubarithmeticrule.h"
#include "Formula/Arithmetic/asubarithmetictheorem.h"
#include "Formula/Boolean/asubrule.h"
#include "Formula/Boolean/asubtheorem.h"
#include "Operator/Arithmetic/setequal.h"

namespace N_DarkLogic {

/**
 *LetFun represents let p1 in p2
 * p1 is x=a or x€X (where a is a constant and X is a Set)
 * p2 is a property
 */
template<typename SubFormulaType, Proposition SubFormulaType2>
struct LetFun;

/**
 *Let represents let p1 in p2
 * p1 is x=a or x€X (where a is a constant and X is a Set)
 * p2 is a property
 */
template<typename SubFormulaType, Proposition SubFormulaType2>
class Let;

template<SubRuleFormula SubPropertyType>
class SubArithmeticRule;

template<SubTheoremFormula SubPropertyType>
class SubArithmeticTheorem;

template<typename VariableType_>
class SubArithmeticRule<SetEqual<SubArithmeticRule<VariableType_>>>;

struct LetStr
{
    static const std::string s_symbol; //let
    static const std::string s_symbol2; //in
};


template<typename SubRuleType>
struct LetFun<SubRuleType,ASubRule>
{
    using ValueType = bool;
    using SubFormulaType = ASubRule;

    LetFun() = default;
    LetFun(const ptr<SubRuleType>& var, const ptr<ASubRule>& prop);

    ValueType operator()() const;

    static constexpr Name name()
    {
        return Name::LET;
    }

    constexpr Arity arity() const
    {
        return 2;
    }

    static constexpr unsigned short priority()
    {
        return 18;
    }

    static constexpr Associativity associativity()
    {
        return Associativity::RIGHT;
    }

    static std::string symbol();
    static constexpr NbArgBefore nbArgBefore()
    {
        return NbArgBefore::E_ONE;
    }

    std::string toString(unsigned short priorityParent=1000) const;

    template<typename SubPropertyType,Proposition SubPropertyType2>
    bool operator==(const LetFun<SubPropertyType,SubPropertyType2>& ope) const;

    const ptr<IISubRuleFormula>& operator[](const size_t& k) const;

    ~LetFun() = default;

    //Attributes
    std::tuple<const ptr<SubRuleType>,const ptr<ASubRule>> m_sonProps;
    std::tuple<const ptr<IISubRuleFormula>,const ptr<IISubRuleFormula>> m_castSonProps;
};

template<size_t N, typename SubRuleType>
typename std::tuple_element<N,std::tuple<const ptr<SubRuleType>, const ptr<ASubRule>>>::type&
get(const LetFun<SubRuleType,ASubRule>& fun);


template<typename SubRuleType>
class Let<SubRuleType,ASubRule> :
        public Operator<LetFun<SubRuleType,ASubRule>>
{
public:
    using FunType = LetFun<SubRuleType,ASubRule>;
    using RuleOpe = Let<ToRule<SubRuleType>, ASubRule>;
    using TheoremOpe = Let<ToTheorem<SubRuleType>, ASubTheorem>;

    Let() = default;
    Let(const ptr<SubRuleType>& var, const ptr<ASubRule>& prop);

    template<typename SubPropertyType, Proposition SubPropertyType2>
    bool operator==(const Let<SubPropertyType,SubPropertyType2>& ope) const;

    const ptr<IISubRuleFormula>& operator[](const size_t& k) const;
};

template<size_t N, typename SubRuleType>
typename std::tuple_element<N,std::tuple<const ptr<SubRuleType>, const ptr<ASubRule>>>::type&
get(const Let<SubRuleType,ASubRule>& fun);



template<typename SubTheoremType>
struct LetFun<SubTheoremType,ASubTheorem>
{
    using ValueType = bool;
    using SubFormulaType = ASubTheorem;

    LetFun() = default;
    LetFun(const ptr<SubTheoremType>& var, const ptr<ASubTheorem>& prop);

    ValueType operator()() const;

    static constexpr Name name()
    {
        return Name::LET;
    }

    constexpr Arity arity() const
    {
        return 2;
    }

    static constexpr unsigned short priority()
    {
        return 18;
    }

    static constexpr Associativity associativity()
    {
        return Associativity::RIGHT;
    }

    static std::string symbol();
    static constexpr NbArgBefore nbArgBefore()
    {
        return NbArgBefore::E_ONE;
    }

    std::string toString(unsigned short priorityParent=1000) const;

    template<typename SubPropertyType, Proposition SubPropertyType2>
    bool operator==(const LetFun<SubPropertyType,SubPropertyType2>& ope) const;    
    const ptr<IISubTheoremFormula>& operator[](const size_t& k) const;

    ~LetFun() = default;

    //Attributes
    std::tuple<const ptr<SubTheoremType>,const ptr<ASubTheorem>> m_sonProps;
    std::tuple<const ptr<IISubTheoremFormula>,const ptr<IISubTheoremFormula>> m_castSonProps;
};

template<size_t N, typename SubTheoremType>
typename std::tuple_element<N,std::tuple<const ptr<SubTheoremType>, const ptr<ASubTheorem>>>::type&
get(const LetFun<SubTheoremType,ASubTheorem>& fun);


template<typename SubTheoremType>
class Let<SubTheoremType,ASubTheorem> :
        public Operator<LetFun<SubTheoremType,ASubTheorem>>
{
public:
    using FunType = LetFun<SubTheoremType,ASubTheorem>;
    using RuleOpe = Let<ToRule<SubTheoremType>, ASubRule>;
    using TheoremOpe = Let<ToTheorem<SubTheoremType>, ASubTheorem>;

    Let() = default;
    Let(const ptr<SubTheoremType>& var, const ptr<ASubTheorem>& prop);

    template<typename SubPropertyType, Proposition SubPropertyType2>
    bool operator==(const Let<SubPropertyType,SubPropertyType2>& ope) const;
    const ptr<IISubTheoremFormula>& operator[](const size_t& k) const;
};

template<size_t N, typename VariableType>
typename std::tuple_element<N,
std::tuple<const ptr<SubArithmeticTheorem<SetEqual<VariableType>>>, const ptr<ASubTheorem>>>::type&
get(const Let<SubArithmeticTheorem<VariableType>, ASubTheorem>& fun);


/**
 * Implementation of LetEqual<SubRuleType,ASubRule>
 */

template<typename SubRuleType>
LetFun<SubRuleType,ASubRule>::
LetFun(const ptr<SubRuleType> &var, const ptr<ASubRule> &prop):
    m_sonProps(var,prop), m_castSonProps(var,prop)
{

}

template<typename SubRuleType>
bool LetFun<SubRuleType,ASubRule>::operator()() const
{
    std::get<0>(m_sonProps)->evaluate();
    return std::get<1>(m_sonProps)->evaluate();
}

template<typename SubRuleType>
std::string LetFun<SubRuleType,ASubRule>::symbol()
{
    return LetStr::s_symbol;
}

template<typename SubRuleType>
std::string LetFun<SubRuleType,ASubRule>::
toString(unsigned short priorityParent) const
{
    if(priorityParent<priority())
    {
        return "("+LetStr::s_symbol+std::get<0>(m_sonProps)->toString()+LetStr::s_symbol2+std::get<1>(m_sonProps)->toString(priority())+")";
    }
    else
    {
        return LetStr::s_symbol+std::get<0>(m_sonProps)->toString()+LetStr::s_symbol2+std::get<1>(m_sonProps)->toString(priority());
    }
}

template<typename SubRuleType> template<typename SubPropertyType, Proposition SubPropertyType2>
bool LetFun<SubRuleType, ASubRule>::
operator==(const LetFun<SubPropertyType,SubPropertyType2> &ope) const
{
    return (*std::get<0>(m_sonProps)==*(std::get<0>(ope.m_sonProps)))&&(*std::get<1>(m_sonProps)==*(std::get<1>(ope.m_sonProps)));
}


template<typename SubRuleType>
Let<SubRuleType,ASubRule>::
Let(const ptr<SubRuleType> &var, const ptr<ASubRule> &prop):
    Operator<LetFun<SubRuleType,ASubRule>>(LET,var,prop)
{

}

template<typename SubRuleType> template<typename SubPropertyType, Proposition SubPropertyType2>
bool Let<SubRuleType,ASubRule>::
operator==(const Let<SubPropertyType,SubPropertyType2> &ope) const
{
    return this->m_fun==ope.m_fun;
}

/**
 * Implementation of LetEqual<SubTheoremType,ASubArithmeticTheorem>
 */

template<typename SubTheoremType>
LetFun<SubTheoremType,ASubTheorem>::
LetFun(const ptr<SubTheoremType> &var, const ptr<ASubTheorem> &prop):
    m_sonProps(var,prop), m_castSonProps(var,prop)
{

}


template<typename SubTheoremType>
bool LetFun<SubTheoremType,ASubTheorem>::operator()() const
{
    std::get<0>(m_sonProps)->evaluate();
    return std::get<1>(m_sonProps)->evaluate();
}

template<typename SubTheoremType>
std::string LetFun<SubTheoremType,ASubTheorem>::symbol()
{
    return LetStr::s_symbol2;
}

template<typename SubTheoremType>
std::string LetFun<SubTheoremType,ASubTheorem>::
toString(unsigned short priorityParent) const
{
    if(priorityParent<priority())
    {
        return "("+LetStr::s_symbol+std::get<0>(m_sonProps)->toString()+LetStr::s_symbol2+std::get<1>(m_sonProps)->toString(priority())+")";
    }
    else
    {
        return LetStr::s_symbol+std::get<0>(m_sonProps)->toString()+LetStr::s_symbol2+std::get<1>(m_sonProps)->toString(priority());
    }
}

template<typename SubTheoremType> template<typename SubPropertyType, Proposition SubPropertyType2>
bool LetFun<SubTheoremType,ASubTheorem>::
operator==(const LetFun<SubPropertyType,SubPropertyType2> &ope) const
{
    return (*std::get<0>(m_sonProps)==(*std::get<0>(ope.m_sonProps)))&&(*std::get<1>(m_sonProps)==(*std::get<1>(ope.m_sonProps)));
}


template<typename SubTheoremType>
Let<SubTheoremType,ASubTheorem>::
Let(const ptr<SubTheoremType> &var, const ptr<ASubTheorem> &prop):
    Operator<LetFun<SubTheoremType,ASubTheorem>>(LET,var,prop)
{

}


template<typename SubTheoremType> template<typename SubPropertyType, Proposition SubPropertyType2>
bool Let<SubTheoremType,ASubTheorem>::
operator==(const Let<SubPropertyType,SubPropertyType2> &ope) const
{
    return this->m_fun==ope.m_fun;
}

/**
  * get methods
  */
template<size_t N, typename SubRuleType>
typename std::tuple_element<N, std::tuple<const ptr<SubRuleType>,const ptr<ASubRule> > >::type&
get(const LetFun<SubRuleType,ASubRule> &fun)
{
    return std::get<N>(fun.m_sonProps);
}

template<size_t N, typename SubRuleType>
typename std::tuple_element<N, std::tuple<const ptr<SubRuleType>, const ptr<ASubRule> > >::type&
get(const Let<SubRuleType, ASubRule> &fun)
{
    return get<N>(fun.m_fun);
}

template<size_t N, typename SubTheoremType>
typename std::tuple_element<N, std::tuple<const ptr<SubTheoremType>, const ptr<ASubTheorem> > >::type&
get(const LetFun<SubTheoremType,ASubTheorem > &fun)
{
    return std::get<N>(fun.m_sonProps);
}

template<size_t N, typename SubTheoremType>
typename std::tuple_element<N,std::tuple<const ptr<SubTheoremType>, const ptr<ASubTheorem>>>::type&
get(const Let<SubTheoremType,ASubTheorem > &fun)
{
    return get<N>(fun.m_fun);
}

template<typename SubRuleType>
const ptr<IISubRuleFormula> &N_DarkLogic::LetFun<SubRuleType, ASubRule>::
operator[](const size_t &k) const
{
    switch(k)
    {
        case 0:
        {
            return std::get<0>(m_castSonProps);
        }
        case 1:
        {
            return std::get<1>(m_castSonProps);
        }
        default:
        {
            throw std::runtime_error("Let operator has only two arguments");
        }
    }
}

template<typename SubRuleType>
const ptr<IISubRuleFormula> &N_DarkLogic::Let<SubRuleType, ASubRule>::
operator[](const size_t &k) const
{
    return this->m_fun[k];
}

template<typename SubTheoremType>
const ptr<IISubTheoremFormula> &N_DarkLogic::LetFun<SubTheoremType, ASubTheorem>::
operator[](const size_t &k) const
{
    switch(k)
    {
        case 0:
        {
            return std::get<0>(m_castSonProps);
        }
        case 1:
        {
            return std::get<1>(m_castSonProps);
        }
        default:
        {
            throw std::runtime_error("Let operator has only two arguments");
        }
    }
}

template<typename SubTheoremType>
const ptr<IISubTheoremFormula>& Let<SubTheoremType, ASubTheorem>::
operator[](const size_t &k) const
{
    return this->m_fun[k];
}

}
#endif // DARK_LOGIC_LETEQUAL_H
