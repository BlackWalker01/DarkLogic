#ifndef LOGICGAME_H
#define LOGICGAME_H
#include <unordered_map>
#include "EnumFun.h"
#include <memory>
#include "player.h"

class LogicGame
{   
public:
    LogicGame() = default;

    void start();

    ~LogicGame() = default;
private:
    bool isOver();
    bool isDemonstrated();
    bool isAlreadyPlayed();
    bool canBeDemonstrated();
    void game();
    void askCall();

    //Methods to manipulate Logic
    void createTheorem();
    void printActions();
    bool pushAction(const std::string& action);
    bool popAction();

    void askPlayer();    

    //Attribute
    std::unique_ptr<Player> m_player;
};

#endif // LOGICGAME_H
