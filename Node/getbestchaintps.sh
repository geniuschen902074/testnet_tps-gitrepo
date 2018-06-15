#!/bin/bash

rpcport=$1
num_blk=$2

bitcoin-cli -rpcport=$rpcport -rpcuser=test -rpcpassword=test getbestchaintps $num_blk
