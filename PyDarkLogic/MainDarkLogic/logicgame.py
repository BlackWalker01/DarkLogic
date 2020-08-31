import MainDarkLogic.darklogic as DarkLogic
from MainDarkLogic.action import Action
from enum import Enum
from .enumfun import EnumFun

import multiprocessing
from MainDarkLogic.dbtheorem import DbTheorem

class Mode(Enum):
    NoMode = 0
    HumanMode = 1
    AIMode = 2
    DeepAIMode = 3


def _createHuman(game):
    game._createHuman()


def _createAI(game):
    game._createAI()


def _createDeepAI(game):
    game._createDeepAI()


def _pushAction(game, action):
    game._pushAction(action)


def _popAction(game, action):
    if DarkLogic.hasAlreadyPlayed():
        DarkLogic.unapply()
        print("Current theorem is")
        DarkLogic.printTheorem()
        return True
    else:
        print("Cannot pop action")
        return False


def printActions():
    actions = DarkLogic.getHumanActions()
    for action in actions:
        print(action.toString())


def _printActions(game, action):
    printActions()


class LogicGame:
    _modeHash = {"human": Mode.HumanMode,
                 "Human": Mode.HumanMode,
                 "HUMAN": Mode.HumanMode,

                 "ai": Mode.AIMode,
                 "AI": Mode.AIMode,

                 "deepai": Mode.DeepAIMode,
                 "deepAi": Mode.DeepAIMode,
                 "deepAI": Mode.DeepAIMode,
                 "DeepAi": Mode.DeepAIMode,
                 "DeepAI": Mode.DeepAIMode,
                 "DEEPAI": Mode.DeepAIMode}

    _modeSwitcher = {Mode.HumanMode: _createHuman,
                     Mode.AIMode: _createAI,
                     Mode.DeepAIMode: _createDeepAI
                     }
    _actionSwitcher = {EnumFun.GET_ACTION: _printActions,
                       EnumFun.PUSH_ACTION: _pushAction,
                       EnumFun.POP_ACTION: _popAction}
    _AI_TIMEOUT = 20  # seconds

    def __init__(self, isAuto=False):
        self._mode = Mode.NoMode
        self._player = None
        self._isAuto = isAuto
        self._eloThm = 1500
        self._nbGames = 150
        self._dbThm = DbTheorem()

    def start(self):
        print("Welcome in LogicGame (v1.3.0)!")

        # create player and init Logic
        self._askPlayer()

        while True:
            self._createTheorem()

            # Start game
            self._game()

    def _askPlayer(self):
        ok = False
        while not ok:
            modeStr = input("Choose the mode (Human/AI/DeepAI):\n")
            mode = LogicGame._modeHash.get(modeStr)
            if mode:
                LogicGame._modeSwitcher[mode](self)
                ok = True
            else:
                print("Unknown mode " + modeStr)

    def _createHuman(self):
        from Human.human import Human
        print("Human Mode")
        self._mode = Mode.HumanMode
        DarkLogic.init(0)
        self._player = Human()

    def _createAI(self):
        from AI.ai import AI
        print("AI Mode")
        self._mode = Mode.AIMode
        nbInstances = multiprocessing.cpu_count()
        # nbInstances = 1
        DarkLogic.init(nbInstances)
        self._player = AI(0, nbInstances, LogicGame._AI_TIMEOUT)

    def _createDeepAI(self):
        from AI.deepai import DeepAI
        print("Deep AI Mode")
        self._mode = Mode.DeepAIMode
        # nbInstances = multiprocessing.cpu_count()
        nbInstances = 1
        DarkLogic.init(nbInstances)
        self._player = DeepAI(0, nbInstances, LogicGame._AI_TIMEOUT)

    def _createTheorem(self):
        # ask user to create theorem
        thName = ""
        thStr = ""
        ok = False
        while not ok:
            if not self._isAuto:
                print("Create a new theorem to start a session")
                thName = input("Theorem name:\n")
                if thName == "":
                    print("Error in theorem name")
                    continue
                thStr = input("Theorem content:\n")
                if thStr == "":
                    print("Error in theorem content")
                    continue
                ok = DarkLogic.makeTheorem(thName, thStr)
                if ok and self._mode == Mode.HumanMode:
                    print("-> getAction() : to print all possible actions")
                    print("-> pushAction(id) : to make action identified by id")
                    print("-> pushAction(ruleName, path) : to make action identified by ruleName (name of the rule to "
                          "apply) and path "
                          " (list of indexes [id1, id2, ..., idn]) in theorem ")
                    print("-> popAction : to cancel the latest action")
            else:
                thm = self._dbThm.getRandomTheorem()
                self._eloThm = thm.elo()
                print(thm.name()+" theorem :'"+thm.content()+"' has been chosen for this game")
                ok = DarkLogic.makeTheorem(thm.name(), thm.content())
        self._player.setTheoremInfo()

    def _pushActionStr(self, action):
        id = int(action)
        actions = DarkLogic.getHumanActions()
        foundAction = False
        ruleName = ""
        for action in actions:
            if id == action.id():
                foundAction = True
                ruleName = action.ruleName()
                break
        if foundAction:
            DarkLogic.apply(id)
            print(self._player.name() + " uses " + ruleName)
            print("Current theorem is")
            DarkLogic.printTheorem()
        else:
            print(action + " is not a valid action")
        return foundAction

    def _pushAction(self, action):
        DarkLogic.getActions()
        print(self._player.name() + " played action with id " + str(action.id()))
        DarkLogic.apply(action.id())
        DarkLogic.printTheorem()

    def _game(self):
        nbAttempts = 0
        maxNbAttempts = 10
        hasWon = False
        self._nbGames += 1
        print("Game n°"+str(self._nbGames))
        while not DarkLogic.isOver():
            print("Attempt n°" + str(nbAttempts + 1) + "/" + str(maxNbAttempts))
            action = self._player.play()
            LogicGame._actionSwitcher[action.fun()](self, action)
            print("____________________________________________________________________________")
            if action.fun() == EnumFun.PUSH_ACTION:
                nbAttempts += 1
                if nbAttempts == maxNbAttempts:
                    break

        if nbAttempts == maxNbAttempts:
            print(self._player.name() + " lost! Too much attempts!")
            exElo = self._player.elo()
            newElo = round(exElo - 30 / (1 + 10 ** ((self._eloThm - exElo) / 400)))
            self._player.setElo(newElo)
        elif DarkLogic.hasAlreadyPlayed():
            if DarkLogic.isDemonstrated():
                print(self._player.name() + " won! " + self._player.name() + " finished the demonstration!")
                hasWon = True
            elif DarkLogic.isAlreadyPlayed():
                print(self._player.name() + " lost! Repetition of theorem!")
            elif DarkLogic.isEvaluated():
                print(
                    self._player.name() + " lost! Cannot (\"back-\")demonstrate that a theorem is false with implications")
            elif not DarkLogic.canBeDemonstrated():
                print(self._player.name() + " lost! This theorem cannot be demonstrated! " +
                      "It can be true or false according to the values of its variables")

            # update player's elo
            W = 1 if hasWon else 0
            exElo = self._player.elo()
            newElo = round(exElo + 30*(W - 1/(1 + 10**((self._eloThm - exElo) / 400))))
            self._player.setElo(newElo)
        else:
            if DarkLogic.isDemonstrated():
                print("The demonstration is already finished!")
            elif not DarkLogic.canBeDemonstrated():
                print("This theorem cannot be demonstrated! " +
                      "It can be true or false according to the values of its variables")

        # clear Logic State
        DarkLogic.clearAll()

        # let player meditates the last game
        self._player.meditate()
