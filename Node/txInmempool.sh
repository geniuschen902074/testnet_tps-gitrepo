#!/bin/bash


#bitcoin-cli -rpcport=$(($pos+20001)) -rpcuser=test -rpcpassword=test getmempoolinfo | python txmempool.py
bitcoin-cli -rpcport=18332 -rpcuser=test -rpcpassword=test getmempoolinfo | python txmempool.py
