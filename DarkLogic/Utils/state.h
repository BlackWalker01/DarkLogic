/*===--- state.h - header for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file will  define states of theorems for DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_STATE_H
#define DARK_LOGIC_STATE_H
#include "logic_global.h"
#include "Formula/valuetypeobject.h"
#include "Operator/ioperator.h"
#include <vector>
#include <tuple>

namespace N_DarkLogic
{

class LOGICSHARED_EXPORT State
{
public:

private:
	std::vector<Name> m_priorityOpe; //operators ordered by priority
	std::vector<Name> m_operators; //operators ordered by appearance in string
	std::vector<std::pair<bool, size_t>> m_terms; //list of terms (for variables (True, idxInVarTypes), for constants (false, idxInConstants) )
	std::vector<VALUE_TYPE> m_varTypes; //list of types of variables (k-th type is the type of k-th variable)
	std::vector<std::pair<VALUE_TYPE, std::vector<double>>> m_constants; //list of (type, value) of constants
};
}

#endif
