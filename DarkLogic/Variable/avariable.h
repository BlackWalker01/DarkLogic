#ifndef DARK_LOGIC_AVARIABLE_H
#define DARK_LOGIC_AVARIABLE_H
#include "term.h"

namespace N_DarkLogic
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
	static std::vector<std::string> getAllVarName();
protected:
	const IDVar m_id;
private:	
	static IDVar getIDVar(const std::string& name_);
	static std::unordered_map<std::string, IDVar> s_nameToID;
};
}

#endif // !DARK_LOGIC_AVARIABLE_H
