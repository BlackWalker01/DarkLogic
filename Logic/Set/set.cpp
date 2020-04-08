#include "set.h"
#include "Formula/Boolean/dbrule.h"
#include "Set/allset.h"

using namespace N_Logic;

//Set initialization
template<> const std::string Set<BooleanSet>::s_name = "BB";
template<> const std::string Set<NaturalIntegerSet>::s_name = "NN";

template<>
std::string Set<BooleanSet>::name()
{
    return s_name;
}

template<>
std::string Set<NaturalIntegerSet>::name()
{
    return s_name;
}
