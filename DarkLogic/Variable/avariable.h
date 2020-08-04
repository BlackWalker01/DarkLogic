/*===--- avariable.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines AVariable class in DarkLogic Library which is abstract class for all variables
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

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
	static std::string getVarNameFromId(const IDVar& idVar);
protected:
	const IDVar m_id;
private:	
	static IDVar getIDVar(const std::string& name_);
	static std::unordered_map<std::string, IDVar> s_nameToID;
	static std::unordered_map<IDVar, std::string> s_idToName;
};
}

#endif // !DARK_LOGIC_AVARIABLE_H
