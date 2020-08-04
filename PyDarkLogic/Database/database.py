import csv
import random as rand
from Database.state import State
import os.path


class Database:

    def __init__(self, name):
        self._name = name
        self._theoremDb = {}
        print("loading memory...")
        self.load()
        print("memory loaded")

    def load(self):
        self._theoremDb = {}
        if os.path.isfile(self._name):
            with open(self._name, newline='') as csvfile:
                reader = csv.DictReader(csvfile)
                for row in reader:
                    state = None
                    if row["value"] != "_":
                        state = State(content=row["content"], name=row["name"],
                                      value=int(row["value"]))
                    else:
                        state = State(content=row["content"], name=row["name"])
                    self._theoremDb[state.theoremContent()] = state
        else:
            with open(self._name, 'w', newline='') as csvfile:
                fieldnames = ["name", "content", "value"]
                writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
                writer.writeheader()
        return self._theoremDb

    def getDatas(self):
        return self._theoremDb

    def export(self, states):
        # update local database
        print("[DEBUG] Update local memory with "+str(len(states))+" states ...")
        for state in states:
            if state.theoremContent() in self._theoremDb:
                exState = self._theoremDb[state.theoremContent()]
                if not exState.isEvaluated():
                    self._theoremDb[state.theoremContent()] = state
                elif state.isEvaluated() and exState.value() > state.value():
                    self._theoremDb[state.theoremContent()] = state
            else:
                self._theoremDb[state.theoremContent()] = state

        # export datas database
        print("[DEBUG] Export datas to long-term memory...")
        with open(self._name, 'w', newline='') as csvfile:
            fieldnames = ["name", "content", "value"]
            writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
            writer.writeheader()
            for state in self._theoremDb.values():
                if state.isEvaluated():
                    writer.writerow({'name': state.theoremName(), 'content': state.theoremContent(),
                                     'value': str(state.value())})
                else:
                    writer.writerow({'name': state.theoremName(), 'content': state.theoremContent(),
                                     'value': "_"})

