/*===--- asubrule.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines ASubRule which is base class for all SubRule in DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_ASUBRULE_H
#define DARK_LOGIC_ASUBRULE_H
#include "Operator/ioperator.h"
#include "iproposition.h"
#include "Formula/isubruleformula.h"

namespace N_DarkLogic
{

class ASubTheorem;
class IISubTheoremFormula;

class ASubRule: public IProposition, public ISubRuleFormula<ASubTheorem>
{
public:
    ASubRule() = default;

    ASubRule(const ASubRule&) = delete;

    virtual std::string name() const;
    virtual ptr<IISubTheoremFormula> applyAnnexe(const Action::Id& actionKey,
                                                const ptr<IISubTheoremFormula> &theorem, DbVarProp& dbVarProp, 
                                                std::vector<Arity>& indexes) const;
    virtual ptr<IISubTheoremFormula> applyAnnexe(const Action::Id& actionKey, const ptr<IISubTheoremFormula>& theorem, 
        DbVarProp& dbVarProp, std::vector<Arity>& indexes, const size_t& logicIdx) const;
    virtual bool isSymetric() const;
    virtual void clearAllIdentifications() const;
    bool testEvaluate() const;

    static void addAbstractTerm(const std::string& varName, std::vector<std::shared_ptr<VariableContainer> > &varList, DbVarContainer &dbVar);

    virtual ~ASubRule() = default;
};

template<> struct ToTheoremStruct<ASubRule> { using Type = ASubTheorem; };

ptr<IOperator> createRuleOperator(const Name name, const Arity& arity=0);
void addRuleOperator(const Name name, std::vector<OperatorOrdering> &opeList, 
                     const std::vector<OperatorOrdering>& hyps, const std::vector<size_t>& hypStack, const size_t &numPar);
}

#endif // DARK_LOGIC_ASUBRULE_H
