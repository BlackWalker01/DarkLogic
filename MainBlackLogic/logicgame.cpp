#include "logicgame.h"
#include "logic.h"
#include "Human/human.h"
#include "AI/ai.h"
#include "action.h"

void LogicGame::start()
{
    askPlayer();
    N_Logic::Logic::init();
    createTheorem();
    game();
}

void LogicGame::createTheorem()
{
    //ask user to create theorem
    std::string thName="";
    std::string thStr="";
    bool ok=false;
    while(!ok)
    {
        std::cout<<"Create a new theorem to start session"<<std::endl;
        std::cout<<"Theorem name:"<<std::endl;
        if(!std::getline(std::cin,thName))
        {
            std::cout<<"Error in theorem name"<<std::endl;
            continue;
        }
        std::cout<<"Theorem content:"<<std::endl;
        if(!std::getline(std::cin,thStr))
        {
            std::cout<<"Error in theorem content"<<std::endl;
            continue;
        }
        //change '?' characters to '€' after let operators
        std::string let="let";
        std::string in="in";
        std::string thStr2="";
        bool hasLet=false;
        for(size_t k=0;k<thStr.size();k++)
        {
            char c=thStr[k];
            if(k<thStr.size()-4 && thStr.substr(k,k+3)==let)
            {
                hasLet=true;
                thStr2+=c;
                continue;
            }
            if(hasLet)
            {
                if(k<thStr.size()-3 && thStr.substr(k,k+2)==in)
                {
                    hasLet=false;
                    thStr2+=c;
                    continue;
                }
                if(thStr[k]=='?')
                {
                    thStr2+="€";
                }
                else
                {
                    thStr2+=c;
                }
            }
            else
            {
                thStr2+=c;
            }
        }
        thStr=thStr2;
        ok=N_Logic::Logic::makeTheorem(thName,thStr);
    }
}

void LogicGame::printActions()
{
    auto actions=N_Logic::Logic::getHumanActions();
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
    std::vector<size_t> actions=N_Logic::Logic::getActions();
    bool foundAction=false;
    for(const auto& actionId: actions)
    {
        if(id==actionId)
        {
            foundAction=true;
            break;
        }
    }
    if(foundAction)
    {
        N_Logic::Logic::apply(id);
        std::cout<<"Current theorem is"<<std::endl;
        N_Logic::Logic::printTheorem();
    }
    else
    {
        std::cout<<action<<" is not a valid action"<<std::endl;
    }
    return foundAction;
}

bool LogicGame::popAction()
{
    if(N_Logic::Logic::hasAlreadyPlayed())
    {
        N_Logic::Logic::unapply();
        std::cout<<"Current theorem is"<<std::endl;
        N_Logic::Logic::printTheorem();
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
        std::cout << "Choose the mode (Human/AI)" << std::endl;
        std::string mode = "";
        std::getline(std::cin, mode);
        if (mode=="Human" || mode=="human"||mode=="HUMAN")
        {
            m_player = std::make_unique<Human>();
            ok = true;
        }
        else if (mode=="ai"|| mode=="AI" || mode=="Ai")
        {
            m_player = std::make_unique<AI>();
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
    return N_Logic::Logic::isOver();
}

bool LogicGame::isDemonstrated()
{
    return N_Logic::Logic::isDemonstrated();
}

bool LogicGame::isAlreadyPlayed()
{
    return N_Logic::Logic::isAlreadyPlayed();
}

bool LogicGame::canBeDemonstrated()
{
    return N_Logic::Logic::canBeDemonstrated();
}

void LogicGame::game()
{
    while(!isOver())
    {
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
                N_Logic::Logic::apply(action->id());
                N_Logic::Logic::printTheorem();
                break;
            }
            case POP_ACTION:
            {
                if (!popAction())
                {
                    std::cout << "Cannot pop action!" << std::endl;
                }
                else
                {
                    N_Logic::Logic::printTheorem();
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
    }
    
    if (isDemonstrated())
    {
        std::cout << "You won! you finished the demonstration!" << std::endl;
    }
    else if (isAlreadyPlayed())
    {
        std::cout << "You lost! you have already had this theorem!" << std::endl;
    }
    else /*if (!canBeDemonstrated())*/
    {
        std::cout << "You lost! you cannot (\"back-\")demonstrate that your theorem is false with implications!" << std::endl;
    }  
    
}
