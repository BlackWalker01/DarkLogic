#ifndef BELONGSTO_H
#define BELONGSTO_H
#include "Operator/operator.h"
#include "Formula/Arithmetic/asubarithmeticrule.h"
#include "Formula/Arithmetic/asubarithmetictheorem.h"

namespace N_Logic {

template<typename SubFormulaType, typename SubFormulaType2>
struct BelongsToFun;

template<typename SubFormulaType, typename SubFormulaType2>
class BelongsTo;

template<SubRuleFormula SubPropertyType>
class SubArithmeticRule;

template<SubTheoremFormula SubPropertyType>
class SubArithmeticTheorem;

struct BelongsToStr
{
    static const std::string s_symbol; //€
};

//Change VariableType to SetType
template<typename SetType>
struct BelongsToFun<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>>
{
    using ValueType = bool;
    using SubFormulaType = ASubArithmeticRule<SetType>;

    BelongsToFun() = default;
    BelongsToFun(const ptr<ASubArithmeticRule<typename SetType::Type>>& var,
                 const ptr<ASubArithmeticRule<SetType>>& prop);

    ValueType operator()() const;

    static constexpr Name name()
    {
        return Name::BELONGSTO;
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

    template<typename SubFormulaType3, typename SubFormulaType4>
    bool operator==(const BelongsToFun<SubFormulaType3,SubFormulaType4>& ope) const;
    const ptr<IISubRuleFormula>& operator[](const size_t& k) const;

    ~BelongsToFun() = default;

    //Attributes
    std::tuple<const ptr<ASubArithmeticRule<typename SetType::Type>>,
    const ptr<ASubArithmeticRule<SetType>>> m_sonProps;
    std::tuple<const ptr<IISubRuleFormula>,
    const ptr<IISubRuleFormula>> m_castSonProps;
};

template<size_t N, typename SetType>
typename std::tuple_element<N,std::tuple<const ptr<ASubArithmeticRule<typename SetType::Type>>,
const ptr<ASubArithmeticRule<SetType>>>>::type&
get(const BelongsToFun<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>>& fun);


template<typename SetType>
class BelongsTo<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>> :
        public Operator<BelongsToFun<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>>>
{
public:
    using RuleOpe = BelongsTo<ASubArithmeticRule<typename SetType::Type>, ASubArithmeticRule<SetType>>;
    using TheoremOpe = BelongsTo<ASubArithmeticTheorem<typename SetType::Type>, ASubArithmeticTheorem<SetType>>;

    BelongsTo() = default;
    BelongsTo(const ptr<ASubArithmeticRule<typename SetType::Type>>& var,
              const ptr<ASubArithmeticRule<SetType>>& prop);

    template<typename SubFormulaType3, typename SubFormulaType4>
    bool operator==(const BelongsTo<SubFormulaType3,SubFormulaType4>& ope) const;
    const ptr<IISubRuleFormula>& operator[](const size_t& k) const;
};

template<size_t N, typename SetType>
typename std::tuple_element<N,std::tuple<const ptr<ASubArithmeticRule<typename SetType::Type>>,
const ptr<ASubArithmeticRule<SetType>>>>::type&
get(const BelongsTo<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>>& fun);



template<typename SetType>
struct BelongsToFun<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType>>
{
    using ValueType = bool;
    using SubFormulaType = ASubArithmeticTheorem<SetType>;

    BelongsToFun() = default;
    BelongsToFun(const ptr<ASubArithmeticTheorem<typename SetType::Type>>& var,
                 const ptr<ASubArithmeticTheorem<SetType>>& prop);

    ValueType operator()() const;

    static constexpr Name name()
    {
        return Name::BELONGSTO;
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

    template<typename SubFormulaType3, typename SubFormulaType4>
    bool operator==(const BelongsToFun<SubFormulaType3,SubFormulaType4>& ope) const;
    const ptr<IISubTheoremFormula>& operator[](const size_t& k) const;

    ~BelongsToFun() = default;

    //Attributes
    std::tuple<const ptr<ASubArithmeticTheorem<typename SetType::Type>>,
    const ptr<ASubArithmeticTheorem<SetType>>> m_sonProps;
    std::tuple<const ptr<IISubTheoremFormula>,
    const ptr<IISubTheoremFormula>> m_castSonProps;
};

template<size_t N, typename SetType>
typename std::tuple_element<N,std::tuple<const ptr<ASubArithmeticTheorem<typename SetType::Type>>,
const ptr<ASubArithmeticTheorem<SetType>>>>::type&
get(const BelongsToFun<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType>>& fun);


template<typename SetType>
class BelongsTo<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType>> :
        public Operator<BelongsToFun<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType>>>
{
public:
    using RuleOpe = BelongsTo<ASubArithmeticRule<typename SetType::Type>, ASubArithmeticRule<SetType>>;
    using TheoremOpe = BelongsTo<ASubArithmeticTheorem<typename SetType::Type>, ASubArithmeticTheorem<SetType>>;

    BelongsTo() = default;
    BelongsTo(const ptr<ASubArithmeticTheorem<typename SetType::Type>>& var,
              const ptr<ASubArithmeticTheorem<SetType>>& prop);

    template<typename SubFormulaType3, typename SubFormulaType4>
    bool operator==(const BelongsTo<SubFormulaType3,SubFormulaType4>& ope) const;
    const ptr<IISubTheoremFormula>& operator[](const size_t& k) const;
};

template<size_t N, typename SetType>
typename std::tuple_element<N,std::tuple<const ptr<ASubArithmeticTheorem<typename SetType::Type>>,
const ptr<ASubArithmeticTheorem<SetType>>>>::type&
get(const BelongsTo<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType>>& fun);


/**
 * Implementation of BelongsTo<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>>
 */

template<typename SetType>
BelongsToFun<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>>::
BelongsToFun(const ptr<ASubArithmeticRule<typename SetType::Type>> &var,
             const ptr<ASubArithmeticRule<SetType>> &prop):
    m_sonProps(var,prop), m_castSonProps(var,prop)
{

}

template<typename SetType>
bool BelongsToFun<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>>::operator()() const
{
    //*std::get<0>(m_sonProps)=std::get<1>(m_sonProps)->evaluate();
    return true;
}

template<typename SetType>
std::string BelongsToFun<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>>::symbol()
{
    return BelongsToStr::s_symbol;
}

template<typename SetType>
std::string BelongsToFun<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>>::
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

template<typename SetType> template<typename SubPropertyType3, typename SubPropertyType4>
bool BelongsToFun<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>>::
operator==(const BelongsToFun<SubPropertyType3,SubPropertyType4> &ope) const
{
    return (*std::get<0>(m_sonProps)==*(std::get<0>(ope.m_sonProps)))&&(*std::get<1>(m_sonProps)==*(std::get<1>(ope.m_sonProps)));
}


template<typename SetType>
BelongsTo<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>>::
BelongsTo(const ptr<ASubArithmeticRule<typename SetType::Type>>& var, const ptr<ASubArithmeticRule<SetType>>& prop):
    Operator<BelongsToFun<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>>>(BELONGSTO,var,prop)
{

}

template<typename SetType> template<typename SubPropertyType3, typename SubPropertyType4>
bool BelongsTo<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>>::
operator==(const BelongsTo<SubPropertyType3,SubPropertyType4> &ope) const
{
    return this->m_fun==ope.m_fun;
}

/**
 * Implementation of BelongsTo<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType>>
 */

template<typename SetType>
BelongsToFun<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType>>::
BelongsToFun(const ptr<ASubArithmeticTheorem<typename SetType::Type>> &var, const ptr<ASubArithmeticTheorem<SetType>> &prop):
    m_sonProps(var,prop), m_castSonProps(var,prop)
{

}


template<typename SetType>
bool BelongsToFun<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType>>::operator()() const
{
    //*std::get<0>(m_sonProps)=std::get<1>(m_sonProps)->evaluate();
    return true;
}

template<typename SetType>
std::string BelongsToFun<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType>>::symbol()
{
    return BelongsToStr::s_symbol;
}

template<typename SetType>
std::string BelongsToFun<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType>>::
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

template<typename SetType> template<typename SubPropertyType3, typename SubPropertyType4>
bool BelongsToFun<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType>>::
operator==(const BelongsToFun<SubPropertyType3,SubPropertyType4> &ope) const
{
    return (*std::get<0>(m_sonProps)==(*std::get<0>(ope.m_sonProps))) && (*std::get<1>(m_sonProps)==(*std::get<1>(ope.m_sonProps)));
}


template<typename SetType>
BelongsTo<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType>>::
BelongsTo(const ptr<ASubArithmeticTheorem<typename SetType::Type>> &var, const ptr<ASubArithmeticTheorem<SetType>> &prop):
    Operator<BelongsToFun<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType>>>(BELONGSTO,var,prop)
{

}


template<typename SetType> template<typename SubPropertyType3, typename SubPropertyType4>
bool BelongsTo<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType>>::
operator==(const BelongsTo<SubPropertyType3,SubPropertyType4> &ope) const
{
    return this->m_fun==ope.m_fun;
}

/**
  * get methods
  */
template<size_t N, typename SetType>
typename std::tuple_element<N,std::tuple<const ptr<const ASubArithmeticRule<typename SetType::Type>>,
const ptr<const ASubArithmeticRule<SetType>>>>::type&
get(const BelongsToFun<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>> &fun)
{
    return std::get<N>(fun.m_sonProps);
}

template<size_t N, typename SetType>
typename std::tuple_element<N,std::tuple<const ptr<ASubArithmeticRule<typename SetType::Type>>,
const ptr<ASubArithmeticRule<SetType>>>>::type&
get(const BelongsTo<ASubArithmeticRule<typename SetType::Type>, ASubArithmeticRule<SetType> > &fun)
{
    return get<N>(fun.m_fun);
}

template<size_t N, typename SetType>
typename std::tuple_element<N,std::tuple<const ptr<ASubArithmeticTheorem<typename SetType::Type>>,
const ptr<ASubArithmeticTheorem<SetType>>>>::type&
get(const BelongsToFun<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType> > &fun)
{
    return std::get<N>(fun.m_sonProps);
}

template<size_t N, typename SetType>
typename std::tuple_element<N,std::tuple<const ptr<const ASubArithmeticTheorem<typename SetType::Type>>,
const ptr<const ASubArithmeticTheorem<SetType>>>>::type&
get(const BelongsTo<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType> > &fun)
{
    return get<N>(fun.m_fun);
}

template<typename SetType>
const ptr<IISubRuleFormula> &BelongsToFun<ASubArithmeticRule<typename SetType::Type>, ASubArithmeticRule<SetType> >::
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

template<typename SetType>
const ptr<IISubRuleFormula> &BelongsTo<ASubArithmeticRule<typename SetType::Type>, ASubArithmeticRule<SetType> >::
operator[](const size_t &k) const
{
    return this->m_fun[k];
}

template<typename SetType>
const ptr<IISubTheoremFormula> &BelongsToFun<ASubArithmeticTheorem<typename SetType::Type>, ASubArithmeticTheorem<SetType> >::
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

template<typename SetType>
const ptr<IISubTheoremFormula> &N_Logic::BelongsTo<ASubArithmeticTheorem<typename SetType::Type>, ASubArithmeticTheorem<SetType> >::
operator[](const size_t &k) const
{
    return this->m_fun[k];
}

}
#endif // BELONGSTO_H
