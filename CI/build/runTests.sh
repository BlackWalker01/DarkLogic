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

./LogicGame --type test --testName non-contadiction --content "!(a&&!a)" --player ai --timeout 0.016
if [ $? -eq 0 ]
then
  :
else
  ret=1
fi

./LogicGame --type test --testName identity --content "a<=>a" --player ai --timeout 1.5
if [ $? -eq 0 ]
then
  :
else
  ret=1
fi

./LogicGame --type test --testName doubleNot --content "a<=>!!a" --player ai --timeout 21.6
if [ $? -eq 0 ]
then
  :
else
  ret=1
fi

./LogicGame --type test --testName non-contadiction --content "!(a&&!a)" --player evalai --timeout 0.016
if [ $? -eq 0 ]
then
  :
else
  ret=1
fi

./LogicGame --type test --testName identity --content "a<=>a" --player evalai --timeout 0.13
if [ $? -eq 0 ]
then
  :
else
  ret=1
fi

./LogicGame --type test --testName doubleNot --content "a<=>!!a" --player evalai --timeout 2.4
if [ $? -eq 0 ]
then
  :
else
  ret=1
fi

./LogicGame --type test --testName contraposition --content "(a=>b)=>(!b=>!a)" --player evalai --timeout 2.6
if [ $? -eq 0 ]
then
  :
else
  ret=1
fi

./LogicGame --type test --testName excluded-middle --content "a||!a" --player evalai --timeout 11.8
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
