/*===---  logic.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file defines DarkLogic API 
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_LOGIC_H
#define DARK_LOGIC_LOGIC_H
#include <unordered_map>
#include "Operator/operator.h"
#include "Formula/Boolean/dbrule.h"
#include "Formula/Boolean/dbtheorem.h"
#include "Utils/utils.h"
#include "Utils/action.h"
#include "Utils/state.h"

namespace N_DarkLogic {

class Logic
{
private:
    Logic();

public:
    //if we use MVSC STL includes
    #ifdef _MSVC_STL_VERSION
        friend std::unique_ptr<Logic> std::make_unique();
    //if we use gcc STL includes
    #else
        friend typename std::_MakeUniq<Logic>::__single_object std::make_unique<Logic>(); //gcc
    #endif
    static void init(const size_t& nbInstances);

    static bool isOver();
    static bool isOver(const size_t& instanceIdx);
    static void clearAll();
    static void clear();
    static void clear(const size_t& instanceIdx);

    //property access methods.
    static bool isDemonstrated();
    static bool isDemonstrated(const size_t& instanceIdx);
    static bool isAlreadyPlayed();
    static bool isAlreadyPlayed(const size_t& instanceIdx);
    static bool canBeDemonstrated();
    static bool canBeDemonstrated(const size_t& instanceIdx);
    static bool evaluate();
    static bool evaluate(const size_t& instanceIdx);
    static bool isEvaluated();
    static bool isEvaluated(const size_t& instanceIdx);
    static const State& getState();
    static const State& getState(const size_t& instanceIdx);
    static std::vector<State> getRuleStates();
    static bool appliedRuleSymetric();
    static bool appliedRuleSymetric(const size_t& instanceIdx);

    static bool makeTheorem(const std::string &name, const std::string &cont);
    static bool learnRule();
    static void printTheorem();
    static void printTheorem(const size_t& instanceIdx);
    static std::string toStrTheorem();
    static std::string toStrTheorem(const size_t& instanceIdx);
    static std::string toNormStrTheorem();

    //operator access methods
    static std::string theoremName();
    static const std::vector<Action::Id>& getActions();
    static const std::vector<Action::Id>& getActions(const size_t& instanceIdx);
    static std::vector<Action> getHumanActions();
    static std::vector<Action> getDemonstration();
    static std::unordered_map<std::string, RuleContent> getRuleContents();

    //demonstration methods
    static void apply(const Action::Id& actionKey);
    static void apply(const size_t& instanceIdx, const Action::Id& actionKey);
    static bool apply(const std::string& actionStr);
    static bool apply(const std::string& _ruleName, const std::vector<Action::Id>& _path);
    static void unapply();
    static void unapply(const size_t& instanceIdx);
    static bool hasAlreadyPlayed();
    static bool hasAlreadyPlayed(const size_t& instanceIdx);

    template<typename Type, typename ...Args>
    static ptr<Type> make_theorem_formula(Args&& ...args)
    {
        return s_masterInstance->_make_theorem_formula<Type>(args...);
    }
    template<typename Type, typename ...Args>
    static ptr<Type> make_theorem_formula(const size_t& logicIdx, Args&& ...args)
    {
        return s_instances[logicIdx]->_make_theorem_formula<Type>(args...);
    }


    ~Logic() = default;

private:
    struct Antecedent
    {
        Antecedent(const Action::Id& nextAction_, const ptr<ASubTheorem>& theorem_, bool isSymmetric_):
            nextAction(nextAction_), theorem(theorem_), isSymmetric(isSymmetric_)
        {}

        Action::Id nextAction;
        ptr<ASubTheorem> theorem;
        bool isSymmetric;
    };    

    //Private methods   
    bool _isOver();
    void _clear();
    template<typename OpeType>
    void insert(const ptr<Rule<OpeType>>& rule);

    //property access methods.
    bool _isDemonstrated();
    bool _isAlreadyPlayed();
    bool _canBeDemonstrated();
    bool _evaluate();
    bool _isEvaluated();
    bool _appliedRuleSymetric();
    const State& _getState() const;
    const std::vector<State> _getRuleStates() const;

    bool _makeTheorem(const std::string& name, const std::string& cont);
    bool _makeTheorem(const std::string& name, const std::string& cont, const size_t& logicIdx);
    void _learnRule();
    void _printTheorem();
    std::string _toStrTheorem() const;
    std::string _toNormStrTheorem();

    //operator access methods
    const std::vector<Action::Id>& _getActions();
    const std::vector<Action::Id>& _getActions(const size_t& logicIdx);
    std::vector<Action> _getHumanActions();
    std::vector<Action> _getDemonstration();
    std::unordered_map<std::string, RuleContent> _getRuleContents() const;

    //demonstration methods
    void _apply(const Action::Id& actionKey);
    bool _apply(const std::string& _ruleName, const std::vector<Action::Id>& _path);
    void _unapply();
    bool _hasAlreadyPlayed();

    template<typename Type, typename ...Args>
    ptr<Type> _make_theorem_formula(Args&& ...args)
    {
        return m_theoremDb.make_theorem_formula<Type>(args...);
    }


    //Attributes
    std::string m_theoremName;
    ptr<ASubTheorem> m_theorem;
    std::vector<Antecedent> m_antecedents;
    DbRule m_rules;
    bool m_isLastRuleSymetric;
    DbTheorem m_theoremDb;

    static std::unique_ptr<Logic> s_masterInstance;
    static std::vector<std::unique_ptr<Logic>> s_instances;
};
}
#endif // DARK_LOGIC_LOGIC_H
