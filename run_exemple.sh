#!/bin/bash
echo "./exemple-options"
./exemple-options
echo " "
echo " "

echo "export OPT_ADDR=127.16.15.1"
echo "./exemple-options"
export OPT_ADDR="127.16.15.1"
./exemple-options
echo " "
echo " "

echo "export OPT_PORT=5000"
echo "./exemple-options --address 127.0.0.1"
export OPT_PORT="5000"
./exemple-options --address 127.0.0.1
echo " "
echo " "

echo "export OPT_AUTO="
echo "./exemple-options -p 6000 -o delay=5"
export OPT_AUTO=
./exemple-options -p 6000 -o delay=5
echo " "
echo " "

echo "./exemple-options -p 6000 -o delay=5,nonauto et d autres arguments"
./exemple-options -p 6000 -o delay=5,nonauto et d autres arguments
