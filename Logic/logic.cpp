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

//Set initialization
template<> const std::string Set<BooleanSet>::s_name="BB";
//template<> const std::unique_ptr<AxiomSet<BooleanSet>> Set<BooleanSet>::s_axiomSet=nullptr;
template<> const std::unique_ptr<DbRule> Set<BooleanSet>::s_rules=std::make_unique<DbRule>();
template<> const std::string Set<NaturalIntegerSet>::s_name="NN";
//template<> const std::unique_ptr<AxiomSet<NaturalIntegerSet>> Set<NaturalIntegerSet>::s_axiomSet=nullptr;
template<> const std::unique_ptr<DbRule> Set<NaturalIntegerSet>::s_rules=std::make_unique<DbRule>();


const std::unordered_map<std::string,ConstantEnum> AbstractTerm::s_constantHash={
    {"false",FALSE_TYPE},
    {"true",TRUE_TYPE},
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

std::unique_ptr<Logic> Logic::instance=nullptr;

Logic::Logic(): m_theorem(nullptr), m_nbAppliedRule(0), m_isLastRuleSymetric(true)
{

}

void Logic::init()
{
    instance=std::make_unique<Logic>();
    Log::Debug("Logic Instantiation");
    instance->_init();
}

bool N_Logic::Logic::isOver()
{
    return instance->_isOver();
}

bool Logic::isDemonstrated()
{
    return instance->_isDemonstrated();
}

bool N_Logic::Logic::isAlreadyPlayed()
{
    return instance->_isAlreadyPlayed();
}

bool N_Logic::Logic::canBeDemonstrated()
{
    return instance->_canBeDemonstrated();
}

bool Logic::evaluate()
{
    return instance->_evaluate();    
}

bool Logic::makeTheorem(const std::string& name, const std::string& cont)
{
    return instance->_makeTheorem(name,cont);
}

void Logic::testTheorem()
{
    //Test part
    std::chrono::high_resolution_clock::time_point start, end;
    size_t nbTest=1000;
    start = std::chrono::high_resolution_clock::now();
    Log::Info("Compute actions for theorem: "+instance->m_theorem->toString());
    std::vector<size_t> ret=getActions();
    Log::Info("Number of possible actions: "+sizeToString(ret.size()));
    for(size_t k=0;k<ret.size();k++)
    {
        size_t action=ret[k];
        apply(action);
        Log::Info("Current theorem: "+instance->m_theorem->toString());
        unapply();
    }
    for(size_t numTest=1;numTest<nbTest;numTest++)
    {
        std::vector<size_t> ret=getActions();
        for(size_t k=0;k<ret.size();k++)
        {
            size_t action=ret[k];
            apply(action);
            unapply();
        }
    }

    end = std::chrono::high_resolution_clock::now();
    double elapsed_seconds = std::chrono::duration<double,std::milli>
                             (end-start).count();
    std::cout<<"All actions at depth=1 applied "<<nbTest<<" times in "<<elapsed_seconds<<" milliseconds"<<std::endl;
}


void Logic::printTheorem()
{
    instance->_printTheorem();
}

std::vector<size_t> Logic::getActions()
{
    return instance->_getActions();
}

std::vector<Action> Logic::getHumanActions()
{
    return instance->_getHumanActions();
}

void Logic::apply(const size_t &actionKey)
{
    instance->_apply(actionKey);    
}

void Logic::unapply()
{
    instance->_unapply();    
}

bool Logic::hasAlreadyPlayed()
{
    return instance->_hasAlreadyPlayed();    
}

template<typename OpeType>
void Logic::insert(const ptr<Rule<OpeType> > &rule)
{
    m_rules.insert(rule);
}

void N_Logic::Logic::_init()
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
    auto orE = std::dynamic_pointer_cast<const Rule<Implication<ASubRule>>>(createRule("||E", "(({HYP}p||q)&&({p,HYP}r)&&({q,HYP}r))=>(({HYP}r)")); //complicated to use!
    insert(orE);

    //IMPLICATION Rules
    auto implI = std::dynamic_pointer_cast<const Rule<Implication<ASubRule>>>(createRule("=>I", "({p,HYP}q)=>({HYP}p=>q)"));
    insert(implI);
    auto implE = std::dynamic_pointer_cast<const Rule<Implication<ASubRule>>>(createRule("=>E", "(({HYP}p)&&({HYP}p=>q))=>({p,p=>q,HYP}q)"));
    insert(implE);


    //FALSE Rules
    auto fi = std::dynamic_pointer_cast<const Rule<Implication<ASubRule>>>(createRule("FI", "({HYP}(p&&!p))=>({HYP}false)"));
    insert(fi);
    auto fe = std::dynamic_pointer_cast<const Rule<Equivalent<ASubRule>>>(createRule("FE", "({p,HYP}false)<=>({HYP}!p)"));
    insert(fe);
    auto fiNot = std::dynamic_pointer_cast<const Rule<Implication<ASubRule>>>(createRule("FI!", "({!p,HYP}p)=>({!p,HYP}false)"));
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

    Log::Debug("Rules Instantiated");
}

bool N_Logic::Logic::_isOver()
{
    return _isDemonstrated() || !_canBeDemonstrated() || _isAlreadyPlayed();
}

bool N_Logic::Logic::_isDemonstrated()
{
    try
    {
        _evaluate();
        return m_isLastRuleSymetric;
        
    }
    catch (std::runtime_error&)
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
    catch (std::runtime_error&)
    {
        return true;
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

bool N_Logic::Logic::_makeTheorem(const std::string& name, const std::string& cont)
{
    Log::Debug("New Theorem: " + name);
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
    return m_rules.getActions(instance->m_theorem, instance->m_nbAppliedRule);
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
    m_theorem = m_rules.apply(actionKey, m_theorem);
    m_isLastRuleSymetric = m_isLastRuleSymetric && m_rules.isLastRuleSymetric(actionKey);
    m_nbAppliedRule++;
}

void N_Logic::Logic::_unapply()
{
    m_nbAppliedRule--;
    auto antecedent = m_antecedents.back().first;
    m_isLastRuleSymetric = m_antecedents.back().second;
    m_rules.unapply(antecedent, m_nbAppliedRule);
    m_theorem = antecedent;
    m_antecedents.pop_back();
}

bool N_Logic::Logic::_hasAlreadyPlayed()
{
    return m_antecedents.size();
}
