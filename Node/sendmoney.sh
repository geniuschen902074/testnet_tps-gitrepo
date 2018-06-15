#!/bin/bash

rpcport=$1
addr=$2
ammount=$3

#bitcoin-cli -rpcport=$(($pos+20001)) -rpcuser=test -rpcpassword=test sendtoaddress $addr $ammount
bitcoin-cli -rpcport=$rpcport -rpcuser=test -rpcpassword=test sendtoaddress $addr $ammount
