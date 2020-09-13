import os

path = os.getcwd()
import sys

sys.path.append(path + "\..\..\Lib")
from DarkLogic import DarkLogic
from threading import Thread, Condition, Lock
from AI.event import Event
from AI.dbnode import DbNode


# friend method
def _start(aiThread):
    aiThread.start_()


# friend method
def _stop(aiThread):
    aiThread.stop_()


class AIThread(Thread):
    _switcher = {Event.EventEnum.START: _start,
                 Event.EventEnum.STOP: _stop}

    def __init__(self, instanceId, ai, master):
        Thread.__init__(self)

        self._instanceId = instanceId
        self._ai = ai
        self._master = master
        self._crtActions = DbNode()

        # start/stop thread
        self._hasStarted = False
        self._mustStop = False
        self._mutexStop = Lock()

        # handling eventss
        self._hasEvents = False
        self._mutex = Lock()
        self._condition_var = Condition(self._mutex)
        self._eventQueue = []

    def hasStarted(self):
        return self._hasStarted

    def pushAction(self, parent, action):
        idx = self._crtActions.push(parent, action)
        valSon = self._ai.realValueOfActions([parent, action])
        self._crtActions.updateValue(idx, valSon)

    def mustStop(self, action=None):
        self._mutexStop.acquire()
        ret = self._mustStop
        self._mutexStop.release()
        return ret

    def computeActions(self):
        DarkLogic.getActions(self._instanceId)

    def updateLogic(self, actionId):
        DarkLogic.apply(self._instanceId, actionId)
        self._crtActions.clear()

    def instanceId(self):
        return self._instanceId

    def start_(self):
        self._hasStarted = True
        DarkLogic.getActions(self._instanceId)

        # compute value of given nodes
        while not self.mustStop():
            self._ai.explore(self._crtActions, self._instanceId)

        # inform master that this thread has finished
        self.stop_()

    def stop_(self):
        self._hasStarted = False
        self._master.stopFromThread_(self._instanceId)

    def run(self):
        while True:
            while not self._hasEvents:
                with self._condition_var:
                    self._condition_var.wait()
            if len(self._eventQueue):
                # Consuming event
                AIThread._switcher[self._eventQueue[0].type()](self)
                self._eventQueue.pop(0)
            self._hasEvents = False
            if self.mustStop():
                self._eventQueue.clear()
                break

    def _pushEvent(self, threadIdx_, type_):
        self._mutex.acquire()
        self._eventQueue.append(Event(threadIdx_, type_))
        self._mutex.release()
        self._hasEvents = True
        with self._condition_var:
            self._condition_var.notify_all()

    def _stop(self):
        self._mutexStop.acquire()
        self._mustStop = True
        self._mutexStop.release()

    def _start(self):
        # print("start ai thread "+str(self._instanceId))
        self._mustStop = False
        if not self.is_alive():
            self.start()
        self._pushEvent(0, Event.EventEnum.START)
