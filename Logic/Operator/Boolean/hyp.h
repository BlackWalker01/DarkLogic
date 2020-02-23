#ifndef HYP_H
#define HYP_H
#include "Operator/operator.h"
#include "Variable/dbvar.h"
#include "Set/booleanset.h"
#include "Formula/Boolean/asubrule.h"
#include <sstream>

namespace N_Logic{

struct HypToStr
{
    static const std::string s_symbol;
};

/**
 * HypFun Function operator
 * {prop1;... propn} implProp
 * return (prop1 && ... && propn) => implProp
 */
template<typename SubPropertyType>
struct HypFun
{
    typedef bool ValueType;

    HypFun(const std::vector<ptr<SubPropertyType>>& props);


    Arity arity() const;
    static constexpr unsigned short priority();
    static constexpr Name name()
    {
        return Name::HYP;
    }

    static constexpr Associativity associativity()
    {
        return Associativity::RIGHT;
    }

    static std::string symbol();
    static constexpr NbArgBefore nbArgBefore()
    {
        return NbArgBefore::E_ZERO;
    }

    std::string toString(unsigned short priorityParent =1000) const;


    bool operator()() const;

    template<typename SubPropertyType2>
    bool operator==(const HypFun<SubPropertyType2>& ope) const;

    const ptr<SubPropertyType>& operator[](const size_t &k) const;

    ~HypFun() = default;
private:
    const std::vector<ptr<SubPropertyType>> m_sonProps;
};

template<typename SubPropertyType>
class Hyp: public Operator<HypFun<SubPropertyType>>
{
public:
    Hyp(const std::vector<ptr<SubPropertyType>>& props);

    template<typename SubPropertyType2>
    bool operator==(const Hyp<SubPropertyType2>& ope) const;
    const ptr<SubPropertyType>& operator[](const size_t &k) const;
};

#include "hyp.hxx"
}

#endif // HYP_H
