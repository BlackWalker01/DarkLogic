import os

path = os.getcwd()
import sys

sys.path.append(path + "\..\MainDarkLogic")
from MainDarkLogic.player import Player

sys.path.append(path + "\..\..\Lib")
from DarkLogic import DarkLogic
from MainDarkLogic.enumfun import EnumFun
from MainDarkLogic.action import Action


def _getAction(args):
    if len(args) != 0:
        print("Bad number of arguments for getAction. getAction takes no argument")
        return None
    return Action(EnumFun.GET_ACTION)


def _pushAction(args):
    id = 0
    if len(args) == 1:
        # check if args[0] can be associated to action {ruleName: args[0], path:[]}
        humanActions = DarkLogic.getHumanActions()
        for action in humanActions:
            if (action.ruleName() == args[0]) and (len(action.path()) == 0):
                return Action(EnumFun.PUSH_ACTION, action.id())

        # check if args[0] can be associated to an action id
        def isNumber(str):
            for c in str:
                if c<'0' or c>'9':
                    return False
            return True
        if isNumber(args[0]):
            id = int(args[0])
            actions = DarkLogic.getActions()
            for actionId in actions:
                if id == actionId:
                    return Action(EnumFun.PUSH_ACTION, id)
        print(args[0] + " is not a valid action")
    elif len(args) == 2:
        ruleName = args[0]
        path = []
        arg = args[1]
        crtIdStr = ""
        id = 0
        nbBracket = 0
        for k in range(len(arg)):
            if arg[k] == '[':
                nbBracket += 1
            elif arg[k] == ']':
                nbBracket -= 1
                if nbBracket == 0 and crtIdStr != "":
                    id = int(crtIdStr)
                    path.append(id)
                    crtIdStr = ""
            elif arg[k] == ' ':
                continue
            else:
                crtIdStr += arg[k]

        actions = DarkLogic.getHumanActions()
        for action in actions:
            if (ruleName == action.ruleName()) and (path == action.path()):
                return Action(EnumFun.PUSH_ACTION, action.id());
        print("{ruleName : " + args[0] + ", path :" + args[1] + "} is not a valid action")
    else:
        print("Bad number of arguments for pushAction. pushAction takes one or two arguments")
    return None


def _popAction(args):
    if len(args) != 0:
        print("Bad number of arguments for popAction. popAction takes no argument")
        return None
    return Action(EnumFun.POP_ACTION)


def _help(args):
    return Action(EnumFun.HELP)


class Human(Player):
    _funHash = {"getaction": EnumFun.GET_ACTION,
                "getAction": EnumFun.GET_ACTION,
                "GetAction": EnumFun.GET_ACTION,
                "GETACTION": EnumFun.GET_ACTION,

                "pushaction": EnumFun.PUSH_ACTION,
                "pushAction": EnumFun.PUSH_ACTION,
                "PushAction": EnumFun.PUSH_ACTION,
                "PUSHACTION": EnumFun.PUSH_ACTION,

                "popaction": EnumFun.POP_ACTION,
                "popAction": EnumFun.POP_ACTION,
                "PopAction": EnumFun.POP_ACTION,
                "POPACTION": EnumFun.POP_ACTION,

                "help": EnumFun.HELP,
                "Help": EnumFun.HELP,
                "HELP": EnumFun.PUSH_ACTION}
    _switcher = {EnumFun.GET_ACTION: _getAction,
                 EnumFun.PUSH_ACTION: _pushAction,
                 EnumFun.POP_ACTION: _popAction,
                 EnumFun.HELP: _help}

    def __init__(self, logicGame):
        super().__init__(logicGame, "You")

    def play(self):
        ok = False
        while not ok:
            cmd = ""
            func = ""
            args = []
            func = ""
            cmd = input("What do you want to do?\n")
            k = 0
            nbPar = 0
            arg = ""
            for k in range(len(cmd)):
                if cmd[k] == '(':
                    nbPar += 1
                elif cmd[k] == ')':
                    if nbPar == 0:
                        print("Error in command: Unexpected ')'")
                        break
                    elif nbPar == 1:
                        if k != len(cmd) - 1:
                            print("Error in command: Unexpected ')' before end of command")
                            break
                        if arg != "":
                            args.append(arg)
                    nbPar -= 1
                elif cmd[k] == '[':
                    while k < len(cmd):
                        arg += cmd[k]
                        if cmd[k] == ']':
                            break
                        k += 1
                elif (cmd[k] == ',') and nbPar == 1:
                    if arg != "":
                        args.append(arg)
                        arg = ""
                    else:
                        print("Error in command: empty argument")
                        break
                elif cmd[k] == ' ':
                    continue
                elif nbPar == 0:
                    func += cmd[k]
                else:
                    arg += cmd[k]

            # call function
            if func != "":
                val = Human._funHash.get(func)
                if val:
                    ret = Human._switcher[val](args)
                    if ret:
                        return ret

            else:
                print("Unknown command " + func)
