#!/bin/bash

#CONF=/home/user/Desktop/rpcscript/testnet-bitcoin.conf
#MCONF=/home/user/Desktop/rpcscript/testnet-mbitcoin.conf


#txfee=$1
datadir=$1

#cp $MCONF /home/user/Desktop/rpcscript/master/bitcoin.conf
bitcoind -daemon -testnet -reindex -txindex=1 -datadir="$datadir" #-paytxfee=$txfee  -datadir="/home/user/Desktop/rpcscript/master" ##port/rpcport default 18333/18332


#cp $CONF /home/user/Desktop/rpcscript/${dirarr[$i]}/bitcoin.conf
#bitcoind -daemon -testnet -reindex -txindex=1 -paytxfee=$txfee -port=$(($i+30001)) -rpcport=$(($i+20001)) -datadir="/home/user/Desktop/rpcscript/${dirarr[$i]}"
