#include "iproposition.h"
#include "asubrule.h"
#include "asubtheorem.h"

using namespace N_Logic;

IDVar N_Logic::IProposition::variableId() const
{
	throw std::logic_error("variableId() can only be called by SubRule<Boolean> or SubTheorem<Boolean>");
}
