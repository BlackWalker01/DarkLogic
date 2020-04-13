#ifndef DARK_LOGIC_HYPBOOLEAN_H
#define DARK_LOGIC_HYPBOOLEAN_H
#include "boolean.h"

namespace N_DarkLogic
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
#endif // !DARK_LOGIC_HYPBOOLEAN_H

