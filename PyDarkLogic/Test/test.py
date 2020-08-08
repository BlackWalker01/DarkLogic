import MainDarkLogic.darklogic as DarkLogic
from Database.database import Database
from enum import Enum
import multiprocessing
from threading import Thread, Condition, Lock
import time
from AI.event import Event
from AI.ai import AI


class TestEnum(Enum):
    HumanIdentity = 1
    HumanDoubleNot = 2
    HumanExcludedMiddle = 3
    PerformanceIdentity = 4
    PerformanceDoubleNot = 5
    PerformanceNonContradiction = 6
    Parser = 7


class TestThread(Thread):
    def __init__(self, testObj, thName, thContent, nbAIThread):
        Thread.__init__(self)
        self._testObj = testObj
        self._thName = thName
        self._thContent = thContent
        self._nbAIThread = nbAIThread
        self._hasEvents = False
        # start thread
        self.start()

    def hasEvents(self):
        return self._hasEvents

    def run(self):
        self._testObj.demonstration(self._thName, self._thContent, self._nbAIThread)


"""
testToString = {
    TestEnum.HumanIdentity: "HumanIdentity",
    TestEnum.HumanDoubleNot: "HumanDoubleNot",
    TestEnum.HumanExcludedMiddle: "HumanExcludedMiddle",
    TestEnum.PerformanceIdentity: "PerformanceIdentity",
    TestEnum.PerformanceDoubleNot: "PerformanceDoubleNot",
    TestEnum.PerformanceNonContradiction: "PerformanceNonContradiction"
}"""


def apply(actionStr):
    print("Apply {" + actionStr + "}")
    assert DarkLogic.applyStr(actionStr), "cannot apply  {" + actionStr + "}"
    DarkLogic.printTheorem()
    print("__________________________________________________")


def run(testName):
    if testName in stringToTest:
        try:
            stringToTest[testName]()
        except RuntimeError:
            # print("[ERROR] " + e.)
            print("Test: " + testName + " [FAIL]")
            print("****************************************************************************")
            print("----------------------------------------------------------------------------")
            print("****************************************************************************")
            return 1
        print("Test: " + testName + " [SUCCESS]")
        print("****************************************************************************")
        print("----------------------------------------------------------------------------")
        print("****************************************************************************")
        return 0
    else:
        print("[ERROR] " + testName + " does not exist")
        return 1


def humanIdentity():
    DarkLogic.init(0)
    print("Identity Demonstration")
    DarkLogic.makeTheorem("identity", "a<=>a")
    DarkLogic.printTheorem()
    print("__________________________________________________")

    apply("arr,[0]")
    apply("arr,[0]")
    apply("arr,[1]")
    apply("arr_True,[]")
    assert DarkLogic.isDemonstrated(), "identity theorem was not demonstrated"


def humanDoubleNot():
    DarkLogic.init(0)
    print("DoubleNot Demonstration")
    DarkLogic.makeTheorem("doubleNot", "a<=>!!a")
    DarkLogic.printTheorem()
    print("__________________________________________________")

    apply("FE,[1]")
    apply("arr,[0]")
    apply("arr,[0]")
    apply("arr_True")
    assert DarkLogic.isDemonstrated(), "DoubleNot theorem was not demonstrated"


def humanExcludedMiddle():
    DarkLogic.init(0)
    print("ExcludedMiddle Demonstration")
    DarkLogic.makeTheorem("ExcludedMiddle", "p||!p")
    DarkLogic.printTheorem()
    print("__________________________________________________")

    apply("arr")
    apply("FI!")
    apply("||Ig_Annexe_0")
    apply("FE")
    apply("FI!_Annexe_7")
    apply("||Ig")
    apply("ax_Annexe_1")

    assert DarkLogic.isDemonstrated(), "ExcludedMiddle theorem was not demonstrated"


def performanceIdentity():
    Test.test.performance("identity", "a<=>a", 1.5)


def performanceDoubleNot():
    Test.test.performance("doubleNot", "a<=>!!a", 7.5)  # normally 1.7


def performanceNonContradiction():
    Test.test.performance("non-contradiction", "!(a&&!a)", 0.016)


def parser():
    DarkLogic.init(0)
    db = Database("Test/dbtest.csv")
    dbStates = db.getDatas()
    print("Total number of theorems in database: " + str(len(dbStates)))
    for dbState in dbStates.values():
        thCreated = DarkLogic.makeTheorem(dbState.theoremName(), dbState.theoremContent())
        assert thCreated, "Theorem name: " + dbState.theoremName() + ", " \
                             "content: " + dbState.theoremContent() + " has not been created"
        dlContent = DarkLogic.toStrTheorem()
        assert dlContent == dbState.theoremContent(), "Bad parsing! content in darklogic is '"\
            + dlContent + "', but original was " + dbState.theoremContent()


stringToTest = {
    "HumanIdentity": humanIdentity,
    "HumanDoubleNot": humanDoubleNot,
    "HumanExcludedMiddle": humanExcludedMiddle,
    "PerformanceIdentity": performanceIdentity,
    "PerformanceDoubleNot": performanceDoubleNot,
    "PerformanceNonContradiction": performanceNonContradiction,
    "Parser": parser
}


class Test:
    test = None

    def __init__(self):
        self._thread = None
        self._mutex = Lock()
        self._condition_var = Condition(self._mutex)
        self._eventQueue = []
        self._hasEvents = False
        self._elapsed_seconds = 0.0

    def performance(self, thName, thContent, maxTime):
        print("CheckPerformance Test on " + thName + " theorem : " + thContent)
        nbThreads = multiprocessing.cpu_count()
        realMaxTime = (maxTime * 1.15) * (4.0 / nbThreads)

        # start AI
        self._thread = TestThread(self, thName, thContent, nbThreads)

        with self._condition_var:
            while not self._hasEvents:
                hasTimedOut = not self._condition_var.wait(20)
                if hasTimedOut:
                    break

        if len(self._eventQueue):
            self._eventQueue.pop()
        self._hasEvents = False
        assert DarkLogic.isDemonstrated(), thName + " cannot be demonstrated"
        assert self._elapsed_seconds < realMaxTime, "Demonstration of " + thName + " theorem must be "
        "done in less than " + str(realMaxTime) + " seconds"
        DarkLogic.clearAll()

    def pushEvent(self, type_):
        self._mutex.acquire()
        self._eventQueue.append(Event(0, type_))
        self._mutex.release()
        self._hasEvents = True
        with self._condition_var:
            self._condition_var.notify_all()

    def demonstration(self, name, content, nbThreads):
        print("Test AI on " + name + " theorem with " + str(nbThreads) + " cores")
        DarkLogic.init(nbThreads)
        ai = AI(0, nbThreads, 60)
        assert DarkLogic.makeTheorem(name, content), "cannot make " + name + " theorem"
        DarkLogic.printTheorem()
        start = time.perf_counter()
        while not DarkLogic.isOver():
            action = ai.play()
            DarkLogic.getActions()
            print(ai.name() + " plays action with id " + str(action.id()))
            DarkLogic.apply(action.id())
            DarkLogic.printTheorem()
            print("____________________________________________________________________________")
        end = time.perf_counter()
        if DarkLogic.hasAlreadyPlayed():
            if DarkLogic.isDemonstrated():
                print(ai.name() + " won! " + ai.name() + " finished the demonstration!")
            elif DarkLogic.isAlreadyPlayed():
                print(ai.name() + " lost! Repetition of theorem!")
            elif DarkLogic.isEvaluated():
                print(
                    ai.name() + " lost! Cannot (\"back-\")demonstrate that a theorem is false with implications")
            elif not DarkLogic.canBeDemonstrated():
                print(ai.name() + " lost! This theorem cannot be demonstrated! " +
                      "It can be true or false according to the values of its variables")
        else:
            if DarkLogic.isDemonstrated():
                print("Game Over! the demonstration is already finished!")
                self._elapsed_seconds = end - start
            elif not DarkLogic.canBeDemonstrated():
                print("Game Over! This theorem cannot be demonstrated! " +
                      "It can be true or false according to the values of its variables")
        self.pushEvent(Event.EventEnum.STOP)


Test.test = Test()
