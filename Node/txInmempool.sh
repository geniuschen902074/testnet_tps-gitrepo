#!/bin/bash

rpcport=$1

#bitcoin-cli -rpcport=$(($pos+20001)) -rpcuser=test -rpcpassword=test getmempoolinfo | python txmempool.py
bitcoin-cli -rpcport=$rpcport -rpcuser=test -rpcpassword=test getmempoolinfo | python txmempool.py
