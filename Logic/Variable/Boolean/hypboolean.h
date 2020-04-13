#ifndef LOGIC_HYPBOOLEAN_H
#define LOGIC_HYPBOOLEAN_H
#include "boolean.h"

namespace N_Logic
{
class HypBoolean: public Boolean
{
public:
	HypBoolean(const std::string& name_);

	constexpr bool isHypVariable() const
	{
		return true;
	}
};
}
#endif // !LOGIC_HYPBOOLEAN_H

