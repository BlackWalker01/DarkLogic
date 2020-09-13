import MainDarkLogic.darklogic as DarkLogic
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
    VAL_MAX_NODE = 101

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
        for k in range(self._maxInstanceIdx):
            self._slaveThreads.append(AIThread(k, self._ai, self))

    def computeActions(self):
        for slave in self._slaveThreads:
            slave.computeActions()

    def updateLogic(self, actionId):
        for slave in self._slaveThreads:
            slave.updateLogic(actionId)

    def mustStop(self, threadIdx):
        return self._slaveThreads[threadIdx].mustStop()

    def start_(self):
        if not self.is_alive():
            self.start()
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
                self._slaveThreads.append(AIThread(k, self._ai, self))

    def _start(self):
        # Dispatch actions between slave threads
        nbActions = 0
        foundDemo = False
        actions = DarkLogic.getActions(0)
        for action in actions:
            self._ai.pushCrtAction([action])
            val = self._ai.valueOfActions([action])
            if val == 0:
                foundDemo = True
                break
            elif val == MasterAIThread.VAL_MAX_NODE:
                continue
            else:
                DarkLogic.apply(0, action)
                subActions = DarkLogic.getActions(0)
                for subAction in subActions:
                    threadIdx = nbActions % len(self._slaveThreads)
                    self._ai.pushCrtAction([action, subAction], threadIdx)
                    subVal = self._ai.valueOfActions([action, subAction])
                    if subVal == 0:
                        foundDemo = True
                        break
                    elif subVal == MasterAIThread.VAL_MAX_NODE:
                        continue
                    else:
                        self._slaveThreads[threadIdx].pushAction(action, subAction)
                        nbActions += 1
                DarkLogic.unapply(0)
                DarkLogic.getActions(0)
                if foundDemo:
                    break

        if not foundDemo:
            # start slave threads
            if nbActions > len(self._slaveThreads):
                for slaveThread in self._slaveThreads:
                    slaveThread._start()
                    self._threadAlive[slaveThread.instanceId()] = slaveThread.instanceId()
            else:
                for k in range(nbActions):
                    self._slaveThreads[k]._start()
                    self._threadAlive[self._slaveThreads[k].instanceId()] = self._slaveThreads[k].instanceId()
        else:
            self._ai.stopFromMasterThread()

    def _stop(self):
        for threadIdx in self._threadAlive.values():
            if not self._slaveThreads[threadIdx].mustStop():
                self._slaveThreads[threadIdx]._stop()

    def _stopFromThread(self, threadIdx):
        # remove threadIdx thread in "living threads"
        del self._threadAlive[threadIdx]

        # if no thread is alive, warn AI
        if len(self._threadAlive) == 0:
            self._ai.stopFromMasterThread()
        # check if the other threads know they must stop
        else:
            self._stop()

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
            if len(self._threadAlive) == 0:
                self._eventQueue.clear()
                break

    def _pushEvent(self, threadIdx_, type_):
        self._mutex.acquire()
        self._eventQueue.append(Event(threadIdx_, type_))
        self._mutex.release()
        self._hasEvents = True
        with self._condition_var:
            self._condition_var.notify_all()
