#ifndef ASUBRULE_H
#define ASUBRULE_H
#include "Operator/ioperator.h"
#include "iproposition.h"
#include "Formula/isubruleformula.h"

namespace N_Logic
{

class ASubTheorem;
class IISubTheoremFormula;

class ASubRule: public IProposition, public ISubRuleFormula<ASubTheorem>
{
public:
    ASubRule(const std::string& name_, const PropType type_);
    ASubRule(const std::string& name_, const DbVar& dbVar, const PropType type_);
    ASubRule(const std::string& name_, const DbVar& dbVar, const DbVar& dbVar2, const PropType type_);
    ASubRule(const std::string& name_, const std::vector<DbVar>& dbVars, const PropType type_);

    ASubRule(const ASubRule&) = delete;

    virtual ptr<IISubTheoremFormula> applyAnnexe(const size_t& actionKey,
                                                        const ptr<IISubTheoremFormula> &theorem, std::vector<Arity>& indexes) const;
    virtual bool isSymetric() const;

    static void addAbstractTerm(const std::string& varName, std::vector<std::shared_ptr<VariableContainer> > &varList, DbVarContainer &dbVar);

    virtual ~ASubRule() = default;
};
ptr<IOperator> createRuleOperator(const Name name, const Arity& arity=0);
void addRuleOperator(const Name name, std::vector<OperatorOrdering> &opeList,
                     const std::vector<OperatorOrdering>& hyps, const size_t &numPar);
}

#endif // ASUBRULE_H
