#ifndef ASUBTHEOREM_H
#define ASUBTHEOREM_H
#include "Operator/ioperator.h"
#include "iproposition.h"
#include "Formula/isubtheoremformula.h"

namespace N_Logic
{
class IISubRuleFormula;

class ASubTheorem: public IProposition, public ISubTheoremFormula<ASubTheorem>
{
public:
    ASubTheorem(const std::string& name_, const PropType type_);
    ASubTheorem(const std::string& name_, const DbVar& dbVar, const PropType type_);
    ASubTheorem(const std::string& name_, const DbVar& dbVar, const DbVar& dbVar2, const PropType type_);
    ASubTheorem(const std::string& name_, const std::vector<DbVar>& dbVars, const PropType type_);
    ASubTheorem(const ASubTheorem &prop) = delete;

    virtual ptr<ASubTheorem> copyTheorem() const =0; //method used by a subRule to create a theorem from this subTheorem
    const std::vector<std::vector<Arity> > &getAllPaths() const; //method to get all possible paths to terms of this subtheorem (once computed)
    virtual const std::vector<std::vector<Arity>>& computeAllPaths()=0; //method to compute all possible paths to terms of this subtheorem
    virtual bool canBeDemonstrated() const;

    //apply rule on subtheorem of this theorem following path from indexes
    virtual ptr<IISubTheoremFormula> ruleApply(const IISubRuleFormula& rule, std::vector<size_t>& indexes,
                                               const size_t& actionKey) const =0;

    virtual ~ASubTheorem() = default;

    //static methods
    static void addAbstractTerm(const std::string &varName, std::vector<std::shared_ptr<VariableContainer>> &varList,
                                DbVarContainer &dbVar);
    static void deleteAfterExcept(std::vector<OperatorOrdering>& ope, std::vector<std::shared_ptr<VariableContainer>> &varList);


private:
    bool testCanBeDemonstrated(const std::vector<ptr<AbstractTerm>>& vars, std::unique_ptr<bool>& crtValue) const;
protected:
    std::vector<std::vector<Arity>> m_allPaths;
};

template<Proposition SubPropertyType>
class Hyp;

ptr<ASubTheorem> create(const std::string &name, const std::string &content);
ptr<IOperator> createTheoremOperator(const Name name, const Arity& arity=0);
void addTheoremOperator(const Name opeName, std::vector<OperatorOrdering> &opeList, const std::vector<OperatorOrdering> &hyps,
                     const size_t &numPar, const std::string& name, size_t& index);
}
#endif // ASUBTHEOREM_H
