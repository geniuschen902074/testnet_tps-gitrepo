#!/bin/bash

num_blk=$1

bitcoin-cli -rpcport=18332 -rpcuser=test -rpcpassword=test getbestchaintps $num_blk
