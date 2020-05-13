#include "MainDarkLogic/logicgame.h"
#include "Tests/test.h"
#include <iostream>

int main(int argc, char** argv)
{
    switch (argc)
    {
    case 1:
    {
        LogicGame game;
        game.start();
        break;
    }
    case 2:
    {
        std::string arg = argv[1];
        if (Test::init(arg))
        {
            return Test::run();
        }
        else
        {
            std::cout << "There is no '"<<arg<<"' test" << std::endl;
            return -1;
        }
    }
    default:
    {
        std::cout << "LogicGame only expects one argument but got " <<(argc  - 1)<< std::endl;
        return -1;
    }
    }
    
    return 0;
}
