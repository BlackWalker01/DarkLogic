#include "logicgame.h"
#include "darklogic.h"
#include "Human/human.h"
#include "AI/ai.h"
#include <memory>
#include <thread>
#include <iostream>
#include "MainDarkLogic/action.h"

LogicGame::LogicGame(bool isAuto):
    m_mode(NoMode), m_player(nullptr), m_isAuto(isAuto), m_eloThm(1500)
{

}

void LogicGame::start()
{
    std::cout << "Welcome in LogicGame (v1.3.0)!" << std::endl;
    
    //create player and init logic
    askPlayer();

    while (true)
    {        
        createTheorem();

        //Start game
        game();
    }    
}

void LogicGame::createTheorem()
{
    if (!m_isAuto)
    {
        //ask user to create theorem
        std::string thName = "";
        std::string thStr = "";
        bool ok = false;
        while (!ok)
        {
            std::cout << "Create a new theorem to start a session" << std::endl;
            std::cout << "Theorem name:" << std::endl;
            if (!std::getline(std::cin, thName))
            {
                std::cout << "Error in theorem name" << std::endl;
                continue;
            }
            std::cout << "Theorem content:" << std::endl;
            if (!std::getline(std::cin, thStr))
            {
                std::cout << "Error in theorem content" << std::endl;
                continue;
            }
            //change '?' characters to '€' after let operators
            std::string let = "let";
            std::string in = "in";
            std::string thStr2 = "";
            bool hasLet = false;
            for (size_t k = 0; k < thStr.size(); k++)
            {
                char c = thStr[k];
                if (k < thStr.size() - 4 && thStr.substr(k, k + 3) == let)
                {
                    hasLet = true;
                    thStr2 += c;
                    continue;
                }
                if (hasLet)
                {
                    if (k < thStr.size() - 3 && thStr.substr(k, k + 2) == in)
                    {
                        hasLet = false;
                        thStr2 += c;
                        continue;
                    }
                    if (thStr[k] == '?')
                    {
                        thStr2 += "€";
                    }
                    else
                    {
                        thStr2 += c;
                    }
                }
                else
                {
                    thStr2 += c;
                }
            }
            thStr = thStr2;
            ok = N_DarkLogic::DarkLogic::makeTheorem(thName, thStr);
            if (ok && m_mode == Mode::HumanMode)
            {
                std::cout << "Choose between :" << std::endl;
                std::cout << "-> getAction() : to print all possible actions" << std::endl;
                std::cout << "-> pushAction(id) : to make action identified by id" << std::endl;
                std::cout << "-> pushAction(ruleName, path) : to make action identified by ruleName (name of the rule to apply) and path"
                    " (list of indexes [id1, id2, ..., idn]) in theorem " << std::endl;
                std::cout << "-> popAction : to cancel the latest action" << std::endl;
            }
            /*std::cout << "[DEBUG] content: " << std::endl;
            N_DarkLogic::DarkLogic::DarkLogic::printTheorem(0);*/
        }
    }
    else
    {
        auto thm = m_db.getRandomTheorem(m_player->elo());
        m_eloThm = thm.elo();
        std::cout << thm.name() + " theorem :'" + thm.content() + "' has been chosen for this game" << std::endl;
        N_DarkLogic::DarkLogic::makeTheorem(thm.name(), thm.content());
    }
}

void LogicGame::makeTheorem(const std::string& name, const std::string& content)
{
    std::cout << "Demonstrate " << name << ": " << content << std::endl;
    N_DarkLogic::DarkLogic::makeTheorem(name, content);
}

void LogicGame::printActions()
{
    auto actions=N_DarkLogic::DarkLogic::getHumanActions();
    for(const auto& action: actions)
    {
        std::cout<<action.toString()<<std::endl;
    }
}

bool LogicGame::pushAction(const std::string &action)
{
    size_t id=0;
    std::stringstream ss;
    ss<<action;
    ss>>id;
    std::vector<N_DarkLogic::Action> actions=N_DarkLogic::DarkLogic::getHumanActions();
    bool foundAction=false;
    std::string ruleName = "";
    for(const auto& action: actions)
    {
        if(id==action.id())
        {
            foundAction=true;
            ruleName = action.ruleName();
            break;
        }
    }
    if(foundAction)
    {
        N_DarkLogic::DarkLogic::apply(id);
        std::cout << m_player->name() << " use " << ruleName << std::endl;
        std::cout<<"Current theorem is"<<std::endl;
        N_DarkLogic::DarkLogic::printTheorem();
    }
    else
    {
        std::cout<<action<<" is not a valid action"<<std::endl;
    }
    return foundAction;
}

bool LogicGame::popAction()
{
    if(N_DarkLogic::DarkLogic::hasAlreadyPlayed())
    {
        N_DarkLogic::DarkLogic::unapply();
        std::cout<<"Current theorem is"<<std::endl;
        N_DarkLogic::DarkLogic::printTheorem();
        return true;
    }
    else
    {
        return false;
    }
}

void LogicGame::askPlayer()
{
    bool ok = false;
    while (!ok)
    {
        std::cout << "Choose the mode (Human/AI/AIDeep)" << std::endl;
        std::string mode = "";
        std::getline(std::cin, mode);
        if (mode=="Human" || mode=="human"||mode=="HUMAN")
        {
            std::cout << "Human Mode" << std::endl;
            m_mode = HumanMode;
            //Init Logic
            N_DarkLogic::DarkLogic::init(0);
            m_player = std::make_unique<Human>();
            ok = true;
        }
        else if (mode=="ai"|| mode=="AI" || mode=="Ai")
        {
            std::cout << "AI Mode" << std::endl;
            m_mode = AIMode;
            //Init Logic
            auto nbInstance = std::thread::hardware_concurrency(); //opti for the moment
            N_DarkLogic::DarkLogic::init(nbInstance);
            m_player = std::make_unique<AI>(AI::MCTS,nbInstance, AI_TIMEOUT);
            ok = true;
        }
        else if (mode == "aideep" || mode == "AIDEEP" || mode == "AiDeep" || mode == "AIDeep")
        {
            std::cout << "AIDeep Mode" << std::endl;
            m_mode = AIMode;
            //Init Logic
            //auto nbInstance = 1 + 1;
            auto nbInstance = (std::thread::hardware_concurrency()); //opti for the moment
            N_DarkLogic::DarkLogic::init(nbInstance);
            m_player = std::make_unique<AI>(AI::DEEP, nbInstance, AI_TIMEOUT);
            ok = true;
        }
        else
        {
            std::cout << "mode '" << mode << "' unknwon" << std::endl;
        }
    }
}

bool LogicGame::isOver()
{
    return N_DarkLogic::DarkLogic::isOver();
}

bool LogicGame::isDemonstrated()
{
    return N_DarkLogic::DarkLogic::isDemonstrated();
}

bool LogicGame::isAlreadyPlayed()
{
    return N_DarkLogic::DarkLogic::isAlreadyPlayed();
}

bool LogicGame::hasAlreadyPlayed()
{
    return N_DarkLogic::DarkLogic::hasAlreadyPlayed();
}

bool LogicGame::canBeDemonstrated()
{
    return N_DarkLogic::DarkLogic::canBeDemonstrated();
}

bool LogicGame::appliedRuleSymetric()
{
    return N_DarkLogic::DarkLogic::appliedRuleSymetric();
}

bool LogicGame::isEvaluated()
{
    return N_DarkLogic::DarkLogic::isEvaluated();
}

void LogicGame::game()
{
    size_t nbAttempts = 0;
    bool hasWon = false;
    std::cout << "Game numero " << m_player->nbGames() << std::endl;
    while(!isOver())
    {
        std::cout << "Attempt numero " << nbAttempts + 1 << "/" << MAX_NB_ATTEMPTS << std::endl;
        auto action = m_player->play();
        switch (action->fun())
        {
            case GET_ACTION:
            {
                printActions();
                break;
            }
            case PUSH_ACTION:
            {
                N_DarkLogic::DarkLogic::getActions();
                std::cout << m_player->name() << " plays action with id " << action->id() << std::endl;
                N_DarkLogic::DarkLogic::apply(action->id());
                N_DarkLogic::DarkLogic::printTheorem();
                nbAttempts++;
                break;
            }
            case POP_ACTION:
            {
                if (!popAction())
                {
                    std::cout << "Cannot pop action!" << std::endl;
                }
                break;
            }
            case HELP:
            {                
                break;
            }
            case NONE:
            {
                break;
            }
        }
        std::cout << "____________________________________________________________________________" << std::endl;
        if (nbAttempts == MAX_NB_ATTEMPTS)
        {
            break;
        }
    }
    
    if (hasAlreadyPlayed())
    {
        if (isDemonstrated())
        {
            std::cout << m_player->name() << " won! " << m_player->name() << " finished the demonstration!" << std::endl;
            hasWon = true;
        }
        else if (isAlreadyPlayed())
        {
            std::cout << m_player->name() << " lost! Repetition of theorem!" << std::endl;
        }
        else if (isEvaluated())
        {
            std::cout << m_player->name() << " lost! Cannot (\"back-\")demonstrate that a theorem is false with implications!" << std::endl;
        }
        else if (!canBeDemonstrated())
        {
            std::cout << m_player->name() << " lost! This theorem cannot be demonstrated! " 
                "It can be true or false according to the values of its variables" << std::endl;
        }
        else /*if (nbAttempts == MAX_NB_ATTEMPTS)*/
        {
            std::cout << m_player->name() << " lost! Too much attempts!" << std::endl;
        }

        if (m_isAuto)
        {
            unsigned short int W = hasWon ? 1 : 0;
            double exElo = m_player->elo();
            double newElo = round(exElo + 30 * (W - 1 / (1 + pow(10,((m_eloThm - exElo) / 400)))));
            m_player->setElo(newElo);
        }
    }
    else
    {
        if (isDemonstrated())
        {
            std::cout <<"Game Over! the demonstration is already finished!" << std::endl;
        }
        else /*if (!canBeDemonstrated())*/
        {
            std::cout << "Game Over! This theorem cannot be demonstrated! "
                "It can be true or false according to the values of its variables" << std::endl;
        }
    }
    
    // clear Logic state
    N_DarkLogic::DarkLogic::clearAll();

    // let player meditates the last game
    m_player->meditate();
}
