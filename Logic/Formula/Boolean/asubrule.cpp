#include "asubrule.h"
#include "Operator/alloperator.h"
#include "Variable/dbvar.h"
#include "Variable/allvariable.h"

using namespace N_Logic;


ASubRule::ASubRule(const std::string &name_, const IProposition::PropType type_):
    IProposition(name_,type_), ISubRuleFormula<ASubTheorem>()
{

}

ASubRule::ASubRule(const std::string &name_, const DbVar &dbVar, const IProposition::PropType type_):
    IProposition(name_,dbVar,type_), ISubRuleFormula<ASubTheorem>()
{

}

ASubRule::ASubRule(const std::string &name_, const DbVar &dbVar, const DbVar &dbVar2, const IProposition::PropType type_):
    IProposition(name_,dbVar,dbVar2,type_), ISubRuleFormula<ASubTheorem>()
{

}

ASubRule::ASubRule(const std::string &name_, const std::vector<DbVar> &dbVars, const IProposition::PropType type_):
    IProposition(name_,dbVars,type_), ISubRuleFormula<ASubTheorem>()
{

}

ptr<IISubTheoremFormula> ASubRule::applyAnnexe(const size_t &, const ptr<IISubTheoremFormula> &, std::vector<Arity> &) const
{
    throw std::runtime_error("SubRule cannot call applyAnnexe method");
}


ptr<IOperator> N_Logic::createRuleOperator(const Name name, const Arity &arity)
{
    switch (name)
    {
        case NONE:
        {
            throw std::runtime_error("Unknown operator");
        }
        case AND:
        {
            return std::make_shared<const And<ASubRule>>();
        }
        case EQUIVALENT:
        {
            return std::make_shared<const Equivalent<ASubRule>>();
        }
        case IMPLICATION:
        {
            return std::make_shared<const Implication<ASubRule>>();
        }
        case NOT:
        {
            return std::make_shared<const Not<ASubRule>>();
        }
        case OR:
        {
            return std::make_shared<const Or<ASubRule>>();
        }
        case HYP:
        {
            std::vector<ptr<ASubRule>> props;
            for(size_t k=0;k<arity;k++)
            {
                props.push_back(nullptr);
            }
            return std::make_shared<const Hyp<ASubRule>>(props);
        }
        case EQUAL:
        {
            return std::make_shared<const Equal<ASubArithmeticRule<size_t>,ASubArithmeticRule<size_t>>>();
        }
        case SETEQUAL:
        {
            return std::make_shared<const SetEqual<SubArithmeticRule<NaturalInteger>>>();
        }
        case LET:
        {
            return std::make_shared<const Let<SubArithmeticRule<NaturalInteger>,ASubRule>>();
        }
        case BELONGSTO:
        {
            return std::make_shared<const BelongsTo<ASubArithmeticRule<size_t>,ASubArithmeticRule<NaturalIntegerSet>>>();
        }
        case PLUS:
        {
            return std::make_shared<const Plus<ASubArithmeticRule<size_t>,ASubArithmeticRule<size_t>>>();
        }
    }

    return nullptr;
}

void N_Logic::addRuleOperator(const Name name, std::vector<OperatorOrdering> &opeList,
                                  const std::vector<OperatorOrdering> &hyps, const size_t& numPar, const size_t& argIndex)
{
    ptr<IOperator> ope=nullptr;
    if((ope=createRuleOperator(name)))
    {
        //if current operator comes just after a Hypothesis operator, then it is a subOperator of this one
        if(hyps.size() && hyps.back().nbPar>=numPar)
        {
            opeList.push_back(OperatorOrdering(ope,numPar,hyps.back().ope->arity()-1));
        }
        else
        {
            opeList.push_back(OperatorOrdering(ope,numPar,argIndex));
        }
    }
    else
    {
        throw std::runtime_error("Unknown operator");
    }
}
