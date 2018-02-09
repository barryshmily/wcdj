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

# launch network; create channel and join peer to channel
cd ../basic-network-multi-goleveldb
./start.sh do-channel-ops 

# Now launch the CLI container in order to install, instantiate chaincode
# and prime the ledger with our 10 cars
#docker-compose -f ./docker-compose.yml up -d cli

echo "=================="
echo "Install the chaincode"
echo "=================="
docker exec -e "CORE_PEER_LOCALMSPID=Org1MSP" \
-e "CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/users/Admin@org1.example.com/msp" \
-e "CORE_PEER_ADDRESS=peer0.org1.example.com:7051" \
cli peer chaincode install -n fabcar -v 1.0 -p github.com/fabcar

docker exec -e "CORE_PEER_LOCALMSPID=Org1MSP" \
-e "CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/users/Admin@org1.example.com/msp" \
-e "CORE_PEER_ADDRESS=peer1.org1.example.com:7051" \
cli peer chaincode install -n fabcar -v 1.0 -p github.com/fabcar

docker exec -e "CORE_PEER_LOCALMSPID=Org2MSP" \
-e "CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org2.example.com/users/Admin@org2.example.com/msp" \
-e "CORE_PEER_ADDRESS=peer0.org2.example.com:7051" \
cli peer chaincode install -n fabcar -v 1.0 -p github.com/fabcar

docker exec -e "CORE_PEER_LOCALMSPID=Org2MSP" \
-e "CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org2.example.com/users/Admin@org2.example.com/msp" \
-e "CORE_PEER_ADDRESS=peer1.org2.example.com:7051" \
cli peer chaincode install -n fabcar -v 1.0 -p github.com/fabcar

echo "=================="
echo "Instantiate the chaincode"
echo "=================="
docker exec -e "CORE_PEER_LOCALMSPID=Org1MSP" \
-e "CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/users/Admin@org1.example.com/msp" \
-e "CORE_PEER_ADDRESS=peer0.org1.example.com:7051" \
cli peer chaincode instantiate -o orderer.example.com:7050 -C mychannel -n fabcar -v 1.0 -c '{"Args":[""]}' -P "OR ('Org1MSP.member','Org2MSP.member')"

sleep 10

:<< gerry
echo "=================="
echo "Invoke the chaincode"
echo "=================="
# 注意，如果是AND策略这里invoke的背书检查会失败，因为只请求了一个背书节点

docker exec -e "CORE_PEER_LOCALMSPID=Org1MSP" \
-e "CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/users/Admin@org1.example.com/msp" \
-e "CORE_PEER_ADDRESS=peer0.org1.example.com:7051" \
cli peer chaincode invoke -o orderer.example.com:7050 -C mychannel -n fabcar -c '{"function":"initLedger","Args":[""]}'

docker exec -e "CORE_PEER_LOCALMSPID=Org1MSP" \
-e "CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/users/Admin@org1.example.com/msp" \
-e "CORE_PEER_ADDRESS=peer0.org1.example.com:7051" \
cli peer chaincode invoke -o orderer.example.com:7050 -C mychannel -n fabcar -c '{"function":"createCar","Args":["CAR100","BMW","320","blue","Gerry"]}'


echo "=================="
echo "Query the chaincode"
echo "=================="
docker exec -e "CORE_PEER_LOCALMSPID=Org1MSP" \
-e "CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/users/Admin@org1.example.com/msp" \
-e "CORE_PEER_ADDRESS=peer0.org1.example.com:7051" \
cli peer chaincode invoke -o orderer.example.com:7050 -C mychannel -n fabcar -c '{"function":"queryAllCars","Args":[""]}'

docker exec -e "CORE_PEER_LOCALMSPID=Org1MSP" \
-e "CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/users/Admin@org1.example.com/msp" \
-e "CORE_PEER_ADDRESS=peer0.org1.example.com:7051" \
cli peer chaincode invoke -o orderer.example.com:7050 -C mychannel -n fabcar -c '{"function":"queryCar","Args":["CAR100"]}'
gerry

printf "\nTotal setup execution time : $(($(date +%s) - starttime)) secs ...\n\n\n"
printf "Start by installing required packages run 'npm install'\n"
printf "Then run 'node enrollAdmin.js', then 'node registerUser'\n\n"
printf "The 'node invoke.js' will fail until it has been updated with valid arguments\n"
printf "The 'node query.js' may be run at anytime once the user has been registered\n\n"
