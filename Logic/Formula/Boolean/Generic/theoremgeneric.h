#ifndef THEOREMGENERIC_H
#define THEOREMGENERIC_H
#include "Formula/Boolean/theorem.h"

namespace N_Logic
{
template<SubTheoremProperty SubPropertyType>
class Theorem: public SubTheorem<SubPropertyType>
{
public:
    Theorem(const ptr<ASubTheorem>& leftSubProp, const ptr<ASubTheorem>& rightSubProp);


    ptr<IISubTheoremFormula> ruleApply(const IISubRuleFormula& rule, std::vector<size_t>& indexes,
                                       const size_t& actionKey) const override final;

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

template<SubTheoremProperty SubPropertyType> struct ToRuleStruct<Theorem<SubPropertyType>> { using Type = Rule<SubPropertyType>; };


template<SubTheoremProperty SubPropertyType>
Theorem<SubPropertyType>::Theorem(const ptr<ASubTheorem> &leftSubProp, const ptr<ASubTheorem> &rightSubProp):
    SubTheorem<SubPropertyType>(leftSubProp,rightSubProp),
    m_value(std::make_unique<std::unique_ptr<bool>>()),
    m_canBeDemonstrated(std::make_unique<std::unique_ptr<bool>>())
{
    _evaluate();
    _canBeDemonstrated();
}

template<SubTheoremProperty SubPropertyType>
bool Theorem<SubPropertyType>::isEvaluated() const
{
    return (*m_value)!=nullptr;
}

template<SubTheoremProperty SubPropertyType>
bool Theorem<SubPropertyType>::canBeDemonstrated() const
{
    return **m_canBeDemonstrated;
}

template<SubTheoremProperty SubPropertyType>
bool Theorem<SubPropertyType>::evaluate() const
{
    return **m_value;
}

template<SubTheoremProperty SubPropertyType>
bool Theorem<SubPropertyType>::testEvaluate() const
{
    return SubTheorem<SubPropertyType>::evaluate();
}

template<SubTheoremProperty SubPropertyType>
void Theorem<SubPropertyType>::_evaluate() const
{
    try
    {
        (*m_value) = std::make_unique<bool>(this->m_son->evaluate());
    }
    catch (VariableException&)
    {

    }
}

template<SubTheoremProperty SubPropertyType>
void Theorem<SubPropertyType>::_canBeDemonstrated() const
{
    *m_canBeDemonstrated = std::make_unique<bool>(ASubTheorem::canBeDemonstrated());
}
}

#endif // THEOREMGENERIC_H
