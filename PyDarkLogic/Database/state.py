class State:
    def __init__(self, name, content, **kwargs):
        self._theoremName = name
        self._theoremContent = content
        self._value = None
        if "value" in kwargs:
            self._value = int(kwargs["value"])

    def theoremName(self):
        return self._theoremName

    def theoremContent(self):
        return self._theoremContent

    def isEvaluated(self):
        return self._value is not None

    def value(self):
        return self._value

    def __str__(self):
        return self._theoremContent + "," + self._theoremName + "," + (str(self._value) if self._value else "_")


"""
id = thContent:action1;action2
"""
"""
def parseId(id):
    k = 0
    c = '/0'
    thContent = ""
    while c != ':':
        c = id[k]
        if c != ':':
            thContent += c
        k += 1

    actions = []
    crtAction = ""
    while k < len(id):
        c = id[k]
        if c == ';':
            if crtAction != "":
                actions.append(int(crtAction))
                crtAction = ""
        else:
            crtAction += c
        k += 1
    if crtAction != "":
        actions.append(int(crtAction))
    return thContent, actions
"""

"""
state = State("first", content="a<=>a", value=10)
print(str(state))
state = State(name="second", content="a||!a", value=10)
print(str(state))"""
