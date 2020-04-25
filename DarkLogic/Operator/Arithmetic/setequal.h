/*===--- setequal.h - header for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines assignment operator in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_SETEQUAL_H
#define DARK_LOGIC_SETEQUAL_H
#include "Operator/operator.h"
#include "Formula/Arithmetic/asubarithmeticrule.h"
#include "Formula/Arithmetic/asubarithmetictheorem.h"

namespace N_DarkLogic {

template<typename SubFormulaType>
struct SetEqualFun;

template<typename SubFormulaType>
class SetEqual;

template<SubRuleFormula SubPropertyType>
class SubArithmeticRule;

template<SubTheoremFormula SubPropertyType>
class SubArithmeticTheorem;

struct SetEqualToStr
{
    static const std::string s_symbol;
};

template<typename VariableType>
struct SetEqualFun<SubArithmeticRule<VariableType>>
{
    using ValueType = void;
    using SubFormulaType = ASubArithmeticRule<ValueType>;

    SetEqualFun() = default;
    SetEqualFun(const ptr<SubArithmeticRule<VariableType>>& var,
                const ptr<ASubArithmeticRule<typename VariableType::ValueType>>& prop);

    void operator()() const;

    static constexpr Name name()
    {
        return Name::SETEQUAL;
    }

    constexpr Arity arity() const
    {
        return 2;
    }

    static constexpr unsigned short priority()
    {
        return 15;
    }

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

    template<typename SubPropertyType2>
    bool operator==(const SetEqualFun<SubPropertyType2>& ope) const;
    const ptr<IISubRuleFormula>& operator[](const size_t& k) const;

    ~SetEqualFun() = default;

    //Attributes
    std::tuple<const ptr<SubArithmeticRule<VariableType>>,const ptr<ASubArithmeticRule<typename VariableType::ValueType>>> m_sonProps;
    std::tuple<const ptr<IISubRuleFormula>,const ptr<IISubRuleFormula>> m_castSonProps;
};

template<size_t N, typename VariableType>
typename std::tuple_element<N,std::tuple<const ptr<SubArithmeticRule<VariableType>>,
const ptr<ASubArithmeticRule<typename VariableType::ValueType>>>>::type&
get(const SetEqualFun<SubArithmeticRule<VariableType>>& fun);


template<typename VariableType>
class SetEqual<SubArithmeticRule<VariableType>> :
        public Operator<SetEqualFun<SubArithmeticRule<VariableType>>>
{
public:
    using RuleOpe = SetEqual<SubArithmeticRule<VariableType>>;
    using TheoremOpe = SetEqual<SubArithmeticTheorem<VariableType>>;

    SetEqual() = default;
    SetEqual(const ptr<SubArithmeticRule<VariableType>>& var, const ptr<ASubArithmeticRule<typename VariableType::ValueType>>& prop);

    template<typename SubPropertyType2>
    bool operator==(const SetEqual<SubPropertyType2>& ope) const;
    const ptr<IISubRuleFormula>& operator[](const size_t& k) const;
};

template<size_t N, typename VariableType>
typename std::tuple_element<N,std::tuple<const ptr<SubArithmeticRule<VariableType>>,
const ptr<ASubArithmeticRule<typename VariableType::ValueType>>>>::type&
get(SetEqual<SubArithmeticRule<VariableType>>& fun);



template<typename VariableType>
struct SetEqualFun<SubArithmeticTheorem<VariableType>>
{
    using ValueType = void;
    using SubFormulaType = ASubArithmeticTheorem<ValueType>;

    SetEqualFun() = default;
    SetEqualFun(const ptr<SubArithmeticTheorem<VariableType>>& var,
                const ptr<ASubArithmeticTheorem<typename VariableType::ValueType>>& prop);

    void operator()() const;

    static constexpr Name name()
    {
        return Name::SETEQUAL;
    }

    constexpr Arity arity() const
    {
        return 2;
    }

    static constexpr unsigned short priority()
    {
        return 15;
    }

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

    template<typename SubPropertyType2>
    bool operator==(const SetEqualFun<SubPropertyType2>& ope) const;

    const ptr<IISubTheoremFormula>& operator[](const size_t& k) const;

    ~SetEqualFun() = default;

    //Attributes
    std::tuple<const ptr<SubArithmeticTheorem<VariableType>>,
    const ptr<ASubArithmeticTheorem<typename VariableType::ValueType>>> m_sonProps;
    std::tuple<const ptr<IISubTheoremFormula>,const ptr<IISubTheoremFormula>> m_castSonProps;
};

template<size_t N, typename VariableType>
typename std::tuple_element<N,std::tuple<const ptr<SubArithmeticTheorem<VariableType>>,
const ptr<ASubArithmeticTheorem<typename VariableType::ValueType>>>>::type&
get(const SetEqualFun<SubArithmeticTheorem<VariableType>>& fun);


template<typename VariableType>
class SetEqual<SubArithmeticTheorem<VariableType>> :
        public Operator<SetEqualFun<SubArithmeticTheorem<VariableType>>>
{
public:
    using RuleOpe = SetEqual<SubArithmeticRule<VariableType>>;
    using TheoremOpe = SetEqual<SubArithmeticTheorem<VariableType>>;

    SetEqual() = default;
    SetEqual(const ptr<SubArithmeticTheorem<VariableType>>& var,
             const ptr<ASubArithmeticTheorem<typename VariableType::ValueType>>& prop);

    template<typename SubPropertyType2>
    bool operator==(const SetEqual<SubPropertyType2>& ope) const;
    const ptr<IISubTheoremFormula>& operator[](const size_t& k) const;
};

template<size_t N, typename VariableType>
typename std::tuple_element<N,std::tuple<const ptr<SubArithmeticTheorem<VariableType>>,
const ptr<ASubArithmeticTheorem<typename VariableType::ValueType>>>>::type&
get(const SetEqual<SubArithmeticTheorem<VariableType>>& fun);


/**
 * Implementation of SetEqual<SubArithmeticRule<VariableType>>
 */

template<typename VariableType>
SetEqualFun<SubArithmeticRule<VariableType>>::
SetEqualFun(const ptr<SubArithmeticRule<VariableType>> &var, const ptr<ASubArithmeticRule<typename VariableType::ValueType>> &prop):
    m_sonProps(var,prop), m_castSonProps(var,prop)
{

}

template<typename VariableType>
void SetEqualFun<SubArithmeticRule<VariableType>>::operator()() const
{
    *std::get<0>(m_sonProps)=std::get<1>(m_sonProps)->evaluate();
}

template<typename VariableType>
std::string SetEqualFun<SubArithmeticRule<VariableType>>::symbol()
{
    return SetEqualToStr::s_symbol;
}

template<typename VariableType>
std::string SetEqualFun<SubArithmeticRule<VariableType>>::
toString(unsigned short priorityParent) const
{
    if(priorityParent<priority())
    {
        return "("+std::get<0>(m_sonProps)->toString()+symbol()+std::get<1>(m_sonProps)->toString(priority())+")";
    }
    else
    {
        return std::get<0>(m_sonProps)->toString()+symbol()+std::get<1>(m_sonProps)->toString(priority());
    }
}

template<typename VariableType> template<typename SubPropertyType2>
bool SetEqualFun<SubArithmeticRule<VariableType>>::
operator==(const SetEqualFun<SubPropertyType2> &ope) const
{
    return (*std::get<0>(m_sonProps)==*(std::get<0>(ope.m_sonProps)))&&(*std::get<1>(m_sonProps)==*(std::get<1>(ope.m_sonProps)));
}


template<typename VariableType>
SetEqual<SubArithmeticRule<VariableType>>::
SetEqual(const ptr<SubArithmeticRule<VariableType>> &var, const ptr<ASubArithmeticRule<typename VariableType::ValueType>> &prop):
    Operator<SetEqualFun<SubArithmeticRule<VariableType>>>(SETEQUAL,var,prop)
{

}


template<typename VariableType> template<typename SubPropertyType2>
bool SetEqual<SubArithmeticRule<VariableType>>::
operator==(const SetEqual<SubPropertyType2> &ope) const
{
    return this->m_fun==ope.m_fun;
}

/**
 * Implementation of SetEqual<VariableType,ASubArithmeticTheorem>
 */

template<typename VariableType>
SetEqualFun<SubArithmeticTheorem<VariableType>>::
SetEqualFun(const ptr<SubArithmeticTheorem<VariableType>> &var, const ptr<ASubArithmeticTheorem<typename VariableType::ValueType>> &prop):
    m_sonProps(var,prop), m_castSonProps(var,prop)
{

}


template<typename VariableType>
void SetEqualFun<SubArithmeticTheorem<VariableType>>::operator()() const
{
    *std::get<0>(m_sonProps)=std::get<1>(m_sonProps)->evaluate();
}

template<typename VariableType>
std::string SetEqualFun<SubArithmeticTheorem<VariableType>>::symbol()
{
    return SetEqualToStr::s_symbol;
}

template<typename VariableType>
std::string SetEqualFun<SubArithmeticTheorem<VariableType>>::
toString(unsigned short priorityParent) const
{
    if(priorityParent<priority())
    {
        return "("+std::get<0>(m_sonProps)->toString()+symbol()+std::get<1>(m_sonProps)->toString(priority())+")";
    }
    else
    {
        return std::get<0>(m_sonProps)->toString()+symbol()+std::get<1>(m_sonProps)->toString(priority());
    }
}

template<typename VariableType> template<typename SubPropertyType2>
bool SetEqualFun<SubArithmeticTheorem<VariableType>>::
operator==(const SetEqualFun<SubPropertyType2> &ope) const
{
    return (*std::get<0>(m_sonProps)==(*std::get<0>(ope.m_sonProps)))&&(*std::get<1>(m_sonProps)==(*std::get<1>(ope.m_sonProps)));
}


template<typename VariableType>
SetEqual<SubArithmeticTheorem<VariableType>>::
SetEqual(const ptr<SubArithmeticTheorem<VariableType>> &var, const ptr<ASubArithmeticTheorem<typename VariableType::ValueType>> &prop):
    Operator<SetEqualFun<SubArithmeticTheorem<VariableType>>>(SETEQUAL,var,prop)
{

}


template<typename VariableType> template<typename SubPropertyType2>
bool SetEqual<SubArithmeticTheorem<VariableType>>::
operator==(const SetEqual<SubPropertyType2> &ope) const
{
    return this->m_fun==ope.m_fun;
}

/**
  * get methods
  */
template<size_t N, typename VariableType>
typename std::tuple_element<N,std::tuple<const ptr<SubArithmeticRule<VariableType>>,
const ptr<ASubArithmeticRule<typename VariableType::ValueType>>>>::type&
get(const SetEqualFun<SubArithmeticRule<VariableType> > &fun)
{
    return std::get<N>(fun.m_sonProps);
}

template<size_t N, typename VariableType>
typename std::tuple_element<N,std::tuple<const ptr<SubArithmeticRule<VariableType>>,
const ptr<ASubArithmeticRule<typename VariableType::ValueType>>>>::type&
get(const SetEqual<SubArithmeticRule<VariableType> > &fun)
{
    return get<N>(fun.m_fun);
}

template<size_t N, typename VariableType>
typename std::tuple_element<N,std::tuple<const ptr<SubArithmeticTheorem<VariableType>>,
const ptr<ASubArithmeticTheorem<typename VariableType::ValueType>>>>::type&
get(const SetEqualFun<SubArithmeticTheorem<VariableType> > &fun)
{
    return std::get<N>(fun.m_sonProps);
}

template<size_t N, typename VariableType>
typename std::tuple_element<N,std::tuple<const ptr<SubArithmeticTheorem<VariableType>>,
const ptr<ASubArithmeticTheorem<typename VariableType::ValueType>>>>::type&
get(const SetEqual<SubArithmeticTheorem<VariableType> > &fun)
{
    return get<N>(fun.m_fun);
}

template<typename VariableType>
const ptr<IISubRuleFormula> &SetEqualFun<SubArithmeticRule<VariableType> >::
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
            throw std::runtime_error("SetEqual operator has only two arguments");
        }
    }
}

template<typename VariableType>
const ptr<IISubRuleFormula> &SetEqual<SubArithmeticRule<VariableType> >::
operator[](const size_t &k) const
{
    return this->m_fun[k];
}

template<typename VariableType>
const ptr<IISubTheoremFormula> &SetEqual<SubArithmeticTheorem<VariableType> >::operator[](const size_t &k) const
{
    return this->m_fun[k];
}

template<typename VariableType>
const ptr<IISubTheoremFormula> &N_DarkLogic::SetEqualFun<SubArithmeticTheorem<VariableType> >::operator[](const size_t &k) const
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
            throw std::runtime_error("SetEqual operator has only two arguments");
        }
    }
}

}
#endif // DARK_LOGIC_SETEQUAL_H
