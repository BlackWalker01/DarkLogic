#ifndef AXIOMSET_H
#define AXIOMSET_H
#include "axiom.h"

namespace N_LogicGame
{
template<typename SetType>
class AxiomSet: public Axiom<Equivalent<ASubRule>>
{
public:
    AxiomSet(const std::string& name_, SubRule<BelongsTo<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>>>& leftFormula,
             ASubRule& rightFormula);
};

template<typename SetType>
AxiomSet<SetType>::AxiomSet(const std::string& name_,
                            SubRule<BelongsTo<ASubArithmeticRule<typename SetType::Type>,ASubArithmeticRule<SetType>>>& leftFormula,
                            ASubRule& rightFormula):
    Axiom<Equivalent<ASubRule> >(name_,leftFormula,rightFormula)
{

}
}
#endif // AXIOMSET_H
