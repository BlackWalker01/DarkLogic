#ifndef ASUBPURERULE_H
#define ASUBPURERULE_H
#include "asubrule.h"

namespace N_Logic
{
class ASubPureRule: public ASubRule
{
public:
    ASubPureRule() = default;

    virtual const ptr<ASubRule>& operator[](const size_t& index) const =0;

    virtual ~ASubPureRule() override = default;
};
}
#endif // ASUBPURERULE_H
