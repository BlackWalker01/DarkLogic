#ifndef ASUBIMPURERULE_H
#define ASUBIMPURERULE_H
#include "asubrule.h"

namespace N_Logic
{
class ASubImpureRule: public ASubRule
{
public:
    ASubImpureRule(const std::string& name_, const PropType type_);
    ASubImpureRule(const std::string& name_, const DbVar& dbVar, const PropType type_);
    ASubImpureRule(const std::string& name_, const DbVar& dbVar, const DbVar& dbVar2, const PropType type_);
    ASubImpureRule(const std::string& name_, const std::vector<DbVar>& dbVars, const PropType type_);

    virtual const ptr<IISubRuleFormula>& operator[](const size_t& index) const =0;

    ~ASubImpureRule() override = default;
};
}
#endif // ASUBIMPURERULE_H
