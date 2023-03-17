#include "logic.h"
/*===--- logic.cpp - include for Logic library --------------*- C++ -*-===*
*
*   Part of the Logic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file implements Logic class for Logic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#include "logic.h"
#include "Formula/Boolean/theorem.h"
#include "Formula/Boolean/rule.h"
#include "Formula/Boolean/axiom.h"
#include "Operator/operator.h"
#include "Logger/log.h"
#include "Formula/Arithmetic/subarithmeticrule.h"
#include "Formula/parser.h"

#include <chrono>
#include <ctime>

using namespace N_DarkLogic;

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

std::unique_ptr<Logic > Logic::s_masterInstance = nullptr;
std::vector<std::unique_ptr<Logic>> Logic::s_instances = {};

Logic::Logic(): m_theorem(nullptr), m_isLastRuleSymetric(true)
{
    //set up rules of the Logic
    //AXIOME Rule
    auto ax = std::static_pointer_cast<const Rule<Equivalent<ASubRule>>>(Parser::createFormula<ASubRule>("ax", "true<=>({p,HYP}p)"));
    insert(ax);

    //AND rules
    auto andI = std::static_pointer_cast<const Rule<Equivalent<ASubRule>>>(Parser::createFormula<ASubRule>("&&I", "(({HYP}p)&&({HYP}q))<=>({HYP}p&&q)"));
    insert(andI);
    auto andE = std::static_pointer_cast<const Rule<Equivalent<ASubRule>>>(Parser::createFormula<ASubRule>("&&E", "({HYP}p&&q)<=>(({HYP}p)&&({HYP}q))"));
    insert(andE);
    auto andEl = std::static_pointer_cast<const Rule<Implication<ASubRule>>>(Parser::createFormula<ASubRule>("&&Eg", "({HYP}p&&q)=>({HYP}p)")); //complicated to use!
    insert(andEl);
    auto andEr = std::static_pointer_cast<const Rule<Implication<ASubRule>>>(Parser::createFormula<ASubRule>("&&Ed", "({HYP}p&&q)=>({HYP}q)")); //complicated to use!
    insert(andEr);
    auto andCom = std::static_pointer_cast<const Rule<Equivalent<ASubRule>>>(Parser::createFormula<ASubRule>("&&Com", "({HYP}p&&q)<=>({HYP}q&&p)"));
    insert(andCom);


    //OR Rules
    auto orI = std::static_pointer_cast<const Rule<Equivalent<ASubRule>>>(Parser::createFormula<ASubRule>("||I", "(({HYP}p)||({HYP}q))<=>({HYP}p||q)"));
    insert(orI);
    auto orIl = std::static_pointer_cast<const Rule<Implication<ASubRule>>>(Parser::createFormula<ASubRule>("||Ig", "({HYP}p)=>({HYP}p||q)"));
    insert(orIl);
    auto orIr = std::static_pointer_cast<const Rule<Implication<ASubRule>>>(Parser::createFormula<ASubRule>("||Id", "({HYP}q)=>({HYP}p||q)"));
    insert(orIr);
    auto orE = std::static_pointer_cast<const Rule<Implication<ASubRule>>>(Parser::createFormula<ASubRule>("||E",
        "(({HYP}p||q)&&({p,HYP}r)&&({q,HYP}r))=>(({HYP}r)")); //complicated to use!
    insert(orE);
    auto orCom = std::static_pointer_cast<const Rule<Equivalent<ASubRule>>>(Parser::createFormula<ASubRule>("||Com", "({HYP}p||q)<=>({HYP}q||p)"));
    insert(orCom);


    //IMPLICATION Rules
    auto implI = std::static_pointer_cast<const Rule<Equivalent<ASubRule>>>(Parser::createFormula<ASubRule>("=>I", "({p,HYP}q)<=>({HYP}p=>q)"));
    insert(implI);
    auto implE = std::static_pointer_cast<const Rule<Implication<ASubRule>>>(Parser::createFormula<ASubRule>("=>E",
        "(({HYP}p)&&({HYP}p=>q))=>({HYP}q)"));
    insert(implE);

    //EQUIVALENT Rules
    auto eqI = std::static_pointer_cast<const Rule<Equivalent<ASubRule>>>(Parser::createFormula<ASubRule>("<=>I",
        "({HYP}p=>q) && ({HYP}q=>p) <=>({HYP}p<=>q)"));
    insert(eqI);
    auto eqCom = std::static_pointer_cast<const Rule<Equivalent<ASubRule>>>(Parser::createFormula<ASubRule>("<=>Com", "({HYP}p <=> {HYP}q)<=>({HYP}q <=> {HYP}p)"));
    insert(eqCom);


    //FALSE Rules
    auto fi = std::static_pointer_cast<const Rule<Equivalent<ASubRule>>>(Parser::createFormula<ASubRule>("FI", "({HYP}(p&&!p))<=>({HYP}false)"));
    insert(fi);
    auto fe = std::static_pointer_cast<const Rule<Equivalent<ASubRule>>>(Parser::createFormula<ASubRule>("FE", "({p,HYP}false)<=>({HYP}!p)"));
    insert(fe);
    auto fiNot = std::static_pointer_cast<const Rule<Equivalent<ASubRule>>>(Parser::createFormula<ASubRule>("FI!", "({!p,HYP}p)<=>({!p,HYP}false)"));
    insert(fiNot);

    //HYPOTHESIS Rules
    auto hypCom1 = std::static_pointer_cast<const Rule<Equivalent<ASubRule>>>(Parser::createFormula<ASubRule>("HypCom1", "({p,HYP,q}r)<=>({p,q,HYP}r)"));
    insert(hypCom1);
    auto hypCom2 = std::static_pointer_cast<const Rule<Equivalent<ASubRule>>>(Parser::createFormula<ASubRule>("HypCom2", "({p,q,HYP}r)<=>({q,p,HYP}r)"));
    insert(hypCom2);

    //WEAKENING Rule
    auto weak = std::static_pointer_cast<const Rule<Implication<ASubRule>>>(Parser::createFormula<ASubRule>("weakening", "({HYP}p)=>({q,HYP}p)"));
    insert(weak);

    //ABSURD REASONNING Rules (ARR)
    auto arr = std::static_pointer_cast<const Rule<Equivalent<ASubRule>>>(Parser::createFormula<ASubRule>("arr", "({!p,HYP}false)<=>({HYP}p)"));
    insert(arr);
    auto doubleNot = std::static_pointer_cast<const Rule<Equivalent<ASubRule>>>(Parser::createFormula<ASubRule>("!!", "({HYP}!!p)<=>({HYP}p)"));
    insert(doubleNot);
}

void Logic::init(const size_t& nbInstances)
{
    s_instances.clear();
    Log::Info("Logic creation...");
    s_masterInstance = std::make_unique<Logic>();
    for (size_t k = 0; k < nbInstances; k++)
    {
        s_instances.push_back(std::make_unique<Logic>());
    }
    Log::Info("Logic Instantiated");
}

bool N_DarkLogic::Logic::isOver()
{
    return s_masterInstance->_isOver();
}

bool N_DarkLogic::Logic::isOver(const size_t& instanceIdx)
{
    return s_instances[instanceIdx]->_isOver();
}

void N_DarkLogic::Logic::clearAll()
{
    s_masterInstance->_clear();
    for (auto& instance : s_instances)
    {
        instance->_clear();
    }
}

void N_DarkLogic::Logic::clear()
{
    s_masterInstance->_clear();
}

void N_DarkLogic::Logic::clear(const size_t& instanceIdx)
{
    s_instances[instanceIdx]->_clear();
}

bool N_DarkLogic::Logic::isDemonstrated()
{
    return s_masterInstance->_isDemonstrated();
}

bool Logic::isDemonstrated(const size_t& instanceIdx)
{
    return s_instances[instanceIdx]->_isDemonstrated();
}

bool N_DarkLogic::Logic::isAlreadyPlayed()
{
    return s_masterInstance->_isAlreadyPlayed();
}

bool N_DarkLogic::Logic::isAlreadyPlayed(const size_t& instanceIdx)
{
    return s_instances[instanceIdx]->_isAlreadyPlayed();
}

bool N_DarkLogic::Logic::canBeDemonstrated()
{
    return s_masterInstance->_canBeDemonstrated();
}

bool N_DarkLogic::Logic::canBeDemonstrated(const size_t& instanceIdx)
{
    return s_instances[instanceIdx]->_canBeDemonstrated();
}

bool N_DarkLogic::Logic::evaluate()
{
    return s_masterInstance->_evaluate();
}

bool Logic::evaluate(const size_t& instanceIdx)
{
    return s_instances[instanceIdx]->_evaluate();
}

bool N_DarkLogic::Logic::isEvaluated()
{
    return s_masterInstance->_isEvaluated();
}

bool N_DarkLogic::Logic::isEvaluated(const size_t& instanceIdx)
{
    return s_instances[instanceIdx]->_isEvaluated();
}

const State& N_DarkLogic::Logic::getState()
{
    return s_masterInstance->_getState();
}

const State& N_DarkLogic::Logic::getState(const size_t& instanceIdx)
{
    return s_instances[instanceIdx]->_getState();
}

std::vector<State> N_DarkLogic::Logic::getRuleStates()
{
    return s_masterInstance->_getRuleStates();
}

bool N_DarkLogic::Logic::appliedRuleSymetric()
{
    return s_masterInstance->_appliedRuleSymetric();
}

bool N_DarkLogic::Logic::appliedRuleSymetric(const size_t& instanceIdx)
{
    return s_instances[instanceIdx]->_appliedRuleSymetric();
}

bool Logic::makeTheorem(const std::string& name, const std::string& cont)
{
    if (!s_masterInstance->_makeTheorem(name, cont))
    {
        return false;
    }
    for (size_t instanceIdx = 0; instanceIdx < s_instances.size(); instanceIdx++)
    {
        if (!s_instances[instanceIdx]->_makeTheorem(name, cont, instanceIdx))
        {
            return false;
        }
    }
    return true;
}

bool N_DarkLogic::Logic::learnRule()
{
    if (isDemonstrated())
    {
        for (auto& instance : s_instances)
        {
            instance->_learnRule();
        }
        return true;
    }
    return false;
}

void N_DarkLogic::Logic::printTheorem()
{
    s_masterInstance->_printTheorem();
}


void Logic::printTheorem(const size_t& instanceIdx)
{
    s_instances[instanceIdx]->_printTheorem();
}

std::string N_DarkLogic::Logic::toStrTheorem()
{
    return s_masterInstance->_toStrTheorem();
}

std::string N_DarkLogic::Logic::toStrTheorem(const size_t& instanceIdx)
{
    return s_instances[instanceIdx]->_toStrTheorem();
}

std::string N_DarkLogic::Logic::toNormStrTheorem()
{
    return s_masterInstance->_toNormStrTheorem();
}

std::string N_DarkLogic::Logic::theoremName()
{
    return s_masterInstance->m_theoremName;
}

size_t N_DarkLogic::Logic::nbTheorems(const size_t& instanceIdx)
{
    return s_instances[instanceIdx]->m_theoremDb.size();
}

const std::vector<Action::Id>& N_DarkLogic::Logic::getActions()
{
    return s_masterInstance->_getActions();
}

const std::vector<Action::Id>& Logic::getActions(const size_t& instanceIdx)
{
    return s_instances[instanceIdx]->_getActions(instanceIdx);
}

std::vector<Action> N_DarkLogic::Logic::getDemonstration()
{
    return s_masterInstance->_getDemonstration();
}

std::unordered_map<std::string, RuleContent> N_DarkLogic::Logic::getRuleContents()
{
    return s_masterInstance->_getRuleContents();
}

std::vector<Action> Logic::getHumanActions()
{
    return s_masterInstance->_getHumanActions();
}

void N_DarkLogic::Logic::apply(const Action::Id& actionKey)
{
    s_masterInstance->_apply(actionKey);
}

void Logic::apply(const size_t& instanceIdx, const Action::Id& actionKey)
{
    s_instances[instanceIdx]->_apply(actionKey);
}

bool N_DarkLogic::Logic::apply(const std::string& actionStr)
{
    std::vector<std::string> args;
    size_t k = 0;
    std::string arg;
    for (; k < actionStr.size(); k++)
    {
        if (actionStr[k] == '[')
        {
            while (k < actionStr.size())
            {
                arg += actionStr[k];
                if (actionStr[k] == ']')
                {
                    break;
                }
                k++;
            }
        }
        else if ((actionStr[k] == ','))
        {
            if (arg != "")
            {
                args.push_back(arg);
                arg = "";
            }
            else
            {
                std::cout << "Error in command: empty argument" << std::endl;
            }

        }
        else if (actionStr[k] == ' ')
        {
            continue;
        }
        else
        {
            arg += actionStr[k];
        }
    }

    if (arg != "")
    {
        args.push_back(arg);
        arg = "";
    }

    std::stringstream ss;
    if (args.size() == 1)
    {
        return apply(args[0], {});
    }
    else if (args.size() == 2)
    {
        std::string ruleName;
        std::vector<N_DarkLogic::Action::Id> path;
        ss << args[0];
        ss >> ruleName;
        ss.clear();
        arg = args[1];
        std::string crtIdStr;
        N_DarkLogic::Action::Id id = 0;
        size_t nbBracket = 0;
        for (size_t k = 0; k < arg.size(); k++)
        {
            if (arg[k] == '[')
            {
                nbBracket++;
            }
            else if (arg[k] == ']')
            {
                nbBracket--;
                if (nbBracket == 0)
                {
                    if (crtIdStr != "")
                    {
                        ss << crtIdStr;
                        ss >> id;
                        ss.clear();
                        path.push_back(id);
                        crtIdStr = "";
                    }
                }
            }
            else if ((arg[k] == ',') && (nbBracket == 1))
            {
                if (crtIdStr != "")
                {
                    ss << crtIdStr;
                    ss >> id;
                    ss.clear();
                    path.push_back(id);
                    crtIdStr = "";
                }
            }
            else if (arg[k] == ' ')
            {
                continue;
            }
            else
            {
                crtIdStr += arg[k];
            }
        }
        return apply(ruleName, path);
    }
    else
    {
        Log::Error("Bad number of arguments for pushAction. pushAction takes one or two arguments");
        return false;
    }
}

bool N_DarkLogic::Logic::apply(const std::string& _ruleName, const std::vector<Action::Id>& _path)
{
	return s_masterInstance->_apply(_ruleName, _path);
}

void N_DarkLogic::Logic::unapply()
{
    s_masterInstance->_unapply();
}

void Logic::unapply(const size_t& instanceIdx)
{
    s_instances[instanceIdx]->_unapply();
}

bool N_DarkLogic::Logic::hasAlreadyPlayed()
{
    return s_masterInstance->_hasAlreadyPlayed();
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


bool N_DarkLogic::Logic::_isOver()
{
    return _isDemonstrated() || !_canBeDemonstrated() || _isAlreadyPlayed();
}

void N_DarkLogic::Logic::_clear()
{
    m_theorem = nullptr;
    m_antecedents.clear();
    m_isLastRuleSymetric = true;
    m_rules.clear();
    m_theoremDb.clear();
}

bool N_DarkLogic::Logic::_isDemonstrated()
{
    if(m_theorem->isEvaluated())
    {
        if (m_theorem->evaluate())
        {
            return true;
        }
        else
        {
            return m_isLastRuleSymetric;
        }       
    }
    return false;

}

bool N_DarkLogic::Logic::_isAlreadyPlayed()
{
    for (const auto& antecedent : m_antecedents)
    {
        auto theorem = antecedent.theorem;
        if ((*theorem) == (*m_theorem))
        {
            return true;
        }
    }
    return false;
}

bool N_DarkLogic::Logic::_canBeDemonstrated()
{    
    if (m_theorem->canBeDemonstrated())
    {
        if (m_theorem->isEvaluated())
        {
            if (!m_theorem->evaluate())
            {
                return m_isLastRuleSymetric;
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool N_DarkLogic::Logic::_evaluate()
{
    return m_theorem->evaluate();
}

bool N_DarkLogic::Logic::_isEvaluated()
{
    return m_theorem->isEvaluated();
}

bool N_DarkLogic::Logic::_appliedRuleSymetric()
{
    return m_isLastRuleSymetric;
}

const State& N_DarkLogic::Logic::_getState() const
{
    return m_theorem->getState();
}

const std::vector<State> N_DarkLogic::Logic::_getRuleStates() const
{
    return m_rules.getStates();
}

bool N_DarkLogic::Logic::_makeTheorem(const std::string& name, const std::string& cont)
{
    try
    {
        m_theoremName = name;
        m_theorem = Parser::createFormula<ASubTheorem>(name, cont);

        //Log::Debug(theorem->toString());
        //Compute actions
        //_getActions();

        return true;
    }
    catch (std::runtime_error & e)
    {
        Log::Error(e.what());
    }
    return false;
}

bool N_DarkLogic::Logic::_makeTheorem(const std::string& name, const std::string& cont, const size_t& logicIdx)
{
    try
    {
        m_theoremName = name;
        
        m_theorem = Parser::createFormula<ASubTheorem>(name, cont);

        //Compute actions
        //_getActions(logicIdx);

        return true;
    }
    catch (std::runtime_error& e)
    {
        Log::Error(e.what());
    }
    return false;
}

void N_DarkLogic::Logic::_learnRule()
{
    
}

void N_DarkLogic::Logic::_printTheorem()
{
    m_theorem->print();
}

std::string N_DarkLogic::Logic::_toStrTheorem() const
{
    return m_theorem->toString();
}

std::string N_DarkLogic::Logic::_toNormStrTheorem()
{
    return m_theorem->toNormalizedString();
}

const std::vector<Action::Id>& N_DarkLogic::Logic::_getActions()
{
    return m_rules.getActions(m_theorem);
}

const std::vector<Action::Id>& N_DarkLogic::Logic::_getActions(const size_t& logicIdx)
{
    return m_rules.getActions(m_theorem, logicIdx);
}

std::vector<Action> N_DarkLogic::Logic::_getHumanActions()
{
    return m_rules.getHumanActions(m_theorem);
}

std::vector<Action> N_DarkLogic::Logic::_getDemonstration()
{
    std::vector<Action> ret;
    if (_isDemonstrated())
    {
        std::vector<Antecedent> antecedents = m_antecedents;
        //go back to first theorem
        for (size_t k = 0; k < antecedents.size(); k++)
        {
            _unapply();
        }

        for (size_t k = 0; k < antecedents.size(); k++)
        {
            Antecedent antecedent = antecedents[k];
            ret.push_back(*(m_rules.getHumanAction(antecedent.theorem, antecedent.nextAction)));
            _apply(antecedent.nextAction);
        }
    }   
    return ret;
}

std::unordered_map<std::string, RuleContent> N_DarkLogic::Logic::_getRuleContents() const
{
    return m_rules.getRuleContents();
}

void N_DarkLogic::Logic::_apply(const Action::Id& actionKey)
{
    auto antecedent = m_theorem;
    m_antecedents.push_back(Antecedent(actionKey, antecedent, m_isLastRuleSymetric ));
    auto couple= m_rules.apply(actionKey);
    m_theorem = couple.first;
    m_isLastRuleSymetric = m_isLastRuleSymetric && couple.second;
}

bool N_DarkLogic::Logic::_apply(const std::string& _ruleName, const std::vector<Action::Id>& _path)
{
    try
    {
        auto antecedent = m_theorem;        
        auto tuple = m_rules.apply(m_theorem, _ruleName, _path);
        m_theorem = std::get<0>(tuple);
        m_isLastRuleSymetric = m_isLastRuleSymetric && std::get<1>(tuple);
        m_antecedents.push_back(Antecedent(std::get<2>(tuple), antecedent, m_isLastRuleSymetric));
        return true;
    }
    catch (std::runtime_error& e)
    {
        Log::Error(e.what());
        return false;
    }
    
}

void N_DarkLogic::Logic::_unapply()
{    
    auto antecedent = m_antecedents.back().theorem;
    m_isLastRuleSymetric = m_antecedents.back().isSymmetric;
    m_rules.unapply();
    m_theorem = antecedent;
    m_antecedents.pop_back();
}

bool N_DarkLogic::Logic::_hasAlreadyPlayed()
{
    return m_antecedents.size();
}
