from AI.ai import AI
from MainDarkLogic.darklogic import DarkLogic


class EvalAI(AI):
    NotOpe = 4

    def __init__(self, maxInstanceIdx, secondTimeout):
        super().__init__(maxInstanceIdx, secondTimeout, name="EvalAI")

    def eval(self, states, threadId):
        state = states[0]
        opeList = state.priorityOpe()
        # first: opeSize
        opeSize = len(opeList)
        # second: nb of double not
        nbDoubleNots = 0
        for ope in opeList:
            parentOpes = ope.parentOperators()
            consecNots = 0
            if ope.name() == EvalAI.NotOpe:
                consecNots += 1
            for parentOpe in parentOpes[::-1]:
                if parentOpe.name() == EvalAI.NotOpe:
                    consecNots += 1
                else:
                    quo = consecNots // 2
                    nbDoubleNots = nbDoubleNots if nbDoubleNots >= quo else quo
                    consecNots = 0
            if consecNots != 0:
                quo = consecNots // 2
                nbDoubleNots = nbDoubleNots if nbDoubleNots >= quo else quo

        # DarkLogic.printTheorem(threadId)
        # print("opeSize = "+str(opeSize))
        # print("nbDoubleNots = "+str(nbDoubleNots))
        return opeSize + nbDoubleNots

    def explore(self, actions, threadId):
        self._crtNode.exploreEval(actions, threadId)

    def value(self):
        return self._crtNode.realValue()