import os

path = os.getcwd()
import sys

sys.path.append(path + "\..\Lib")
from DarkLogic import DarkLogic


def init(nbInstances):
    """ init(self: int) -> None """
    DarkLogic.init(nbInstances)


# create theorem
def makeTheorem(name, cont):
    """ makeTheorem(self: str, arg0: str) -> bool """
    return DarkLogic.makeTheorem(name, cont)


# demonstration functions
def apply(*args, **kwargs):
    """
    apply(*args, *args,**kwargs)
    Overloaded function.

    1. apply(self: int) -> None

    2. apply(self: int, arg0: int) -> None

    3. apply(self: str, arg0: List[int]) -> bool
    """
    DarkLogic.apply(*args, **kwargs)


def applyStr(actionStr):
    """ applyStr(self: str) -> bool """
    return DarkLogic.applyStr(actionStr)


def unapply(*args, **kwargs):
    """
    unapply(*args, *args,**kwargs)
    Overloaded function.

    1. unapply() -> None

    2. unapply(self: int) -> None
    """
    return DarkLogic.unapply(*args, **kwargs)


def getActions(*args, **kwargs):
    """
    getActions(*args, *args,**kwargs)
    Overloaded function.

    1. getActions() -> List[int]

    2. getActions(self: int) -> List[int]
    """
    return DarkLogic.getActions(*args, **kwargs)


def getHumanActions():
    """ getHumanActions() -> List[N_DarkLogic::Action] """
    return DarkLogic.getHumanActions()


def getRuleContents():
    """ getRuleContents() -> Dict[str, N_DarkLogic::RuleContent] """
    return DarkLogic.getRuleContents()


# get information on theorem state
def isOver(*args, **kwargs):
    """
    isOver(*args, *args,**kwargs)
    Overloaded function.

    1. isOver() -> bool

    2. isOver(self: int) -> bool
    """
    return DarkLogic.isOver(*args, **kwargs)


def isDemonstrated(*args, **kwargs):
    """
    isDemonstrated(*args, *args,**kwargs)
    Overloaded function.

    1. isDemonstrated() -> bool

    2. isDemonstrated(self: int) -> bool
    """
    return DarkLogic.isDemonstrated(*args, **kwargs)


def isAlreadyPlayed(*args, **kwargs):
    """
    isAlreadyPlayed(*args, *args,**kwargs)
    Overloaded function.

    1. isAlreadyPlayed() -> bool

    2. isAlreadyPlayed(self: int) -> bool
    """
    return DarkLogic.isAlreadyPlayed(*args, **kwargs)


def hasAlreadyPlayed(*args, **kwargs):
    """
    hasAlreadyPlayed(*args, *args,**kwargs)
    Overloaded function.

    1. hasAlreadyPlayed() -> bool

    2. hasAlreadyPlayed(self: int) -> bool
    """
    return DarkLogic.hasAlreadyPlayed(*args, **kwargs)


def canBeDemonstrated(*args, **kwargs):
    """
    canBeDemonstrated(*args, *args,**kwargs)
    Overloaded function.

    1. canBeDemonstrated() -> bool

    2. canBeDemonstrated(self: int) -> bool
    """
    return DarkLogic.canBeDemonstrated(*args, **kwargs)


def evaluate(*args, **kwargs):
    """
    evaluate(*args, *args,**kwargs)
    Overloaded function.

    1. evaluate() -> bool

    2. evaluate(self: int) -> bool
    """
    return DarkLogic.evaluate(*args, **kwargs)


def isEvaluated(*args, **kwargs):
    """
    isEvaluated(*args, **kwargs)
    Overloaded function.

    1. isEvaluated() -> bool

    2. isEvaluated(self: int) -> bool
    """
    return DarkLogic.isEvaluated(*args, **kwargs)


def appliedRuleSymmetric(*args, **kwargs):
    """
    appliedRuleSymetric(*args, *args,**kwargs)
    Overloaded function.

    1. appliedRuleSymetric() -> bool

    2. appliedRuleSymetric(self: int) -> bool
    """
    return DarkLogic.appliedRuleSymetric(*args, **kwargs)


def getState(*args, **kwargs):
    """
    getState(*args, *args,**kwargs)
    Overloaded function.

    1. getState() -> N_DarkLogic::State

    2. getState(self: int) -> N_DarkLogic::State
    """
    return DarkLogic.getState(*args, **kwargs)


def getRuleStates():
    """ getRuleStates() -> List[N_DarkLogic::State] """
    return DarkLogic.getRuleStates()


# get information on theorem content
def theoremName():
    """ theoremName() -> str """
    return DarkLogic.theoremName()


def printTheorem(*args, **kwargs):
    """
    printTheorem(*args, *args,**kwargs)
    Overloaded function.

    1. printTheorem() -> None

    2. printTheorem(self: int) -> None
    """
    return DarkLogic.printTheorem(*args, **kwargs)


def toStrTheorem(*args, **kwargs):
    """
    toStrTheorem(*args, *args,**kwargs)
    Overloaded function.

    1. toStrTheorem() -> str

    2. toStrTheorem(self: int) -> str
    """
    return DarkLogic.toStrTheorem(*args, **kwargs)


def toNormStrTheorem():
    """ toNormStrTheorem() -> str """
    return DarkLogic.toNormStrTheorem()

def nbTheorems(instanceIdx):
    return DarkLogic.nbTheorems(instanceIdx)

# clear methods when the demonstration is over
def clearAll():
    """ clearAll() -> None """
    return DarkLogic.clearAll()


def clear(*args, **kwargs):
    """
    clear(*args, *args,**kwargs)
    Overloaded function.

    1. clear() -> None

    2. clear(self: int) -> None
    """
    return DarkLogic.clear(*args, **kwargs)
