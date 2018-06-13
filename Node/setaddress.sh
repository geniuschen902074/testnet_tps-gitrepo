#!/bin/bash

#one argument: #node
#dirarr=(A B C D E F G H I J)

num_node=$1

for(( i=0 ; i<$((num_node)) ; i=$((i+1)) ))
do
	bitcoin-cli -rpcport=18332 -rpcuser=test -rpcpassword=test getnewaddress
done
