#ifndef ASUBIMPURETHEOREM_H
#define ASUBIMPURETHEOREM_H
#include "asubtheorem.h"

namespace N_Logic
{

class ASubImpureTheorem: public ASubTheorem
{
public:
    ASubImpureTheorem(const std::string& name_, const PropType type_);
    ASubImpureTheorem(const std::string& name_, const DbVar& dbVar, const PropType type_);
    ASubImpureTheorem(const std::string& name_, const DbVar& dbVar, const DbVar& dbVar2, const PropType type_);
    ASubImpureTheorem(const std::string& name_, const std::vector<DbVar>& dbVars, const PropType type_);

    virtual const ptr<IISubTheoremFormula>& operator[](const size_t& index) const =0;
    TheoremFormulaType getFormulaType() const override final;

    ~ASubImpureTheorem() override = default;
};
}
#endif // ASUBIMPURETHEOREM_H
