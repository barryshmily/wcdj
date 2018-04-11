#!/bin/bash


echo "=================="
echo "Create the channel"
echo "=================="

export CORE_PEER_LOCALMSPID="Org1MSP"
export CORE_PEER_MSPCONFIGPATH="/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.byfn-solo/users/Admin@org1.byfn-solo/msp"
peer channel create -o orderer.byfn-solo:7050 -c mychannel -f /opt/gopath/src/github.com/hyperledger/fabric/peer/channel-artifacts/channel.tx

echo "=================="
echo "Join the channel"
echo "=================="
# Join peer0.org1.example.com to the channel.
export CORE_PEER_ADDRESS="peer0-org1.byfn-solo:7051"
export CORE_PEER_LOCALMSPID="Org1MSP"
export CORE_PEER_MSPCONFIGPATH="/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.byfn-solo/users/Admin@org1.byfn-solo/msp"
peer channel join -b mychannel.block

echo "=================="
echo "Install the chaincode"
echo "=================="
export CORE_PEER_ADDRESS="peer0-org1.byfn-solo:7051"
export CORE_PEER_LOCALMSPID="Org1MSP"
export CORE_PEER_MSPCONFIGPATH="/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.byfn-solo/users/Admin@org1.byfn-solo/msp"
peer chaincode install -n fabcar -v 1.0 -p github.com/chaincode/fabcar

echo "=================="
echo "Instantiate the chaincode"
echo "=================="
export CORE_PEER_ADDRESS="peer0-org1.byfn-solo:7051"
export CORE_PEER_LOCALMSPID="Org1MSP"
export CORE_PEER_MSPCONFIGPATH="/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.byfn-solo/users/Admin@org1.byfn-solo/msp"
peer chaincode instantiate -o orderer.byfn-solo:7050 -C mychannel -n fabcar -v 1.0 -c '{"Args":[""]}' -P "OR ('Org1MSP.member','Org2MSP.member')"

sleep 10

echo "=================="
echo "Invoke the chaincode"
echo "=================="
export CORE_PEER_ADDRESS="peer0-org1.byfn-solo:7051"
export CORE_PEER_LOCALMSPID="Org1MSP"
export CORE_PEER_MSPCONFIGPATH="/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.byfn-solo/users/Admin@org1.byfn-solo/msp"
peer chaincode invoke -o orderer.byfn-solo:7050 -C mychannel -n fabcar -c '{"function":"initLedger","Args":[""]}'

echo "=================="
echo "Query the chaincode"
echo "=================="
export CORE_PEER_ADDRESS="peer0-org1.byfn-solo:7051"
export CORE_PEER_LOCALMSPID="Org1MSP"
export CORE_PEER_MSPCONFIGPATH="/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.byfn-solo/users/Admin@org1.byfn-solo/msp"
peer chaincode invoke -o orderer.byfn-solo:7050 -C mychannel -n fabcar -c '{"function":"queryAllCars","Args":[""]}'