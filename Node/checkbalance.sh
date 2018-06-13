#!/bin/bash


#bitcoin-cli -rpcport=$(($pos_offset+20001)) -rpcuser=test -rpcpassword=test getbalance
bitcoin-cli -rpcport=18332 -rpcuser=test -rpcpassword=test getbalance
