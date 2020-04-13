/*===--- aarithmeticformula.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines AArithMeticFormulas in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_AARITHMETICFORMULA_H
#define DARK_LOGIC_AARITHMETICFORMULA_H
#include "Formula/formula.h"

namespace N_DarkLogic
{

enum ArithType
{
    PLUS_FORMULA,
    MINUS_FORMULA,
    MULT_FORMULA,
    DIV_FORMULA,
    SET_FORMULA,
    NAT_INT_FORMULA,
    NAT_INT_SET_FORMULA,
    BOOL_SET_FORMULA
};

template<typename ValueType>
class AArithMeticFormula: public AbstractFormula<ValueType>
{
public:
    AArithMeticFormula() = default;

    virtual constexpr ArithType type() const = 0;

    //method to get sub properties
    virtual size_t arity() const =0;
    //virtual DbVar<BooleanSet>& getExtVars()=0;

    virtual bool isEqual(const ASubArithmeticRule<ValueType>& prop) const =0;
    virtual bool isEqual(const ASubArithmeticTheorem<ValueType>& prop) const =0;

    virtual ~AArithMeticFormula() = default;
};

template<typename ValueType>
inline bool operator==(const ASubArithmeticRule<ValueType>& rule, const ASubArithmeticTheorem<ValueType>& thm);
template<typename ValueType>
inline bool operator==(const ASubArithmeticTheorem<ValueType>& th, const ASubArithmeticTheorem<ValueType>& th2);
template<typename ValueType>
inline bool operator==(const ASubArithmeticRule<ValueType>& th, const ASubArithmeticRule<ValueType>& th2);

template<typename ValueType>
struct ToArithType
{
    static ArithType convert();
};
}
#endif // DARK_LOGIC_AARITHMETICFORMULA_H
