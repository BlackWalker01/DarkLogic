#ifndef LOGICGAME_H
#define LOGICGAME_H
#include <unordered_map>
#include "EnumFun.h"
#include <memory>
#include "player.h"
#include "dbtheorem.h"
#include "utils.h"

#define AI_TIMEOUT 20 //seconds
#define MAX_NB_ATTEMPTS 10

class LogicGame
{   
    enum class Mode
    {
        NoMode,
        HumanMode,
        AIMode
    };
public:    

    LogicGame(const std::unordered_map<ConfigType, std::string>& config);

    void start();

    ~LogicGame() = default;
private:
    bool isOver();
    bool isDemonstrated();
    bool isAlreadyPlayed();
    bool hasAlreadyPlayed();
    bool canBeDemonstrated();
    bool appliedRuleSymetric();
    bool isEvaluated();
    void game();

    //Methods to manipulate BlackLogic
    void createTheorem();
    void makeTheorem(const std::string& name, const std::string& content);
    void printActions();
    bool pushAction(const std::string& action);
    bool popAction();

    void askPlayer(); 
    void createPlayer(const std::string& mode);

    //Attribute
    Mode m_mode;
    std::unique_ptr<Player> m_player;
    DbTheorem m_db;
    bool m_isAuto;
    unsigned int m_eloThm;
};

#endif // LOGICGAME_H
