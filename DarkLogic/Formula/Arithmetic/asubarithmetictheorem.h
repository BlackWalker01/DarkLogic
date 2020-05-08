/*===--- asubarithmetictheorem.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines ASubArithmeticTheorem in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_ASUBARITHMETICTHEOREM_H
#define DARK_LOGIC_ASUBARITHMETICTHEOREM_H
#include "aarithmeticformula.h"
#include "Formula/isubtheoremformula.h"

namespace N_DarkLogic
{
template<ArithmeticType ValueType>
class ASubArithmeticTheorem: public AArithMeticFormula<ValueType>, public ISubTheoremFormula<ASubArithmeticTheorem<ValueType>>
{
public:
    ASubArithmeticTheorem() = default;
    ASubArithmeticTheorem(const ASubArithmeticTheorem& formula) = delete;

    const std::vector<std::vector<Arity> > &getAllPaths() const;
    virtual const std::vector<std::vector<Arity>>& computeAllPaths()=0;
    const std::vector<std::vector<Arity> >& getImplPaths() const; //method to get all possible paths for implications to terms of this subtheorem (once computed)
    virtual const std::vector<std::vector<Arity>>& computeImplPaths() = 0; //method to compute all possible paths to terms of this subtheorem

    virtual ~ASubArithmeticTheorem() = default;

protected:
    std::vector<std::vector<Arity>> m_allPaths;
    std::vector<std::vector<Arity>> m_implPaths;
};

template<ArithmeticType ValueType> struct ToRuleStruct<ASubArithmeticTheorem<ValueType>> { using Type = ASubArithmeticRule<ValueType>; };

template<ArithmeticType ValueType>
const std::vector<std::vector<Arity> > &ASubArithmeticTheorem<ValueType>::getAllPaths() const
{
    return m_allPaths;
}
template<ArithmeticType ValueType>
inline const std::vector<std::vector<Arity>>& ASubArithmeticTheorem<ValueType>::getImplPaths() const
{
    return m_implPaths;
}
}
#endif // DARK_LOGIC_ASUBARITHMETICTHEOREM_H
