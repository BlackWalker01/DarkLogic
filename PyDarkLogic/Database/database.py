import csv
import random as rand
from Database.state import State
import os.path


class Database:

    def __init__(self, name):
        self._name = name
        self._theoremDb = {}
        self._nbEvaluatedThm = 0
        print("loading memory...")
        self.load()
        print("memory loaded")

    def load(self):
        self._theoremDb = {}
        if os.path.isfile(self._name):
            with open(self._name, newline='') as csvfile:
                reader = csv.DictReader(csvfile)
                hasToExport = False
                for row in reader:
                    state = None
                    if row["value"] != "_":
                        self._nbEvaluatedThm += 1
                        state = State(content=row["content"], name=row["name"],
                                      value=int(row["value"]))
                    else:
                        state = State(content=row["content"], name=row["name"])
                    if "training" in row and (row["training"] == "False" or row["training"] == "True"):
                        state.setIsForTraining(row["training"] == "True")
                    else:
                        print(f"[WARNING] '{state.theoremContent()}' theorem was not marked as "
                              "training or validation example!!")
                        state.setIsForTraining(rand.randrange(10) > 0)
                        hasToExport = True
                    self._theoremDb[state.theoremContent()] = state
                if hasToExport:
                    self.export([])
        else:
            with open(self._name, 'w', newline='') as csvfile:
                fieldnames = ["name", "content", "value", "training"]
                writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
                writer.writeheader()
        return self._theoremDb

    def getDatas(self):
        return self._theoremDb

    def nbEvaluatedThm(self):
        return self._nbEvaluatedThm

    def export(self, states):
        # update local database
        print("[DEBUG] Update local memory with " + str(len(states)) + " states ...")
        for state in states:
            if state.theoremContent() in self._theoremDb:
                exState = self._theoremDb[state.theoremContent()]
                state.setIsForTraining(exState.isForTraining())
                if not exState.isEvaluated():
                    self._theoremDb[state.theoremContent()] = state
                elif state.isEvaluated() and exState.value() > state.value():
                    self._theoremDb[state.theoremContent()] = state
            else:
                state.setIsForTraining(rand.randrange(10) > 0)
                self._theoremDb[state.theoremContent()] = state

        # export datas database
        print("[DEBUG] Export datas to long-term memory...")
        with open(self._name, 'w', newline='') as csvfile:
            fieldnames = ["name", "content", "value", "training"]
            writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
            writer.writeheader()
            for state in self._theoremDb.values():
                if state.isEvaluated():
                    writer.writerow({'name': state.theoremName(), 'content': state.theoremContent(),
                                     'value': str(state.value()), 'training': str(state.isForTraining())})
                else:
                    writer.writerow({'name': state.theoremName(), 'content': state.theoremContent(),
                                     'value': "_", 'training': str(state.isForTraining())})
