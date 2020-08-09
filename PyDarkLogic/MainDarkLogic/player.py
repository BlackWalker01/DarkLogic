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
        self._elo = elo
        print(self._name+"'s elo is "+str(self._elo))
