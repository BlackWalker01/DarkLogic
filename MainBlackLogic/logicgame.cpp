#include "logicgame.h"
#include "logic.h"

void LogicGame::start()
{
    N_Logic::Logic::init();

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
        }
        thStr=thStr2;
        ok=N_Logic::Logic::makeTheorem(thName,thStr);
    }
}
