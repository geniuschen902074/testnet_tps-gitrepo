#!/bin/bash

num_blk=$1

bitcoin-cli  -rpcuser=test -rpcpassword=test getbestchaintps $num_blk
