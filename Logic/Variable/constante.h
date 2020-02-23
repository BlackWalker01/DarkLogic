#ifndef CONSTANTE_H
#define CONSTANTE_H
#include "term.h"

namespace N_Logic {
template<typename SetType_>
class Constante: public AbstractTerm
{
public:
    typedef SetType_ SetType;
    typedef typename SetType::Type ValueType;

    Constante(const std::string& name_);
    Constante(const ValueType& val);
    Constante(const Constante& cste);

    //get operators
    constexpr ValueType evaluate() const;
    bool isConstant() const override;
    static constexpr bool isConstantExpr()
    {
        return true;
    }

    //comparison operators
    template<typename T>
    constexpr bool operator==(const T&) const;
    constexpr bool operator==(const ValueType& val2) const;
    template<typename SetType2>
    constexpr bool operator==(const Constante<SetType2>&) const;
    constexpr bool operator==(const Constante<SetType>& var) const;

    static std::string valToString(const ValueType& val);
    static ValueType stringToVal(const std::string& str);

    virtual ~Constante() override = default;
protected:
    const ValueType m_val;
};

#include "constante.hxx"
}
#endif // CONSTANTE_H
