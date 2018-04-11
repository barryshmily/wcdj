

http://fabric-chaintool.readthedocs.io/en/latest/
http://fabric-chaintool.readthedocs.io/en/latest/getting-started/


chaintool is a utility to assist in various phases of Hyperledger Fabric chaincode development, such as compilation, test, packaging, and deployment. A chaincode app developer may express the interface to their application in a highlevel interface definition language, and chaintool will generate (1) chaincode stubs and (2) package the chaincode for convenient deployment.


## install

[Ubuntu 安装 JDK 7 / JDK8 的两种方式](https://www.cnblogs.com/a2211009/p/4265225.html)

[fabric-chaintool](https://github.com/hyperledger/fabric-chaintool/releases)

[Golang Chaincode Platform](http://fabric-chaintool.readthedocs.io/en/latest/platforms/golang/#golang-chaincode-platform)

[Node SDK - fabric-client](https://www.npmjs.com/package/fabric-client)

``` 
# chaintool -h


chaintool version: v1.0.0

Usage: chaintool [general-options] action [action-options]

General Options:
  -v, --version  Print the version and exit
  -h, --help

Actions:
  package -> Package the chaincode into a CAR file for deployment
  ls -> List the contents of a CAR file
  buildcar -> Build the chaincode project from a CAR file
  inspect -> Retrieves metadata from a running instance
  build -> Build the chaincode project
  env -> Display variables used in the build environment
  proto -> Compiles a CCI file to a .proto
  clean -> Clean the chaincode project
  unpack -> Unpackage a CAR file

(run "chaintool <action> -h" for action specific help)
```

## test

* Phase 1: Chaincode Development

* Phase 2: Client Development

* Phase 3: Deployment

* Phase 4: Interacting with your service

demo:

https://github.com/hyperledger/fabric-chaintool/tree/master/examples/example02

* app - contains a org.hyperledger.chaincode.golang platform based chaincode application.
	* This is the code deployed to the blockchain
* client - client applications for interacting with the chaincode application
	* nodejs - A simple demonstration of using nodejs.
	* cljs - A complete client for example02 written in ClojureScript


```
~/fabric-chaintool-master/examples/example02# tree 
.
├── app
│   ├── chaincode.yaml
│   └── src
│       ├── chaincode
│       │   └── chaincode_example02.go
│       └── interfaces
│           ├── appinit.cci
│           └── org.hyperledger.chaincode.example02.cci
├── client
│   ├── cljs
│   │   ├── Makefile
│   │   ├── project.clj
│   │   ├── protos
│   │   │   ├── appinit.proto
│   │   │   └── org.hyperledger.chaincode.example02.proto
│   │   ├── sample.config
│   │   └── src
│   │       ├── example02
│   │       │   ├── api.cljs
│   │       │   ├── connection.cljs
│   │       │   ├── main.cljs
│   │       │   ├── protobuf.cljs
│   │       │   └── rpc.cljs
│   │       └── fabric_sdk
│   │           ├── channel.cljs
│   │           ├── core.cljs
│   │           ├── eventhub.cljs
│   │           ├── macros.clj
│   │           └── user.cljs
│   └── nodejs
│       ├── client.config
│       ├── client.js
│       ├── lib
│       │   └── util.js
│       ├── package.json
│       └── protos
│           ├── appinit.proto
│           └── org.hyperledger.chaincode.example02.proto
└── README.md

13 directories, 26 files
```

### Step 1 - Fabric environment

You will need a functioning peer that has chaintool v0.10.1 or higher available in the $PATH. You may check the version of chaintool you have with 'chaintool -h'. Once confirmed, start the peer with peer node start as you normally would. It is advised to keep the configuration as simple as possible (1 VP, no security, noops consensus)

### Step 2 - Package the chaincode application

```
~/fabric-chaintool-master/examples/example02/app# chaintool package
~/fabric-chaintool-master/examples/example02/app# tree 
.
├── build
│   └── org.hyperledger.chaincode.example02-0.1-SNAPSHOT.car
├── chaincode.yaml
└── src
    ├── chaincode
    │   └── chaincode_example02.go
    └── interfaces
        ├── appinit.cci
        └── org.hyperledger.chaincode.example02.cci

4 directories, 5 files
```

### Step 3 - Compile the client

```
~/fabric-chaintool-master/examples/example02/client/cljs# make
~/fabric-chaintool-master/examples/example02/client/cljs# node target/nodecljs/main.js --help
Usage: example02 [options]

Options Summary:
      --config CONFIG                   path/to/client.config
  -p, --path ID                         path/to/chaincode.car ('install' only)
  -i, --chaincodeId ID   mycc           ChaincodeID
  -v, --version VERSION  1              Chaincode version
      --channelId ID     mychannel      Channel ID
  -c, --command CMD      check-balance  One of [install instantiate make-payment delete-account check-balance]
  -a, --args ARGS                       JSON formatted arguments to submit
  -h, --help
```

### Step 4 - Deploy the CAR


```
~/fabric-chaintool-master/examples/example02/client/cljs# node target/nodecljs/main.js -c install --config ../nodejs/client.config -p /root/fabric-chaintool-master/examples/example02/app/build/org.hyperledger.chaincode.example02-0.1-SNAPSHOT.car 
```


## refer

[Chaincode Tutorials](http://hyperledger-fabric.readthedocs.io/en/latest/chaincode.html)

[Chaincode for Developers](http://hyperledger-fabric.readthedocs.io/en/latest/chaincode4ade.html)

https://github.com/hyperledger/fabric-samples/tree/v1.1.0-rc1/chaincode-docker-devmode

[Interacting With Chaintool Managed Applications](http://fabric-chaintool.readthedocs.io/en/latest/client-development/#protocol)

[Google Protocol Buffers (protobufs)](https://developers.google.com/protocol-buffers/)


对比：

智能合约在线编辑和测试：https://editor.hyperchain.cn/

使用的开发语言：https://zh.wikipedia.org/wiki/Solidity