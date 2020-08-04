import time
import random as rand
import MainDarkLogic.darklogic as DarkLogic
from Database.state import State


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
        self._isEvaluated = False
        self._aiValue = None
        self._sons = {}
        self._isLoss = False

    def actionId(self):
        return self._actionId

    def threadId(self):
        return self._threadId

    def depth(self):
        return self._depth

    def value(self):
        return self._value

    def aiValue(self):
        return self._aiValue

    def setAIValue(self, aivalue):
        self._aiValue = aivalue

    def isEvaluated(self):
        return self._isEvaluated

    def isAIValuated(self):
        return not (self._aiValue is None)

    def pushCrtAction(self, actionId, threadIdx):
        if actionId in self._sons and self._sons[actionId]:
            self._sons[actionId].setThreadIdx(threadIdx)
        else:
            self._sons[actionId] = Node(actionId=actionId, threadId=threadIdx, depth=self._depth + 1)

    def setThreadIdx(self, threadIdx):
        self._threadId = threadIdx
        for subNode in self._sons.values():
            if subNode:
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
                    node.exploreDepthStatic(maxDepth)
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

    def exploreDepthStatic(self, maxDepth):
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
                    retValue = node.exploreDepthStatic(maxDepth)

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

    def exploreDeep(self, actions):
        actionIdx = 0
        maxDepth = 0
        while True:
            # init Nodes
            states = []
            nodeList = []
            crtActions = actions[actionIdx:]

            if len(crtActions) > 0:
                for action in crtActions:
                    actionIdx += 1
                    node = self._sons[action]
                    if not node.isEvaluated():
                        node.preExplore(nodeList, states)
                        if node.value() < Node.VAL_INIT:
                            self._value = node.value() + 1
                    if Node._ai.mustStop(self._threadId):
                        return
                    if len(states) == Node._ai.MaxNbNode:
                        break

            if Node._ai.mustStop(self._threadId):
                return
            if len(states) != Node._ai.MaxNbNode:
                # We need to go deeper!
                nodes = self.getDeepExploreNodes(actions)
                rand.shuffle(nodes)
                for node in nodes:
                    node.exploreDepthDeep(nodeList, states)
                    if node.value() < Node.VAL_INIT:
                        self._value = node.value() + 1
                    if Node._ai.mustStop(self._threadId):
                        return
                    if len(states) == Node._ai.MaxNbNode:
                        break

            # evaluate nodes
            Node._ai.evaluate(nodeList, states)

    def exploreDepthDeep(self, nodeList, states):
        DarkLogic.apply(self._threadId, self._actionId)
        actions = DarkLogic.getActions(self._threadId)
        for action in actions:
            if not self._sons[action]:
                self._sons[action] = Node(actionId=action, threadId=self._threadId, depth=self._depth + 1)
            node = self._sons[action]
            if not node.isEvaluated():
                node.preExplore(nodeList, states)
                if node.value() < Node.VAL_INIT:
                    self._value = node.value() + 1
            if Node._ai.mustStop(self._threadId):
                break
            if len(states) == Node._ai.MaxNbNode:
                break

        if not Node._ai.mustStop(self._threadId) and len(states) != Node._ai.MaxNbNode:
            # We need to go deeper!
            nodes = self.getDeepExploreNodes(self._sons.keys())
            rand.shuffle(nodes)
            for node in nodes:
                node.exploreDepthDeep(nodeList, states)
                if node.value() < Node.VAL_INIT:
                    self._value = node.value() + 1
                if Node._ai.mustStop(self._threadId):
                    break
                if len(states) == Node._ai.MaxNbNode:
                    break

        DarkLogic.unapply(self._threadId)

    def computeAIValue(self):
        minValue = Node.VAL_MAX
        allNode = True
        for node in self._sons.values():
            if node:
                if node.isAIValuated():
                    crtValue = node.computeAIValue()
                    if minValue > crtValue:
                        minValue = crtValue
                else:
                    allNode = False
            else:
                allNode = False
        if allNode:
            if minValue != self._aiValue - 1:
                self._aiValue = minValue + 1
        return self._aiValue

    def getDeepExploreNodes(self, actions):
        minNodes = []
        minVal = Node.VAL_MAX
        for action in actions:
            node = self._sons[action]
            if not node:
                self._sons[action] = Node(actionId=action, threadId=self._threadId, depth=self._depth + 1)
                node = self._sons[action]
            if node.value() == Node.VAL_MAX:
                continue
            if node.isAIValuated():
                # print("Node is AIEvaluated!!?")
                crtValue = node.computeAIValue()
                if crtValue < minVal:
                    minVal = crtValue
                    minNodes.clear()
                    minNodes.append(node)
                elif crtValue == minVal:
                    minNodes.append(node)
            else:
                if minVal != -1:
                    minVal = -1
                    minNodes.clear()
                minNodes.append(node)

        return minNodes

    def preExplore(self, nodeList, states):
        # play crt move
        DarkLogic.apply(self._threadId, self._actionId)

        # check if it is a node which leads to loss
        if DarkLogic.isAlreadyPlayed(self._threadId):
            self._value = Node.VAL_MAX
        elif not DarkLogic.canBeDemonstrated(self._threadId):
            self._value = Node.VAL_MAX
            self._isLoss = True
        # check if it is a node which leads to win
        elif DarkLogic.isDemonstrated(self._threadId):
            self._value = 0
            # stop reflexion because AI found a demonstration
            Node._ai.stopThread(self._threadId)
        else:
            actions = DarkLogic.getActions(self._threadId)
            for action in actions:
                self._sons[action] = None
            nodeList.append(self)
            states.append(Node._ai.getTrueState(self._threadId))
        self._isEvaluated = True

        # unplay crt move
        DarkLogic.unapply(self._threadId)

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
        valWinner = minNodes[rand.randint(0, len(minNodes) - 1)]
        winner = self._sons[valWinner]
        # self._sons.clear()
        return winner

    def getDeepBestNode(self):
        minNodes = []
        minVal = Node.VAL_MAX
        minAIVal = Node.VAL_MAX
        for key in self._sons.keys():
            son = self._sons[key]
            if son.value() < minVal:
                minVal = son.value()
                minAIVal = son.aiValue()
                minNodes.clear()
                minNodes.append(key)
            elif son.value() == minVal:
                if son.isAIValuated():
                    if son.aiValue() < minAIVal:
                        minAIVal = son.aiValue()
                        minNodes = [key]
                    elif son.aiValue() == minAIVal:
                        minNodes.append(key)

        valWinner = minNodes[rand.randint(0, len(minNodes) - 1)]
        winner = self._sons[valWinner]
        # self._sons.clear()
        return winner

    def getDemoMode(self):
        winner = None
        minNodes = []
        minVal = Node.VAL_INIT - 1
        for key in self._sons:
            son = self._sons[key]
            if son:
                if son.value() < minVal:
                    minVal = son.value()
                    minNodes.clear()
                    minNodes.append(key)
                elif son.value() == minVal:
                    minNodes.append(key)
        if len(minNodes):
            valWinner = minNodes[rand.randint(0, len(minNodes) - 1)]
            winner = self._sons[valWinner]
            # self._sons.clear()
        return winner

    def setRoot(self):
        self._threadId = 0
        self._decrDepth()

    def nbNode(self):
        ret = 1
        for son in self._sons.values():
            if son:
                ret += son.nbNode()
        return ret

    def _decrDepth(self):
        self._depth -= 1
        for son in self._sons.values():
            if son:
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

    def getTrainNodes(self, x, y):
        DarkLogic.getActions()
        print(str(len(self._sons))+" nodes to explore")
        for key in self._sons:
            node = self._sons[key]
            if node:
                if node.isEvaluated():
                    print("explore " + str(node.actionId()) + " at depth = " + str(1) + " with value = " + str(
                        node.value()))
                    if node.value() != Node.VAL_INIT and not (node.value() == Node.VAL_MAX and not node.isLoss()):
                        """print("train with node id = " + str(node.actionId()) + " at depth = " + str(
                            1) + " with value = " + str(node.value()))"""
                        Node._ai.getTrainingStates(node.value(), x, y)
                    node.getDepthTrainNodes(x, y, 1)

    def getDepthTrainNodes(self, x, y, depth):
        DarkLogic.getActions()
        DarkLogic.apply(self._actionId)
        # print("explore " + str(self.actionId()) + " at depth = " + str(depth) + " with value = " + str(self.value()))
        for key in self._sons:
            node = self._sons[key]
            if node:
                if node.isEvaluated():
                    if node.value() != Node.VAL_INIT and not (node.value() == Node.VAL_MAX and not node.isLoss()):
                        """print("train with node id = " + str(node.actionId()) + " at depth = " + str(
                            depth + 1) + " with value = " + str(node.value()))"""
                        Node._ai.getTrainingStates(node.value(), x, y)
                    node.getDepthTrainNodes(x, y, depth + 1)
        DarkLogic.unapply()

    def isLoss(self):
        return self._isLoss

    def getDbStates(self):
        ret = []
        DarkLogic.getActions()
        name = DarkLogic.theoremName()
        content = DarkLogic.toNormStrTheorem()
        if self.isEvaluated() and self.value() != Node.VAL_INIT and not (self.value() == Node.VAL_MAX
                                                                         and not self.isLoss()):
            ret.append(State(name=name, content=content, value=self._value))
        else:
            ret.append(State(name=name, content=content))
        for key in self._sons:
            node = self._sons[key]
            if node:
                node.getDeepDbStates(ret, 1)
        return ret

    def getDeepDbStates(self, dbStates, depth):
        DarkLogic.getActions()
        DarkLogic.apply(self._actionId)
        name = DarkLogic.theoremName()
        content = DarkLogic.toNormStrTheorem()
        if self.isEvaluated() and self.value() != Node.VAL_INIT and not (self.value() == Node.VAL_MAX
                                                                         and not self.isLoss()):
            dbStates.append(State(name=name, content=content, value=self._value))
        else:
            dbStates.append(State(name=name, content=content))
        for key in self._sons:
            node = self._sons[key]
            if node:
                node.getDeepDbStates(dbStates, depth + 1)
        DarkLogic.unapply()
