#!/bin/sh

export LD_LIBRARY_PATH=$(pwd):$LD_LIBRARY_PATH

# C++ tests
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

# Python Tests
export PYTHONPATH=$(pwd)
cd ../PyDarkLogic

python3 main.py --type test --testName HumanIdentity
ret=$?

python3 main.py --type test --testName HumanDoubleNot
ret=$? && $ret

python3 main.py --type test --testName HumanExcludedMiddle
ret=$? && $ret

python3 main.py --type test --testName PerformanceIdentity
ret=$? && $ret

python3 main.py --type test --testName PerformanceDoubleNot
ret=$? && $ret

python3 main.py --type test --testName PerformanceNonContradiction
ret=$? && $ret

python3 main.py --type test --testName Parser
ret=$? && $ret

exit $ret
