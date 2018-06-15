#!/bin/bash

rpcport=$1
numblktomine=$2 #default 1

#bitcoin-cli -rpcport=$(($pos+20001)) -rpcuser=test -rpcpassword=test generate $numblktomine
bitcoin-cli -rpcport=$rpcport -rpcuser=test -rpcpassword=test generate $numblktomine
