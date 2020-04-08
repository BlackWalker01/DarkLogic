#ifndef NATURALINTEGERSET_H
#define NATURALINTEGERSET_H
#include "set.h"

namespace N_Logic {

class NaturalIntegerSet: public Set<NaturalIntegerSet>
{
public:
    NaturalIntegerSet() = default;
    using Type=UInt;

    ~NaturalIntegerSet() = default;
};
}
#endif // NATURALINTEGERSET_H
