#ifndef ISUBTHEOREMFORMULA_H
#define ISUBTHEOREMFORMULA_H
#include "iisubtheoremformula.h"

namespace N_Logic {

template<typename ATheoremType_>
class ISubTheoremFormula: public IISubTheoremFormula
{
public:
    typedef ATheoremType_ ATheoremType;
    ISubTheoremFormula() = default;

    virtual ~ISubTheoremFormula() = default;
};
}
#endif // ISUBTHEOREMFORMULA_H
