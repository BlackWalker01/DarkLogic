#ifndef LOGIC_H
#define LOGIC_H
#include <unordered_map>
#include "logic_global.h"
#include "Operator/operator.h"
#include "Formula/Boolean/dbrule.h"

namespace N_Logic {

class LOGICSHARED_EXPORT Logic
{
private:
    Logic();

public:
    //friend typename std::_MakeUniq<Logic>::__single_object std::make_unique<Logic>(); //mingw
    friend std::unique_ptr<Logic> std::make_unique();
    static void init();

    static bool isOver();

    //property access methods.
    static bool isDemonstrated();
    static bool isAlreadyPlayed();
    static bool canBeDemonstrated();
    static bool evaluate();
    static bool makeTheorem(const std::string &name, const std::string &cont);
    static void printTheorem();

    //operator access methods
    static std::vector<size_t> getActions();
    static std::vector<Action> getHumanActions();

    //demonstration methods
    static void apply(const size_t& actionKey);
    static void unapply();
    static bool hasAlreadyPlayed();

    ~Logic() = default;

private:
    //Attributes
    ptr<ASubTheorem> m_theorem;
    std::vector<std::pair<ptr<ASubTheorem>,bool>> m_antecedents;
    DbRule m_rules;
    size_t m_nbAppliedRule;
    bool m_isLastRuleSymetric;

    static std::unique_ptr<Logic> instance;

    //Private methods   
    void _init();
    bool _isOver();
    static void testTheorem();
    template<typename OpeType>
    void insert(const ptr<Rule<OpeType>>& rule);

    //property access methods.
    bool _isDemonstrated();
    bool _isAlreadyPlayed();
    bool _canBeDemonstrated();
    bool _evaluate();
    bool _makeTheorem(const std::string& name, const std::string& cont);
    void _printTheorem();

    //operator access methods
    std::vector<size_t> _getActions();
    std::vector<Action> _getHumanActions();

    //demonstration methods
    void _apply(const size_t& actionKey);
    void _unapply();
    bool _hasAlreadyPlayed();
};
}
#endif // LOGIC_H
