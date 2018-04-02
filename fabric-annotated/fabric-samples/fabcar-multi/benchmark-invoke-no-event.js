'use strict';
/*
* Copyright IBM Corp All Rights Reserved
*
* SPDX-License-Identifier: Apache-2.0
*/
/*
 * Chaincode Invoke
 */

var Fabric_Client = require('fabric-client');
var path = require('path');
var util = require('util');
var os = require('os');

var async = require('async');

//
var fabric_client = new Fabric_Client();

// setup the fabric network
var channel = fabric_client.newChannel('mychannel');
var peer = fabric_client.newPeer('grpc://localhost:7051');
var peer2 = fabric_client.newPeer('grpc://localhost:9051');
channel.addPeer(peer);
channel.addPeer(peer2);
var order = fabric_client.newOrderer('grpc://localhost:7050')
channel.addOrderer(order);

//
var member_user = null;
var store_path = path.join(__dirname, 'hfc-key-store');
console.log('Store path:'+store_path);
var user = null;

var args = process.argv.splice(2);
var total=args[0];
var concurrency = args[1];
var offset = 0;
if (args.length == 0) {
    total = 1000;
    concurrency = 10;	
}
if (args.length >= 3) {
    offset = args[2];
}
var first_index = offset * total;

// create the key value store as defined in the fabric-client/config/default.json 'key-value-store' setting
function do_invoke(index, callback) {
var tx_id = null;
var t1 = new Date().getTime();
Fabric_Client.newDefaultKeyValueStore({ path: store_path
}).then((state_store) => {
    if (index != first_index) {
        return user;
    }
    console.log("index", index);
	// assign the store to the fabric client
	fabric_client.setStateStore(state_store);
	var crypto_suite = Fabric_Client.newCryptoSuite();
	// use the same location for the state store (where the users' certificate are kept)
	// and the crypto store (where the users' keys are kept)
	var crypto_store = Fabric_Client.newCryptoKeyStore({path: store_path});
	crypto_suite.setCryptoKeyStore(crypto_store);
	fabric_client.setCryptoSuite(crypto_suite);

	// get the enrolled user from persistence, this user will sign all requests
	user = fabric_client.getUserContext('user1', true);
	return user
}).then((user_from_store) => {
	if (user_from_store && user_from_store.isEnrolled()) {
		console.log('Successfully loaded user1 from persistence');
		member_user = user_from_store;
	} else {
		throw new Error('Failed to get user1.... run registerUser.js');
	}

	// get a transaction id object based on the current user assigned to fabric client
	tx_id = fabric_client.newTransactionID();
	console.log("Assigning transaction_id: ", tx_id._transaction_id);

	// createCar chaincode function - requires 5 args, ex: args: ['CAR12', 'Honda', 'Accord', 'Black', 'Tom'],
	// changeCarOwner chaincode function - requires 2 args , ex: args: ['CAR10', 'Barry'],
	// must send the proposal to endorsing peers
    var car = "CAR" + index;
	var request = {
		//targets: let default to the peer assigned to the client
		chaincodeId: 'fabcar',
		fcn: 'createCar',
		args: [car, 'Chevy', 'Volt', 'Red', 'Nick'],
		chainId: 'mychannel',
		txId: tx_id
	};

	// send the transaction proposal to the peers
	return channel.sendTransactionProposal(request);
}).then((results) => {
	var proposalResponses = results[0];
	var proposal = results[1];
	let isProposalGood = false;
	if (proposalResponses && proposalResponses[0].response &&
		proposalResponses[0].response.status === 200) {
			isProposalGood = true;
			console.log('Transaction proposal was good');
		} else {
			console.error('Transaction proposal was bad');
		}
	if (isProposalGood) {
		console.log(util.format(
			'Successfully sent Proposal and received ProposalResponse: Status - %s, message - "%s"',
			proposalResponses[0].response.status, proposalResponses[0].response.message));

		// build up the request for the orderer to have the transaction committed
		var request = {
			proposalResponses: proposalResponses,
			proposal: proposal
		};

		// set the transaction listener and set a timeout of 30 sec
		// if the transaction did not get committed within the timeout period,
		// report a TIMEOUT status
		var transaction_id_string = tx_id.getTransactionID(); //Get the transaction ID string to be used by the event processing
		var promises = [];

		var sendPromise = channel.sendTransaction(request);
		promises.push(sendPromise); //we want the send transaction first, so that we know where to check status

		// get an eventhub once the fabric client has a user assigned. The user
		// is required bacause the event registration must be signed

		return Promise.all(promises);
	} else {
		console.error('Failed to send Proposal or receive valid response. Response null or status is not 200. exiting...');
		throw new Error('Failed to send Proposal or receive valid response. Response null or status is not 200. exiting...');
	}
}).then((results) => {
	console.log('Send transaction promise and event listener promise have completed');
	// check the results in the order the promises were added to the promise all list
	if (results && results[0] && results[0].status === 'SUCCESS') {
		console.log('Successfully sent transaction to the orderer.');
	} else {
		console.error('Failed to order the transaction. Error code: ' + response.status);
	}

	var t2 = new Date().getTime();
	console.error("tx_id:" + tx_id._transaction_id , ", time:", t2 - t1);
	callback();
}).catch((err) => {
	console.error('Failed to invoke successfully :: ' + err);
	var t2 = new Date().getTime();
	console.error("tx_id:" + tx_id._transaction_id , ", time:", t2 - t1);
	callback();
});
 
}

//--------------------------------------------------------

var tStart = new Date().getTime();

var queue = async.queue(do_invoke, concurrency);

queue.drain = function() {
    console.log("all task is processing");
    var tEnd = new Date().getTime();
    console.log("---------------total time:", tEnd -tStart);
}

queue.saturated = function() {
    console.log("all worker to be used...");
}

queue.empty = function() {
}

total = parseInt(total);
offset = parseInt(offset);
console.log("begin:", offset * total, ", end:", (1 + offset)* total)

for(var i = offset * total; i < (1 + offset) * total; i++) {
    queue.push(i);
}




