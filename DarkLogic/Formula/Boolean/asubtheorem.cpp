#include "subtheorem.h"
#include "Utils/utils.h"
#include "Variable/allvariable.h"
#include "Formula/Arithmetic/VariableFormula/variabletheoremformula.h"

using namespace N_DarkLogic;

const std::vector<std::vector<Arity> > &ASubTheorem::getAllPaths() const
{
    return m_allPaths;
}

const std::vector<std::vector<Arity>>& N_DarkLogic::ASubTheorem::getImplPaths() const
{
    return m_implPaths;
}

bool N_DarkLogic::ASubTheorem::isEvaluated() const
{
    throw std::logic_error("SubTheorem cannot call isEvaluated method");
}

/**
 * Default implementation of canBeDemonstrated method
 * return true if for all values of its variables, the theorem is evaluate to true (or false) 
 */
bool N_DarkLogic::ASubTheorem::canBeDemonstrated() const
{
    std::unique_ptr<bool> eval = nullptr;
    return testCanBeDemonstrated(getExtVars()->getVars(), eval);    
}

std::vector<std::pair<Evaluater::ConfigEval, bool>> N_DarkLogic::ASubTheorem::getConfigEvals() const
{
    return std::vector<std::pair<Evaluater::ConfigEval, bool>>();
}

std::unordered_map<IDVar,IDVar> N_DarkLogic::ASubTheorem::getVarToEval() const
{
    return std::unordered_map<IDVar, IDVar>();
}

std::vector<Evaluater::ConfigEval> ASubTheorem::getCompatibleConfigs(const Evaluater::ConfigEval& commonConfig,
    const std::unordered_map<IDVar,IDVar>& internalVars) const
{
    return std::vector<Evaluater::ConfigEval>();
}

/**
 * return true if the theorem has one crtValue
 * crtValue is the current evaluation of the theorem
*/
bool N_DarkLogic::ASubTheorem::testCanBeDemonstrated(const std::vector<ptr<AVariable>>& vars,
    std::unique_ptr<bool>& crtValue) const
{
    //try to evaluate directly 
    try
    {
        if (crtValue)
        {
            bool newValue = getHiddenValue();
            return (*crtValue) == newValue;
        }
        else
        {
            crtValue = std::make_unique<bool>(getHiddenValue());
            return true;
        }    
    }
    //else simulate with all possible values for variables
    catch (const VariableException&)
    {
        bool ret = true;
        for (size_t k = 0; k < vars.size(); k++)
        {
            auto var = vars[k];
            auto remainVars = vars;
            remainVars.erase(remainVars.begin() + k);

            //if var is not Boolean continue
            if (var->valueType() != VALUE_TYPE::BOOL_TYPE)
            {
                continue;
            }

            auto boolVar = std::static_pointer_cast<const Boolean>(var);

            //try to evaluate theorem with var=true
            (*boolVar) = (true);
            ret = testCanBeDemonstrated(remainVars, crtValue);
            if (!ret)
            {
                boolVar->unset();
                return false;
            }


            //try to evaluate theorem with var= false
            (*boolVar) = (false);
            ret = testCanBeDemonstrated(remainVars, crtValue);
            if (!ret)
            {
                boolVar->unset();
                return false;
            }

            //unset var
            boolVar->unset();
        }
        return ret;
    }
}
