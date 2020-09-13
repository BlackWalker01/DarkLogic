import random as rand


class DbNode:
    def __init__(self):
        self._idxToActions = {}
        self._valToActions = {}
        self._idxToVal = {}

    def push(self, parentAction, action):
        idx = len(self._idxToActions)
        self._idxToActions[idx] = [parentAction, action]
        return idx

    def updateValue(self, actionIdx, value, withPrint=False):
        actions = self._idxToActions[actionIdx]
        if actionIdx in self._idxToVal:
            exValue = self._idxToVal[actionIdx]
            if exValue != value:
                # remove from latest value
                k = 0
                for _, exActions in self._valToActions[exValue]:
                    if actions == exActions:
                        break
                    k += 1
                del self._valToActions[exValue][k]
                if len(self._valToActions[exValue]) == 0:
                    del self._valToActions[exValue]
                # update new value
                if value in self._valToActions:
                    self._valToActions[value].append((actionIdx, actions))
                else:
                    self._valToActions[value] = [(actionIdx, actions)]
                self._idxToVal[actionIdx] = value
        else:
            # update new value
            if value in self._valToActions:
                self._valToActions[value].append((actionIdx, actions))
            else:
                self._valToActions[value] = [(actionIdx, actions)]
            self._idxToVal[actionIdx] = value
        if withPrint:
            print("updateValue: ")
            print(self._idxToActions)
            print(self._idxToVal)
            print(self._valToActions)

    def removeIdx(self, actionIdx):
        actions = self._idxToActions[actionIdx]
        val = self._idxToVal[actionIdx]
        k = 0
        for _, actionList in self._valToActions[val]:
            if actions == actionList:
                break
            k += 1
        del self._valToActions[val][k]
        if len(self._valToActions[val]) == 0:
            del self._valToActions[val]
        del self._idxToActions[actionIdx]
        del self._idxToVal[actionIdx]

    def actions(self):
        return self._idxToActions

    def getBestAction(self):
        minVal = sorted(self._valToActions.keys())[0]
        actionList = self._valToActions[minVal]
        return actionList[rand.randint(0, len(actionList) - 1)]

    def size(self):
        return len(self._idxToActions)

    def clear(self):
        self._idxToActions.clear()
        self._valToActions.clear()
        self._idxToVal.clear()
