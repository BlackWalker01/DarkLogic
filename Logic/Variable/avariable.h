#ifndef LOGIC_AVARIABLE_H
#define LOGIC_AVARIABLE_H
#include "term.h"

namespace N_Logic
{
class AVariable: public AbstractTerm
{
public:
	AVariable(const std::string& name_);

	IDVar id() const;
	virtual constexpr bool isHypVariable() const
	{
		return false;
	}
protected:
	const IDVar m_id;
private:
	static IDVar getIDVar(const std::string& name_);
	static std::unordered_map<std::string, IDVar> s_nameToID;
};
}

#endif // !LOGIC_AVARIABLE_H
