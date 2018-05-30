#!/bin/bash

numblktomine=$1 #default 1

#bitcoin-cli -rpcport=$(($pos+20001)) -rpcuser=test -rpcpassword=test generate $numblktomine
bitcoin-cli  -rpcuser=test -rpcpassword=test generate $numblktomine
