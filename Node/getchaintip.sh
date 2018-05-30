#!/bin/bash


#bitcoin-cli -rpcport=$(($pos+20001)) -rpcuser=test -rpcpassword=test getchaintips | python chaintip.py
bitcoin-cli  -rpcuser=test -rpcpassword=test getchaintips | python chaintip.py
