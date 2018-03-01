#!/bin/bash
#
# Copyright IBM Corp All Rights Reserved
#
# SPDX-License-Identifier: Apache-2.0
#
# Exit on first error
set -e

# don't rewrite paths for Windows Git Bash users
export MSYS_NO_PATHCONV=1

starttime=$(date +%s)

function FabricInit()
{
	# launch network; create channel and join peer to channel
	cd ../basic-network-one-goleveldb

	if [ "$1" == "restart-container" ]; then
	   ./start.sh $1
	   echo "restart-container, done"
	   exit 0
	
	elif [ "$1" == "do-channel-ops" ]; then
	   ./start.sh $1
	   echo "start, to do-channel-ops"
	
	else 
	   echo "FabricInit err, usage: $0 do-channel-ops | restart-container"
	   exit 1
	fi
}

function FabricAppInit()
{
	# Now launch the CLI container in order to install, instantiate chaincode
	# and prime the ledger with our 10 cars
	docker-compose -f ./docker-compose.yml up -d cli
	
	echo "=================="
	echo "Install the chaincode"
	echo "=================="
	docker exec -e "CORE_PEER_LOCALMSPID=Org1MSP" \
	-e "CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/users/Admin@org1.example.com/msp" \
	cli peer chaincode install -n fabcar -v 1.0 -p github.com/fabcar
	
	echo "=================="
	echo "Instantiate the chaincode"
	echo "=================="
	docker exec -e "CORE_PEER_LOCALMSPID=Org1MSP" \
	-e "CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/users/Admin@org1.example.com/msp" \
	cli peer chaincode instantiate -o orderer.example.com:7050 -C mychannel -n fabcar -v 1.0 -c '{"Args":[""]}' -P "OR ('Org1MSP.member','Org2MSP.member')"
	
	sleep 10
	
	echo "=================="
	echo "Invoke the chaincode"
	echo "=================="
	docker exec -e "CORE_PEER_LOCALMSPID=Org1MSP" \
	-e "CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/users/Admin@org1.example.com/msp" \
	cli peer chaincode invoke -o orderer.example.com:7050 -C mychannel -n fabcar -c '{"function":"initLedger","Args":[""]}'
	
	
	printf "\nTotal setup execution time : $(($(date +%s) - starttime)) secs ...\n\n\n"
	printf "Start by installing required packages run 'npm install'\n"
	printf "Then run 'node enrollAdmin.js', then 'node registerUser'\n\n"
	printf "The 'node invoke.js' will fail until it has been updated with valid arguments\n"
	printf "The 'node query.js' may be run at anytime once the user has been registered\n\n"
}


if [ "$1" != "" ]; then
   FabricInit $1
   echo "FabricInit done"

   FabricAppInit
   echo "FabricAppInit done"
   exit 0

else 
   echo "usage: $0 do-channel-ops | restart-container"
   exit 1
fi

exit 0