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
        self.addTheorem("Non-contradiction", "!(a&&!a)", 500)
        self.addTheorem("Semi-Identity", "a=>a", 700)
        self.addTheorem("Semi-DoubleNot", "a=>!!a", 750)
        self.addTheorem("Identity", "a<=>a", 1200)
        self.addTheorem("DoubleNot", "a<=>!!a", 1450)
        self.addTheorem("Identity and True", "(a<=>a)&&True", 1750)
        self.addTheorem("Double-Not and True", "(a<=>!!a)&&True", 1750)
        self.addTheorem("Semi-Identity and Semi-Double-Not", "(a=>a)&&(a=>!!a)", 1800)
        self.addTheorem("Contraposition", "(a=>b)=>(!b=>!a)", 1850)
        self.addTheorem("Semi-Identity and Semi-Double-Not and True", "(a=>a)&&(a=>!!a)&&True", 1900)
        self.addTheorem("Contraposition and True", "((a=>b)=>(!b=>!a))&&True", 1950)
        self.addTheorem("ExcludedMiddle", "a||!a", 2400)
        self.addTheorem("Semi-De Morgan law [R]", "!(a&&b)=>(!a||!b)", 2950)
        self.addTheorem("Semi-De Morgan law [L]", "(!a||!b)=>!(a&&b)", 2950)
        self.addTheorem("Semi-De Morgan law (2) [R]", "!(a||b)=>(!a&&!b)", 2950)
        self.addTheorem("Semi-De Morgan law (2) [L]", "(!a&&!b)<=>!(a||b)", 2950)
        self.addTheorem("Double-And", "a&&a<=>a", 3000)
        self.addTheorem("Double-Or", "a||a<=>a", 3000)
        self.addTheorem("Identity-Double-Not", "(a<=>a)&&(a<=>!!a)", 3000)
        self.addTheorem("Peirce law", "((a=>b)=>a)=>a", 3000)
        self.addTheorem("Modus ponens", "((a=>b)&&a)=>b", 3050)
        self.addTheorem("De Morgan law", "!(a&&b)<=>(!a||!b)", 3150)
        self.addTheorem("De Morgan law (2)", "!(a||b)<=>(!a&&!b)", 3250)
        self.addTheorem("Modus tollens", "((a=>b)&&!b)=>!a", 3300)
        self.addTheorem("Modus barbara", "((a=>b)&&(b=>c))=>(a=>c)", 3350)
        self.addTheorem("&& Distributivity", "(a&&(b||c))<=>((a&&b)||(a&&c))", 3350)
        self.addTheorem("|| Distributivity", "(a||(b&&c))<=>((a||b)&&(a||c))", 3350)

    def getRandomTheorem(self, elo=10 ** 6):
        allKeys = []
        remainKeys = []
        for key in self._db.keys():
            allKeys.append(key)
            if elo - 400 <= self._db[key].elo() <= elo + 400:
                remainKeys.append(key)
        if len(remainKeys) > 0:
            return self._db[remainKeys[rand.randint(0, len(remainKeys) - 1)]]
        else:
            return self._db[allKeys[rand.randint(0, len(allKeys) - 1)]]

    def addTheorem(self, name, content, elo):
        thm = Theorem(name, content, elo)
        if content not in self._db:
            self._db[content] = thm
