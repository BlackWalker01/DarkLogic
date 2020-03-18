#include "logicgame.h"
#include "logic.h"

const std::unordered_map<std::string,LogicGame::Fun> LogicGame::s_funHash=
{{"getaction",GET_ACTION},
 {"getAction",GET_ACTION},
 {"GetAction",GET_ACTION},
 {"GETACTION",GET_ACTION},

{"pushaction",PUSH_ACTION},
 {"pushAction",PUSH_ACTION},
 {"PushAction",PUSH_ACTION},
 {"PUSHACTION",PUSH_ACTION},

{"popaction",POP_ACTION},
 {"popAction",POP_ACTION},
 {"PopAction",POP_ACTION},
 {"POPACTION",POP_ACTION},

 {"help",HELP},
  {"Help",HELP},
  {"HELP",PUSH_ACTION},
};

void LogicGame::start()
{
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
        askCall();
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

void LogicGame::askCall()
{
    std::string cmd="";
    std::string func="";
    std::string args="";
    bool ok=false;
    while(!ok)
    {
        std::cout<<"What do you want to do?"<<std::endl;
        if(!std::getline(std::cin,cmd))
        {
            std::cout<<"Error in command"<<std::endl;
            continue;
        }

        size_t k=0;
        size_t firstParIdx=0;
        size_t nbPar=0;
        for(;k<cmd.size();k++)
        {
            if(cmd[k]=='(')
            {
                nbPar++;
                if(nbPar==1)
                {
                    firstParIdx=k;
                    func=cmd.substr(0,k);
                }
            }
            else if(cmd[k]==')')
            {
                if(nbPar==0)
                {
                    std::cout<<"Error in command"<<std::endl;
                    continue;
                }
                nbPar--;
                if(nbPar==0)
                {
                    args=cmd.substr(firstParIdx+1,k-firstParIdx-1);
                }
            }
        }

        //call function
        auto it=s_funHash.find(func);
        if(it!=s_funHash.end())
        {
            switch (it->second)
            {
                case GET_ACTION:
                {
                    printActions();
                    ok=true;
                    break;
                }
                case PUSH_ACTION:
                {
                    ok=pushAction(args);
                    break;
                }
                case POP_ACTION:
                {
                    ok=popAction();
                    break;
                }
                case HELP:
                {
                    ok=true;
                    break;
                }
            }
        }
        else
        {
            std::cout<<"Unknown command "<<func<<std::endl;
        }
    }

}

