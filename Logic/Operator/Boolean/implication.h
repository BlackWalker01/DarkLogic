#ifndef IMPLICATION_H
#define IMPLICATION_H
#include "Operator/operator.h"
#include "Formula/Boolean/asubrule.h"

namespace N_Logic{

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
    typedef bool ValueType;

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
private:
    std::tuple<const ptr<const SubPropertyType>,const ptr<const SubPropertyType>> m_sonProps;
};

template<size_t N, Proposition SubPropertyType>
const ptr<SubPropertyType>&
get(const ImplFun<SubPropertyType>& ope);

template<Proposition SubPropertyType>
class Implication : public Operator<ImplFun<SubPropertyType>>
{
public:
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


#endif // IMPLICATION_H
