import os

path = os.getcwd()
import sys
import time
import random as rand

sys.path.append(path + "\..\..\Lib")
from DarkLogic import DarkLogic


class Node:
    """description of class"""
    # static ai
    _ai = None
    VAL_MAX = 101
    VAL_INIT = VAL_MAX - 1

    def __init__(self, **kwargs):
        # First Root node
        if "ai" in kwargs:
            Node._ai = kwargs["ai"]
            self._actionId = 0
            self._threadId = 0
            self._depth = 0
        else:
            self._actionId = kwargs["actionId"]
            self._threadId = kwargs["threadId"]
            self._depth = kwargs["depth"]
        self._value = Node.VAL_INIT
        self._sons = {}

    def actionId(self):
        return self._actionId

    def threadId(self):
        return self._threadId

    def depth(self):
        return self._depth

    def value(self):
        return self._value

    def nbSimu(self):
        return self._nbSimu

    def pushCrtAction(self, actionId, threadIdx):
        if actionId in self._sons:
            self._sons[actionId].setThreadIdx(threadIdx)
        else:
            self._sons[actionId] = Node(actionId=actionId, threadId=threadIdx, depth=self._depth + 1)

    def setThreadIdx(self, threadIdx):
        self._threadId = threadIdx
        for subNode in self._sons.values():
            subNode.setThreadIdx(threadIdx)

    def isRoot(self):
        return self._threadId == 0

    def exploreStatic(self, actions):
        maxDepth = self.minDepth()
        threadId = self._sons[actions[0]].threadId()
        start = time.perf_counter()
        while not Node._ai.mustStop(threadId):
            for action in actions:
                node = self._sons[action]
                retValue = Node.VAL_MAX
                if node.value() < Node.VAL_MAX:
                    node.exploreDeepStatic(maxDepth)
                if self._value > retValue + 1:
                    self._value = retValue + 1
                # if must stop exploration, stop it
                if Node._ai.mustStop(threadId):
                    break

            if maxDepth >= 3:
                end = time.perf_counter()
                elapsed_seconds = end - start
                print("[DEBUG] thread id: " + str(threadId) + " has finished depth '" + str(maxDepth)
                      + "' in " + str(elapsed_seconds) + " seconds")
            maxDepth += 1

        return self._value

    def exploreDeepStatic(self, maxDepth):
        # play crt move
        DarkLogic.apply(self._threadId, self._actionId)
        """print("crt action id: " + str(self._actionId) + ", at depth="
              + str(self._depth) + ", crt theorem is: " + DarkLogic.toStrTheorem(self._threadId))"""

        # no need to go deeper
        if self._depth == maxDepth:
            # check if it is a node which leads to loss
            if DarkLogic.isAlreadyPlayed(self._threadId) or not DarkLogic.canBeDemonstrated(self._threadId):
                self._value = Node.VAL_MAX
            # check if it is a node which leads to win
            elif DarkLogic.isDemonstrated(self._threadId):
                self._value = 0
                # stop reflexion because AI found a demonstration
                Node._ai.stopThread(self._threadId)
        elif not Node._ai.mustStop(self._threadId):
            # get actions
            actions = DarkLogic.getActions(self._threadId)

            # add all subnodes if they have not been created yet
            if self._depth == maxDepth - 1:
                for action in actions:
                    self._sons[action] = Node(actionId=action, threadId=self._threadId, depth=self._depth + 1)

            # explore subNodes
            hasOnlyLosses = True
            for action in actions:
                # explore node associated with action
                node = self._sons[action]
                retValue = Node.VAL_MAX
                if node.value() < Node.VAL_MAX:
                    retValue = node.exploreDeepStatic(maxDepth)

                # update m_value
                if retValue == Node.VAL_MAX:
                    if hasOnlyLosses:
                        self._value = Node.VAL_MAX
                else:
                    hasOnlyLosses = False
                    if retValue == Node.VAL_INIT and self._value == Node.VAL_MAX:
                        self._value = Node.VAL_INIT
                    elif self._value > retValue + 1:
                        self._value = retValue + 1

                # if must stop exploration, stop it
                if Node._ai.mustStop(self._threadId):
                    break

        # unplay crt move
        DarkLogic.unapply(self._threadId)

        return self._value

    def getBestNode(self):
        minNodes = []
        minVal = Node.VAL_MAX
        for key in self._sons.keys():
            son = self._sons[key]
            if son.value() < minVal:
                minVal = son.value()
                minNodes.clear()
                minNodes.append(key)
            elif son.value() == minVal:
                minNodes.append(key)
        print(minNodes)
        valWinner = minNodes[rand.randint(0, len(minNodes)-1)]
        winner = self._sons[valWinner]
        self._sons.clear()
        return winner

    def getDemoMode(self):
        winner = None
        minNodes = []
        minVal = Node.VAL_INIT - 1
        for key in self._sons:
            son = self._sons[key]
            if son.value() < minVal:
                minVal = son.value()
                minNodes.clear()
                minNodes.append(key)
            elif son.value() == minVal:
                minNodes.append(key)
        if len(minNodes):
            valWinner = minNodes[rand.randint(0, len(minNodes)-1)]
            winner = self._sons[valWinner]
            self._sons.clear()
        return winner

    def setRoot(self):
        self._threadId = 0
        self._decrDepth()

    def nbNode(self):
        ret = 1
        for son in self._sons.values():
            ret += son.nbNode()
        return ret

    def _decrDepth(self):
        self._depth -= 1
        for son in self._sons.values():
            son._decrDepth()

    def minDepth(self):
        if len(self._sons):
            ret = Node.VAL_MAX
            for son in self._sons.values():
                if son.value() < Node.VAL_MAX:
                    crtDepth = son.minDepth()
                    if crtDepth + 1 < ret:
                        ret = crtDepth + 1
            return ret
        else:
            return 0
