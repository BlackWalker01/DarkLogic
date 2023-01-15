from AI.ai import AI
from Database.database import Database
import MainDarkLogic.darklogic as DarkLogic
import numpy as np
import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import layers
from tensorflow.python.lib.io import file_io
from keras import backend as K
# from sklearn.utils import class_weight

import random as rand
from threading import Lock
import multiprocessing


class NeuralAI(AI):
    MaxNbNode = 100
    NbDiffOperators = 7
    OperatorParams = NbDiffOperators + 2
    NbOperators = 30
    NbTerms = NbOperators
    OpeStateSize = NbOperators + NbDiffOperators
    MaxDepth = 25
    MultExamples = 500
    MaxGameBefLearning = 100
    # INIT_LR = 1 * 10 ** -3 """To optimize val_loss"""
    INIT_LR = 1 * 10 ** -3
    EARLY_STOPPING = 30
    ModelFile = "AI/deepAIModel"
    DbName = "Database/deepaiMemory.csv"

    def __init__(self, maxInstanceIdx, secondTimeout):
        super().__init__(maxInstanceIdx, secondTimeout, name="NeuralAI")
        self._theoremName = ""
        self._theorem = ""
        # get Rule States
        self._trueRuleStates = []
        ruleStates = DarkLogic.getRuleStates()
        for ruleState in ruleStates:
            self._trueRuleStates.append(makeTrueState(ruleState))
        self._inputSize = (len(self._trueRuleStates) + 1) * NeuralAI.NbOperators * 19
        self._storeNodes = []
        self._db = Database(NeuralAI.DbName)
        self._gamesSinceLastLearning = 0
        self._lr = NeuralAI.INIT_LR
        if modelExist():
            print("load DeepAI brain model")
            self._model = keras.models.load_model(NeuralAI.ModelFile)
            self._lr = K.eval(self._model.optimizer.lr)
            print(f"Found learning rate = {self._lr}")
            # self._lr = self._lr * 5
            # compileModel(self._model, self._lr)
            self._train()
        else:
            # create model
            print("create new DeepAI brain model")
            # self._model = createModel(len(self._trueRuleStates) + 1)
            self._model = createModel(1)
            compileModel(self._model, self._lr)
            self._model.save(NeuralAI.ModelFile)
            self._train()
        self._model = extractTestModel(self._model)
        self._modelMutex = Lock()
        self._elo = 2187  # 1418
        rand.seed()
        # self._train()

    def getTrueState(self, threadIdx):
        # return [makeTrueState(DarkLogic.getState(threadIdx))] + self._trueRuleStates
        return [makeTrueState(DarkLogic.getState(threadIdx))]

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
                                  use_multiprocessing=True, verbose = 0)
        self._modelMutex.release()
        realOuts = eval(out)
        for node, realOut in zip(nodes, realOuts):
            node.setAIValue(realOut)

    def explore(self, dbNode, threadId):
        self._crtNode.exploreDeep(dbNode.actions(), threadId)

    def _train(self):
        x_train = []
        x_test = []
        y_train = []
        y_test = []
        print("DeepAI is preparing for training...")
        # node.getTrainNodes(x, y)
        self._db = Database(NeuralAI.DbName)
        dbStates = self._db.getDatas()
        nbExcludedTh = 0
        class_nb = {}
        for cl in range(NeuralAI.MaxDepth + 1):
            class_nb[cl] = 0
        print("Total number of theorems in database: " + str(len(dbStates)))
        dbStateIdx = -1
        remDbStates = list(dbStates.values())
        rand.shuffle(remDbStates)
        # NbMax = 200000
        NbMax = 0
        if NbMax < len(dbStates):
            NbMaxUnevaluatedThm = NbMax - self._db.nbEvaluatedThm() if NbMax > self._db.nbEvaluatedThm() else 0
            NbMaxEvaluatedThm = self._db.nbEvaluatedThm()
        else:
            NbMaxUnevaluatedThm = len(dbStates) - self._db.nbEvaluatedThm()
            NbMaxEvaluatedThm = self._db.nbEvaluatedThm()
        print("Must select " + str(NbMaxUnevaluatedThm) + " unevaluated theorems")
        print("Must select " + str(NbMaxEvaluatedThm) + " evaluated theorems")
        NbEvaluated = 0
        NbUnevaluated = 0
        lastEvalPrint = 0
        lastUnevalPrint = 0
        for dbState in remDbStates:
            dbStateIdx += 1
            if NbEvaluated > lastEvalPrint and NbEvaluated % 10000 == 0:
                lastEvalPrint = NbEvaluated
                print(str(NbEvaluated) + " evaluated theorems have been seen")
            if NbUnevaluated > lastUnevalPrint and NbUnevaluated % 10000 == 0:
                lastUnevalPrint = NbUnevaluated
                print(str(NbUnevaluated) + " unevaluated theorems have been seen")
            DarkLogic.makeTheorem(dbState.theoremName(), dbState.theoremContent())
            state = DarkLogic.getState()
            DarkLogic.clearAll()
            if len(state.operators()) > NeuralAI.NbOperators:
                if dbState.isEvaluated() and NbMaxEvaluatedThm == self._db.nbEvaluatedThm():
                    NbMaxUnevaluatedThm += 1
                    NbMaxEvaluatedThm -= 1
                continue
            if dbState.isEvaluated():
                if NbEvaluated == NbMaxEvaluatedThm:
                    continue
                cl = dbState.value() if dbState.value() < NeuralAI.MaxDepth else NeuralAI.MaxDepth
                class_nb[cl] += 1
                l = list(range(len(self._trueRuleStates)))
                # rand.shuffle(l)
                if dbState.isForTraining():
                    # x_train.append([makeTrueState(state), l])
                    x_train.append([makeTrueState(state)])
                    # y_train.append(nthColounmOfIdentiy(cl))
                    y_train.append(cl)
                else:
                    # x_test.append([makeTrueState(state), l])
                    x_test.append([makeTrueState(state)])
                    # y_test.append(nthColounmOfIdentiy(cl))
                    y_test.append(cl)
                NbEvaluated += 1
                if NbUnevaluated == NbMaxUnevaluatedThm and NbEvaluated == NbMaxEvaluatedThm:
                    break
            else:
                if NbUnevaluated == NbMaxUnevaluatedThm:
                    continue
                l = list(range(len(self._trueRuleStates)))
                # rand.shuffle(l)
                if dbState.isForTraining():
                    # x_train.append([makeTrueState(state), l])
                    x_train.append([makeTrueState(state)])
                    # y_train.append(createZeroTab(DeepAI.MaxDepth + 1))
                    y_train.append(-1)
                else:
                    # x_test.append([makeTrueState(state), l])
                    x_test.append([makeTrueState(state)])
                    # y_test.append(createZeroTab(DeepAI.MaxDepth + 1))
                    y_test.append(-1)
                NbUnevaluated += 1
                if NbUnevaluated == NbMaxUnevaluatedThm and NbEvaluated == NbMaxEvaluatedThm:
                    break

        print("Selected " + str(NbUnevaluated) + " unevaluated theorems")
        print("Selected " + str(NbEvaluated) + " evaluated theorems")

        # if we keep some examples
        if len(x_train):
            # check class_weight
            if NbUnevaluated != 0:
                class_nb[-1] = 1 / NbUnevaluated
            NbExamples = len(x_train) + len(x_test)
            print("Keep " + str(NbExamples) + " examples")
            class_weights = {}
            for val in class_nb:
                nb_cl = class_nb[val]
                print(f"-> There is {nb_cl} elmts of class {val}")
                if nb_cl >= NbExamples - 1:
                    print("[WARNING] Useless to train if almost all examples are from one class! Exit")
                    return
                if nb_cl != 0:
                    class_weights[val] = 1 / nb_cl
                else:
                    class_weights[val] = 0

            """# shuffle examples
            print("shuffle " + str(len(x_train)) + " examples ...")
            randList = list(range(len(x_train)))
            rand.shuffle(randList)
            newX = []
            newY = []
            for pos in randList:
                newX.append(x_train[pos])
                newY.append(y_train[pos])
            x_train = newX
            y_train = newY"""

            # prepare for training
            batch_size = 100
            nb_epochs = 10000
            print("training on " + str(len(x_train)) + " examples")
            # earlyStop = keras.callbacks.EarlyStopping(monitor='val_loss', min_delta=0.001, patience=20, verbose=1)
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
            if len(x_test) == 0:
                print("[ERROR] Database should contain validation examples, but there is only training examples! Leave training...")
                return
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
            minLoss = 10 ** 100
            lastDecLoss = 0  # last epoch since loss has decreased
            # init minValLoss
            print("Validation of current model")
            if modelExist():
                # load best model
                print("load last model")
                self._model = keras.models.load_model(NeuralAI.ModelFile)
                # compileModel(self._model, self._lr)
                bestModel = self._model
            else:
                compileModel(self._model, self._lr)
                bestModel = self._model
            print("__________________________________________________________________________")
            crtMinValLoss, crtMaxValAcc = validation(self._model, testBatches_x, testBatches_y,
                                                batch_size, class_weights, self._trueRuleStates, self._inputSize)
            print("VAL_LOSS = " + str(crtMinValLoss))
            print("VAL_ACCURACY = " + str(crtMaxValAcc))
            minValLoss = 10 ** 100
            maxValAcc = 0
            lastValAcc = 0
            lastDecValLoss = 0  # nb epochs since validation loss has decreased
            lastIncValAcc = 0 # nb epochs since validation accuracy has increased

            """"print("create new model")
            self._model = createModel(len(self._trueRuleStates) + 1)
            compileModel(self._model, lr)"""
            for epoch in range(nb_epochs):
                print("epoch n°" + str(epoch + 1) + "/" + str(nb_epochs))
                # training...
                loss, accuracy = training(self._model, trainBatches_x, trainBatches_y,
                                          batch_size, class_weights, self._trueRuleStates, self._inputSize)
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
                                                    batch_size, class_weights, self._trueRuleStates, self._inputSize)
                print("VAL_LOSS = " + str(val_loss))
                print("VAL_ACCURACY = " + str(val_accuracy))
                if val_accuracy > lastValAcc:
                    print("VAL_ACC is increasing! :)")
                    # maxValAcc = val_accuracy
                    lastIncValAcc = 0
                    """print("Save temporary model...")
                    self._model.save("AI/tmpModel")"""
                    if val_accuracy > crtMaxValAcc:
                        print("Improvement compared to old model!!!")
                        maxValAcc = val_accuracy
                        crtMaxValAcc = maxValAcc
                elif val_accuracy == lastValAcc:
                    """print("Save temporary model...")
                    self._model.save("AI/tmpModel")"""
                    if lastIncValAcc > 0:
                        lastIncValAcc += 1
                        print(f"VAL_ACC is stable but has been decreasing for {lastIncValAcc} epochs! :/")
                    else:
                        lastIncValAcc = 0
                        print(f"VAL_ACC is stable! :|")
                else:
                    lastIncValAcc += 1
                    print(f"VAL_ACC has been decreasing for {lastIncValAcc} epochs! :(")
                lastValAcc = val_accuracy

                """if val_loss < minValLoss:
                    print("VAL_LOSS decreasing")
                    minValLoss = val_loss
                    lastDecValLoss = 0
                    if minValLoss < crtMinValLoss:
                        print("Improvement compared to old model!!!")
                        crtMinValLoss = minValLoss
                else:
                    print("VAL_LOSS increasing")
                    lastDecValLoss += 1"""

                if lastDecLoss == 3:
                    self._lr = self._lr / 5
                    print("adapt learning rate: " + str(self._lr))
                    compileModel(self._model, self._lr)
                    lastDecLoss = 0
                    minLoss = loss  # keep latest loss for minimal loss
                    print("new current minimal loss: "+str(minLoss))
                """if lastDecValLoss == NeuralAI.EARLY_STOPPING:
                    print("Early-stopping!")
                    break"""
                if lastIncValAcc == NeuralAI.EARLY_STOPPING:
                    print("Early-stopping!")
                    break

                """if val_loss <= crtMinValLoss:
                    print("Save model")
                    self._model.save(NeuralAI.ModelFile)
                    bestModel = self._model"""
                if val_accuracy >= crtMaxValAcc:
                    print("Save model")
                    self._model.save(NeuralAI.ModelFile)
                    bestModel = self._model
                print("_______________________________________________________________________________________")
            if modelExist():
                # load best model
                print("load best model")
                self._model = keras.models.load_model(NeuralAI.ModelFile)
                self._model = extractTestModel(self._model)
            else:
                self._model = bestModel
                self._model = extractTestModel(self._model)
            print("_______________________________________________________________________________________")

    def _storeCrtNode(self):
        self._storeNodes.append(self._crtNode)

    def meditate(self):
        if len(self._storeNodes) > 0:
            self._gamesSinceLastLearning += 1
            DarkLogic.makeTheorem(self._theoremName, self._theorem)
            # update if deepAI found a demonstration
            revNodes = self._storeNodes[::-1]
            if revNodes[0].value() < NeuralAI.MaxDepth:
                val = revNodes[0].value()
                for k in range(1, len(revNodes)):
                    node = revNodes[k]
                    node.setValue(val + k)
            self._db.export(self._storeNodes[0].getDbStates())
            if self._gamesSinceLastLearning == NeuralAI.MaxGameBefLearning:
                if self._model.optimizer:
                    self._lr = K.eval(self._model.optimizer.lr) * 5
                    compileModel(self._model, self._lr)
                else:
                    self._lr = self._lr * 5
                    print(f"[WARNING] Failed to get learning rate from model! Take learning rate = {self._lr}")
                self._train()
                self._gamesSinceLastLearning = 0
            self._storeNodes.clear()
            DarkLogic.clearAll()
        super().meditate()

    def getTrainingStates(self, val, x, y):
        trueState = makeTrueState(DarkLogic.getState())
        hasToMult = True
        if val > NeuralAI.MaxDepth:
            val = NeuralAI.MaxDepth + 1
            hasToMult = False
        trueOut = nthColounmOfIdentiy(val)
        # print("shape = "+str(np.shape(trueOut)))
        mult = NeuralAI.MultExamples if hasToMult else 1
        for k in range(mult):
            crtState = [trueState]
            l = list(range(len(self._trueRuleStates)))
            # rand.shuffle(l)
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
        return len(state.operators()) < NeuralAI.NbOperators and len(state.terms()) < NeuralAI.NbTerms


def makeOrderedOpeTab(ope):
    ret = []
    opeId = int(ope.name())
    for k in range(opeId):
        ret.append(0.0)
    ret.append(1.0)
    for k in range(opeId + 1, NeuralAI.NbDiffOperators):
        ret.append(0.0)
    for parentOpe in ope.parentOperators():
        ret.append(float(parentOpe.idx() + 1))
    for k in range(NeuralAI.OpeStateSize - len(ret)):
        ret.append(0.0)
    return ret


class Residual(tf.keras.Model):  # @save
    def __init__(self, num_channels, use_1x1conv=False, strides=1):
        super().__init__()
        self.conv1 = tf.keras.layers.Conv2D(
            num_channels, padding='same', kernel_size=3, strides=strides, kernel_initializer="random_normal")
        self.conv2 = tf.keras.layers.Conv2D(
            num_channels, kernel_size=3, padding='same', kernel_initializer="random_normal")
        self.conv3 = None
        if use_1x1conv:
            self.conv3 = tf.keras.layers.Conv2D(
                num_channels, kernel_size=1, strides=strides, kernel_initializer="random_normal")
        self.bn1 = tf.keras.layers.BatchNormalization()
        self.bn2 = tf.keras.layers.BatchNormalization()

    def call(self, x, training=None, mask=None):
        y = tf.keras.activations.relu(self.bn1(self.conv1(x)))
        y = self.bn2(self.conv2(y))
        if self.conv3 is not None:
            x = self.conv3(x)
        y += x
        return tf.keras.activations.relu(y)


class ResnetBlock(tf.keras.layers.Layer):
    def __init__(self, num_channels, num_residuals, first_block=False,
                 **kwargs):
        super(ResnetBlock, self).__init__(**kwargs)
        self.residual_layers = []
        for i in range(num_residuals):
            if i == 0 and not first_block:
                self.residual_layers.append(
                    Residual(num_channels, use_1x1conv=True, strides=2))
            else:
                self.residual_layers.append(Residual(num_channels))

    def call(self, X, training=True, mask=None):
        for layer in self.residual_layers.layers:
            X = layer(X)
        return X


class DeResidual(tf.keras.Model):  # @save
    def __init__(self, num_channels, strides=1):
        super().__init__()
        self.conv1 = tf.keras.layers.Conv2DTranspose(
            num_channels, padding='same', kernel_size=3, strides=strides, kernel_initializer="random_normal")
        self.conv2 = tf.keras.layers.Conv2DTranspose(
            num_channels, kernel_size=3, padding='same', kernel_initializer="random_normal")
        self.bn1 = tf.keras.layers.BatchNormalization()
        self.bn2 = tf.keras.layers.BatchNormalization()

    def call(self, x, training=None, mask=None):
        y = tf.keras.activations.relu(self.bn1(self.conv1(x)))
        y = self.bn2(self.conv2(y))
        return tf.keras.activations.relu(y)


class DeResnetBlock(tf.keras.layers.Layer):
    def __init__(self, num_channels, num_residuals, first_block=False,
                 **kwargs):
        super(DeResnetBlock, self).__init__(**kwargs)
        self.residual_layers = []
        for i in range(num_residuals):
            if i == 0 and not first_block:
                self.residual_layers.append(
                    DeResidual(num_channels, strides=2))
            else:
                self.residual_layers.append(DeResidual(num_channels))

    def call(self, X, training=True, mask=None):
        for layer in self.residual_layers.layers:
            X = layer(X)
        return X


def createModel(inputSize):
    inputs = keras.Input(shape=(inputSize,
                                NeuralAI.NbOperators, NeuralAI.OpeStateSize + 3),
                         name="img")  # shape (H, W, C)
    norm = tf.keras.layers.LayerNormalization()(inputs)
    x = tf.keras.layers.Conv2D(128, kernel_size=3, padding='same', kernel_initializer="random_normal")(norm)
    x = tf.keras.layers.BatchNormalization()(x)
    x = tf.keras.layers.Activation('relu')(x)
    block_1_output = tf.keras.layers.MaxPool2D(pool_size=3, padding='same')(x)

    block_2_output = ResnetBlock(128, 1, first_block=True)(block_1_output)

    block_3_output = ResnetBlock(256, 2)(block_2_output)

    dense_input = layers.GlobalAveragePooling2D()(block_3_output)  # block_3_output
    dense_1 = layers.Dense(256, activation="relu")(dense_input)
    x = layers.Dropout(0.5)(dense_1)
    x = dense_input
    dense_2 = layers.Dense(NeuralAI.MaxDepth + 1)(x)
    main_output = layers.Activation("softmax", name="main_output")(dense_2)

    """# reverse network
    x = layers.Dense(256, activation="relu")(dense_2)
    output_0 = layers.subtract([x, dense_1], name="output_0")

    x = layers.Dropout(0.5)(x)
    x = layers.Dense(128, activation="relu")(x)
    output_1 = layers.subtract([x, dense_input], name="output_1")

    x = tf.keras.layers.Reshape((1, 1, 128))(x)
    x = tf.keras.layers.UpSampling2D((16, 5))(x)"""
    """x = DeResnetBlock(64, 2)(x)
    output_2 = layers.subtract([x, block_2_output], name="output_2")"""

    """"# x = tf.keras.layers.MaxPool2D((2, 2), strides=2)(x)
    x = DeResnetBlock(128, 1)(x)
    output_2 = layers.subtract([x, block_1_output], name="output_2")

    x = tf.keras.layers.UpSampling2D((3, 3))(x)
    x = tf.keras.layers.MaxPool2D(3, strides=1)(x)
    x = tf.keras.layers.BatchNormalization()(x)
    x = tf.keras.layers.Conv2DTranspose(NeuralAI.OpeStateSize + 3, kernel_size=(2, 3),
                                        kernel_initializer="random_normal")(x)
    output_3 = layers.subtract([x, norm], name="output_3")"""

    """model = keras.Model(inputs, outputs, name="deepai")
    opt = keras.optimizers.Adam(learning_rate=5 * 10 ** (-5))
    model.compile(loss='categorical_crossentropy', optimizer=opt, metrics=['accuracy'])"""

    trainModel = keras.Model(inputs, [main_output], name="deepai_train")
    """opt = keras.optimizers.Adam(learning_rate=1 * 10 ** (-5))
    trainModel.compile(loss=['categorical_crossentropy', 'mse'], loss_weights=[10, 10], optimizer=opt,
                       metrics=['accuracy'])"""
    # trainModel.summary()
    # model = extractTestModel(trainModel)
    return trainModel


def extractTestModel(trainModel):
    inputs = trainModel.get_layer("img").output
    outputs = trainModel.get_layer("main_output").output
    model = keras.Model(inputs, outputs, name="deepai_evaluate")
    return model


def makeZeroTab(n):
    ret = []
    for k in range(n):
        ret.append(0.0)
    return ret


def makeTrueState(state):
    priorityOpes = state.priorityOpe()
    terms = state.terms()
    priorityState = []
    termState = []
    idToPos = {}
    pos = 0

    # priority operators
    for ope in priorityOpes:
        priorityState.append(makeOrderedOpeTab(ope))
    for k in range(NeuralAI.NbOperators - len(priorityState)):
        priorityState.append(makeZeroTab(NeuralAI.OpeStateSize))

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
    for k in range(NeuralAI.NbTerms - len(termState)):
        termState.append(makeZeroTab(3))

    ret = []
    for priorOpe, term in zip(priorityState, termState):
        ret.append(priorOpe + term)

    return ret


def nthColounmOfIdentiy(pos):
    # assert(pos < DeepAI.MaxDepth +1)
    ret = []
    k = 0
    firstPos = min(pos, NeuralAI.MaxDepth)
    while k < firstPos:
        ret.append(0.0)
        k += 1
    ret.append(1.0)
    while k < NeuralAI.MaxDepth:
        ret.append(0.0)
        k += 1
    return ret


def eval(outputs):
    ret = []
    for output in outputs:
        # stocchastic mode
        indices = []
        idx = 0
        for pred in output[:-1]:
            idx += round(pred * 1000)
            indices.append(idx)
        indices.append(1000)
        rndNb = rand.randrange(0, 1000)
        crtVal = 0
        for id in indices:
            if rndNb < id:
                break
            crtVal += 1
        ret.append(round(crtVal))
    return ret


def training(model, trainBatches_x, trainBatches_y, batch_size, class_weights, trueRuleStates, inputSize):
    # training...
    print("training...")
    loss = 0
    main_output_loss = 0
    output_losses = [0 for i in range(1, len(model.outputs))]
    accuracy = 0
    accIdx = len(model.outputs) + 1 if len(model.outputs) > 1 else 1
    for numBatch in range(len(trainBatches_x)):
        crtBatch = trainBatches_x[numBatch]
        batch = []
        out_1 = []
        sample_weight = []
        for ex, cl in zip(crtBatch, trainBatches_y[numBatch]):
            state = [ex[0]]
            """l = ex[1]
            for pos in l:
                state.append(trueRuleStates[pos])"""
            batch.append(state)
            sample_weight.append(class_weights[cl])
            if cl >= 0:
                out_1.append(nthColounmOfIdentiy(cl))
            else:
                out_1.append(createZeroTab(NeuralAI.MaxDepth + 1))
        sample_weight = np.array(sample_weight)
        batch = np.array(batch)
        out = [np.array(out_1)]
        for k in range(1, len(model.outputs)):
            output = model.outputs[k]
            shape = [len(crtBatch)]
            for i in range(1, len(output.shape)):
                shape.append(output.shape[i])
            out.append(np.zeros(shape))
        if len(out) == 1:
            out = out[0]
        history = model.train_on_batch(batch, out, sample_weight=sample_weight)
        # print(history)
        # print(model.metrics_names)
        loss = (loss * numBatch + history[0] * (len(batch) / batch_size)) / (numBatch + 1)
        main_output_loss = (main_output_loss * numBatch + history[1] * (len(batch) / batch_size)) / (numBatch + 1)
        for k in range(len(output_losses)):
            output_losses[k] = (output_losses[k] * numBatch + history[k + 2] *
                                (len(batch) / batch_size)) / (numBatch + 1)
        accuracy = (accuracy * numBatch + history[accIdx] * (len(batch) / batch_size)) / (numBatch + 1)
        if numBatch % 200 == 199:
            print("batch n°" + str(numBatch + 1) + "/" + str(len(trainBatches_x)))
            print("loss = " + str(loss))
            print("main_output loss "+str(main_output_loss))
            for k in range(len(output_losses)):
                print("output_"+str(k)+" loss " + str(output_losses[k]))
            print("accuracy = " + str(accuracy))
            print("____________")
    return loss, accuracy


def validation(model, testBatches_x, testBatches_y, batch_size, class_weights, trueRuleStates, inputSize):
    print("validation...")
    val_loss = 0
    val_accuracy = 0
    accIdx = len(model.outputs) + 1 if len(model.outputs) > 1 else 1
    for numBatch in range(len(testBatches_x)):
        crtBatch = testBatches_x[numBatch]
        batch = []
        out_1 = []
        sample_weight = []
        for ex, cl in zip(crtBatch, testBatches_y[numBatch]):
            state = [ex[0]]
            """l = ex[1]
            for pos in l:
                state.append(trueRuleStates[pos])"""
            batch.append(state)
            sample_weight.append(class_weights[cl])
            if cl >= 0:
                out_1.append(nthColounmOfIdentiy(cl))
            else:
                out_1.append(createZeroTab(NeuralAI.MaxDepth + 1))
        sample_weight = np.array(sample_weight)
        batch = np.array(batch)
        out = [np.array(out_1)]
        for k in range(1, len(model.outputs)):
            output = model.outputs[k]
            shape = [len(crtBatch)]
            for i in range(1, len(output.shape)):
                shape.append(output.shape[i])
            out.append(np.zeros(shape))
        if len(out) == 1:
            out = out[0]

        """print("batch.shape = "+str(batch.shape))
        for idx, o in enumerate(out):
            print(f"out_{idx} has shape "+str(out[idx].shape))
            print(out[idx])
        print("sample_weight.shape = "+str(sample_weight.shape))
        model.summary()"""
        history = model.test_on_batch(batch, out, sample_weight=sample_weight)
        # print(history)
        # print(model.metrics_names)
        val_loss = (val_loss * numBatch + history[0] * (len(batch) / batch_size)) / (numBatch + 1)
        val_accuracy = (val_accuracy * numBatch + history[accIdx] * (len(batch) / batch_size)) / (numBatch + 1)
        if numBatch % 200 == 199:
            print("batch n°" + str(numBatch + 1) + "/" + str(len(testBatches_x)))
            print("val_loss = " + str(val_loss))
            print("val_accuracy = " + str(val_accuracy))
    return val_loss, val_accuracy


def createZeroTab(size):
    ret = []
    for k in range(size):
        ret.append(0)
    return ret


def compileModel(model, lr=0.001):
    opt = keras.optimizers.Adam(learning_rate=lr)
    # opt = keras.optimizers.SGD(learning_rate=lr)
    print(f"Compile Model with learning rate = {lr}")
    model.compile(loss=['categorical_crossentropy'],
                  loss_weights=[5 * 10 ** -4], optimizer=opt,
                  metrics=['accuracy'])

def modelExist():
    return file_io.file_exists(NeuralAI.ModelFile)