#!/bin/bash

addr=$1
ammount=$2

#bitcoin-cli -rpcport=$(($pos+20001)) -rpcuser=test -rpcpassword=test sendtoaddress $addr $ammount
bitcoin-cli -rpcport=18332 -rpcuser=test -rpcpassword=test sendtoaddress $addr $ammount
