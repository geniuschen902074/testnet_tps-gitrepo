#!/bin/bash

if [ "$#" -ne 13 ]; then
	echo "usage: ./launcher.sh [(host)name of (machine)container where the dns seeder runs] seedname order txfee datadir fNoRetarget fAllowMinDiffBlk nTargetTimeSpan nTargetSpace seednum ip port rpcport"
	exit 1
fi

dnsholder=$1
seedname=$2
order=$3

txfee=$4
datadir=$5
fPNR=$6
fPAMDB=$7
nPTTS=$8
nPTS=$9

seednum=${10}
ip=${11}
port=${12}
rpcport=${13}

#set up static ip, NS record, etc

#copy bitcoin-seeder/ to master container
#run up the master container
#in it, sipa seeder(bitcoin-seeder/) is compiled and run

sleep 3

g++ -std=c++11 launcher.cpp -o launcher
./launcher $dnsholder $seedname $order $txfee $datadir $fPNR $fPAMDB $nPTTS $nPTS $seednum $ip $port $rpcport > setup
cp setup Node/

#copy Node/ to each client container's folder
#run up each client container
#in each of them, new core and Node/Node.cpp is compiled and run
cd Node/
rm -f Node
g++ -std=c++11 Node.cpp -o Node
./Node
