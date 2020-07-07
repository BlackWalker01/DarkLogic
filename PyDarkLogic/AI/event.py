from enum import Enum

class Event(object):
    class EventEnum(Enum):
        START=0
        STOP=1
        STOP_THREAD=2
    def __init__(self, threadIdx, type):
        self._threadIdx=threadIdx
        self._type=type

    def threadIdx(self):
        return self._threadIdx

    def type(self):
        return self._type