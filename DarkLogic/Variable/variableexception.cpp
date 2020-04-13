/*===--- variableexception.cpp - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file implements VariableException class for DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#include "variableexception.h"

using namespace N_DarkLogic;

const std::string VariableException::s_endWhat = " not initialized";

VariableException::VariableException(const std::string& nameVar):
	m_nameVar(nameVar)
{

}

std::string N_DarkLogic::VariableException::whatStr() const
{
	return m_nameVar+s_endWhat;
}

