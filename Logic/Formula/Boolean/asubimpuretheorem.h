#ifndef ASUBIMPURETHEOREM_H
#define ASUBIMPURETHEOREM_H
#include "asubtheorem.h"

namespace N_Logic
{

class ASubImpureTheorem: public ASubTheorem
{
public:
    ASubImpureTheorem() = default;

    virtual const ptr<IISubTheoremFormula>& operator[](const size_t& index) const =0;
    TheoremFormulaType getFormulaType() const override final;

    ~ASubImpureTheorem() override = default;
};
}
#endif // ASUBIMPURETHEOREM_H
