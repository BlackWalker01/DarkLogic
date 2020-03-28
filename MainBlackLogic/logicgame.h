#ifndef LOGICGAME_H
#define LOGICGAME_H
#include <unordered_map>
#include "EnumFun.h"
#include <memory>
#include "player.h"

class LogicGame
{   
    enum Mode
    {
        NoMode,
        HumanMode,
        AIMode
    };
public:
    LogicGame();

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
    void askCall();

    //Methods to manipulate Logic
    void createTheorem();
    void printActions();
    bool pushAction(const std::string& action);
    bool popAction();

    void askPlayer();    

    //Attribute
    Mode m_mode;
    std::unique_ptr<Player> m_player;
};

#endif // LOGICGAME_H
