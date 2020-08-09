import random as rand


class Theorem:
    def __init__(self, name, content, elo):
        self._name = name
        self._content = content
        self._elo = elo

    def name(self):
        return self._name

    def content(self):
        return self._content

    def elo(self):
        return self._elo


class DbTheorem:
    def __init__(self):
        self._db = {}
        self.addTheorem("Identity", "b0<=>b0", 1500)
        self.addTheorem("DoubleNot", "b0<=>!!b0", 1500)
        self.addTheorem("ExcludedMiddle", "b0||!b0", 1800)
        self.addTheorem("Peirce law", "((b0=>b1)=>b0)=>b0", 1900)
        self.addTheorem("Non-contradiction", "!(b0&&!b0)", 800)
        self.addTheorem("De Morgan law", "!(b0&&b1)<=>(!b0||!b1)", 2000)
        self.addTheorem("De Morgan law (2)", "!(b0||b1)<=>(!b0&&!b1)", 2000)
        self.addTheorem("Contraposition", "(b0=>b1)=>(!b1=>!b0)", 1900)
        self.addTheorem("Modus ponens", "((b0=>b1)&&b0)=>b1", 1900)
        self.addTheorem("Modus tollens", "((b0=>b1)&&!b1)=>!b0", 2000)
        self.addTheorem("Modus barbara", "((b0=>b1)&&(b1=>b2))=>(b0=>b2)", 2200)
        self.addTheorem("&& Distributivity", "(b0&&(b1||b2))<=>((b0&&b1)||(b0&&b2))", 2200)
        self.addTheorem("|| Distributivity", "(b0||(b1&&b2))<=>((b0||b1)&&(b0||b2))", 2200)

    def getRandomTheorem(self):
        keys = []
        for key in self._db.keys():
            keys.append(key)
        return self._db[keys[rand.randint(0, len(keys) - 1)]]

    def addTheorem(self, name, content, elo):
        thm = Theorem(name, content, elo)
        if content not in self._db:
            self._db[content] = thm
