#!/bin/bash
#
# Copyright IBM Corp All Rights Reserved
#
# SPDX-License-Identifier: Apache-2.0
#
# Exit on first error, print all commands.
set -e

# don't rewrite paths for Windows Git Bash users
export MSYS_NO_PATHCONV=1

function StartContainer()
{
	docker-compose -f docker-compose.yml down
	docker-compose -f docker-compose.yml up -d ca.example.com orderer.example.com peer0.org1.example.com
}

function RestartContainer()
{
	docker-compose -f docker-compose.yml stop
	docker-compose -f docker-compose.yml start ca.example.com orderer.example.com peer0.org1.example.com
}

function ChannelOps()
{
	# wait for Hyperledger Fabric to start
	# incase of errors when running later commands, issue export FABRIC_START_TIMEOUT=<larger number>
	export FABRIC_START_TIMEOUT=10
	#echo ${FABRIC_START_TIMEOUT}
	sleep ${FABRIC_START_TIMEOUT}
	
	echo "=================="
	echo "Create the channel"
	echo "=================="
	export CHANNEL_NAME="mychannel";
	
	# Create the channel
	docker exec -e "CORE_PEER_LOCALMSPID=Org1MSP" \
	-e "CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/msp/users/Admin@org1.example.com/msp" \
	peer0.org1.example.com peer channel create -o orderer.example.com:7050 -c ${CHANNEL_NAME} -f /etc/hyperledger/configtx/channel.tx
	
	
	echo "=================="
	echo "Join the channel"
	echo "=================="
	
	# Join peer0.org1.example.com to the channel.
	docker exec -e "CORE_PEER_LOCALMSPID=Org1MSP" \
	-e "CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/msp/users/Admin@org1.example.com/msp" \
	peer0.org1.example.com peer channel join -b ${CHANNEL_NAME}.block
}


if [ "$1" == "restart-container" ]; then
   RestartContainer
   echo "RestartContainer, done"
   exit 0

elif [ "$1" == "do-channel-ops" ]; then
   StartContainer
   echo "to do-channel-ops"

   ChannelOps

else 
   echo "usage: $0 do-channel-ops | restart-container"
   exit 1
fi

exit 0