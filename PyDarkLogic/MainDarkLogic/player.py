class Player:
    def __init__(self, name):
        self._name = name
        self._elo = 1200

    def name(self):
        return self._name

    def play(self):
        raise Exception("play method should be overidden")

    def meditate(self):
        pass

    def setTheoremInfo(self):
        pass

    def elo(self):
        return self._elo

    def setElo(self, elo):
        exElo = self._elo
        self._elo = elo
        comment = ""
        if elo >= exElo:
            comment = "(+"+str(exElo - elo)+")"
        else:
            comment = "(-" + str(exElo - elo) + ")"
        print(self._name+"'s elo is "+str(self._elo)+" "+comment)
