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
            const ptr<ASubTheorem>& rightFormula):
        SubTheorem<Let<SubTheoremType,ASubTheorem>>(leftFormula,rightFormula)
    {}

    ~Theorem() override = default;
};

template<typename SubTheoremType>
ptr<ASubTheorem> SubTheorem<Let<SubTheoremType, ASubTheorem> >::copyTheorem() const
{
    return std::make_shared<const Theorem<Let<SubTheoremType, ASubTheorem>>>(get<0>(*m_son),get<1>(*m_son));
}

}

#endif // THEOREMLET_H
