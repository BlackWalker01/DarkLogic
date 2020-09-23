#!/bin/sh

export LD_LIBRARY_PATH=$(pwd):$LD_LIBRARY_PATH
ret=0

# C++ tests
./LogicGame --type test --testName checkHumanIdentity
if [ $? -eq 0 ]
then
  :
else
  ret=1
fi

./LogicGame --type test --testName checkHumanDoubleNot
if [ $? -eq 0 ]
then
  :
else
  ret=1
fi

./LogicGame --type test --testName checkHumanExcludedMiddle
if [ $? -eq 0 ]
then
  :
else
  ret=1
fi

./LogicGame --type test --testName checkPerformanceIdentity
if [ $? -eq 0 ]
then
  :
else
  ret=1
fi

./LogicGame --type test --testName checkPerformanceDoubleNot
if [ $? -eq 0 ]
then
  :
else
  ret=1
fi

./LogicGame --type test --testName checkPerformanceNonContradiction
if [ $? -eq 0 ]
then
  :
else
  ret=1
fi

# Python Tests
export PYTHONPATH=$(pwd)
cd ../PyDarkLogic

python3 main.py --type test --testName HumanIdentity
if [ $? -eq 0 ]
then
  :
else
  ret=1
fi

python3 main.py --type test --testName HumanDoubleNot
if [ $? -eq 0 ]
then
  :
else
  ret=1
fi

python3 main.py --type test --testName HumanExcludedMiddle
if [ $? -eq 0 ]
then
  :
else
  ret=1
fi

python3 main.py --type test --testName PerformanceIdentity
if [ $? -eq 0 ]
then
  :
else
  ret=1
fi

python3 main.py --type test --testName PerformanceDoubleNot
if [ $? -eq 0 ]
then
  :
else
  ret=1
fi

python3 main.py --type test --testName PerformanceNonContradiction
if [ $? -eq 0 ]
then
  :
else
  ret=1
fi

python3 main.py --type test --testName Parser
if [ $? -eq 0 ]
then
  :
else
  ret=1
fi

exit $ret
