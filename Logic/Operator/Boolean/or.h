#ifndef OR_H
#define OR_H
#include "Operator/operator.h"
#include "Formula/Boolean/asubrule.h"

namespace N_Logic{

struct OrToStr
{
    static const std::string s_symbol;
};

/**
 * Or Function operator
 * return prop1 || prop2
 */
template<typename SubPropertyType>
struct OrFun
{
    typedef bool ValueType;

    OrFun() = default;
    OrFun(const ptr<SubPropertyType>& prop1, const ptr<SubPropertyType>& prop2);


    static constexpr Name name()
    {
        return Name::OR;
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

    template<typename SubPropertyType2>
    bool operator==(const OrFun<SubPropertyType2>& ope) const;

    const ptr<SubPropertyType>& operator[](const size_t &k) const;

    ~OrFun() = default;
private:
    std::tuple<const ptr<SubPropertyType>,const ptr<SubPropertyType>> m_sonProps;
};

template<size_t N, typename SubPropertyType>
const ptr<SubPropertyType>&
get(const OrFun<SubPropertyType>& ope);

template<typename SubPropertyType>
class Or : public Operator<OrFun<SubPropertyType>>
{
public:
    Or() = default;
    Or(const ptr<SubPropertyType> &prop1, const ptr<SubPropertyType> &prop2);

    template<typename SubPropertyType2>
    bool operator==(const Or<SubPropertyType2>& ope) const;
    const ptr<SubPropertyType>& operator[](const size_t &k) const;
};

template<size_t N, typename SubPropertyType>
const ptr<SubPropertyType>&
get(const Or<SubPropertyType>& ope);


#include "or.hxx"
}


#endif // OR_H
