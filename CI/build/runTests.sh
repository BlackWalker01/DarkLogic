#!/bin/sh

export LD_LIBRARY_PATH=$(pwd):$LD_LIBRARY_PATH

./LogicGame checkHumanIdentity
ret=$?

./LogicGame checkHumanDoubleNot
ret=$? && $ret

./LogicGame checkHumanExcludedMiddle
ret=$? && $ret

./LogicGame checkPerformanceIdentity
ret=$? && $ret

./LogicGame checkPerformanceDoubleNot
ret=$? && $ret

./LogicGame checkPerformanceNonContradiction
ret=$? && $ret

exit $ret