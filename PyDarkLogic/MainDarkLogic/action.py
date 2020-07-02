class Action:
    def __init__(self, fun, id=None):
        self._fun = fun
        self._id = id

    def fun(self):
        return self._fun

    def id(self):
        return self._id
