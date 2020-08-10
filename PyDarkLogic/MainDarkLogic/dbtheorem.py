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
        self.addTheorem("Identity", "a<=>a", 1500)
        self.addTheorem("Semi-Identity", "a=>a", 1200)
        self.addTheorem("DoubleNot", "a<=>!!a", 1500)
        self.addTheorem("Semi-DoubleNot", "a=>!!a", 1200)
        self.addTheorem("ExcludedMiddle", "a||!a", 1800)
        self.addTheorem("Peirce law", "((a=>b)=>a)=>a", 1900)
        self.addTheorem("Non-contradiction", "!(a&&!a)", 800)
        self.addTheorem("De Morgan law", "!(a&&b)<=>(!a||!b)", 2000)
        self.addTheorem("De Morgan law (2)", "!(a||b)<=>(!a&&!b)", 2000)
        self.addTheorem("Contraposition", "(a=>b)=>(!b=>!a)", 1900)
        self.addTheorem("Modus ponens", "((a=>b)&&a)=>b", 1900)
        self.addTheorem("Modus tollens", "((a=>b)&&!b)=>!a", 2000)
        self.addTheorem("Modus barbara", "((a=>b)&&(b=>c))=>(a=>c)", 2200)
        self.addTheorem("&& Distributivity", "(a&&(b||c))<=>((a&&b)||(a&&c))", 2200)
        self.addTheorem("|| Distributivity", "(a||(b&&c))<=>((a||b)&&(a||c))", 2200)

    def getRandomTheorem(self):
        keys = []
        for key in self._db.keys():
            keys.append(key)
        return self._db[keys[rand.randint(0, len(keys) - 1)]]

    def addTheorem(self, name, content, elo):
        thm = Theorem(name, content, elo)
        if content not in self._db:
            self._db[content] = thm
