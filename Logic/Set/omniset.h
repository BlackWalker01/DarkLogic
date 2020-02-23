#ifndef OMNISET_H
#define OMNISET_H
#include "set.h"

namespace N_Logic {
template<typename SetType_>
class OmniSet: Set<OmniSet<SetType_>>
{
public:
    using SetType=SetType_;
    using Type=SetType;

    OmniSet();
};

template<typename SetType_>
OmniSet<SetType_>::OmniSet()
{

}
}

#endif // OMNISET_H
