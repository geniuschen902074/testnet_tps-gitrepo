#!/bin/bash

rpcport=$1

#bitcoin-cli -rpcport=$(($pos+20001)) -rpcuser=test -rpcpassword=test getchaintips | python chaintip.py
bitcoin-cli -rpcport=$rpcport -rpcuser=test -rpcpassword=test getchaintips | python chaintip.py
