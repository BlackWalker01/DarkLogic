import MainDarkLogic.darklogic as DarkLogic
from MainDarkLogic.player import Player
from AI.masteraithread import MasterAIThread
from AI.node import Node
from threading import Thread, Condition, Lock
import time
from AI.event import Event
from MainDarkLogic.action import Action
from MainDarkLogic.enumfun import EnumFun


class AI(Player):
    def __init__(self, type_, maxInstanceIdx, secondTimeout):
        super().__init__("AI")
        self._type = type_
        self._secondTimeout = secondTimeout
        self._maxInstanceIdx = maxInstanceIdx
        self._masterThread = None
        # self._masterThread = MasterAIThread(maxInstanceIdx, self)
        self._crtNode = Node(ai=self)
        self._hasEvents = False
        self._eventQueue = []
        self._mutex = Lock()
        self._condition_var = Condition(self._mutex)
        self._elo = 1565

    def play(self):
        # start chrono
        start = time.perf_counter()
        newNode = self._crtNode.getDemoMode()
        nbNode = None
        hasTimedout = False
        if not newNode:
            self._masterThread = MasterAIThread(self._maxInstanceIdx, self)
            self._masterThread.start_()
            while not self._hasEvents:
                with self._condition_var:
                    hasTimedout = not self._condition_var.wait(self._secondTimeout)
                    if hasTimedout:
                        break

            if hasTimedout:
                self._masterThread.stop_()
            # waiting for threads to stop
            while not self._hasEvents:
                with self._condition_var:
                    self._condition_var.wait()
            if len(self._eventQueue):
                self._eventQueue.pop()
            self._hasEvents = False

            nbNode = self._crtNode.nbNode()
            newNode = self.getBestNode()
            print("[DEBUG] Nb explored nodes: " + str(nbNode))

        else:
            self._masterThread.computeActions()

        self._storeCrtNode()
        self._crtNode = newNode
        self._crtNode.setRoot()
        self._masterThread.updateLogic(self._crtNode.actionId())
        end = time.perf_counter()
        elapsed_seconds = end - start
        if nbNode:
            print("[DEBUG] AI play action " + str(self._crtNode.actionId()) + " with value " +
                  str(self.value()) +
                  " in " + str(elapsed_seconds) + " seconds")
            print("[DEBUG] AI exploration speed: " + str(nbNode / elapsed_seconds) + " nodes/s")

        return Action(EnumFun.PUSH_ACTION, self._crtNode.actionId())

    def pushCrtAction(self, actionId, threadIdx):
        self._crtNode.pushCrtAction(actionId, threadIdx)

    def stopFromMasterThread(self):
        self._pushEvent(Event.EventEnum.STOP)

    def explore(self, actions):
        self._crtNode.exploreStatic(actions)

    def mustStop(self, threadIdx):
        return self._masterThread.mustStop(threadIdx)

    def stopThread(self, threadIdx):
        self._masterThread.stopThread(threadIdx)

    def getMaster(self):
        return self._masterThread

    def type(self):
        return self._type

    def timeout(self):
        return self._secondTimeout

    def _pushEvent(self, type_):
        self._mutex.acquire()
        self._eventQueue.append(Event(0, type_))
        self._mutex.release()
        self._hasEvents = True
        with self._condition_var:
            self._condition_var.notify_all()

    def _storeCrtNode(self):
        pass

    def meditate(self):
        self._crtNode = Node(ai=self)

    def getBestNode(self):
        return self._crtNode.getBestNode()

    def value(self):
        return self._crtNode.value()
