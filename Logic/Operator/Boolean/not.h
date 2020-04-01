#ifndef NOT_H
#define NOT_H
#include "Operator/operator.h"
#include "Formula/Boolean/asubrule.h"

namespace N_Logic{

struct NotToStr
{
    static const std::string s_symbol;
};

/**
 * Not Function operator
 * return not prop1
 */
template<Proposition SubPropertyType>
struct NotFun
{
    typedef bool ValueType;

    NotFun() = default;
    NotFun(const ptr<SubPropertyType>& prop1);


    static constexpr Name name()
    {
        return Name::NOT;
    }

    constexpr Arity arity() const
    {
        return 1;
    }

    static constexpr unsigned short priority();

    static constexpr Associativity associativity()
    {
        return Associativity::LEFT;
    }

    static std::string symbol();
    static constexpr NbArgBefore nbArgBefore()
    {
        return NbArgBefore::E_ZERO;
    }


    bool operator()() const;

    std::string toString(unsigned short priorityParent=1000) const;

    template<Proposition SubPropertyType2>
    bool operator==(const NotFun<SubPropertyType2>& ope) const;

    const ptr<SubPropertyType>& operator[](const size_t &k) const;

    ~NotFun() = default;
private:
    const ptr<SubPropertyType> m_sonProp;
};

template<Proposition SubPropertyType>
const ptr<SubPropertyType>&
get(const NotFun<SubPropertyType>& ope);

template<Proposition SubPropertyType>
class Not : public Operator<NotFun<SubPropertyType>>
{
public:
    Not() = default;
    Not(const ptr<SubPropertyType> &prop1);

    template<Proposition SubPropertyType2>
    bool operator==(const Not<SubPropertyType2>& ope) const;
    const ptr<SubPropertyType>& operator[](const size_t &k) const;
};

template<Proposition SubPropertyType>
const ptr<SubPropertyType>&
get(const Not<SubPropertyType>& ope);

#include "not.hxx"
}

#endif // NOT_H
