#!/bin/bash

#one argument: #node
#dirarr=(A B C D E F G H I J)

rpcport=$1
num_node=$2

for(( i=0 ; i<$((num_node)) ; i=$((i+1)) ))
do
	bitcoin-cli -rpcport=$rpcport -rpcuser=test -rpcpassword=test getnewaddress
done
