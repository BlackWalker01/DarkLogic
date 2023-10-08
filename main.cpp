#include "MainDarkLogic/logicgame.h"
#include "Tests/test.h"
#include <iostream>

int main(int argc, char** argv)
{
    //parse arguments
    std::string type = "game";
    std::string argName = "";
    bool isArgName = false;
    std::unordered_map<std::string, bool> strToBool = { {"false", false}, {"False", false}, {"FALSE", false}, 
                                                        {"true", true}, {"True", true}, {"TRUE", true} };
    std::unordered_map<std::string, ConfigType> strToConfigType =
    { { "type", ConfigType::TYPE },
    {"Type", ConfigType::TYPE},
    {"TYPE", ConfigType::TYPE},

    { "testname", ConfigType::TESTNAME },
    {"testName", ConfigType::TESTNAME},
    {"TESTNAME", ConfigType::TESTNAME},

    {"content", ConfigType::CONTENT },
    {"Content", ConfigType::CONTENT},
    {"CONTENT", ConfigType::CONTENT},

    { "auto", ConfigType::AUTO },
    {"Auto", ConfigType::AUTO},
    {"AUTO", ConfigType::AUTO},

    { "player", ConfigType::PLAYER },
    {"Player", ConfigType::PLAYER},
    {"PLAYER", ConfigType::PLAYER},

    { "timeout", ConfigType::TIMEOUT },
    {"Timeout", ConfigType::TIMEOUT},
    {"TIMEOUT", ConfigType::TIMEOUT} };
    std::unordered_map<ConfigType, std::string> config;
    ConfigType configType = ConfigType::TYPE;
    for (int k = 1; k < argc; k++)
    {
        std::string crtArg = argv[k];
        isArgName = (crtArg.substr(0, 2) == "--");
        if (isArgName)
        {
            argName = crtArg.substr(2);
            auto it = strToConfigType.find(argName);
            if (it == strToConfigType.end())
            {
                std::string errorMsg = "LogicGame.exe expects ";
                for (auto strToConf : strToConfigType)
                {
                    errorMsg += "\"";
                    errorMsg += strToConf.first;
                    errorMsg += "\", ";
                }
                errorMsg.pop_back();
                errorMsg.pop_back();
                errorMsg += " as argument name but got : " + argName;
                std::cout << errorMsg << std::endl;
                return 1;
            }
            else
            {
                configType = strToConfigType[argName];
            }
        }
        else
        {
            config[configType] = crtArg;
            /*
            switch (configType)
            {
            case ConfigType::TYPE:

            case ConfigType::TESTNAME:
            case ConfigType::AUTO:
            case ConfigType::PLAYER:
            case ConfigType::TIMEOUT:
            }

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
            }*/
        }
    }

    if (!config.contains(ConfigType::TYPE))
    {
        config[ConfigType::TYPE] = "game";
    }

    if (!config.contains(ConfigType::AUTO))
    {
        config[ConfigType::AUTO] = "false";
    }

    //start application
    if (config[ConfigType::TYPE] == "game")
    {
        LogicGame game(config);
        game.start();
    }
    else if (config[ConfigType::TYPE] == "test")
    {
        if (Test::init(config))
        {
            return Test::run();
        }
        else
        {
            return 1;
        }
    }
    else
    {
        std::cout << "--type only expects \"game\" or \"test\" as argument but got : " << type << std::endl;
        return 1;
    }
    return 0;
}
