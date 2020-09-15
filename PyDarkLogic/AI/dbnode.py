import random as rand


class DbNode:
    def __init__(self):
        self._idxToActions = {}
        self._valToActions = {}
        self._idxToVal = {}

    def push(self, action):
        self._idxToActions[action] = action
        return action

    def updateValue(self, actionIdx, value, withPrint=False):
        action = self._idxToActions[actionIdx]
        if actionIdx in self._idxToVal:
            exValue = self._idxToVal[actionIdx]
            if exValue != value:
                # remove from latest value
                k = 0
                for exAction in self._valToActions[exValue]:
                    if action == exAction:
                        break
                    k += 1
                del self._valToActions[exValue][k]
                if len(self._valToActions[exValue]) == 0:
                    del self._valToActions[exValue]
                # update new value
                if value in self._valToActions:
                    self._valToActions[value].append(action)
                else:
                    self._valToActions[value] = [action]
                self._idxToVal[actionIdx] = value
        else:
            # update new value
            if value in self._valToActions:
                self._valToActions[value].append(action)
            else:
                self._valToActions[value] = [action]
            self._idxToVal[actionIdx] = value
        if withPrint:
            print("updateValue: ")
            print(self._idxToActions)
            print(self._idxToVal)
            print(self._valToActions)

    def removeIdx(self, actionIdx):
        action = self._idxToActions[actionIdx]
        val = self._idxToVal[actionIdx]
        k = 0
        for crtAction in self._valToActions[val]:
            if action == crtAction:
                break
            k += 1
        del self._valToActions[val][k]
        if len(self._valToActions[val]) == 0:
            del self._valToActions[val]
        del self._idxToActions[actionIdx]
        del self._idxToVal[actionIdx]

    def actions(self):
        return self._idxToActions.values()

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
