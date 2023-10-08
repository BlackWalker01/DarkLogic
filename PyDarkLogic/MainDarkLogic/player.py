from threading import Thread, Condition, Lock
from AI.event import Event


def _start(player, threadIdx):
    player._start()


def _stop(player, threadIdx):
    player._stop()

def _pushAction(player, action):
    player._pushEvent(action, Event.EventEnum.STOP_THREAD)

class Player(Thread):
    _switcher = {Event.EventEnum.START: _start,
                 Event.EventEnum.STOP: _stop}
    def __init__(self, logicGame, name):
        Thread.__init__(self)

        self._name = name
        self._elo = 1200
        self._logicGame = logicGame

        # start/stop thread
        self._hasThreadStarted = False
        self._mustThreadStop = False
        self._mutexThreadStop = Lock()

        # handling events
        self._hasThreadEvents = False
        self._mutexThread = Lock()
        self._eventThreadQueue = []
        self._condition_thread_var = Condition(self._mutexThread)

    def name(self):
        return self._name

    def hasThreadStarted(self):
        return self._hasThreadStarted

    def mustThreadStop(self, action=None):
        self._mutexThreadStop.acquire()
        ret = self._mustThreadStop
        self._mutexThreadStop.release()
        return ret

    def start_(self):
        if not self.is_alive():
            self.start()
        self._pushThreadEvent(0, Event.EventEnum.START)

    def stop_(self):
        self._logicGame.pushEvent(Event.EventEnum.STOP)

    def play(self):
        raise Exception("play method should be overidden")

    def meditate(self):
        pass

    def setTheoremInfo(self):
        pass

    def elo(self):
        return self._elo

    def setSecondTimeout(self, secondTimeout):
        pass

    def run(self):
        while True:
            while not self._hasThreadEvents:
                with self._condition_thread_var:
                    self._condition_thread_var.wait()
            while len(self._eventThreadQueue):
                ev = self._eventThreadQueue[0]
                Player._switcher[ev.type()](self, ev.threadIdx())
                self._eventThreadQueue.pop(0)
            self._hasThreadEvents = False
            if self.mustThreadStop():
                self._eventThreadQueue.clear()
                break

    def setElo(self, elo):
        exElo = self._elo
        self._elo = elo
        comment = ""
        if elo >= exElo:
            comment = "(+" + str(elo - exElo) + ")"
        else:
            comment = "(-" + str(exElo - elo) + ")"
        print(self._name + "'s elo is " + str(self._elo) + " " + comment)

    def _pushThreadEvent(self, threadIdx_, type_):
        self._mutexThread.acquire()
        self._eventThreadQueue.append(Event(threadIdx_, type_))
        self._mutexThread.release()
        self._hasThreadEvents = True
        with self._condition_thread_var:
            self._condition_thread_var.notify_all()

    def _stop(self):
        self._mutexThreadStop.acquire()
        self._mustThreadStop = True
        self._mutexThreadStop.release()

    def _start(self):
        self._hasThreadStarted = True
        action = self.play()
        self._logicGame.pushEvent(action)

        # inform logicGame that this thread has finished
        self.stop_()