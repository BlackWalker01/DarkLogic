#ifndef LOGICGAME_H
#define LOGICGAME_H
#include <unordered_map>

class LogicGame
{
    enum Fun
    {
        GET_ACTION,
        PUSH_ACTION,
        POP_ACTION,
        HELP
    };

public:
    LogicGame() = default;

    void start();
private:
    bool isOver();
    void game();
    void askCall();

    //Methods to manipulate Logic
    void createTheorem();
    void printActions();
    bool pushAction(const std::string& action);
    bool popAction();


    static const std::unordered_map<std::string,Fun> s_funHash;
};

#endif // LOGICGAME_H
