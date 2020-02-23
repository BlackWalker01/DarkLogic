#ifndef THEOREMLET_H
#define THEOREMLET_H
#include "Formula/Boolean/theorem.h"

namespace N_Logic
{
template<typename SubTheoremType>
class Theorem<Let<SubTheoremType,ASubTheorem>>:
        public SubTheorem<Let<SubTheoremType,ASubTheorem>>
{
public:
    Theorem(const std::string& name_, const ptr<SubTheoremType>& leftFormula,
            const ptr<ASubTheorem>& rightFormula):
        SubTheorem<Let<SubTheoremType,ASubTheorem>>(name_,leftFormula,rightFormula)
    {}

    ~Theorem() override = default;
};

template<typename SubTheoremType>
ptr<ASubTheorem> SubTheorem<Let<SubTheoremType, ASubTheorem> >::copyTheorem() const
{
    return std::make_shared<const Theorem<Let<SubTheoremType, ASubTheorem>>>(m_name,get<0>(*m_son),get<1>(*m_son));
}

}

#endif // THEOREMLET_H
