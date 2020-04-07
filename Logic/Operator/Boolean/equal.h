#ifndef EQUAL_H
#define EQUAL_H
#include "Operator/operator.h"
#include "Formula/Arithmetic/asubarithmeticrule.h"
#include "Formula/Arithmetic/asubarithmetictheorem.h"

namespace N_Logic{

template<ArithmeticType ValueType>
class ASubArithmeticTheorem;

struct EqualToStr
{
    static const std::string s_symbol;
};

/**
 * Equal Function operator
 * return form1 + form2
 * SubFormulaType1 and SubFormulaType2 are either ASubArithmeticTheorem two of them or ASubArithmeticRule two of them
 */
template<typename SubFormulaType1, typename SubFormulaType2>
struct EqualFun;

template<typename ValueType1, typename ValueType2>
struct EqualFun<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2>>
{
    using ValueType = bool;
    using SubFormulaType = ASubRule;

    EqualFun() = default;
    EqualFun(const ptr<ASubArithmeticRule<ValueType1>>& prop1, const ptr<ASubArithmeticRule<ValueType2>>& prop2);

    ValueType operator()() const;

    static constexpr Name name()
    {
        return Name::EQUAL;
    }

    constexpr Arity arity() const
    {
        return 2;
    }

    static constexpr unsigned short priority()
    {
        return 9;
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
    bool operator==(const EqualFun<SubFormulaType3, SubFormulaType4>& ope) const;
    const ptr<IISubRuleFormula>& operator[](const size_t& k) const;

    ~EqualFun() = default;

    //Attributes
    std::tuple<const ptr<ASubArithmeticRule<ValueType1>>,
    const ptr<ASubArithmeticRule<ValueType2>>> m_sonProps;
    std::tuple<const ptr<IISubRuleFormula>,
    const ptr<IISubRuleFormula>> m_castSonProps;
};

template<size_t N, typename ValueType1, typename ValueType2>
typename std::tuple_element<N,std::tuple<const ptr<ASubArithmeticRule<ValueType1>>,
const ptr<ASubArithmeticRule<ValueType2>>>>::type&
get(const EqualFun<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2>>& fun);


template<typename SubValueType>
struct EqualFun<ASubArithmeticRule<SubValueType>, ASubArithmeticRule<SubValueType>>
{
    using ValueType = bool;
    using SubFormulaType = ASubRule;

    EqualFun() = default;
    EqualFun(const ptr<ASubArithmeticRule<SubValueType>>& prop1, const ptr<ASubArithmeticRule<SubValueType>>& prop2);

    ValueType operator()() const;
    static constexpr Name name()
    {
        return Name::EQUAL;
    }

    constexpr Arity arity() const
    {
        return 2;
    }

    static constexpr unsigned short priority()
    {
        return 9;
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
    bool operator==(const EqualFun<SubFormulaType3, SubFormulaType4>& ope) const;
    const ptr<IISubRuleFormula>& operator[](const size_t& k) const;

    ~EqualFun() = default;

    //Attributes
    std::tuple<const ptr<ASubArithmeticRule<SubValueType>>,
    const ptr<ASubArithmeticRule<SubValueType>>> m_sonProps;
    std::tuple<const ptr<IISubRuleFormula>,
    const ptr<IISubRuleFormula>> m_castSonProps;
};

template<size_t N, typename ValueType>
const ptr<ASubArithmeticRule<ValueType>>& get(const EqualFun<ASubArithmeticRule<ValueType>, ASubArithmeticRule<ValueType>>& fun);



template<typename ValueType1, typename ValueType2>
struct EqualFun<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2>>
{
    using ValueType = bool;
    using SubFormulaType = ASubTheorem;

    EqualFun() = default;
    EqualFun(const ptr<ASubArithmeticTheorem<ValueType1>>& prop1, const ptr<ASubArithmeticTheorem<ValueType2>>& prop2);

    ValueType operator()() const;

    static constexpr Name name()
    {
        return Name::EQUAL;
    }

    constexpr Arity arity() const
    {
        return 2;
    }

    static constexpr unsigned short priority()
    {
        return 9;
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

    void add(ASubArithmeticTheorem<ValueType1> &prop);
    void add(ASubArithmeticTheorem<ValueType2> &prop);

    std::string toString(unsigned short priorityParent=1000) const;

    template<typename SubFormulaType3, typename SubFormulaType4>
    bool operator==(const EqualFun<SubFormulaType3, SubFormulaType4>& ope) const;
    const ptr<IISubTheoremFormula>& operator[](const size_t& k) const;

    ~EqualFun() = default;

    //Attributes
    std::tuple<const ptr<ASubArithmeticTheorem<ValueType1>>,
    const ptr<ASubArithmeticTheorem<ValueType2>>> m_sonProps;
    std::tuple<const ptr<IISubTheoremFormula>,
    const ptr<IISubTheoremFormula>> m_castSonProps;
};

template<size_t N, typename ValueType1, typename ValueType2>
typename std::tuple_element<N,std::tuple<const ptr<ASubArithmeticTheorem<ValueType1>>,
const ptr<ASubArithmeticTheorem<ValueType2>>>>::type&
get(const EqualFun<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2>>& fun);

template<typename SubValueType>
struct EqualFun<ASubArithmeticTheorem<SubValueType>, ASubArithmeticTheorem<SubValueType>>
{
    using ValueType = bool;
    using SubFormulaType = ASubTheorem;

    EqualFun() = default;
    EqualFun(const ptr<ASubArithmeticTheorem<SubValueType>>& prop1,
             const ptr<ASubArithmeticTheorem<SubValueType>>& prop2);

    ValueType operator()() const;

    static constexpr Name name()
    {
        return Name::EQUAL;
    }

    constexpr Arity arity() const
    {
        return 2;
    }

    static constexpr unsigned short priority()
    {
        return 9;
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
    bool operator==(const EqualFun<SubFormulaType3, SubFormulaType4>& ope) const;

    const ptr<IISubTheoremFormula>& operator[](const size_t& k) const;

    ~EqualFun() = default;

    //Attributes
    std::tuple<const ptr<ASubArithmeticTheorem<SubValueType>>,
    const ptr<ASubArithmeticTheorem<SubValueType>>> m_sonProps;
    std::tuple<const ptr<IISubTheoremFormula>,
    const ptr<IISubTheoremFormula>> m_castSonProps;
};

template<size_t N, typename ValueType>
const ptr<ASubArithmeticTheorem<ValueType>>&
get(const EqualFun<ASubArithmeticTheorem<ValueType>, ASubArithmeticTheorem<ValueType>>& fun);

template<typename SubFormulaType1, typename SubFormulaType2>
struct EqualTypeComputer;


template<typename ValueType1, typename ValueType2>
struct EqualTypeComputer<ASubArithmeticRule<ValueType1>,ASubArithmeticRule<ValueType2>>
{
    using BracketType=IISubRuleFormula;
};

template<typename ValueType1, typename ValueType2>
struct EqualTypeComputer<ASubArithmeticTheorem<ValueType1>,ASubArithmeticTheorem<ValueType2>>
{
    using BracketType=IISubTheoremFormula;
};

template<typename SubFormulaType1, typename SubFormulaType2>
class Equal : public Operator<EqualFun<SubFormulaType1,SubFormulaType2>>
{
public:
    typedef typename EqualTypeComputer<SubFormulaType1,SubFormulaType2>::BracketType BracketType;
    using RuleOpe = Equal<ToRule<SubFormulaType1>, ToRule<SubFormulaType2>>;
    using TheoremOpe = Equal<ToTheorem<SubFormulaType1>, ToTheorem<SubFormulaType2>>;

    Equal() = default;
    Equal(const ptr<SubFormulaType1> &prop1, const ptr<SubFormulaType2> &prop2);

    typedef typename EqualFun<SubFormulaType1,SubFormulaType2>::ValueType ValueType;

    template<typename SubFormulaType3, typename SubFormulaType4>
    bool operator==(const Equal<SubFormulaType3, SubFormulaType4>& ope) const;

    const ptr<BracketType>& operator[](const size_t& k) const;
};

template<size_t N, typename SubFormulaType1, typename SubFormulaType2>
typename std::tuple_element<N,std::tuple<const ptr<SubFormulaType1>,const ptr<SubFormulaType2>>>::type&
get(const Equal<SubFormulaType1,SubFormulaType2>& fun);



/**
 * Equal constructors
 */
template<typename SubFormulaType1, typename SubFormulaType2>
Equal<SubFormulaType1,SubFormulaType2>::Equal(const ptr<SubFormulaType1> &prop1, const ptr<SubFormulaType2> &prop2):
    Operator<EqualFun<SubFormulaType1,SubFormulaType2>>(EQUAL,prop1,prop2)
{
    /*static_assert (std::is_same<SubPropertyType,ASubTheorem>::value||std::is_same<SubPropertyType,ASubRule>::value,
                   "SubPropertyType in Equal operator must be a subTheorem or a subRule" );*/
}

template<typename SubFormulaType1, typename SubFormulaType2>
const ptr<typename Equal<SubFormulaType1,SubFormulaType2>::BracketType>&
Equal<SubFormulaType1,SubFormulaType2>::operator[](const size_t &k) const
{
    return this->m_fun[k];
}


/**
 * EqualFun constructors
 */

template<typename ValueType1, typename ValueType2>
EqualFun<ASubArithmeticRule<ValueType1>,ASubArithmeticRule<ValueType2>>::EqualFun(const ptr<ASubArithmeticRule<ValueType1>> &prop1,
                                                                                const ptr<ASubArithmeticRule<ValueType2>> &prop2):
    m_sonProps(prop1,prop2), m_castSonProps(prop1,prop2)
{

}


template<typename SubValueType>
EqualFun<ASubArithmeticRule<SubValueType>,ASubArithmeticRule<SubValueType>>::EqualFun(const ptr<ASubArithmeticRule<SubValueType>> &prop1,
                                                                                const ptr<ASubArithmeticRule<SubValueType>> &prop2):
    m_sonProps(prop1,prop2), m_castSonProps(prop1,prop2)
{

}


template<typename ValueType1, typename ValueType2>
EqualFun<ASubArithmeticTheorem<ValueType1>,ASubArithmeticTheorem<ValueType2>>::EqualFun(const ptr<ASubArithmeticTheorem<ValueType1>> &prop1,
                                                                                const ptr<ASubArithmeticTheorem<ValueType2>> &prop2):
    m_sonProps(prop1,prop2), m_castSonProps(prop1,prop2)
{

}


template<typename SubValueType>
EqualFun<ASubArithmeticTheorem<SubValueType>,ASubArithmeticTheorem<SubValueType>>::EqualFun(const ptr<ASubArithmeticTheorem<SubValueType>> &prop1,
                                                                                const ptr<ASubArithmeticTheorem<SubValueType>> &prop2):
    m_sonProps(prop1,prop2), m_castSonProps(prop1,prop2)
{

}

/**
 * Operator ()
 */

template<typename ValueType1, typename ValueType2>
bool
EqualFun<ASubArithmeticRule<ValueType1>,ASubArithmeticRule<ValueType2>>::operator()() const
{
    return static_cast<ValueType>(std::get<0>(m_sonProps)->evaluate()) == static_cast<ValueType>(std::get<1>(m_sonProps)->evaluate());
}

template<typename ValueType>
bool
EqualFun<ASubArithmeticRule<ValueType>,ASubArithmeticRule<ValueType>>::operator()() const
{
    return std::get<0>(m_sonProps)->evaluate() == std::get<1>(m_sonProps)->evaluate();
}

template<typename ValueType1, typename ValueType2>
bool
EqualFun<ASubArithmeticTheorem<ValueType1>,ASubArithmeticTheorem<ValueType2>>::operator()() const
{
    return static_cast<ValueType>(std::get<0>(m_sonProps)->evaluate()) == static_cast<ValueType>(std::get<1>(m_sonProps)->evaluate());
}

template<typename ValueType>
bool
EqualFun<ASubArithmeticTheorem<ValueType>,ASubArithmeticTheorem<ValueType>>::operator()() const
{
    return std::get<0>(m_sonProps)->evaluate() == std::get<1>(m_sonProps)->evaluate();
}


template<typename ValueType1, typename ValueType2>
void EqualFun<ASubArithmeticTheorem<ValueType1>,ASubArithmeticTheorem<ValueType2>>::add(ASubArithmeticTheorem<ValueType1> &prop)
{
    std::get<0>(m_sonProps)=&prop;
}

template<typename ValueType1, typename ValueType2>
void EqualFun<ASubArithmeticTheorem<ValueType1>,ASubArithmeticTheorem<ValueType2>>::add(ASubArithmeticTheorem<ValueType2> &prop)
{
    std::get<1>(m_sonProps)=&prop;
}


/**
  * symbol methods
  */
template<typename ValueType1, typename ValueType2>
std::string N_Logic::EqualFun<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> >::symbol()
{
    return EqualToStr::s_symbol;
}

template<typename SubValueType>
std::string N_Logic::EqualFun<ASubArithmeticTheorem<SubValueType>, ASubArithmeticTheorem<SubValueType> >::symbol()
{
    return EqualToStr::s_symbol;
}

template<typename ValueType1, typename ValueType2>
std::string N_Logic::EqualFun<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> >::symbol()
{
    return EqualToStr::s_symbol;
}

template<typename SubValueType>
std::string N_Logic::EqualFun<ASubArithmeticRule<SubValueType>, ASubArithmeticRule<SubValueType> >::symbol()
{
    return EqualToStr::s_symbol;
}

/**
 * toString methods
 */

template<typename ValueType1, typename ValueType2>
std::string EqualFun<ASubArithmeticRule<ValueType1>,ASubArithmeticRule<ValueType2>>::toString(unsigned short priorityParent) const
{
    if(priorityParent<priority())
    {
        return "("+std::get<0>(m_sonProps)->toString(priority())+symbol()+std::get<1>(m_sonProps)->toString(priority())+")";
    }
    else
    {
        return std::get<0>(m_sonProps)->toString(priority())+symbol()+std::get<1>(m_sonProps)->toString(priority());
    }
}

template<typename ValueType>
std::string EqualFun<ASubArithmeticRule<ValueType>,ASubArithmeticRule<ValueType>>::toString(unsigned short priorityParent) const
{
    if(priorityParent<priority())
    {
        return "("+std::get<0>(m_sonProps)->toString(priority())+symbol()+std::get<1>(m_sonProps)->toString(priority())+")";
    }
    else
    {
        return std::get<0>(m_sonProps)->toString(priority())+symbol()+std::get<1>(m_sonProps)->toString(priority());
    }
}

template<typename ValueType1, typename ValueType2>
std::string EqualFun<ASubArithmeticTheorem<ValueType1>,ASubArithmeticTheorem<ValueType2>>::toString(unsigned short priorityParent) const
{
    if(priorityParent<priority())
    {
        return "("+std::get<0>(m_sonProps)->toString(priority())+symbol()+std::get<1>(m_sonProps)->toString(priority())+")";
    }
    else
    {
        return std::get<0>(m_sonProps)->toString(priority())+symbol()+std::get<1>(m_sonProps)->toString(priority());
    }
}

template<typename ValueType>
std::string EqualFun<ASubArithmeticTheorem<ValueType>,ASubArithmeticTheorem<ValueType>>::toString(unsigned short priorityParent) const
{
    if(priorityParent<priority())
    {
        return "("+std::get<0>(m_sonProps)->toString(priority())+symbol()+std::get<1>(m_sonProps)->toString(priority())+")";
    }
    else
    {
        return std::get<0>(m_sonProps)->toString(priority())+symbol()+std::get<1>(m_sonProps)->toString(priority());
    }
}


/**
 * operator ==
 */

template<typename ValueType1, typename ValueType2> template<typename SubFormulaType3, typename SubFormulaType4>
bool EqualFun<ASubArithmeticRule<ValueType1>,ASubArithmeticRule<ValueType2>>::operator==(const EqualFun<SubFormulaType3,SubFormulaType4> &ope) const
{
    return ((*std::get<0>(m_sonProps))==(*std::get<0>(ope.m_sonProps)))&&((*std::get<1>(m_sonProps))==(*std::get<1>(ope.m_sonProps)));
}

template<typename ValueType> template<typename SubFormulaType3, typename SubFormulaType4>
bool EqualFun<ASubArithmeticRule<ValueType>,ASubArithmeticRule<ValueType>>::operator==(const EqualFun<SubFormulaType3,SubFormulaType4> &ope) const
{
    return ((*std::get<0>(m_sonProps))==(*std::get<0>(ope.m_sonProps)))&&((*std::get<1>(m_sonProps))==(*std::get<1>(ope.m_sonProps)));
}

template<typename ValueType1, typename ValueType2> template<typename SubFormulaType3, typename SubFormulaType4>
bool EqualFun<ASubArithmeticTheorem<ValueType1>,ASubArithmeticTheorem<ValueType2>>::operator==(const EqualFun<SubFormulaType3,SubFormulaType4> &ope) const
{
    return ((*std::get<0>(m_sonProps))==(*std::get<0>(ope.m_sonProps)))&&((*std::get<1>(m_sonProps))==(*std::get<1>(ope.m_sonProps)));
}

template<typename ValueType> template<typename SubFormulaType3, typename SubFormulaType4>
bool EqualFun<ASubArithmeticTheorem<ValueType>,ASubArithmeticTheorem<ValueType>>::operator==(const EqualFun<SubFormulaType3,SubFormulaType4> &ope) const
{
    return ((*std::get<0>(m_sonProps))==(*std::get<0>(ope.m_sonProps)))&&((*std::get<1>(m_sonProps))==(*std::get<1>(ope.m_sonProps)));
}


/**
  * get methods
  */
template<size_t N, typename ValueType1, typename ValueType2>
typename std::tuple_element<N,std::tuple<const ptr<const ASubArithmeticRule<ValueType1>>,
const ptr<const ASubArithmeticRule<ValueType2>>>>::type&
get(const EqualFun<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2>>& fun)
{
    return std::get<N>(fun.m_sonProps);
}

template<size_t N, typename ValueType>
const ptr<const ASubArithmeticRule<ValueType>>&
get(const EqualFun<ASubArithmeticRule<ValueType>, ASubArithmeticRule<ValueType>>& fun)
{
    return std::get<N>(fun.m_sonProps);
}

template<size_t N, typename ValueType1, typename ValueType2>
typename std::tuple_element<N,std::tuple<const ptr<const ASubArithmeticTheorem<ValueType1>>,
const ptr<const ASubArithmeticTheorem<ValueType2>>>>::type&
get(const EqualFun<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2>>& fun)
{
    return std::get<N>(fun.m_sonProps);
}

template<size_t N, typename ValueType>
const ptr<ASubArithmeticTheorem<ValueType>> &get(const EqualFun<ASubArithmeticTheorem<ValueType>, ASubArithmeticTheorem<ValueType>>& fun)
{
    return std::get<N>(fun.m_sonProps);
}

template<size_t N, typename SubFormulaType1, typename SubFormulaType2>
typename std::tuple_element<N,std::tuple<const ptr<const SubFormulaType1>,const ptr<const SubFormulaType2>>>::type&
get(const Equal<SubFormulaType1,SubFormulaType2>& ope)
{
    return get<N>(ope.m_fun);
}

template<typename ValueType1, typename ValueType2>
const ptr<IISubRuleFormula> &
N_Logic::EqualFun<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> >::operator[](const size_t &k) const
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
            throw std::runtime_error("Equal operator has only two arguments");
        }
    }
}

template<typename SubValueType>
const ptr<IISubRuleFormula > &
N_Logic::EqualFun<ASubArithmeticRule<SubValueType>, ASubArithmeticRule<SubValueType> >::operator[](const size_t &k) const
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
            throw std::runtime_error("Equal operator has only two arguments");
        }
    }
}

template<typename SubValueType>
const ptr<IISubTheoremFormula > &N_Logic::EqualFun<ASubArithmeticTheorem<SubValueType>,
ASubArithmeticTheorem<SubValueType> >::operator[](const size_t &k) const
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
            throw std::runtime_error("Equal operator has only two arguments");
        }
    }
}

template<typename ValueType1, typename ValueType2>
const ptr<IISubTheoremFormula> &
EqualFun<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> >::operator[](const size_t &k) const
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
            throw std::runtime_error("Equal operator has only two arguments");
        }
    }
}

template<typename SubFormulaType, typename SubFormulaType2> template<typename SubFormulaType3, typename SubFormulaType4>
bool Equal<SubFormulaType,SubFormulaType2>::operator==(const Equal<SubFormulaType3, SubFormulaType4> &ope) const
{
    return this->m_fun==ope.m_fun;
}


//#include "equal.hxx"
}
#endif // EQUAL_H

