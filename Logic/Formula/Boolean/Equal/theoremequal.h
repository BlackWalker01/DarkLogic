#ifndef THEOREMEQUAL_H
#define THEOREMEQUAL_H
#include "Formula/Boolean/theorem.h"

namespace N_Logic
{
template<typename ValueType1, typename ValueType2>
class Theorem<Equal<ASubArithmeticTheorem<ValueType1>,ASubArithmeticTheorem<ValueType2>>>:
        public SubTheorem<Equal<ASubArithmeticTheorem<ValueType1>,ASubArithmeticTheorem<ValueType2>>>
{
public:
    Theorem(const ptr<ASubArithmeticTheorem<ValueType1>>& leftFormula,
            const ptr<ASubArithmeticTheorem<ValueType2>>& rightFormula):
        SubTheorem<Equal<ASubArithmeticTheorem<ValueType1>,ASubArithmeticTheorem<ValueType2>>>(leftFormula,rightFormula)
    {}

    ~Theorem() override = default;
};

template<typename ValueType1, typename ValueType2>
ptr<ASubTheorem> N_Logic::SubTheorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> > >::copyTheorem() const
{
    return std::make_shared<const Theorem<Equal<ASubArithmeticTheorem<ValueType1>, ASubArithmeticTheorem<ValueType2> >>>(
                                                                                            get<0>(*m_son),get<1>(*m_son));
}

}

#endif // THEOREMEQUAL_H
