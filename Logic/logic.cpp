#include "logic.h"
#include "logic.h"
#include "logic.h"
#include "logic.h"
#include "logic.h"
#include "Formula/Boolean/theorem.h"
#include "Formula/Boolean/rule.h"
#include "Formula/Boolean/axiom.h"
#include "Operator/operator.h"
#include "Logger/log.h"
#include "Formula/Arithmetic/subarithmeticrule.h"

#include <chrono>
#include <ctime>

using namespace N_Logic;

const std::unordered_map<std::string,Name> IOperator::s_nameHash={
    {"&&",AND},
    {"<=>",EQUIVALENT},
    {"=>",IMPLICATION},
    {"!",NOT},
    {"||",OR},
    {"{",HYP},
    {"==",EQUAL},
    {"€",BELONGSTO},
    {"=",SETEQUAL},
    {"in",LET}, //let ... in ...
    {"+",PLUS}};

const std::unordered_map<std::string,std::string> IOperator::s_mightBeOpeName=initMightBeOpeName();

const std::unordered_map<std::string,std::string> IOperator::s_stringsToSkip=
{{" "," "},{"let","let"}};

const std::unordered_map<std::string,std::string> IOperator::s_stringsMightSkip=initStringMightSkip();


const std::unordered_map<std::string,ConstantEnum> AbstractTerm::s_constantHash={
    {"false",FALSE_TYPE},
    {"False",FALSE_TYPE},
    {"FALSE",FALSE_TYPE},
    {"true",TRUE_TYPE},
    {"True",TRUE_TYPE},
    {"TRUE",TRUE_TYPE},
    {"BB",ConstantEnum::BOOLEAN_SET_TYPE},
    {"NN",NATURALINTEGERSET_TYPE}
};

const std::string AndToStr::s_symbol="&&";
const std::string EquivalentToStr::s_symbol="<=>";
const std::string HypToStr::s_symbol="{";
const std::string ImplToStr::s_symbol="=>";
const std::string NotToStr::s_symbol="!";
const std::string OrToStr::s_symbol="||";
const std::string EqualToStr::s_symbol="==";
const std::string SetEqualToStr::s_symbol="=";
const std::string PlusToStr::s_symbol="+";
const std::string LetStr::s_symbol="let ";
const std::string LetStr::s_symbol2=" in ";
const std::string BelongsToStr::s_symbol="€";

std::vector<std::unique_ptr<Logic>> Logic::s_instances = {};

Logic::Logic(): m_theorem(nullptr), m_nbAppliedRule(0), m_isLastRuleSymetric(true)
{
    //set up rules of the logic
    //AXIOME Rule
    auto ax = std::dynamic_pointer_cast<const Rule<Equivalent<ASubRule>>>(createRule("ax", "true<=>({p,HYP}p)"));
    insert(ax);

    //AND rules
    auto andI = std::dynamic_pointer_cast<const Rule<Equivalent<ASubRule>>>(createRule("&&I", "(({HYP}p)&&({HYP}q))<=>({HYP}p&&q)"));
    insert(andI);
    auto andE = std::dynamic_pointer_cast<const Rule<Equivalent<ASubRule>>>(createRule("&&E", "({HYP}p&&q)<=>(({HYP}p)&&({HYP}q))"));
    insert(andE);

    auto andEl = std::dynamic_pointer_cast<const Rule<Implication<ASubRule>>>(createRule("&&Eg", "({HYP}p&&q)=>({HYP}p)")); //complicated to use!
    insert(andEl);
    auto andEr = std::dynamic_pointer_cast<const Rule<Implication<ASubRule>>>(createRule("&&Ed", "({HYP}p&&q)=>({HYP}q)")); //complicated to use!
    insert(andEr);

    //OR Rules
    auto orI = std::dynamic_pointer_cast<const Rule<Equivalent<ASubRule>>>(createRule("||I", "(({HYP}p)||({HYP}q))<=>({HYP}p||q)"));
    insert(orI);
    auto orIl = std::dynamic_pointer_cast<const Rule<Implication<ASubRule>>>(createRule("||Ig", "({HYP}p)=>({HYP}p||q)"));
    insert(orIl);
    auto orIr = std::dynamic_pointer_cast<const Rule<Implication<ASubRule>>>(createRule("||Id", "({HYP}q)=>({HYP}p||q)"));
    insert(orIr);
    auto orE = std::dynamic_pointer_cast<const Rule<Implication<ASubRule>>>(createRule("||E", 
        "(({HYP}p||q)&&({p,HYP}r)&&({q,HYP}r))=>(({HYP}r)")); //complicated to use!
    insert(orE);

    //IMPLICATION Rules
    auto implI = std::dynamic_pointer_cast<const Rule<Equivalent<ASubRule>>>(createRule("=>I", "({p,HYP}q)<=>({HYP}p=>q)"));
    insert(implI);
    auto implE = std::dynamic_pointer_cast<const Rule<Implication<ASubRule>>>(createRule("=>E", 
        "(({HYP}p)&&({HYP}p=>q))=>({HYP}q)")); //complicated to use!
    insert(implE);

    //EQUIVALENT Rules
    auto eqI = std::dynamic_pointer_cast<const Rule<Equivalent<ASubRule>>>(createRule("<=>I", 
        "({HYP}p=>q) && ({HYP}q=>p) <=>({HYP}p<=>q)"));
    insert(eqI);


    //FALSE Rules
    auto fi = std::dynamic_pointer_cast<const Rule<Equivalent<ASubRule>>>(createRule("FI", "({HYP}(p&&!p))<=>({HYP}false)"));
    insert(fi);
    auto fe = std::dynamic_pointer_cast<const Rule<Equivalent<ASubRule>>>(createRule("FE", "({p,HYP}false)<=>({HYP}!p)"));
    insert(fe);
    auto fiNot = std::dynamic_pointer_cast<const Rule<Equivalent<ASubRule>>>(createRule("FI!", "({!p,HYP}p)<=>({!p,HYP}false)"));
    insert(fiNot);

    //HYPOTHESIS Rules
    auto hypCom1 = std::dynamic_pointer_cast<const Rule<Equivalent<ASubRule>>>(createRule("HypCom1", "({p,HYP,q}r)<=>({p,q,HYP}r)"));
    insert(hypCom1);
    auto hypCom2 = std::dynamic_pointer_cast<const Rule<Equivalent<ASubRule>>>(createRule("HypCom2", "({p,q,HYP}r)<=>({q,p,HYP}r)"));
    insert(hypCom2);

    //WEAKENING Rule
    auto weak = std::dynamic_pointer_cast<const Rule<Implication<ASubRule>>>(createRule("weakening", "({HYP}p)=>({q,HYP}p)"));
    insert(weak);

    //ABSURD REASONNING Rules (ARR)
    auto arr = std::dynamic_pointer_cast<const Rule<Equivalent<ASubRule>>>(createRule("arr", "({!p,HYP}false)<=>({HYP}p)"));
    insert(arr);
    auto doubleNot = std::dynamic_pointer_cast<const Rule<Equivalent<ASubRule>>>(createRule("!!", "({HYP}!!p)<=>({HYP}p)"));
    insert(doubleNot);
}

void Logic::init(const size_t& nbInstances)
{
    for (size_t k = 0; k < nbInstances; k++)
    {
        s_instances.push_back(std::make_unique<Logic>());
    }
    Log::Debug("Logic Instantiated");
}

bool N_Logic::Logic::isOver(const size_t& instanceIdx)
{
    return s_instances[instanceIdx]->_isOver();
}

bool Logic::isDemonstrated(const size_t& instanceIdx)
{
    return s_instances[instanceIdx]->_isDemonstrated();
}

bool N_Logic::Logic::isAlreadyPlayed(const size_t& instanceIdx)
{
    return s_instances[instanceIdx]->_isAlreadyPlayed();
}

bool N_Logic::Logic::canBeDemonstrated(const size_t& instanceIdx)
{
    return s_instances[instanceIdx]->_canBeDemonstrated();
}

bool Logic::evaluate(const size_t& instanceIdx)
{
    return s_instances[instanceIdx]->_evaluate();
}

bool N_Logic::Logic::isEvaluated(const size_t& instanceIdx)
{
    return s_instances[instanceIdx]->_isEvaluated();
}

bool N_Logic::Logic::appliedRuleSymetric(const size_t& instanceIdx)
{
    return s_instances[instanceIdx]->_appliedRuleSymetric();
}

bool Logic::makeTheorem(const std::string& name, const std::string& cont)
{
    for (size_t instanceIdx = 0; instanceIdx < s_instances.size(); instanceIdx++)
    {
        if (!s_instances[instanceIdx]->_makeTheorem(name, cont))
        {
            return false;
        }
    }
    return true;
}


void Logic::printTheorem(const size_t& instanceIdx)
{
    s_instances[instanceIdx]->_printTheorem();
}

std::vector<size_t> Logic::getActions(const size_t& instanceIdx)
{
    return s_instances[instanceIdx]->_getActions();
}

std::vector<Action> Logic::getHumanActions(const size_t& instanceIdx)
{
    return s_instances[instanceIdx]->_getHumanActions();
}

void Logic::apply(const size_t& instanceIdx, const size_t &actionKey)
{
    s_instances[instanceIdx]->_apply(actionKey);
}

void Logic::unapply(const size_t& instanceIdx)
{
    s_instances[instanceIdx]->_unapply();
}

bool Logic::hasAlreadyPlayed(const size_t& instanceIdx)
{
    return s_instances[instanceIdx]->_hasAlreadyPlayed();
}

template<typename OpeType>
void Logic::insert(const ptr<Rule<OpeType> > &rule)
{
    m_rules.insert(rule);
}

bool N_Logic::Logic::_isOver()
{
    return _isDemonstrated() || !_canBeDemonstrated() || _isAlreadyPlayed();
}

bool N_Logic::Logic::_isDemonstrated()
{
    try
    {
        if (_evaluate())
        {
            return true;
        }
        else
        {
            return m_isLastRuleSymetric;
        }       
    }
    catch (const VariableException&)
    {
        return false;
    }
}

bool N_Logic::Logic::_isAlreadyPlayed()
{
    for (const auto& pairTheorem : m_antecedents)
    {
        auto theorem = pairTheorem.first;
        if ((*theorem) == (*m_theorem))
        {
            return true;
        }
    }
    return false;
}

bool N_Logic::Logic::_canBeDemonstrated()
{
    try
    {
        if (!_evaluate())
        {
            return m_isLastRuleSymetric;
        }
        return true;
    }
    catch (const VariableException&)
    {
        return m_theorem->canBeDemonstrated();
    }    
}

bool N_Logic::Logic::_evaluate()
{
    if (m_theorem)
    {
        return m_theorem->evaluate();
    }
    else
    {
        throw std::runtime_error("Cannot evaluate an invalid theorem");
    }
}

bool N_Logic::Logic::_isEvaluated()
{
    if (m_theorem)
    {
        try
        {
            m_theorem->evaluate();
            return true;
        }
        catch (const VariableException&)
        {
            return false;
        }
    }
    return false;
}

bool N_Logic::Logic::_appliedRuleSymetric()
{
    return m_isLastRuleSymetric;
}

bool N_Logic::Logic::_makeTheorem(const std::string& name, const std::string& cont)
{
    try
    {
        m_theorem = createTheorem(name, cont);

        //Compute actions
        _getActions();

        return true;
    }
    catch (std::runtime_error & e)
    {
        Log::Error(e.what());
    }
    return false;
}

void N_Logic::Logic::_printTheorem()
{
    m_theorem->print();
}

std::vector<size_t> N_Logic::Logic::_getActions()
{
    return m_rules.getActions(m_theorem, m_nbAppliedRule);
}

std::vector<Action> N_Logic::Logic::_getHumanActions()
{
    _getActions();
    return m_rules.getHumanActions();
}

void N_Logic::Logic::_apply(const size_t& actionKey)
{
    auto antecedent = m_theorem;
    m_antecedents.push_back({ antecedent, m_isLastRuleSymetric });
    auto couple= m_rules.apply(actionKey, m_theorem);
    m_theorem = couple.first;
    m_isLastRuleSymetric = m_isLastRuleSymetric && couple.second;
    m_nbAppliedRule++;
}

void N_Logic::Logic::_unapply()
{    
    auto antecedent = m_antecedents.back().first;
    m_isLastRuleSymetric = m_antecedents.back().second;
    m_rules.unapply(antecedent, m_nbAppliedRule);
    m_theorem = antecedent;
    m_antecedents.pop_back();
    m_nbAppliedRule--;
}

bool N_Logic::Logic::_hasAlreadyPlayed()
{
    return m_antecedents.size();
}
