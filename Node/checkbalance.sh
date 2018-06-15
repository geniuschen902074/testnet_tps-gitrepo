#!/bin/bash

rpcport=$1

#bitcoin-cli -rpcport=$(($pos_offset+20001)) -rpcuser=test -rpcpassword=test getbalance
bitcoin-cli -rpcport=$rpcport -rpcuser=test -rpcpassword=test getbalance
