#ifndef PLUS_H
#define PLUS_H
#include "Operator/operator.h"
#include "Formula/Arithmetic/asubarithmeticrule.h"
#include "Formula/Arithmetic/asubarithmetictheorem.h"

namespace N_Logic{

template<typename ValueType1, typename ValueType2>
struct ValueTypeComputer;

template<typename ValueType1, typename ValueType2>
struct ValueTypeComputer
{
    typedef ValueType1 ValueType;
};

template<typename ValueType>
class ASubArithmeticRule;

template<typename ValueType>
class ASubArithmeticTheorem;

struct PlusToStr
{
    static const std::string s_symbol;
};

/**
 * Plus Function operator
 * return form1 + form2
 * SubFormulaType1 and SubFormulaType2 are either ASubArithmeticTheorem two of them or ASubArithmeticRule two of them
 */
template<typename SubFormulaType1, typename SubFormulaType2>
struct PlusFun;

template<typename ValueType1, typename ValueType2>
struct PlusFun<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2>>
{
    typedef typename ValueTypeComputer<ValueType1, ValueType2>::ValueType  ValueType;

    PlusFun() = default;
    PlusFun(const ptr<ASubArithmeticRule<ValueType1>>& prop1, const ptr<ASubArithmeticRule<ValueType2>>& prop2);

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
        return 6;
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

    template<typename SubFormulaType3, typename SubFormulaType4>
    bool operator==(const PlusFun<SubFormulaType3, SubFormulaType4>& ope) const;
    const ptr<IISubRuleFormula>& operator[](const size_t& k) const;

    ~PlusFun() = default;

    //Attributes
    std::tuple<const ptr<ASubArithmeticRule<ValueType1>>,const ptr<ASubArithmeticRule<ValueType2>>> m_sonProps;
    std::tuple<const ptr<IISubRuleFormula>,const ptr<IISubRuleFormula>> m_castSonProps;
};

template<size_t N, typename ValueType1, typename ValueType2>
typename std::tuple_element<N,std::tuple<const ptr<ASubArithmeticRule<ValueType1>>, const ptr<ASubArithmeticRule<ValueType2>>>>::type&
get(const PlusFun<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2>>& fun);


template<typename ValueType_>
struct PlusFun<ASubArithmeticRule<ValueType_>, ASubArithmeticRule<ValueType_>>
{
    typedef ValueType_ ValueType;

    PlusFun() = default;
    PlusFun(const ptr<ASubArithmeticRule<ValueType>>& prop1, const ptr<ASubArithmeticRule<ValueType>>& prop2);

    ValueType operator()() const;

    static constexpr Name name()
    {
        return Name::PLUS;
    }

    constexpr Arity arity() const
    {
        return 2;
    }

    static constexpr unsigned short priority()
    {
        return 6;
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

    template<typename SubFormulaType3, typename SubFormulaType4>
    bool operator==(const PlusFun<SubFormulaType3, SubFormulaType4>& ope) const;
    const ptr<IISubRuleFormula>& operator[](const size_t& k) const;

    ~PlusFun() = default;

    //Attributes
    std::tuple<const ptr<ASubArithmeticRule<ValueType>>, const ptr<ASubArithmeticRule<ValueType>>> m_sonProps;
    std::tuple<const ptr<IISubRuleFormula>,const ptr<IISubRuleFormula>> m_castSonProps;
};

template<size_t N, typename ValueType>
const ptr<ASubArithmeticRule<ValueType>>& get(const PlusFun<ASubArithmeticRule<ValueType>, ASubArithmeticRule<ValueType>>& fun);



template<typename ValueType1, typename ValueType2>
struct PlusFun<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2>>
{
    typedef typename ValueTypeComputer<ValueType1, ValueType2>::ValueType  ValueType;

    PlusFun() = default;
    PlusFun(const ptr<ASubArithmeticTheorem<ValueType1>>& prop1, const ptr<ASubArithmeticTheorem<ValueType2>>& prop2);

    ValueType operator()() const;

    static constexpr Name name()
    {
        return Name::PLUS;
    }

    constexpr Arity arity() const
    {
        return 2;
    }

    static constexpr unsigned short priority()
    {
        return 6;
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

    template<typename SubFormulaType3, typename SubFormulaType4>
    bool operator==(const PlusFun<SubFormulaType3, SubFormulaType4>& ope) const;
    const ptr<IISubTheoremFormula>& operator[](const size_t& k) const;

    ~PlusFun() = default;

    //Attributes
    std::tuple<const ptr<ASubArithmeticTheorem<ValueType1>>,const ptr<ASubArithmeticTheorem<ValueType2>>> m_sonProps;
    std::tuple<const ptr<IISubTheoremFormula>,const ptr<IISubTheoremFormula>> m_castSonProps;
    static const std::string s_symbol;
};

template<size_t N, typename ValueType1, typename ValueType2>
typename std::tuple_element<N,std::tuple<const ptr<ASubArithmeticTheorem<ValueType1>>,
const ptr<ASubArithmeticTheorem<ValueType2>>>>::type&
get(const PlusFun<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2>>& fun);

template<typename ValueType_>
struct PlusFun<ASubArithmeticTheorem<ValueType_>, ASubArithmeticTheorem<ValueType_>>
{
    typedef ValueType_ ValueType;

    PlusFun() = default;
    PlusFun(const ptr<ASubArithmeticTheorem<ValueType>>& prop1, const ptr<ASubArithmeticTheorem<ValueType>>& prop2);

    ValueType operator()() const;

    static constexpr Name name()
    {
        return Name::PLUS;
    }

    constexpr Arity arity() const
    {
        return 2;
    }

    static constexpr unsigned short priority()
    {
        return 6;
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

    template<typename SubFormulaType3, typename SubFormulaType4>
    bool operator==(const PlusFun<SubFormulaType3, SubFormulaType4>& ope) const;
    const ptr<IISubTheoremFormula>& operator[](const size_t& k) const;

    ~PlusFun() = default;

    //Attributes
    std::tuple<const ptr<ASubArithmeticTheorem<ValueType>>,const ptr<ASubArithmeticTheorem<ValueType>>> m_sonProps;
    std::tuple<const ptr<IISubTheoremFormula>,const ptr<IISubTheoremFormula>> m_castSonProps;
};

template<size_t N, typename ValueType>
const ptr<ASubArithmeticTheorem<ValueType>>& get(const PlusFun<ASubArithmeticTheorem<ValueType>, ASubArithmeticTheorem<ValueType>>& fun);


template<typename SubFormulaType1, typename SubFormulaType2>
struct PlusTypeComputer;

template<typename ValueType1, typename ValueType2>
struct PlusTypeComputer<ASubArithmeticRule<ValueType1>,ASubArithmeticRule<ValueType2>>
{
    using BracketType=IISubRuleFormula;
};

template<typename ValueType1, typename ValueType2>
struct PlusTypeComputer<ASubArithmeticTheorem<ValueType1>,ASubArithmeticTheorem<ValueType2>>
{
    using BracketType=IISubTheoremFormula;
};

template<typename SubFormulaType1, typename SubFormulaType2>
class Plus : public Operator<PlusFun<SubFormulaType1,SubFormulaType2>>
{
public:
    using BracketType = typename PlusTypeComputer<SubFormulaType1,SubFormulaType2>::BracketType;
    Plus() = default;
    Plus(const ptr<SubFormulaType1> &prop1, const ptr<SubFormulaType2> &prop2);

    typedef typename PlusFun<SubFormulaType1,SubFormulaType2>::ValueType ValueType;

    template<typename SubFormulaType3, typename SubFormulaType4>
    bool operator==(const Plus<SubFormulaType3, SubFormulaType4>& ope) const;
    const ptr<BracketType>& operator[](const size_t& k) const;
};

template<size_t N, typename SubFormulaType1, typename SubFormulaType2>
typename std::tuple_element<N,std::tuple<const ptr<SubFormulaType1>,const ptr<SubFormulaType2>>>::type&
get(const Plus<SubFormulaType1,SubFormulaType2>& fun);



/**
 * Plus constructors
 */
template<typename SubFormulaType1, typename SubFormulaType2>
Plus<SubFormulaType1,SubFormulaType2>::Plus(const ptr<SubFormulaType1> &prop1, const ptr<SubFormulaType2> &prop2):
    Operator<PlusFun<SubFormulaType1,SubFormulaType2>>(PLUS,prop1,prop2)
{
    /*static_assert (std::is_same<SubPropertyType,ASubTheorem>::value||std::is_same<SubPropertyType,ASubRule>::value,
                   "SubPropertyType in Plus operator must be a subTheorem or a subRule" );*/
}

template<typename SubFormulaType1, typename SubFormulaType2>
const ptr<typename Plus<SubFormulaType1,SubFormulaType2>::BracketType> &
Plus<SubFormulaType1,SubFormulaType2>::operator[](const size_t &k) const
{
    return this->m_fun[k];
}


template<typename SubFormulaType1, typename SubFormulaType2> template<typename SubFormulaType3, typename SubFormulaType4>
bool Plus<SubFormulaType1,SubFormulaType2>::operator==(const Plus<SubFormulaType3,SubFormulaType4> &ope) const
{
    return this->m_fun==ope.m_fun;
}

/**
 * PlusFun constructors
 */

template<typename ValueType1, typename ValueType2>
PlusFun<ASubArithmeticRule<ValueType1>,ASubArithmeticRule<ValueType2>>::PlusFun(const ptr<ASubArithmeticRule<ValueType1>> &prop1,
                                                                                const ptr<ASubArithmeticRule<ValueType2>> &prop2):
    m_sonProps(prop1,prop2), m_castSonProps(prop1,prop2)
{

}

template<typename SubValueType>
PlusFun<ASubArithmeticRule<SubValueType>,ASubArithmeticRule<SubValueType>>::PlusFun(const ptr<ASubArithmeticRule<SubValueType>> &prop1,
                                                                                const ptr<ASubArithmeticRule<SubValueType>> &prop2):
    m_sonProps(prop1,prop2), m_castSonProps(prop1,prop2)
{

}


template<typename ValueType1, typename ValueType2>
PlusFun<ASubArithmeticTheorem<ValueType1>,ASubArithmeticTheorem<ValueType2>>::PlusFun(const ptr<ASubArithmeticTheorem<ValueType1>> &prop1,
                                                                                const ptr<ASubArithmeticTheorem<ValueType2>> &prop2):
    m_sonProps(prop1,prop2), m_castSonProps(prop1,prop2)
{

}

template<typename SubValueType>
PlusFun<ASubArithmeticTheorem<SubValueType>,ASubArithmeticTheorem<SubValueType>>::PlusFun(const ptr<ASubArithmeticTheorem<SubValueType>> &prop1,
                                                                                const ptr<ASubArithmeticTheorem<SubValueType>> &prop2):
    m_sonProps(prop1,prop2), m_castSonProps(prop1,prop2)
{

}


/**
 * Operator ()
 */

template<typename ValueType1, typename ValueType2>
typename PlusFun<ASubArithmeticRule<ValueType1>,ASubArithmeticRule<ValueType2>>::ValueType
PlusFun<ASubArithmeticRule<ValueType1>,ASubArithmeticRule<ValueType2>>::operator()() const
{
    return static_cast<ValueType>(std::get<0>(m_sonProps)->evaluate()) + static_cast<ValueType>(std::get<1>(m_sonProps)->evaluate());
}

template<typename ValueType>
ValueType
PlusFun<ASubArithmeticRule<ValueType>,ASubArithmeticRule<ValueType>>::operator()() const
{
    return std::get<0>(m_sonProps)->evaluate() + std::get<1>(m_sonProps)->evaluate();
}

template<typename ValueType1, typename ValueType2>
typename PlusFun<ASubArithmeticTheorem<ValueType1>,ASubArithmeticTheorem<ValueType2>>::ValueType
PlusFun<ASubArithmeticTheorem<ValueType1>,ASubArithmeticTheorem<ValueType2>>::operator()() const
{
    return static_cast<ValueType>(std::get<0>(m_sonProps)->evaluate()) + static_cast<ValueType>(std::get<1>(m_sonProps)->evaluate());
}

template<typename ValueType>
ValueType
PlusFun<ASubArithmeticTheorem<ValueType>,ASubArithmeticTheorem<ValueType>>::operator()() const
{
    return std::get<0>(m_sonProps)->evaluate() + std::get<1>(m_sonProps)->evaluate();
}

/**
 * toString methods
 */

template<typename ValueType1, typename ValueType2>
std::string PlusFun<ASubArithmeticRule<ValueType1>,ASubArithmeticRule<ValueType2>>::toString(unsigned short priorityParent) const
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
std::string PlusFun<ASubArithmeticRule<ValueType>,ASubArithmeticRule<ValueType>>::toString(unsigned short priorityParent) const
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
std::string PlusFun<ASubArithmeticTheorem<ValueType1>,ASubArithmeticTheorem<ValueType2>>::toString(unsigned short priorityParent) const
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
std::string PlusFun<ASubArithmeticTheorem<ValueType>,ASubArithmeticTheorem<ValueType>>::toString(unsigned short priorityParent) const
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
bool PlusFun<ASubArithmeticRule<ValueType1>,ASubArithmeticRule<ValueType2>>::operator==(const PlusFun<SubFormulaType3,SubFormulaType4> &ope) const
{
    return ((*std::get<0>(m_sonProps))==(*std::get<0>(ope.m_sonProps)))&&((*std::get<1>(m_sonProps))==(*std::get<1>(ope.m_sonProps)));
}

template<typename ValueType> template<typename SubFormulaType3, typename SubFormulaType4>
bool PlusFun<ASubArithmeticRule<ValueType>,ASubArithmeticRule<ValueType>>::operator==(const PlusFun<SubFormulaType3,SubFormulaType4> &ope) const
{
    return ((*std::get<0>(m_sonProps))==(*std::get<0>(ope.m_sonProps)))&&((*std::get<1>(m_sonProps))==(*std::get<1>(ope.m_sonProps)));
}

template<typename ValueType1, typename ValueType2> template<typename SubFormulaType3, typename SubFormulaType4>
bool PlusFun<ASubArithmeticTheorem<ValueType1>,ASubArithmeticTheorem<ValueType2>>::operator==(const PlusFun<SubFormulaType3,SubFormulaType4> &ope) const
{
    return ((*std::get<0>(m_sonProps))==(*std::get<0>(ope.m_sonProps)))&&((*std::get<1>(m_sonProps))==(*std::get<1>(ope.m_sonProps)));
}

template<typename ValueType> template<typename SubFormulaType3, typename SubFormulaType4>
bool PlusFun<ASubArithmeticTheorem<ValueType>,ASubArithmeticTheorem<ValueType>>::operator==(const PlusFun<SubFormulaType3,SubFormulaType4> &ope) const
{
    return ((*std::get<0>(m_sonProps))==(*std::get<0>(ope.m_sonProps)))&&((*std::get<1>(m_sonProps))==(*std::get<1>(ope.m_sonProps)));
}


/**
  * get methods
  */
template<size_t N, typename ValueType1, typename ValueType2>
typename std::tuple_element<N, std::tuple<const ptr<ASubArithmeticRule<ValueType1>>, const ptr<ASubArithmeticRule<ValueType2> > >>::type&
get(const PlusFun<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2>>& fun)
{
    return std::get<N>(fun.m_sonProps);
}

template<size_t N, typename ValueType> const ptr<ASubArithmeticRule<ValueType>>&
get(const PlusFun<ASubArithmeticRule<ValueType>, ASubArithmeticRule<ValueType>>& fun)
{
    return std::get<N>(fun.m_sonProps);
}

template<size_t N, typename ValueType1, typename ValueType2>
typename std::tuple_element<N,std::tuple<const ptr<ASubArithmeticTheorem<ValueType1>>,
const ptr<ASubArithmeticTheorem<ValueType2>>>>::type&
get(const PlusFun<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2>>& fun)
{
    return std::get<N>(fun.m_sonProps);
}

template<size_t N, typename ValueType> const ptr<ASubArithmeticTheorem<ValueType>>&
get(const PlusFun<ASubArithmeticTheorem<ValueType>, ASubArithmeticTheorem<ValueType>>& fun)
{
    return std::get<N>(fun.m_sonProps);
}

template<size_t N, typename SubFormulaType1, typename SubFormulaType2>
typename std::tuple_element<N,std::tuple<const ptr<SubFormulaType1>,const ptr<SubFormulaType2>>>::type&
get(const Plus<SubFormulaType1,SubFormulaType2>& ope)
{
    return get<N>(ope.m_fun);
}

/**
 * symbol methods
 */

template<typename ValueType1, typename ValueType2>
std::string N_Logic::PlusFun<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> >::symbol()
{
    return PlusToStr::s_symbol;
}

template<typename ValueType>
std::string N_Logic::PlusFun<ASubArithmeticRule<ValueType>, ASubArithmeticRule<ValueType> >::symbol()
{
    return PlusToStr::s_symbol;
}

template<typename ValueType1, typename ValueType2>
std::string N_Logic::PlusFun<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> >::symbol()
{
    return PlusToStr::s_symbol;
}

template<typename ValueType>
std::string N_Logic::PlusFun<ASubArithmeticTheorem<ValueType>, ASubArithmeticTheorem<ValueType> >::symbol()
{
    return PlusToStr::s_symbol;
}

template<typename ValueType1, typename ValueType2>
const ptr<IISubRuleFormula> &PlusFun<ASubArithmeticRule<ValueType1>, ASubArithmeticRule<ValueType2> >::operator[](const size_t &k) const
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
            throw std::runtime_error("Plus operator has only two arguments");
        }
    }
}

template<typename ValueType_>
const ptr<IISubRuleFormula> &N_Logic::PlusFun<ASubArithmeticRule<ValueType_>, ASubArithmeticRule<ValueType_> >::operator[](const size_t &k) const
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
            throw std::runtime_error("Plus operator has only two arguments");
        }
    }
}

template<typename ValueType1, typename ValueType2>
const ptr<IISubTheoremFormula> &PlusFun<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> >::operator[](const size_t &k) const
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
            throw std::runtime_error("Plus operator has only two arguments");
        }
    }
}

template<typename ValueType_>
const ptr<IISubTheoremFormula > &
PlusFun<ASubArithmeticTheorem<ValueType_>, ASubArithmeticTheorem<ValueType_> >::operator[](const size_t &k) const
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
            throw std::runtime_error("Plus operator has only two arguments");
        }
    }
}

//#include "plus.hxx"
}
#endif // PLUS_H
