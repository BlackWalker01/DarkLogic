#include "human.h"
#include "logic.h"
#include "MainDarkLogic/action.h"
#include "MainDarkLogic/EnumFun.h"
#include <sstream>
#include <iostream>

const std::unordered_map<std::string, Fun> Human::s_funHash =
{ {"getaction",GET_ACTION},
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

Human::Human(): Player("you")
{
}

std::shared_ptr<const Action> Human::play()
{
    std::string cmd = "";
    std::string func = "";
    std::string args = "";
    bool ok = false;
    while (!ok)
    {
        std::cout << "What do you want to do?" << std::endl;
        if (!std::getline(std::cin, cmd))
        {
            std::cout << "Error in command" << std::endl;
            continue;
        }

        size_t k = 0;
        size_t firstParIdx = 0;
        size_t nbPar = 0;
        for (; k < cmd.size(); k++)
        {
            if (cmd[k] == '(')
            {
                nbPar++;
                if (nbPar == 1)
                {
                    firstParIdx = k;
                    func = cmd.substr(0, k);
                }
            }
            else if (cmd[k] == ')')
            {
                if (nbPar == 0)
                {
                    std::cout << "Error in command" << std::endl;
                    continue;
                }
                nbPar--;
                if (nbPar == 0)
                {
                    args = cmd.substr(firstParIdx + 1, k - firstParIdx - 1);
                }
            }
        }

        //call function
        auto it = s_funHash.find(func);
        if (it != s_funHash.end())
        {
            switch (it->second)
            {
            case GET_ACTION:
            {
                return std::make_shared<const Action>(GET_ACTION);
            }
            case PUSH_ACTION:
            {
                size_t id = 0;
                std::stringstream ss;
                ss << args;
                ss >> id;
                std::vector<size_t> actions = N_DarkLogic::DarkLogic::getActions(0);
                for (const auto& actionId : actions)
                {
                    if (id == actionId)
                    {
                        return std::make_shared<const Action>(PUSH_ACTION,id);
                    }
                }
                std::cout << args << " is not a valid action" << std::endl;
                break;
            }
            case POP_ACTION:
            {
                return std::make_shared<const Action>(POP_ACTION);
            }
            case HELP:
            {
                return std::make_shared<const Action>(HELP);
            }
            case NONE:
            {
                break;
            }
            }
        }
        else
        {
            std::cout << "Unknown command " << func << std::endl;
        }
    }
    return nullptr;
}
