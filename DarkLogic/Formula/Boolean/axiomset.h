/*===--- axiom.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines AxiomSet class in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_AXIOMSET_H
#define DARK_LOGIC_AXIOMSET_H
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
#endif // DARK_LOGIC_AXIOMSET_H
