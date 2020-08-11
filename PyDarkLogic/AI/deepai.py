from AI.ai import AI
from Database.database import Database
import MainDarkLogic.darklogic as DarkLogic
import numpy as np
import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import layers
from tensorflow.python.lib.io import file_io
from sklearn.utils import class_weight

import random as rand
from threading import Lock
import multiprocessing


class DeepAI(AI):
    MaxNbNode = 50
    NbDiffOperators = 7
    OperatorParams = NbDiffOperators + 2
    NbOperators = 30
    NbTerms = NbOperators
    MaxDepth = 25
    MultExamples = 500
    MaxGameBefLearning = 5
    ModelFile = "AI/deepAIModel"
    DbName = "Database/deepaiMemory.csv"

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
        self._db = Database(DeepAI.DbName)
        self._gamesSinceLastLearning = 0
        if file_io.file_exists(DeepAI.ModelFile):
            print("load DeepAI brain model")
            self._model = keras.models.load_model(DeepAI.ModelFile)
        else:
            # create model
            print("create new DeepAI brain model")
            self._model = createModel(len(self._trueRuleStates) + 1)
            self._model.save(DeepAI.ModelFile)
        self._modelMutex = Lock()
        self._elo = 1146
        self._train()

    def getTrueState(self, threadIdx):
        return [makeTrueState(DarkLogic.getState(threadIdx))] + self._trueRuleStates

    """def getTrueState(self):
        return [makeTrueState(DarkLogic.getState())] + self._trueRuleStates"""

    def setTheoremInfo(self):
        self._theoremName = DarkLogic.theoremName()
        self._theorem = DarkLogic.toNormStrTheorem()

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
        x = []
        y = []
        print("DeepAI is preparing for training...")
        # node.getTrainNodes(x, y)
        dbStates = self._db.getDatas()
        nbExcludedTh = 0
        nbSelectedTh = 0
        class_nb = {}
        for cl in range(DeepAI.MaxDepth + 1):
            class_nb[cl] = 0
        print("Total number of theorems in database: " + str(len(dbStates)))
        dbStateIdx = -1
        for dbState in dbStates.values():
            dbStateIdx += 1
            if dbStateIdx != 0 and dbStateIdx % 25000 == 0:
                print(str(dbStateIdx) + " has been seen")
            if dbState.isEvaluated():
                nbSelectedTh += 1
                cl = dbState.value() if dbState.value() < DeepAI.MaxDepth else DeepAI.MaxDepth
                class_nb[cl] += 1
                l = list(range(len(self._trueRuleStates)))
                DarkLogic.makeTheorem(dbState.theoremName(), dbState.theoremContent())
                state = DarkLogic.getState()
                DarkLogic.clearAll()
                x.append([makeTrueState(state), l])
                if dbState.value() > DeepAI.MaxDepth:
                    y.append(nthColounmOfIdentiy(DeepAI.MaxDepth))
                else:
                    y.append(nthColounmOfIdentiy(dbState.value()))

        # if we keep some examples
        if len(x):
            # check class_weight
            print("Keep " + str(len(x)) + " examples")
            class_weights = {}
            for val in class_nb:
                nb_cl = class_nb[val]
                if nb_cl >= len(x) - 1:
                    print("[WARNING] Useless to train if almost all examples are from one class! Exit")
                    return
                if nb_cl != 0:
                    class_weights[val] = 1 / nb_cl
                else:
                    class_weights[val] = 0

            # shuffle examples
            print("shuffle " + str(len(x)) + " examples ...")
            randList = list(range(len(x)))
            newX = []
            newY = []
            newValues = []
            for pos in range(len(x)):
                newX.append(x[pos])
                newY.append(y[pos])
            x = newX
            y = newY
            values = newValues

            # prepare for training
            batch_size = 50
            nb_epochs = 1000
            pos = int(0.9 * len(x))
            # x = np.array(x)
            # y = np.array(y)
            values = np.array(values)
            x_train = x[:pos]
            x_test = x[pos:]
            y_train = y[:pos]
            y_test = y[pos:]
            print("training on " + str(len(x_train)) + " examples")
            earlyStop = keras.callbacks.EarlyStopping(monitor='val_loss', min_delta=0.001, patience=20, verbose=1)
            trainBatches_x = []
            trainBatches_y = []
            testBatches_x = []
            testBatches_y = []
            batch_x = []
            batch_y = []
            # prepare train batches
            for k in range(len(x_train)):
                batch_x.append(x_train[k])
                batch_y.append(y_train[k])
                if len(batch_x) == batch_size:
                    trainBatches_x.append(batch_x)
                    batch_x = []
                    trainBatches_y.append(batch_y)
                    batch_y = []
            if len(batch_x) > 0:
                trainBatches_x.append(batch_x)
                batch_x = []
                trainBatches_y.append(batch_y)
                batch_y = []

            # prepare test batches
            for k in range(len(x_test)):
                batch_x.append(x_test[k])
                batch_y.append(y_test[k])
                if len(batch_x) == batch_size:
                    testBatches_x.append(batch_x)
                    batch_x = []
                    testBatches_y.append(batch_y)
                    batch_y = []
            if len(batch_x) > 0:
                testBatches_x.append(batch_x)
                batch_x = []
                testBatches_y.append(batch_y)
                batch_y = []

            # fit
            minLoss = 10 ** 10
            lastDecLoss = 0  # last epoch since loss has decreased
            # init minValLoss
            print("Validation of current model")
            crtMinValLoss, val_acc = validation(self._model, testBatches_x, testBatches_y,
                                                batch_size, self._trueRuleStates)
            print("VAL_LOSS = " + str(crtMinValLoss))
            print("VAL_ACCURACY = " + str(val_acc))
            minValLoss = 10 ** 10
            lastDecValLoss = 0  # last epoch since loss has decreased
            lr = 1 * 10 ** (-5)
            print("create new model")
            self._model = createModel(len(self._trueRuleStates) + 1)
            opt = keras.optimizers.Adam(learning_rate=lr)
            self._model.compile(loss='categorical_crossentropy', optimizer=opt, metrics=['accuracy'])
            for epoch in range(nb_epochs):
                print("epoch n°" + str(epoch + 1) + "/" + str(nb_epochs))
                # training...
                loss, accuracy = training(self._model, trainBatches_x, trainBatches_y,
                                          batch_size, class_weights, self._trueRuleStates)
                print("LOSS = " + str(loss))
                print("ACCURACY = " + str(accuracy))
                if loss < minLoss:
                    print("LOSS decreasing!")
                    minLoss = loss
                    lastDecLoss = 0
                else:
                    print("LOSS increasing!")
                    lastDecLoss += 1

                # validation...
                val_loss, val_accuracy = validation(self._model, testBatches_x, testBatches_y,
                                                    batch_size, self._trueRuleStates)
                print("VAL_LOSS = " + str(val_loss))
                print("VAL_ACCURACY = " + str(val_accuracy))
                if val_loss < minValLoss:
                    print("VAL_LOSS decreasing")
                    minValLoss = val_loss
                    lastDecValLoss = 0
                    if minValLoss < crtMinValLoss:
                        print("Improvement compared to old model!!!")
                        crtMinValLoss = minValLoss
                else:
                    print("VAL_LOSS increasing")
                    lastDecValLoss += 1

                if lastDecLoss == 3:
                    lr = lr / 10
                    print("adapt learning rate: " + str(lr))
                    opt = keras.optimizers.Adam(learning_rate=lr)
                    self._model.compile(loss='categorical_crossentropy', optimizer=opt, metrics=['accuracy'])
                    lastDecLoss = 0
                if lastDecValLoss == 10:
                    print("Early-stopping!")
                    break

                if val_loss <= crtMinValLoss:
                    print("Save model")
                    self._model.save(DeepAI.ModelFile)
                print("_______________________________________________________________________________________")
            if file_io.file_exists(DeepAI.ModelFile):
                # load best model
                print("load best model")
                self._model = keras.models.load_model(DeepAI.ModelFile)
            print("_______________________________________________________________________________________")

    def _storeCrtNode(self):
        self._storeNodes.append(self._crtNode)

    def meditate(self):
        if len(self._storeNodes) > 0:
            self._gamesSinceLastLearning += 1
            DarkLogic.makeTheorem(self._theoremName, self._theorem)
            self._db.export(self._storeNodes[0].getDbStates())
            if self._gamesSinceLastLearning == DeepAI.MaxGameBefLearning:
                self._train()
                self._gamesSinceLastLearning = 0
            self._storeNodes.clear()
            DarkLogic.clearAll()
        super().meditate()

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

    def canEvaluate(self, state):
        return len(state.operators()) < DeepAI.NbOperators and len(state.terms()) < DeepAI.NbTerms


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


def createModel(inputSize):
    inputs = keras.Input(shape=(inputSize,
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

    model = keras.Model(inputs, outputs, name="deepai")
    opt = keras.optimizers.Adam(learning_rate=5 * 10 ** (-5))
    model.compile(loss='categorical_crossentropy', optimizer=opt, metrics=['accuracy'])
    return model


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
        ret.append(round(crtVal))
    return ret


def training(model, trainBatches_x, trainBatches_y, batch_size, class_weights, trueRuleStates):
    # training...
    print("training...")
    loss = 0
    accuracy = 0
    for numBatch in range(len(trainBatches_x)):
        crtBatch = trainBatches_x[numBatch]
        batch = []
        for ex in crtBatch:
            state = [ex[0]]
            l = ex[1]
            for pos in l:
                state.append(trueRuleStates[pos])
            batch.append(state)
        batch = np.array(batch)
        out = np.array(trainBatches_y[numBatch])
        history = model.train_on_batch(batch, out, class_weight=class_weights)
        loss = (loss * numBatch + history[0] * (len(batch) / batch_size)) / (numBatch + 1)
        accuracy = (accuracy * numBatch + history[1] * (len(batch) / batch_size)) / (numBatch + 1)
        if numBatch % 30 == 29:
            print("batch n°" + str(numBatch + 1) + "/" + str(len(trainBatches_x)))
            print("loss = " + str(loss))
            print("accuracy = " + str(accuracy))
    return loss, accuracy


def validation(model, testBatches_x, testBatches_y, batch_size, trueRuleStates):
    print("validation...")
    val_loss = 0
    val_accuracy = 0
    for numBatch in range(len(testBatches_x)):
        crtBatch = testBatches_x[numBatch]
        batch = []
        for ex in crtBatch:
            state = [ex[0]]
            l = ex[1]
            for pos in l:
                state.append(trueRuleStates[pos])
            batch.append(state)
        batch = np.array(batch)
        out = np.array(testBatches_y[numBatch])
        history = model.test_on_batch(batch, out)
        val_loss = (val_loss * numBatch + history[0] * (len(batch) / batch_size)) / (numBatch + 1)
        val_accuracy = (val_accuracy * numBatch + history[1] * (len(batch) / batch_size)) / (numBatch + 1)
        if numBatch % 30 == 29:
            print("batch n°" + str(numBatch + 1) + "/" + str(len(testBatches_x)))
            print("val_loss = " + str(val_loss))
            print("val_accuracy = " + str(val_accuracy))
    return val_loss, val_accuracy
