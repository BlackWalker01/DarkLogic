class Player:
    def __init__(self, name):
        self._name=name

    def name(self):
        return self._name

    def play(self):
        raise Exception("play method should be overidden")

    def meditate(self):
        pass

    def setTheoremInfo(self, thName, thContent):
        pass
