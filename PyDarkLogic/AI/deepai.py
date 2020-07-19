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
    MaxNbNode = 20
    NbDiffOperators = 7
    OperatorParams = NbDiffOperators + 2
    NbOperators = 20
    NbTerms = NbOperators
    MaxDepth = 25

    def __init__(self, type_, maxInstanceIdx, secondTimeout):
        super().__init__(type_, maxInstanceIdx, secondTimeout)
        # get Rule States
        self._trueRuleStates = []
        ruleStates = DarkLogic.getRuleStates()
        for ruleState in ruleStates:
            self._trueRuleStates.append(makeTrueState(ruleState))

        # create model
        inputs = keras.Input(shape=(95, 20, 19), name="img")  # shape (H, W, C)
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
        opt = keras.optimizers.Adam(learning_rate=0.01)
        self._model.compile(loss='categorical_crossentropy', optimizer=opt, metrics=['accuracy'])
        self._modelMutex = Lock()
        # self.train()

    """
    nodeLists: 
    - type is list of list of node
    """

    def evaluate(self, nodeLists, threadIdx):
        nodes = []
        trueStates = []
        # get all states
        for nodeList in nodeLists:
            # init at last action of nodeList
            for node in nodeList:
                DarkLogic.getActions(threadIdx)
                DarkLogic.apply(threadIdx, node.actionId())
            nodes.append(nodeList[-1])

            # get current state
            trueStates.append([makeTrueState(DarkLogic.getState(threadIdx))] + self._trueRuleStates)

            # restart from init point
            for node in nodeList:
                DarkLogic.unapply(threadIdx)

        # evaluate states
        trueStates = np.array(trueStates)
        self._modelMutex.acquire()
        out = self._model.predict(trueStates, batch_size=len(nodeLists), workers=multiprocessing.cpu_count(),
                                  use_multiprocessing=True)
        self._modelMutex.release()
        realOuts = eval(out)
        for node, realOut in zip(nodes, realOuts):
            node.setAIValue(realOut)

    def explore(self, actions):
        self._crtNode.exploreDeep(actions)

    def train(self):
        print("DeepAI is training...")
        x = []
        y = []
        # create 1000 examples
        N = 1000
        for k in range(N):
            l = list(range(len(self._trueRuleStates)))
            newRuleStates = []
            n = rand.randint(0, len(self._trueRuleStates) - 1)
            newRuleStates.append(self._trueRuleStates[l[n]])
            for pos in l:
                newRuleStates.append(self._trueRuleStates[pos])
            x.append(np.array(newRuleStates))
            y.append(np.array(nthColounmOfIdentiy(1)))
        pos = int(0.9 * N)
        x = np.array(x)
        y = np.array(y)
        x_train = x[:pos]
        x_test = x[pos:]
        y_train = y[:pos]
        y_test = y[pos:]
        callbacks = [keras.callbacks.EarlyStopping(monitor='val_accuracy', min_delta=0.001, patience=20, verbose=1)]
        self._model.fit(x_train, y_train,
                        batch_size=20, epochs=1, validation_data=(x_test, y_test), callbacks=callbacks)


def nthColounmOfIdentiy(pos):
    # assert(pos < DeepAI.MaxDepth +1)
    ret = []
    k = 0
    while k < pos:
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
