#ifndef LOGICGAME_UTILS_H
#define LOGICGAME_UTILS_H
#include "DarkLogic/Utils/action.h"

using Id = N_DarkLogic::Action::Id;

enum class ConfigType
{
    TYPE,
    TESTNAME,
    CONTENT,
    AUTO,
    PLAYER,
    TIMEOUT
};
#endif // !LOGICGAME_UTILS_H