/*===--- asubrule.cpp - source for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file implements asubrule class for DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#include "asubrule.h"
#include "Operator/alloperator.h"
#include "Variable/dbvar.h"
#include "Variable/allvariable.h"
#include "Formula/Arithmetic/VariableFormula/variableruleformula.h"

using namespace N_DarkLogic;

std::string N_DarkLogic::ASubRule::name() const
{
    throw std::runtime_error("SubRule cannot call name method");
}

ptr<IISubTheoremFormula> ASubRule::applyAnnexe(const Action::Id &, const ptr<IISubTheoremFormula> &, DbVarProp&, std::vector<Arity> &) const
{
    throw std::runtime_error("SubRule cannot call applyAnnexe method");
}

ptr<IISubTheoremFormula> ASubRule::applyAnnexe(const Action::Id&, const ptr<IISubTheoremFormula>&, DbVarProp&, std::vector<Arity>&,
    const size_t&) const
{
    throw std::runtime_error("SubRule cannot call applyAnnexe method");
}

bool ASubRule::isSymetric() const
{
	return false;
}

void N_DarkLogic::ASubRule::clearAllIdentifications() const
{
    throw std::runtime_error("SubRule cannot call clearAllIdentifications method");
}

bool N_DarkLogic::ASubRule::testEvaluate() const
{
	return evaluate();
}
