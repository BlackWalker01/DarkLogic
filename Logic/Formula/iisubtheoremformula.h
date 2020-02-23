#ifndef IISUBTHEOREMFORMULA_H
#define IISUBTHEOREMFORMULA_H
#include "Utils/utils.h"

namespace N_Logic
{
class IISubTheoremFormula
{
public:
    enum TheoremFormulaType
    {
        TERMINAL,
        SUBPURETH,
        SUBIMPURETH,
        NONTERMARITH
    };

    IISubTheoremFormula() = default;

    virtual TheoremFormulaType getFormulaType() const;

    virtual ~IISubTheoremFormula() = default;
};
}
#endif // IISUBTHEOREMFORMULA_H
