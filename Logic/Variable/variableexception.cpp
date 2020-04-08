#include "variableexception.h"

using namespace N_Logic;

const std::string VariableException::s_endWhat = " not initialized";

VariableException::VariableException(const std::string& nameVar):
	m_nameVar(nameVar)
{

}

std::string N_Logic::VariableException::whatStr() const
{
	return m_nameVar+s_endWhat;
}

