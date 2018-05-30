#!/bin/bash

if [ "$#" -ne 9 ]; then
	echo "usage: ./launcher.sh [(host)name of (machine)container where the dns seeder runs] seedname order txfee datadir fNoRetarget fAllowMinDiffBlk nTargetTimeSpan nTargetSpace"
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

#set up static ip, NS record, etc

#copy bitcoin-seeder/ to master container
#run up the master container
#in it, sipa seeder(bitcoin-seeder/) is compiled and run

sleep 3

g++ -std=c++11 launcher.cpp -o launcher
./launcher $dnsholder $seedname $order $txfee $datadir $fPNR $fPAMDB $nPTTS $nPTS
cp setup Node/

#copy Node/ to each client container's folder
#run up each client container
#in each of them, new core and Node/Node.cpp is compiled and run
