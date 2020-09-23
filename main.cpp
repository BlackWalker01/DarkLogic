#include "MainDarkLogic/logicgame.h"
#include "Tests/test.h"
#include <iostream>

int main(int argc, char** argv)
{
    //parse arguments
    std::string type = "game";
    std::string testName = "";
    bool isAuto = false;
    std::string argName = "";
    bool isArgName = false;
    std::unordered_map<std::string, std::string> argNames = { {"type", "type"}, {"testName", "testName"}, {"auto", "auto"} };
    std::unordered_map<std::string, bool> strToBool = { {"false", false}, {"False", false}, {"FALSE", false}, 
                                                        {"true", true}, {"True", true}, {"TRUE", true} };
    for (int k = 1; k < argc; k++)
    {
        std::string crtArg = argv[k];
        isArgName = (crtArg.substr(0, 2) == "--");
        if (isArgName)
        {
            argName = crtArg.substr(2);
            auto it = argNames.find(argName);
            if (it == argNames.end())
            {
                std::cout << "LogicGame.exe expects \"type\", \"testName\" or \"auto\" as argument name but got : " << argName << std::endl;
                return 1;
            }
        }
        else
        {
            if (argName == "")
            {
                continue;
            }
            else
            {
                if (argName == "type")
                {
                    type = crtArg;
                }
                else if (argName == "testName")
                {
                    testName = crtArg;
                }
                else if (argName == "auto")
                {
                    auto it = strToBool.find(crtArg);
                    if (it != strToBool.end())
                    {
                        isAuto = it->second;
                    }
                    else
                    {
                        std::cout << "\"auto\" argument is boolean type but got : " << crtArg << std::endl;
                        return 1;
                    }
                }
            }
        }
    }

    //start application
    if (type == "game")
    {
        LogicGame game(isAuto);
        game.start();
    }
    else if (type == "test")
    {
        if (Test::init(testName))
        {
            return Test::run();
        }
        else
        {
            std::cout << "There is no '" << testName << "' test" << std::endl;
            return 1;
        }
    }
    else
    {
        std::cout << "--type only expects \"game\" or \"test\" as argument but got : " << type << std::endl;
    }
    return 0;
}
