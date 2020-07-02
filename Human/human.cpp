#include "human.h"
#include "darklogic.h"
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
    bool ok = false;
    while (!ok)
    {
        std::string cmd = "";
        std::string func = "";
        std::vector<std::string> args;
        std::cout << "What do you want to do?" << std::endl;
        if (!std::getline(std::cin, cmd))
        {
            std::cout << "Error in command" << std::endl;
            continue;
        }
        
        func = "";
        size_t k = 0;
        size_t nbPar = 0;
        std::string arg;
        for (; k < cmd.size(); k++)
        {
            if (cmd[k] == '(')
            {
                nbPar++;
            }
            else if (cmd[k] == ')')
            {
                if (nbPar == 0)
                {
                    std::cout << "Error in command: Unxpected ')'" << std::endl;
                    break;
                }
                else if (nbPar == 1)
                {
                    if (k != cmd.size() - 1)
                    {
                        std::cout << "Error in command: Unexpected ')' before end of command" << std::endl;
                        break;
                    }
                    if (arg != "")
                    {
                        args.push_back(arg);
                    }
                    
                }
                nbPar--;
            }
            else if (cmd[k] == '[')
            {
                while (k < cmd.size())
                {
                    arg += cmd[k];
                    if (cmd[k] == ']')
                    {
                        break;
                    }
                    k++;
                }
            }
            else if ((cmd[k] == ',') && (nbPar == 1))
            {
                if (arg != "")
                {
                    args.push_back(arg);
                    arg = "";
                }
                else
                {
                    std::cout << "Error in command: empty argument" << std::endl;
                }
                
            }
            else if (cmd[k] == ' ')
            {
                continue;
            }
            else if (nbPar == 0)
            {
                func += cmd[k];
            }
            else
            {
                arg += cmd[k];
            }

            
        }

        //call function
        if (func != "")
        {
            auto it = s_funHash.find(func);
            if (it != s_funHash.end())
            {
                switch (it->second)
                {
                case GET_ACTION:
                {
                    if (args.size() != 0)
                    {
                        std::cout << "Bad number of arguments for getAction. getAction takes no argument" << std::endl;
                        break;
                    }
                    return std::make_shared<const Action>(GET_ACTION);
                }
                case PUSH_ACTION:
                {
                    size_t id = 0;
                    std::stringstream ss;
                    if (args.size() == 1)
                    {
                        //check if args[0] can be associated to action : {ruleName : args[0], path : []}
                        auto humanActions = N_DarkLogic::DarkLogic::getHumanActions();
                        for (const auto& action : humanActions)
                        {
                            if ((action.ruleName() == args[0]) && (action.path().size() ==0))
                            {
                                return std::make_shared<const Action>(PUSH_ACTION, action.id());
                            }
                        }

                        //check if args[0] can be associated to an action id
                        auto isNumber = [](const std::string& str)
                        {
                            for (auto c : str)
                            {
                                if (c < '0' || c>'9')
                                {
                                    return false;
                                }
                            }
                            return true;
                        };

                        if (isNumber(args[0]))
                        {
                            ss << args[0];
                            ss >> id;
                            auto actions = N_DarkLogic::DarkLogic::getActions();
                            for (const auto& actionId : actions)
                            {
                                if (id == actionId)
                                {
                                    return std::make_shared<const Action>(PUSH_ACTION, id);
                                }
                            }
                        }                        
                        std::cout << args[0] << " is not a valid action" << std::endl;
                    }
                    else if (args.size() == 2)
                    {
                        std::string ruleName;
                        std::vector<N_DarkLogic::Action::Id> path;
                        ss << args[0];
                        ss >> ruleName;
                        ss.clear();
                        arg = args[1];
                        std::string crtIdStr;
                        N_DarkLogic::Action::Id id = 0;
                        size_t nbBracket = 0;
                        for (size_t k = 0; k < arg.size(); k++)
                        {
                            if (arg[k] == '[')
                            {
                                nbBracket++;
                            }
                            else if (arg[k] == ']')
                            {
                                nbBracket--;
                                if (nbBracket == 0)
                                {
                                    if (crtIdStr != "")
                                    {
                                        ss << crtIdStr;
                                        ss >> id;
                                        ss.clear();
                                        path.push_back(id);
                                        crtIdStr = "";
                                    }
                                }
                            }
                            else if ((arg[k] == ',') && (nbBracket == 1))
                            {
                                if (crtIdStr != "")
                                {
                                    ss << crtIdStr;
                                    ss >> id;
                                    ss.clear();
                                    path.push_back(id);
                                    crtIdStr = "";
                                }                                
                            }
                            else if (arg[k] == ' ')
                            {
                                continue;
                            }
                            else
                            {
                                crtIdStr += arg[k];
                            }
                        }
                        auto actions = N_DarkLogic::DarkLogic::getHumanActions();
                        for (const auto& action : actions)
                        {
                            if ((ruleName == action.ruleName()) && (path == action.path()))
                            {
                                return std::make_shared<const Action>(PUSH_ACTION, action.id());
                            }
                        }
                        std::cout << "{ruleName : "<< args[0] << ", path :"<< args[1] <<"} is not a valid action" << std::endl;
                    }
                    else
                    {
                        std::cout << "Bad number of arguments for pushAction. pushAction takes one or two arguments" << std::endl;
                    }
                    
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
    }
    return nullptr;
}
