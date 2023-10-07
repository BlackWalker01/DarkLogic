import MainDarkLogic.darklogic as DarkLogic
from MainDarkLogic.action import Action
from enum import Enum
from .enumfun import EnumFun

import multiprocessing
from MainDarkLogic.dbtheorem import DbTheorem
from threading import Thread, Condition, Lock


class Mode(Enum):
    NoMode = 0
    Human = 1
    BasicAI = 2
    EvalAI = 3
    NeuralAI = 4


def _createHuman(game):
    game._createHuman()


def _createBasicAI(game):
    game._createBasicAI()


def _createEvalAI(game):
    game._createEvalAI()


def _createNeuralAI(game):
    game._createNeuralAI()


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
    _modeHash = {"human": Mode.Human,
                 "Human": Mode.Human,
                 "HUMAN": Mode.Human,

                 "basicai": Mode.BasicAI,
                 "BasicAI": Mode.BasicAI,
                 "BASICAI": Mode.BasicAI,

                 "evalai": Mode.EvalAI,
                 "EvalAI": Mode.EvalAI,
                 "EVALAI": Mode.EvalAI,

                 "neuralai": Mode.NeuralAI,
                 "neuralAi": Mode.NeuralAI,
                 "neuralAI": Mode.NeuralAI,
                 "NeuralAi": Mode.NeuralAI,
                 "NeuralAI": Mode.NeuralAI,
                 "NEURALAI": Mode.NeuralAI}

    _modeSwitcher = {Mode.Human: _createHuman,
                     Mode.BasicAI: _createBasicAI,
                     Mode.EvalAI: _createEvalAI,
                     Mode.NeuralAI: _createNeuralAI
                     }
    _actionSwitcher = {EnumFun.GET_ACTION: _printActions,
                       EnumFun.PUSH_ACTION: _pushAction,
                       EnumFun.POP_ACTION: _popAction}
    _AI_TIMEOUT = 300  # seconds

    def __init__(self, isAuto=False):
        self._mode = Mode.NoMode
        self._player = None
        self._isAuto = isAuto
        self._eloThm = 1500
        self._nbGames = 100
        self._dbThm = DbTheorem()
        self._hasEvents = False
        self._mutex = Lock()
        self._eventQueue = []
        self._condition_var = Condition(self._mutex)

    def start(self):
        print("Welcome in LogicGame "+DarkLogic.version()+"!")

        # create player and init Logic
        self._askPlayer()

        while True:
            self._createTheorem()

            # Start game
            self._game()

    def _askPlayer(self):
        ok = False
        while not ok:
            modeStr = input("Choose the mode (Human/BasicAI/EvalAI/NeuralAI):\n")
            mode = LogicGame._modeHash.get(modeStr)
            if mode:
                LogicGame._modeSwitcher[mode](self)
                ok = True
            else:
                print("Unknown mode " + modeStr)

    def _createHuman(self):
        from Human.human import Human
        print("Human Mode")
        self._mode = Mode.Human
        DarkLogic.init(0)
        self._player = Human(self)

    def _createBasicAI(self):
        from AI.ai import AI
        print("Basic AI Mode")
        self._mode = Mode.BasicAI
        nbInstances = multiprocessing.cpu_count()
        # nbInstances = 1
        DarkLogic.init(nbInstances)
        self._player = AI(self, nbInstances, LogicGame._AI_TIMEOUT)

    def _createEvalAI(self):
        from AI.evalai import EvalAI
        print("Eval AI Mode")
        self._mode = Mode.EvalAI
        nbInstances = multiprocessing.cpu_count()
        # nbInstances = 1
        DarkLogic.init(nbInstances)
        self._player = EvalAI(self, nbInstances, LogicGame._AI_TIMEOUT)

    def _createNeuralAI(self):
        from AI.neuralai import NeuralAI
        print("Neural AI Mode")
        self._mode = Mode.NeuralAI
        # nbInstances = multiprocessing.cpu_count()
        nbInstances = 1
        DarkLogic.init(nbInstances)
        self._player = NeuralAI(self, nbInstances, LogicGame._AI_TIMEOUT, isDeterminist = True)

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
                if ok and self._mode == Mode.Human:
                    print("-> getAction() : to print all possible actions")
                    print("-> pushAction(id) : to make action identified by id")
                    print("-> pushAction(ruleName, path) : to make action identified by ruleName (name of the rule to "
                          "apply) and path "
                          " (list of indexes [id1, id2, ..., idn]) in theorem ")
                    print("-> popAction : to cancel the latest action")
            else:
                thm = self._dbThm.getRandomTheorem(self._player.elo())
                self._eloThm = thm.elo()
                print(thm.name() + " theorem :'" + thm.content() + "' has been chosen for this game")
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
        hasWon = False
        self._nbGames += 1
        print("Game n°" + str(self._nbGames))
        import time
        remain_time = LogicGame._AI_TIMEOUT
        action = None

        while not DarkLogic.isOver():
            # start chrono
            start = time.perf_counter()
            print("Attempt n°" + str(nbAttempts + 1) + ", it remains "+str(remain_time)+" seconds")
            self._player.setSecondTimeout(remain_time)
            self._player.start_()
            # wait for player to make its move
            while not self._hasEvents:
                with self._condition_var:
                    hasTimedout = not self._condition_var.wait(remain_time)
                    if hasTimedout:
                        break

            if hasTimedout:
                """ self._player.stop_()"""
                # waiting for threads to stop
                while not self._hasEvents:
                    with self._condition_var:
                        self._condition_var.wait()
            if len(self._eventQueue):
                action = self._eventQueue[0]
                self._eventQueue.pop()

            self._hasEvents = False
            self._eventQueue.clear()

            end = time.perf_counter()
            elapsed_seconds = end - start
            remain_time -= elapsed_seconds

            if hasTimedout or action == None or not isinstance(action, Action):
                print("____________________________________________________________________________")
                break

            LogicGame._actionSwitcher[action.fun()](self, action)
            print("____________________________________________________________________________")
            if action.fun() == EnumFun.PUSH_ACTION:
                nbAttempts += 1

        if DarkLogic.hasAlreadyPlayed():
            if DarkLogic.isDemonstrated():
                print(self._player.name() + " won! " + self._player.name() + " finished the demonstration!")
                hasWon = True
            elif DarkLogic.isAlreadyPlayed():
                print(self._player.name() + " lost! Repetition of theorem!")
            elif DarkLogic.isEvaluated():
                print(self._player.name() + "lost! Cannot (\"back-\")demonstrate that a theorem is false with "
                                            "implications")
            elif not DarkLogic.canBeDemonstrated():
                print(self._player.name() + " lost! This theorem cannot be demonstrated! " +
                      "It can be true or false according to the values of its variables")
            elif hasTimedout:
                print(self._player.name() + " lost! Time out!")

        else:
            if DarkLogic.isDemonstrated():
                print("The demonstration is already finished!")
            elif not DarkLogic.canBeDemonstrated():
                print("This theorem cannot be demonstrated! " +
                      "It can be true or false according to the values of its variables")
            elif hasTimedout:
                print(self._player.name() + " lost! Time out!")

        # update player's elo
        W = 1 if hasWon else 0
        exElo = self._player.elo()
        newElo = round(exElo + 30 * (W - 1 / (1 + 10 ** ((self._eloThm - exElo) / 400))))
        self._player.setElo(newElo)

        # clear Logic State
        DarkLogic.clearAll()

        # let player meditates the last game
        self._player.meditate()

    def pushEvent(self, action):
        self._mutex.acquire()
        self._eventQueue.append(action)
        self._mutex.release()
        self._hasEvents = True
        with self._condition_var:
            self._condition_var.notify_all()
