/*===--- variableexception.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines exception thrown by variables in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_VARIABLEEXCEPTION_H
#define DARK_LOGIC_VARIABLEEXCEPTION_H
#include "Utils/exception.h"
#include <string>

namespace N_DarkLogic
{
class VariableException : public Exception<1>
{
public:
	VariableException(const std::string& nameVar);

	std::string whatStr() const;
private:
	const std::string m_nameVar;
	static const std::string s_endWhat;
};
}


#endif // DARK_LOGIC_VARIABLEEXCEPTION_H
