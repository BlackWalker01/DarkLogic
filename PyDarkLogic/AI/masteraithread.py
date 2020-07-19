import os

path = os.getcwd()
import sys

sys.path.append(path + "\..\..\Lib")
from DarkLogic import DarkLogic
from threading import Thread, Condition, Lock
from AI.event import Event
from AI.aithread import AIThread


def _start(masterAIThread, threadIdx):
    masterAIThread._start()


def _stop(masterAIThread, threadIdx):
    masterAIThread._stop()


def _stopFromThread(masterAIThread, threadIdx):
    masterAIThread._stopFromThread(threadIdx)


class MasterAIThread(Thread):
    _switcher = {Event.EventEnum.START: _start,
                 Event.EventEnum.STOP: _stop,
                 Event.EventEnum.STOP_THREAD: _stopFromThread}

    def __init__(self, maxInstanceIdx, ai):
        Thread.__init__(self)
        if maxInstanceIdx < 1:
            raise Exception("AI needs at least one instance!")
        self._ai = ai
        self._maxInstanceIdx = maxInstanceIdx
        self._hasEvents = False
        self._mutex = Lock()
        self._eventQueue = []
        self._condition_var = Condition(self._mutex)
        self._slaveThreads = []
        self._threadAlive = {}

        # start thread
        self.start()

    def computeActions(self):
        for slave in self._slaveThreads:
            slave.computeActions()

    def updateLogic(self, actionId):
        for slave in self._slaveThreads:
            slave.updateLogic(actionId)

    def mustStop(self, threadIdx):
        return self._slaveThreads[threadIdx].mustStop()

    def start_(self):
        self._pushEvent(0, Event.EventEnum.START)

    def stop_(self):
        self._pushEvent(0, Event.EventEnum.STOP)

    def stopFromThread_(self, threadIdx):
        self._pushEvent(threadIdx, Event.EventEnum.STOP_THREAD)

    def stopThread(self, threadIdx):
        self._slaveThreads[threadIdx]._stop()

    def init(self):
        if len(self._slaveThreads) == 0:
            for k in range(self._maxInstanceIdx):
                self._slaveThreads.append(AIThread(k, self._ai))

    def _start(self):
        # Dispatch actions between slave threads
        actions = DarkLogic.getActions()
        for k in range(len(actions)):
            self._slaveThreads[k % len(self._slaveThreads)].pushAction(actions[k])

        # start slave threads
        if len(actions) > len(self._slaveThreads):
            for slaveThread in self._slaveThreads:
                slaveThread._start()
                self._threadAlive[slaveThread.instanceId()] = slaveThread.instanceId()
        else:
            for k in range(len(actions)):
                self._slaveThreads[k]._start()
                self._threadAlive[self._slaveThreads[k].instanceId()] = self._slaveThreads[k].instanceId()

    def _stop(self):
        for slaveThread in self._slaveThreads:
            slaveThread._stop()

    def _stopFromThread(self, threadIdx):
        # remove threadIdx thread in "living threads"
        del self._threadAlive[threadIdx]

        # if no thread is alive, warn AI
        if len(self._threadAlive) == 0:
            self._ai.stopFromMasterThread()
        # check if the other threads know they must stop
        else:
            for threadIdx in self._threadAlive.values():
                if not self._slaveThreads[threadIdx].mustStop():
                    self._slaveThreads[threadIdx]._stop()

    def run(self):
        while True:
            while not self._hasEvents:
                with self._condition_var:
                    self._condition_var.wait()
            while len(self._eventQueue):
                ev = self._eventQueue[0]
                self._switcher[ev.type()](self, ev.threadIdx())
                self._eventQueue.pop(0)
            self._hasEvents = False

    def _pushEvent(self, threadIdx_, type_):
        self._mutex.acquire()
        self._eventQueue.append(Event(threadIdx_, type_))
        self._mutex.release()
        self._hasEvents = True
        with self._condition_var:
            self._condition_var.notify_all()