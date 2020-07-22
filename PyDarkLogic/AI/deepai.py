from AI.ai import AI

import os

path = os.getcwd()
import sys

sys.path.append(path + "\..\..\Lib")
from DarkLogic import DarkLogic

import numpy as np
import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import layers
from tensorflow.python.lib.io import file_io

import random as rand
from threading import Lock
import multiprocessing

def makeOrderedOpeTab(ope):
    ret = []
    opeId = int(ope.name())
    k = 0
    while k < opeId:
        ret.append(0.0)
        k += 1
    ret.append(1.0)
    k += 1
    while k < DeepAI.NbDiffOperators:
        ret.append(0.0)
        k += 1
    ret.append(float(ope.nbHyps()))
    ret.append(float(ope.argIdx()))
    return ret


def makeOpeTab(opeName):
    ret = []
    opeId = int(opeName)
    k = 0
    while k < opeId:
        ret.append(0.0)
        k += 1
    ret.append(1.0)
    k += 1
    while k < DeepAI.NbDiffOperators:
        ret.append(0.0)
        k += 1
    return ret


def makeZeroTab(n):
    ret = []
    for k in range(n):
        ret.append(0.0)
    return ret


def makeTrueState(state):
    priorityOpes = state.priorityOpe()
    opes = state.operators()
    terms = state.terms()
    priorityState = []
    opeState = []
    termState = []
    idToPos = {}
    pos = 0

    # priority operators
    for ope in priorityOpes:
        priorityState.append(makeOrderedOpeTab(ope))
    for k in range(DeepAI.NbOperators - len(priorityState)):
        priorityState.append(makeZeroTab(DeepAI.OperatorParams))

    # operators
    for ope in opes:
        opeState.append(makeOpeTab(ope))
    for k in range(DeepAI.NbOperators - len(opeState)):
        opeState.append(makeZeroTab(DeepAI.NbDiffOperators))

    # terms
    for term in terms:
        trueTerm = [float(term.isVariable()), float(term.type())]
        if term.isVariable():
            termId = term.id()
            if termId in idToPos:
                trueTerm.append(float(idToPos[termId]))
            else:
                idToPos[termId] = pos
                trueTerm.append(float(pos))
                pos += 1
        else:
            trueTerm.append(float(term.val()))
        termState.append(trueTerm)
    for k in range(DeepAI.NbTerms - len(termState)):
        termState.append(makeZeroTab(3))

    ret = []
    for priorOpe, ope, term in zip(priorityState, opeState, termState):
        ret.append(priorOpe + ope + term)
    return ret


class DeepAI(AI):
    MaxNbNode = 50
    NbDiffOperators = 7
    OperatorParams = NbDiffOperators + 2
    NbOperators = 30
    NbTerms = NbOperators
    MaxDepth = 25
    MultExamples = 1000

    def __init__(self, type_, maxInstanceIdx, secondTimeout):
        super().__init__(type_, maxInstanceIdx, secondTimeout)
        self._theoremName = ""
        self._theorem = ""
        # get Rule States
        self._trueRuleStates = []
        ruleStates = DarkLogic.getRuleStates()
        for ruleState in ruleStates:
            self._trueRuleStates.append(makeTrueState(ruleState))
        self._storeNodes = []
        if file_io.file_exists("deepAIModel"):
            self._model = keras.models.load_model("deepAIModel")
        else:
            # create model
            inputs = keras.Input(shape=(len(self._trueRuleStates) + 1,
                                        DeepAI.NbOperators, 19), name="img")  # shape (H, W, C)
            x = layers.Conv2D(128, 3, activation="relu")(inputs)
            x = layers.Conv2D(256, 3, activation="relu")(x)
            block_1_output = layers.MaxPooling2D(3)(x)

            x = layers.Conv2D(256, 3, activation="relu", padding="same")(block_1_output)
            x = layers.Conv2D(256, 3, activation="relu", padding="same")(x)
            block_2_output = layers.add([x, block_1_output])

            x = layers.Conv2D(256, 3, activation="relu", padding="same")(block_2_output)
            x = layers.Conv2D(256, 3, activation="relu", padding="same")(x)
            block_3_output = layers.add([x, block_2_output])

            x = layers.Conv2D(256, 3, activation="relu")(block_3_output)
            x = layers.GlobalAveragePooling2D()(x)
            x = layers.Dense(256, activation="relu")(x)
            x = layers.Dropout(0.5)(x)
            outputs = layers.Dense(DeepAI.MaxDepth + 1, activation="softmax")(x)

            self._model = keras.Model(inputs, outputs, name="deepai")
            self._model.summary()
            opt = keras.optimizers.Adam(learning_rate=0.002)
            self._model.compile(loss='categorical_crossentropy', optimizer=opt, metrics=['accuracy'])
            self._model.save("deepAIModel")
        self._modelMutex = Lock()
        # self.train()

    def getTrueState(self, threadIdx):
        return [makeTrueState(DarkLogic.getState(threadIdx))] + self._trueRuleStates

    """def getTrueState(self):
        return [makeTrueState(DarkLogic.getState())] + self._trueRuleStates"""

    def setTheoremInfo(self, thName, thContent):
        self._theoremName = thName
        self._theorem = thContent
    """
    nodeLists: 
    - type is list of list of node
    """

    def evaluate(self, nodes, trueStates):
        # evaluate states
        trueStates = np.array(trueStates)
        self._modelMutex.acquire()
        out = self._model.predict(trueStates, batch_size=len(trueStates), workers=multiprocessing.cpu_count(),
                                  use_multiprocessing=True)
        self._modelMutex.release()
        realOuts = eval(out)
        for node, realOut in zip(nodes, realOuts):
            node.setAIValue(realOut)

    def explore(self, actions):
        self._crtNode.exploreDeep(actions)

    def _train(self):
        DarkLogic.makeTheorem(self._theoremName, self._theorem)
        node = self._storeNodes[0]
        x = []
        y = []
        print("DeepAI is preparing for training...")
        node.getTrainNodes(x, y)

        # create examples
        for k in range(DeepAI.MultExamples):
            l = list(range(len(self._trueRuleStates)))
            newRuleStates = []
            n = rand.randint(0, len(self._trueRuleStates) - 1)
            newRuleStates.append(self._trueRuleStates[l[n]])
            for pos in l:
                newRuleStates.append(self._trueRuleStates[pos])
            x.append(np.array(newRuleStates))
            y.append(np.array(nthColounmOfIdentiy(1)))

        # shuffle examples
        print("shuffle "+str(len(x))+" examples ...")
        randList = list(range(len(x)))
        newX = []
        newY = []
        for pos in range(len(x)):
            newX.append(x[pos])
            newY.append(y[pos])
        x = newX
        y = newY

        # fit
        pos = int(0.9 * len(x))
        x = np.array(x)
        y = np.array(y)
        x_train = x[:pos]
        x_test = x[pos:]
        y_train = y[:pos]
        y_test = y[pos:]
        print("training on "+str(len(x_train))+" training examples")
        callbacks = [keras.callbacks.EarlyStopping(monitor='val_loss', min_delta=0.001, patience=20, verbose=1)]
        self._model.fit(x_train, y_train,
                        batch_size=50, epochs=100, validation_data=(x_test, y_test), callbacks=callbacks)
        self._model.save("deepAIModel")

    def _storeCrtNode(self):
        self._storeNodes.append(self._crtNode)

    def meditate(self):
        self._train()
        self._storeNodes.clear()

    def getTrainingStates(self, val, x, y):
        trueState = makeTrueState(DarkLogic.getState())
        hasToMult = True
        if val > DeepAI.MaxDepth:
            val = DeepAI.MaxDepth + 1
            hasToMult = False
        trueOut = nthColounmOfIdentiy(val)
        # print("shape = "+str(np.shape(trueOut)))
        mult = DeepAI.MultExamples if hasToMult else 1
        for k in range(mult):
            crtState = [trueState]
            l = list(range(len(self._trueRuleStates)))
            rand.shuffle(l)
            for pos in l:
                crtState.append(self._trueRuleStates[pos])
            x.append(crtState)
            y.append(trueOut)

    def getBestNode(self):
        return self._crtNode.getDeepBestNode()

    def value(self):
        if self._crtNode.isAIValuated():
            return self._crtNode.aiValue()
        return self._crtNode.value()

def nthColounmOfIdentiy(pos):
    # assert(pos < DeepAI.MaxDepth +1)
    ret = []
    k = 0
    firstPos = min(pos, DeepAI.MaxDepth)
    while k < firstPos:
        ret.append(0.0)
        k += 1
    ret.append(1.0)
    while k < DeepAI.MaxDepth:
        ret.append(0.0)
        k += 1
    return ret


def eval(outputs):
    ret = []
    for output in outputs:
        crtVal = 0
        k = 0
        for pred in output:
            crtVal += pred * k
            k += 1
        ret.append(int(crtVal))
    return ret
