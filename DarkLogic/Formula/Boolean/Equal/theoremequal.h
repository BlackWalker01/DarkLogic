/*===--- theoremequal.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines Theorem<Equal> in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_THEOREMEQUAL_H
#define DARK_LOGIC_THEOREMEQUAL_H
#include "Formula/Boolean/theorem.h"

namespace N_DarkLogic
{
template<typename ValueType1, typename ValueType2>
class Theorem<Equal<ASubArithmeticTheorem<ValueType1>,ASubArithmeticTheorem<ValueType2>>>:
        public SubTheorem<Equal<ASubArithmeticTheorem<ValueType1>,ASubArithmeticTheorem<ValueType2>>>
{
public:
    Theorem(const ptr<ASubArithmeticTheorem<ValueType1>>& leftFormula,
        const ptr<ASubArithmeticTheorem<ValueType2>>& rightFormula);

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

template<typename ValueType1, typename ValueType2>
Theorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::
Theorem(const ptr<ASubArithmeticTheorem<ValueType1>>& leftFormula,
    const ptr<ASubArithmeticTheorem<ValueType2>>& rightFormula) :
    SubTheorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2>>>(leftFormula, rightFormula),
    m_value(std::make_unique<std::unique_ptr<bool>>()),
    m_canBeDemonstrated(std::make_unique<std::unique_ptr<bool>>())
{
    _evaluate();
    _canBeDemonstrated();
}

template<typename ValueType1, typename ValueType2>
ptr<ASubTheorem> SubTheorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::copyTheorem() const
{
    return std::make_shared<const Theorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> >>>(
                                                                                            get<0>(*m_son),get<1>(*m_son));
}

template<typename ValueType1, typename ValueType2>
bool Theorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::isEvaluated() const
{
    return (*m_value)!=nullptr;
}

template<typename ValueType1, typename ValueType2>
bool Theorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::canBeDemonstrated() const
{
    return **m_canBeDemonstrated;
}

template<typename ValueType1, typename ValueType2>
bool Theorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::evaluate() const
{
    return **m_value;
}

template<typename ValueType1, typename ValueType2>
bool Theorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::testEvaluate() const
{
    return SubTheorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::evaluate();
}

template<typename ValueType1, typename ValueType2>
void Theorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::_evaluate() const
{
    try
    {
        (*m_value) = std::make_unique<bool>(this->m_son->evaluate());
    }
    catch (VariableException&)
    {

    }
}

template<typename ValueType1, typename ValueType2>
void Theorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::_canBeDemonstrated() const
{
    *m_canBeDemonstrated = std::make_unique<bool>(ASubTheorem::canBeDemonstrated());
}

}

#endif // DARK_LOGIC_THEOREMEQUAL_H
