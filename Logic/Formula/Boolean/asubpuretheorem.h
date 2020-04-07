#ifndef ASUBPURETHEOREM_H
#define ASUBPURETHEOREM_H
#include "asubtheorem.h"

namespace N_Logic
{
class ASubPureTheorem: public ASubTheorem
{
public:
    ASubPureTheorem() = default;


    virtual const ptr<ASubTheorem>& operator[](const size_t& index) const =0;
    TheoremFormulaType getFormulaType() const override final;

    virtual ~ASubPureTheorem() = default;
};
}

#endif // ASUBPURETHEOREM_H
