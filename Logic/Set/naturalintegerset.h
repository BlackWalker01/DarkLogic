#ifndef NATURALINTEGERSET_H
#define NATURALINTEGERSET_H
#include "set.h"

namespace N_Logic {

class NaturalIntegerSet: public Set<NaturalIntegerSet>
{
public:
    NaturalIntegerSet();
    using Type=UInt;

    ~NaturalIntegerSet();
};
}
#endif // NATURALINTEGERSET_H
