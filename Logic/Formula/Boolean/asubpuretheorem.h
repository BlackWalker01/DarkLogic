#ifndef ASUBPURETHEOREM_H
#define ASUBPURETHEOREM_H
#include "asubtheorem.h"

namespace N_Logic
{
class ASubPureTheorem: public ASubTheorem
{
public:
    ASubPureTheorem(const std::string& name_, const PropType type_);
    ASubPureTheorem(const std::string& name_, const DbVar& dbVar, const PropType type_);
    ASubPureTheorem(const std::string& name_, const DbVar& dbVar, const DbVar& dbVar2, const PropType type_);
    ASubPureTheorem(const std::string& name_, const std::vector<DbVar>& dbVars, const PropType type_);


    virtual const ptr<ASubTheorem>& operator[](const size_t& index) const =0;
    TheoremFormulaType getFormulaType() const override final;

    virtual ~ASubPureTheorem() = default;
};
}

#endif // ASUBPURETHEOREM_H
