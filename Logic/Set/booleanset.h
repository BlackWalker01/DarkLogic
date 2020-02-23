#ifndef BOOLEANSET_H
#define BOOLEANSET_H
#include "set.h"

namespace N_Logic {

class BooleanSet: public Set<BooleanSet>
{
public:
    BooleanSet() = default;
    using Type=bool;

    void init();

    ~BooleanSet() = default;
};
}

#endif // BOOLEANSET_H
