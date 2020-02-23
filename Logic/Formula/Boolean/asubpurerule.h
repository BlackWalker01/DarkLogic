#ifndef ASUBPURERULE_H
#define ASUBPURERULE_H
#include "asubrule.h"

namespace N_Logic
{
class ASubPureRule: public ASubRule
{
public:
    ASubPureRule(const std::string& name_, const PropType type_);
    ASubPureRule(const std::string& name_, const DbVar& dbVar, const PropType type_);
    ASubPureRule(const std::string& name_, const DbVar& dbVar, const DbVar& dbVar2, const PropType type_);
    ASubPureRule(const std::string& name_, const std::vector<DbVar>& dbVars, const PropType type_);

    virtual const ptr<ASubRule>& operator[](const size_t& index) const =0;

    virtual ~ASubPureRule() override = default;
};
}
#endif // ASUBPURERULE_H
