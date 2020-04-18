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
        const ptr<ASubArithmeticTheorem<SetType>>& rightFormula);

    ~Theorem() = default;


    bool isEvaluated() const override final;
    bool canBeDemonstrated() const override final;
    bool evaluate() const override final;

private:
    void _evaluate() const;
    void _canBeDemonstrated() const;

    const std::unique_ptr<std::unique_ptr<bool>> m_value;
    const std::unique_ptr<std::unique_ptr<bool>> m_canBeDemonstrated;
};

template<typename SetType>
Theorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>, ASubArithmeticTheorem<SetType>>>::
Theorem(const ptr<ASubArithmeticTheorem<typename SetType::Type>>& leftFormula,
    const ptr<ASubArithmeticTheorem<SetType>>& rightFormula):
    SubTheorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>, ASubArithmeticTheorem<SetType>>>(leftFormula, rightFormula),
    m_value(std::make_unique<std::unique_ptr<bool>>()), 
    m_canBeDemonstrated(std::make_unique<std::unique_ptr<bool>>())
{
    _evaluate();
    _canBeDemonstrated();
}


template<typename SetType>
ptr<ASubTheorem> SubTheorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>,ASubArithmeticTheorem<SetType>> >::copyTheorem() const
{
    return std::make_shared<Theorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>,
            ASubArithmeticTheorem<SetType>>>>(get<0>(*m_son),get<1>(*m_son));
}

template<typename SetType>
inline bool Theorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>, ASubArithmeticTheorem<SetType>>>::isEvaluated() const
{
    return (*m_value)!=nullptr;
}

template<typename SetType>
inline bool Theorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>, ASubArithmeticTheorem<SetType>>>::evaluate() const
{
    return **m_value;
}

template<typename SetType>
inline bool Theorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>, ASubArithmeticTheorem<SetType>>>::canBeDemonstrated() const
{
    return **m_canBeDemonstrated;
}

template<typename SetType>
inline void Theorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>, ASubArithmeticTheorem<SetType>>>::_evaluate() const
{
    try
    {
        (*m_value) = std::make_unique<bool>(this->m_son->evaluate());
    }
    catch (VariableException&)
    {

    }
}

template<typename SetType>
inline void Theorem<BelongsTo<ASubArithmeticTheorem<typename SetType::Type>, ASubArithmeticTheorem<SetType>>>::_canBeDemonstrated() const
{
    (*m_canBeDemonstrated) = std::make_unique<bool>(ASubTheorem::canBeDemonstrated());
}

}

#endif // THEOREMBELONGSTO_H
