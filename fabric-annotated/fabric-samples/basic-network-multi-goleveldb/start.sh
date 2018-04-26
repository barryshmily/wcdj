#!/bin/bash
#
# Copyright IBM Corp All Rights Reserved
#
# SPDX-License-Identifier: Apache-2.0
#
# Exit on first error, print all commands.
#set -ev
# Exit on first error
set -e

# don't rewrite paths for Windows Git Bash users
export MSYS_NO_PATHCONV=1


function StartDocker()
{
   #docker-compose -f docker-compose.yml down
   docker-compose -f docker-compose.yml up -d ca0.example.com ca1.example.com orderer.example.com peer0.org1.example.com peer1.org1.example.com peer0.org2.example.com peer1.org2.example.com cli
}

function ChannelOps()
{
:<< gerry
echo "PROBLEM"
# PROBLEM: Error: genesis block file not found open mychannel.block: no such file or directory
# BECAUSE: create mychannel.block just in the current directory

# Create the channel
docker exec -e "CORE_PEER_LOCALMSPID=Org1MSP" -e "CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/msp/users/Admin@org1.example.com/msp" peer0.org1.example.com peer channel create -o orderer.example.com:7050 -c mychannel -f /etc/hyperledger/configtx/channel.tx

# Join peer0.org1.example.com to the channel.
docker exec -e "CORE_PEER_LOCALMSPID=Org1MSP" -e "CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/msp/users/Admin@org1.example.com/msp" peer0.org1.example.com peer channel join -b mychannel.block
# Join peer1.org1.example.com to the channel.
docker exec -e "CORE_PEER_LOCALMSPID=Org1MSP" -e "CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/msp/users/Admin@org1.example.com/msp" peer1.org1.example.com peer channel join -b mychannel.block
# Join peer0.org2.example.com to the channel.
docker exec -e "CORE_PEER_LOCALMSPID=Org2MSP" -e "CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/msp/users/Admin@org2.example.com/msp" peer0.org2.example.com peer channel join -b mychannel.block
# Join peer1.org2.example.com to the channel.
docker exec -e "CORE_PEER_LOCALMSPID=Org2MSP" -e "CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/msp/users/Admin@org2.example.com/msp" peer1.org2.example.com peer channel join -b mychannel.block
gerry


echo "=================="
echo "Create the channel"
echo "=================="

export CHANNEL_NAME="mychannel";
#export CORE_PEER_LOCALMSPID="Org1MSP"; 
#export CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/users/Admin@org1.example.com/msp;
#export CORE_PEER_ADDRESS=peer0.org1.example.com:7051; 

docker exec \
-e "CORE_PEER_LOCALMSPID=Org1MSP" \
-e "CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/users/Admin@org1.example.com/msp" \
-e "CORE_PEER_ADDRESS=peer0.org1.example.com:7051" \
cli peer channel create -o orderer.example.com:7050 -c ${CHANNEL_NAME} -f /etc/hyperledger/configtx/channel.tx


echo "=================="
echo "Join the channel"
echo "=================="

docker exec \
-e "CORE_PEER_LOCALMSPID=Org1MSP" \
-e "CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/users/Admin@org1.example.com/msp" \
-e "CORE_PEER_ADDRESS=peer0.org1.example.com:7051" \
cli peer channel join -b ${CHANNEL_NAME}.block

docker exec \
-e "CORE_PEER_LOCALMSPID=Org1MSP" \
-e "CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/users/Admin@org1.example.com/msp" \
-e "CORE_PEER_ADDRESS=peer1.org1.example.com:7051" \
cli peer channel join -b ${CHANNEL_NAME}.block

docker exec \
-e "CORE_PEER_LOCALMSPID=Org2MSP" \
-e "CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org2.example.com/users/Admin@org2.example.com/msp" \
-e "CORE_PEER_ADDRESS=peer0.org2.example.com:7051" \
cli peer channel join -b ${CHANNEL_NAME}.block

docker exec \
-e "CORE_PEER_LOCALMSPID=Org2MSP" \
-e "CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org2.example.com/users/Admin@org2.example.com/msp" \
-e "CORE_PEER_ADDRESS=peer1.org2.example.com:7051" \
cli peer channel join -b ${CHANNEL_NAME}.block

}


if [ "$1" == "no-channel-ops" ]; then
   StartDocker
   echo "no-channel-ops, done"
   exit 0

elif [ "$1" == "do-channel-ops" ]; then
   StartDocker
   echo "to do-channel-ops"

   # wait for Hyperledger Fabric to start
   # incase of errors when running later commands, issue export FABRIC_START_TIMEOUT=<larger number>
   export FABRIC_START_TIMEOUT=10
   #echo ${FABRIC_START_TIMEOUT}
   sleep ${FABRIC_START_TIMEOUT}

   ChannelOps

else 
   echo "usage: $0 do-channel-ops | no-channel-ops"
   exit 1
fi

exit 0
