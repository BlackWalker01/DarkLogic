#ifndef THEOREMLET_H
#define THEOREMLET_H
#include "Formula/Boolean/theorem.h"
#include "subtheoremlet.h"

namespace N_Logic
{
template<typename SubTheoremType>
class Theorem<Let<SubTheoremType,ASubTheorem>>:
        public SubTheorem<Let<SubTheoremType,ASubTheorem>>
{
public:
    Theorem(const ptr<SubTheoremType>& leftFormula,
        const ptr<ASubTheorem>& rightFormula);

    ~Theorem() override = default;

    bool isEvaluated() const override final;
    bool canBeDemonstrated() const override final;
    bool evaluate() const override final;
    bool testEvaluate() const override final;

private:
    void _evaluate() const;
    void _canBeDemonstrated() const;

    const std::unique_ptr<std::unique_ptr<bool>> m_value;
    const std::unique_ptr<std::unique_ptr<bool>> m_canBeDemonstrated;
};

template<typename SubTheoremType>
Theorem<Let<SubTheoremType, ASubTheorem> >::
Theorem(const ptr<SubTheoremType>& leftFormula,
    const ptr<ASubTheorem>& rightFormula) :
    SubTheorem<Let<SubTheoremType, ASubTheorem>>(leftFormula, rightFormula),
    m_value(std::make_unique<std::unique_ptr<bool>>()),
    m_canBeDemonstrated(std::make_unique<std::unique_ptr<bool>>())
{
    _evaluate();
    _canBeDemonstrated();
}

template<typename SubTheoremType>
ptr<ASubTheorem> SubTheorem<Let<SubTheoremType, ASubTheorem> >::copyTheorem() const
{
    return std::make_shared<const Theorem<Let<SubTheoremType, ASubTheorem>>>(get<0>(*m_son),get<1>(*m_son));
}

template<typename SubTheoremType>
bool Theorem<Let<SubTheoremType, ASubTheorem> >::isEvaluated() const
{
    return (*m_value)!=nullptr;
}

template<typename SubTheoremType>
bool Theorem<Let<SubTheoremType, ASubTheorem> >::canBeDemonstrated() const
{
    return **m_value;
}

template<typename SubTheoremType>
bool Theorem<Let<SubTheoremType, ASubTheorem> >::evaluate() const
{
    return **m_value;
}

template<typename SubTheoremType>
bool Theorem<Let<SubTheoremType, ASubTheorem> >::testEvaluate() const
{
    return SubTheorem<Let<SubTheoremType, ASubTheorem> >::evaluate();
}

template<typename SubTheoremType>
void Theorem<Let<SubTheoremType, ASubTheorem> >::_evaluate() const
{
    try
    {
        (*m_value) = std::make_unique<bool>(this->m_son->evaluate());
    }
    catch (VariableException&)
    {

    }
}

template<typename SubTheoremType>
void Theorem<Let<SubTheoremType, ASubTheorem> >::_canBeDemonstrated() const
{
    *m_canBeDemonstrated = std::make_unique<bool>(ASubTheorem::canBeDemonstrated());
}


}

#endif // THEOREMLET_H
