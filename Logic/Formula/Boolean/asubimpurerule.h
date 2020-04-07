#ifndef ASUBIMPURERULE_H
#define ASUBIMPURERULE_H
#include "asubrule.h"

namespace N_Logic
{
class ASubImpureRule: public ASubRule
{
public:
    ASubImpureRule() = default;

    virtual const ptr<IISubRuleFormula>& operator[](const size_t& index) const =0;

    ~ASubImpureRule() override = default;
};
}
#endif // ASUBIMPURERULE_H
