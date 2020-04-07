#ifndef THEOREMBELONGSTO_H
#define THEOREMBELONGSTO_H
#include "Formula/Boolean/theorem.h"

namespace N_Logic
{
template<typename SetType>
class Theorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType>>>:
        public SubTheorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType>>>
{
public:
    Theorem(const ptr<ASubArithmeticTheorem<typename SetType::Type>>& leftFormula,
            const ptr<ASubArithmeticTheorem<SetType>>& rightFormula):
        SubTheorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType>>>(leftFormula,rightFormula)
    {}

    ~Theorem() = default;
};

template<typename SetType>
ptr<ASubTheorem> SubTheorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType>> >::copyTheorem() const
{
    return std::make_shared<Theorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>,
            ASubArithmeticTheorem<SetType>>>>(get<0>(*m_son),get<1>(*m_son));
}

}

#endif // THEOREMBELONGSTO_H
