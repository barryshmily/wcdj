

# Hyperledger Fabric v1 Test Mannal

gerryyang
2018-02

---
- [Hyperledger Fabric v1 Test Mannal](#hyperledger-fabric-v1-test-mannal)
  * [0 基础概念](#0-----)
    + [基本架构](#----)
    + [关键特性](#----)
    + [基本流程](#----)
    + [Block结构](#block--)
    + [核心概念](#----)
  * [1 First network安装构建](#1-first-network----)
    + [1.1 Uninstall old versions](#11-uninstall-old-versions)
    + [1.2 Use the aufs storage drivers](#12-use-the-aufs-storage-drivers)
    + [1.3 Install Docker CE Preparation](#13-install-docker-ce-preparation)
    + [1.4 Add Docker’s official GPG key](#14-add-docker-s-official-gpg-key)
    + [1.5 Use the following command to set up the stable repository](#15-use-the-following-command-to-set-up-the-stable-repository)
    + [1.6 Install Docker CE](#16-install-docker-ce)
    + [1.7 Install docker-compose](#17-install-docker-compose)
    + [1.8 Download Platform-specific Binaries](#18-download-platform-specific-binaries)
    + [1.9 Building Your First Network](#19-building-your-first-network)
      - [1.9.1 Generate Network Artifacts](#191-generate-network-artifacts)
      - [1.9.2 Bring Up the Network](#192-bring-up-the-network)
      - [1.9.3 Bring Down the Network](#193-bring-down-the-network)
  * [2 Frist network源码分析](#2-frist-network----)
    + [2.0 Crypto Generator](#20-crypto-generator)
    + [2.1 Configuration Transaction Generator](#21-configuration-transaction-generator)
    + [2.2 交易配置](#22-----)
      - [2.2.1 configtx.yaml](#221-configtxyaml)
    + [2.3 容器配置](#23-----)
      - [2.3.1 docker-compose-cli.yaml](#231-docker-compose-cliyaml)
      - [2.3.2 docker-compose-base.yaml](#232-docker-compose-baseyaml)
      - [2.3.3 peer-base.yaml](#233-peer-baseyaml)
    + [2.4 启动容器](#24-----)
      - [2.4.1 order - orderer.example.com](#241-order---ordererexamplecom)
        * [容器启动的配置信息](#---------)
        * [关于`Hyperledger Fabric Ordering Service`的介绍](#---hyperledger-fabric-ordering-service----)
        * [**Protocol definition**](#--protocol-definition--)
        * [**Service types**](#--service-types--)
        * [**Ledger types**](#--ledger-types--)
        * [Experimenting with the orderer service](#experimenting-with-the-orderer-service)
        * [Profiling (Not Suggested)](#profiling--not-suggested-)
      - [2.4.2 peer - peer0.org1.example.com/...](#242-peer---peer0org1examplecom-)
        * [容器启动的配置信息](#----------1)
      - [2.4.3 cli](#243-cli)
        * [容器启动的配置信息](#----------2)
    + [2.5 编译](#25---)
    + [2.6 源码解析](#26-----)
      - [2.6.1 first-network/script/script.sh](#261-first-network-script-scriptsh)
        * [(1) createChannel](#-1--createchannel)
        * [(2) joinChannel](#-2--joinchannel)
        * [(3) updateAnchorPeers](#-3--updateanchorpeers)
        * [(4) installChaincode](#-4--installchaincode)
        * [(5) instantiateChaincode](#-5--instantiatechaincode)
        * [(6) chaincodeQuery](#-6--chaincodequery)
        * [(7) chaincodeInvoke](#-7--chaincodeinvoke)
      - [2.6.2 protos](#262-protos)
      - [2.6.3 peer](#263-peer)
        * [1 node](#1-node)
        * [2 channel](#2-channel)
        * [3 chaincode](#3-chaincode)
      - [2.6.4 order](#264-order)
    + [2.7 node sdk](#27-node-sdk)
      - [ubuntu](#ubuntu)
      - [centos](#centos)
      - [install fabric client api](#install-fabric-client-api)
      - [debug](#debug)
      - [some tools](#some-tools)
    + [2.8 fabric-sdk-go](#28-fabric-sdk-go)
    + [2.9 DB](#29-db)
      - [2.9.1 CouchDB](#291-couchdb)
      - [2.9.2 LevelDB](#292-leveldb)
    + [2.10 Order](#210-order)
      - [peer join操作如何选择和哪个order通信](#peer-join---------order--)
    + [2.11  kafka](#211--kafka)
  * [3 数据同步](#3-----)
  * [ledger落盘策略](#ledger----)
    + [order](#order)
  * [4 blockchain-explorer](#4-blockchain-explorer)
  * [5 链码（Chaincode/智能合约）](#5----chaincode------)
    + [账本状态交互API](#------api)
    + [交易信息相关API](#------api)
    + [参数读取API](#----api)
    + [其他API](#--api)
  * [6 性能测试](#6-----)
  * [7 Hyperledger Composer](#7-hyperledger-composer)
  * [8 需要解决的问题](#8--------)
  * [9 一些验证](#9-----)
    + [overlay网络，chaincode容器启动失败](#overlay---chaincode------)
    + [instantiation是否需要满足背书策略](#instantiation----------)
    + [commit逻辑](#commit--)
    + [txid重复检查](#txid----)
    + [在Fabric 1.0.4版本和1.1.x版本中，LevelDB, CouchDB修改wordstate后，Fabric如何处理？](#-fabric-104---11x----leveldb--couchdb--wordstate--fabric-----)
  * [关键日志](#----)
    + [peer启动日志](#peer----)
    + [order启动日志](#order----)
    + [join channel (peer 日志)](#join-channel--peer----)
    + [install chaincode (peer 日志)](#install-chaincode--peer----)
    + [instantiate chaincode (peer 日志)](#instantiate-chaincode--peer----)
    + [instantiate chaincode (order 日志)](#instantiate-chaincode--order----)
    + [invork (执行peer 日志)](#invork----peer----)
    + [invork (同步peer 日志)](#invork----peer----)
    + [invork (order 日志)](#invork--order----)
    + [query (执行peer 日志)](#query----peer----)
    + [query (同步peer 日志)](#query----peer----)
    + [query (order 日志)](#query--order----)
    + [commit时](#commit-)
  * [Refer](#refer)
    + [Fabric官方介绍](#fabric----)
    + [Demo](#demo)
    + [官方文档](#----)
    + [智能合约](#----)
    + [ProtoBuf](#protobuf)
    + [gRPC](#grpc)
    + [yaml](#yaml)
    + [社区动态](#----)
    + [Bugs](#bugs)
    + [综述](#--)
    + [kafka相关](#kafka--)
    + [性能测试工具](#------)
    + [Docker](#docker)
    + [前端](#--)
    + [SDK](#sdk)
    + [NodeJS](#nodejs)
    + [GoLang](#golang)
    + [论坛](#--)
    + [万达区块链](#-----)
    + [腾讯区块链](#-----)
    + [比特币](#---)
    + [IBM](#ibm)
    + [MySQL](#mysql)
    + [Chaincode dev](#chaincode-dev)
    + [Q&A](#q-a)

<small><i><a href='http://ecotrust-canada.github.io/markdown-toc/'>Table of contents generated with markdown-toc</a></i></small>



## 0 基础概念

* Hyperledger Fabric is a blockchain framework implementation and one of the Hyperledger projects hosted by The Linux Foundation. Intended as a foundation for developing applications or solutions with a modular architecture, Hyperledger Fabric allows components, such as consensus and membership services, to be plug-and-play. Hyperledger Fabric leverages container technology to host smart contracts called “chaincode” that comprise the application logic of the system. Hyperledger Fabric was initially contributed by Digital Asset and IBM, as a result of the first hackathon.
* 159 engineers from 28 organizations contributed to project to advance open blockchain products and services.

### 基本架构

![arch.png](https://github.com/gerryyang/wcdj/blob/master/fabric-annotated/pic/arch.png?raw=true)

### 关键特性

1. Channels for sharing confidential information
2. Ordering Service delivers transactions consistently to peers in the network
3. Endorsement policies for transactions
4. CouchDB world state supports wide range of queries
5. Bring-your-own Membership Service Provider (MSP)


### 基本流程

[交易流程](http://hyperledger-fabric.readthedocs.io/en/release/txflow.html)


### Block结构

[Block v1.0的结构](https://blockchain-fabric.blogspot.com/2017/04/hyperledger-fabric-v10-block-structure.html)

``` go
type Block struct {
    Header   *BlockHeader   
    Data     *BlockData     
    Metadata *BlockMetadata 
}
```

### 核心概念

[Glossary](http://hyperledger-fabric.readthedocs.io/en/release/glossary.html)


## 1 First network安装构建

下面是关于使用[fabric](https://github.com/hyperledger/fabric)应用区块链的一个场景。 [first network](http://hyperledger-fabric.readthedocs.io/en/latest/build_network.html)是一个关于转账的例子，其代码可通过`git clone https://github.com/hyperledger/fabric-samples.git`下载到本地。 若只是测试例子则不用下载fabric源码，通过下载镜像即可。若修改功能，则需要下载fabric源码并修改其内部模块。在开发环境配置好`GOPATH`，然后通过`go get github.com/hyperledger/fabric`下载fabric源码到开发环境。

**特别说明：**

如果要使用不同的fabric版本测试，可以通过下面方法进行切换。
http://hyperledger-fabric.readthedocs.io/en/latest/samples.html

```
# git clone -b master https://github.com/hyperledger/fabric-samples.git
# cd fabric-samples
# git checkout -b {TAG}
# git tag
v1.0.2
v1.0.6
v1.1.0-alpha
v1.1.0-preview
v1.1.0-rc1
# git checkout -b v1.1.0-rc1
# cd fabric-samples/scripts 
# ./fabric-preload.sh $tag
```
切换版本时，只需要重新执行下面命令即可。
```
# ./fabric-preload.sh 1.1.0-rc1
# ./fabric-preload.sh 1.0.4
```

比如：
```
hyperledger/fabric-ca                                                                                    x86_64-1.1.0-rc1    8a6c8c2e2ebf        2 weeks ago         283MB
hyperledger/fabric-tools                                                                                 x86_64-1.1.0-rc1    006c689ec08e        2 weeks ago         1.46GB
hyperledger/fabric-orderer                                                                               x86_64-1.1.0-rc1    10afc128d402        2 weeks ago         180MB
hyperledger/fabric-peer                                                                                  x86_64-1.1.0-rc1    6b44b1d021cb        2 weeks ago         187MB
hyperledger/fabric-javaenv                                                                               x86_64-1.1.0-rc1    ea263125afb1        2 weeks ago         1.52GB
hyperledger/fabric-ccenv                                                                                 x86_64-1.1.0-rc1    65c951b9681f        2 weeks ago         1.39GB
hyperledger/fabric-baseos                                                                                x86_64-0.4.6        220e5cf3fb7f        3 weeks ago         151MB
hyperledger/fabric-ca                                                                                    latest              8e691b3509bf        4 months ago        238MB
hyperledger/fabric-ca                                                                                    x86_64-1.0.4        8e691b3509bf        4 months ago        238MB
hyperledger/fabric-tools                                                                                 latest              6051774928a6        4 months ago        1.33GB
hyperledger/fabric-tools                                                                                 x86_64-1.0.4        6051774928a6        4 months ago        1.33GB
hyperledger/fabric-couchdb                                                                               latest              cf24b91dfeb1        4 months ago        1.5GB
hyperledger/fabric-couchdb                                                                               x86_64-1.0.4        cf24b91dfeb1        4 months ago        1.5GB
hyperledger/fabric-kafka                                                                                 latest              7a9d6f3c4a7c        4 months ago        1.29GB
hyperledger/fabric-kafka                                                                                 x86_64-1.0.4        7a9d6f3c4a7c        4 months ago        1.29GB
hyperledger/fabric-zookeeper                                                                             latest              53c4a0d95fd4        4 months ago        1.3GB
hyperledger/fabric-zookeeper                                                                             x86_64-1.0.4        53c4a0d95fd4        4 months ago        1.3GB
hyperledger/fabric-orderer                                                                               latest              b17741e7b036        4 months ago        151MB
hyperledger/fabric-orderer                                                                               x86_64-1.0.4        b17741e7b036        4 months ago        151MB
hyperledger/fabric-peer                                                                                  latest              1ce935adc397        4 months ago        154MB
hyperledger/fabric-peer                                                                                  x86_64-1.0.4        1ce935adc397        4 months ago        154MB
hyperledger/fabric-javaenv                                                                               latest              a517b70135c7        4 months ago        1.41GB
hyperledger/fabric-javaenv                                                                               x86_64-1.0.4        a517b70135c7        4 months ago        1.41GB
hyperledger/fabric-ccenv                                                                                 latest              856061b1fed7        4 months ago        1.28GB
hyperledger/fabric-ccenv                                                                                 x86_64-1.0.4        856061b1fed7        4 months ago        1.28GB
hyperledger/fabric-baseos                                                                                x86_64-0.3.2        bbcbb9da2d83        6 months ago        129MB
```


> **What does this demonstrate?**
Chaincode MUST be installed on a peer in order for it to successfully perform read/write operations against the ledger. Furthermore, a chaincode container is not started for a peer until an init or traditional transaction - read/write - is performed against that chaincode (e.g. query for the value of “a”). The transaction causes the container to start. Also, all peers in a channel maintain an exact copy of the ledger which comprises the blockchain to store the immutable, sequenced record in blocks, as well as a state database to maintain a snapshot of the current state. This includes those peers that do not have chaincode installed on them (like peer1.org1.example.com in the above example) . Finally, the chaincode is accessible after it is installed (like peer1.org2.example.com in the above example) because it has already been instantiated.

这个例子可以说明：

1. `chaincode`(智能合约，例子中使用go语言实现)，必须安装到某一个`peer`上才能完成对`ledger`的读写操作。
2. 在初始化或者在交易发生读写操作时，才运行包含`chaincode`的容器。
3. 另外，所有在一个`channel`里的`peers`，维护了一个`ledger`的副本，这个副本由`blockchain`组成，而每个block保存了不可更改，和排序的记录。同时，一个`state database`用于维护当前状态的快照。
4. 对于没有安装`chaincode`的peer，也同样有这个`ledger`副本。
5. 在某个`peer`安装`chaincode`之后，可以在`ledger`读取到最新的数据。


下面是执行这个例子的步骤，本文在`ubuntu TRUSTY 14.04 64位`环境下测试，注意要求安装机器的内存大于1GB，否则编译demo时会提示内存不足。

几个重要的环境配置：

```
# ubuntu源
/etc/apt/sources.list 

# ubuntu docker 配置
/etc/default/docker

# Docker镜像的默认位置
/var/lib/docker

# go的编译环境
export GOPATH=/root/go
export GOROOT=/root/LAMP/golang/go_1_9_2
export PATH=$GOROOT/bin:$PATH
```

补充说明：

> 其他版本可访问下面页面进行下载
>
> https://store.docker.com/search?type=edition&offering=community
> 比如：centos
> https://docs.docker.com/install/linux/docker-ce/centos/

### centos安装fabric的一些问题

1. 安装docker-ce失败：Requires: container-selinux >= 2.9

解决方法：使用rpm包安装。参考：https://github.com/docker/for-linux/issues/21
```
wget ftp://ftp.icm.edu.pl/vol/rzm6/linux-centos-vault/7.3.1611/extras/x86_64/Packages/container-selinux-2.9-4.el7.noarch.rpm
rpm -Uvh container-selinux-2.9-4.el7.noarch.rpm
yum install docker-ce-17.12.1.ce-1.el7.centos.x86_64.rpm
```

2. storage driver

https://docs.docker.com/storage/storagedriver/device-mapper-driver/#configure-docker-with-the-devicemapper-storage-driver


3. Error starting daemon: Error initializing network controller: list bridge addresses failed: no available network

sudo route del -net 172.16.0.0/12

http://blog.csdn.net/longxing_123/article/details/78044840


4. open /run/docker/containerd/daemon/io.containerd.runtime.v1.linux/moby no such file

```
yum install http://mirror.centos.org/centos/7/os/x86_64/Packages/libseccomp-2.3.1-3.el7.x86_64.rpm
yum install docker-ce
```
https://github.com/moby/moby/issues/35906

5. ltdl.h: No such file

yum install libtool-ltdl-devel

https://stackoverflow.com/questions/43626320/ltdl-h-not-found-error-while-building-chaincode

6. [Errno 14] curl#60 - "Peer's Certificate issuer is not recognized

vi /etc/yum.conf
修改 sslcacert


7. nopt缺失

```
sudo npm install nopt
sudo npm install npmlog
sudo npm install semver
sudo npm install rimraf
sudo npm install sqlite3@3.1.3 --unsafe-perm
```

8. 修改Docker的根目录

老版本方法：
Docker默认的根目录是/var/lib/docker/，这样会导致Docker images文件占用/root分区空间，故将其修改到/data分区下。修改Docker在Centos下的配置文件/etc/sysconfig/docker（Ubuntu下为/etc/default/docker）。

OPTIONS='--selinux-enabled -b=none -g /data/home/gerryyang/root_docker'

新版方法：
https://stackoverflow.com/questions/24309526/how-to-change-the-docker-image-installation-directory
https://docs.docker.com/v1.11/engine/reference/commandline/daemon/#daemon-configuration-file

```
# /etc/docker/daemon.json
{
  "storage-driver": "devicemapper",
  "graph": "/data/home/gerryyang/root_docker"
}
```

9. docker save and load

docker save hyperledger/fabric-tools > fabric-tools.tar  
docker import fabric-ca.tar hyperledger/fabric-tools 

https://docs.docker.com/engine/reference/commandline/save/#options
https://stackoverflow.com/questions/36925261/what-is-the-difference-between-import-and-load-in-docker

### 1.0 Install go

https://golang.org/dl/

wget https://dl.google.com/go/go1.10.linux-amd64.tar.gz


### 1.1 Uninstall old versions
如果之前环境上安装过docker，且版本比较老，则可以通过下述命令移除老版本。注意，只会移除老的docker engine，不会删除已安装的镜像。
```
apt-get remove docker docker-engine docker.io
```

### 1.2 Use the aufs storage drivers
Docker CE now uses the overlay2 storage driver by default, and it is recommended that you use it instead of aufs. 
If you need to use aufs, you will need to do additional preparation.
For Ubuntu 16.04 and higher, the Linux kernel includes support for OverlayFS, and Docker CE will use the overlay2 storage driver by default. If you need to use aufs instead, you need to configure it manually. See aufs
Unless you have a strong reason not to, install the linux-image-extra-* packages, which allow Docker to use the aufs storage drivers.

新版本的`Docker CE`，现在默认使用`overlay2`作为存储层的驱动，而不再是`aufs`。若要使用`aufs`则需要安装`linux-image-extra-* packages`。
```
sudo apt-get update
sudo apt-get install linux-image-extra-$(uname -r) linux-image-extra-virtual
```

### 1.3 Install Docker CE Preparation

Install using the repository
Install packages to allow apt to use a repository over HTTPS

在安装`Docker CE`前，需要先完成下面工具的安装。
```
sudo apt-get install \
    apt-transport-https \
    ca-certificates \
    curl \
    software-properties-common
```

### 1.4 Add Docker’s official GPG key
```
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
```

### 1.5 Use the following command to set up the stable repository

添加用于下载Docker镜像的源。然后需要先更新下package index，然后开始安装`docker-ce`。
```
sudo add-apt-repository \
   "deb [arch=amd64] https://download.docker.com/linux/ubuntu \
   $(lsb_release -cs) \
   stable"

# Update the apt package index after adding the repo
sudo apt-get update
```	

### 1.6 Install Docker CE

```
# INSTALL DOCKER CE
sudo apt-get install docker-ce
```

安装`docker-ce`成功后，可以通过下面的方式验证。
```
root@ubuntu-s-1vcpu-3gb-nyc3-01:~# docker version
Client:
 Version:      17.09.0-ce
 API version:  1.32
 Go version:   go1.8.3
 Git commit:   afdb6d4
 Built:        Tue Sep 26 22:42:38 2017
 OS/Arch:      linux/amd64

Server:
 Version:      17.09.0-ce
 API version:  1.32 (minimum version 1.12)
 Go version:   go1.8.3
 Git commit:   afdb6d4
 Built:        Tue Sep 26 22:41:20 2017
 OS/Arch:      linux/amd64
 Experimental: false
```

### 1.7 Install docker-compose

然后安装`docker-compose`，可参考https://docs.docker.com/compose/install/。

``` bash
sudo curl -L https://github.com/docker/compose/releases/download/1.17.0/docker-compose-`uname -s`-`uname -m` -o /usr/local/bin/docker-compose

# 更改执行权限
sudo chmod +x /usr/local/bin/docker-compose
```

安装`docker-compose`成功后，可以通过下面的方式验证。
```
root@ubuntu-s-1vcpu-3gb-nyc3-01:~# docker-compose version
docker-compose version 1.17.0, build ac53b73
docker-py version: 2.5.1
CPython version: 2.7.13
OpenSSL version: OpenSSL 1.0.1t  3 May 2016
```

### 1.8 Download Platform-specific Binaries

```
curl -sSL https://goo.gl/x2LhaE | bash -s 1.1.0-preview
```
如果下载或执行上述命令报错，直接下载脚本到本地执行
https://github.com/hyperledger/fabric/blob/master/scripts/bootstrap.sh


`bootstrap.sh`执行脚本会下载`Hyperledger Fabric` 涉及到的相关docker images。可以通过下面命令grep到所有fabric用到的镜像。
```
docker images | grep hyperledger* 
```

下载的版本信息可以通过参数指定，或者使用参数默认值：

``` bash
# current version of fabric released
export VERSION=${1:-1.0.4}
# current version of fabric-ca released
export CA_VERSION=${2:-$VERSION}
# current version of thirdparty images (couchdb, kafka and zookeeper) released
export THIRDPARTY_IMAGE_VERSION=0.4.5
export ARCH=$(echo "$(uname -s|tr '[:upper:]' '[:lower:]'|sed 's/mingw64_nt.*/windows/')-$(uname -m | sed 's/x86_64/amd64/g')" | awk '{print tolower($0)}')
#Set MARCH variable i.e ppc64le,s390x,x86_64,i386
MARCH=`uname -m`
```

比如：
``` 
===> List out hyperledger docker images
hyperledger/fabric-ca           latest              8e691b3509bf        4 weeks ago         238MB
hyperledger/fabric-ca           x86_64-1.0.4        8e691b3509bf        4 weeks ago         238MB
hyperledger/fabric-tools        latest              6051774928a6        4 weeks ago         1.33GB
hyperledger/fabric-tools        x86_64-1.0.4        6051774928a6        4 weeks ago         1.33GB
hyperledger/fabric-couchdb      latest              cf24b91dfeb1        4 weeks ago         1.5GB
hyperledger/fabric-couchdb      x86_64-1.0.4        cf24b91dfeb1        4 weeks ago         1.5GB
hyperledger/fabric-kafka        latest              7a9d6f3c4a7c        4 weeks ago         1.29GB
hyperledger/fabric-kafka        x86_64-1.0.4        7a9d6f3c4a7c        4 weeks ago         1.29GB
hyperledger/fabric-zookeeper    latest              53c4a0d95fd4        4 weeks ago         1.3GB
hyperledger/fabric-zookeeper    x86_64-1.0.4        53c4a0d95fd4        4 weeks ago         1.3GB
hyperledger/fabric-orderer      latest              b17741e7b036        4 weeks ago         151MB
hyperledger/fabric-orderer      x86_64-1.0.4        b17741e7b036        4 weeks ago         151MB
hyperledger/fabric-peer         latest              1ce935adc397        4 weeks ago         154MB
hyperledger/fabric-peer         x86_64-1.0.4        1ce935adc397        4 weeks ago         154MB
hyperledger/fabric-javaenv      latest              a517b70135c7        4 weeks ago         1.41GB
hyperledger/fabric-javaenv      x86_64-1.0.4        a517b70135c7        4 weeks ago         1.41GB
hyperledger/fabric-ccenv        latest              856061b1fed7        4 weeks ago         1.28GB
hyperledger/fabric-ccenv        x86_64-1.0.4        856061b1fed7        4 weeks ago         1.28GB
```
> Look at the names for each image; these are the components that will ultimately comprise our Hyperledger Fabric network. You will also notice that you have two instances of the same image ID - one tagged as “x86_64-1.x.x” and one tagged as “latest”.
> 

`bootstrap.sh`执行后，还会生成平台依赖的几个二进制工具，测试例子中会用到。每个工具的作用，官方文档中也有相关介绍。

* cryptogen
* configtxgen
* configtxlator
* peer


### 1.9 Building Your First Network

环境准备好之后，开始[下载要测试的samples](http://hyperledger-fabric.readthedocs.io/en/latest/samples.html)。本文主要测试`first-network`这个sample。

```
cd fabric-samples/first-network 
```

We provide a fully annotated script - `byfn.sh` - that leverages these `Docker images` to quickly bootstrap `a Hyperledger Fabric network comprised of 4 peers representing two different organizations, and an orderer node`. It will also `launch a container` to run a scripted execution that will join peers to a channel, deploy and instantiate chaincode and drive execution of transactions against the deployed chaincode.

测试例子通过`byfn.sh`脚本来运行。

``` bash
root@gerryyang:~/fabric/fabric-samples/first-network# ./byfn.sh --help
./byfn.sh: illegal option -- -
Usage: 
  byfn.sh -m up|down|restart|generate [-c <channel name>] [-t <timeout>] [-d <delay>] [-f <docker-compose-file>] [-s <dbtype>]
  byfn.sh -h|--help (print this message)
    -m <mode> - one of 'up', 'down', 'restart' or 'generate'
      - 'up' - bring up the network with docker-compose up
      - 'down' - clear the network with docker-compose down
      - 'restart' - restart the network
      - 'generate' - generate required certificates and genesis block
    -c <channel name> - channel name to use (defaults to "mychannel")
    -t <timeout> - CLI timeout duration in seconds (defaults to 10000)
    -d <delay> - delay duration in seconds (defaults to 3)
    -f <docker-compose-file> - specify which docker-compose file use (defaults to docker-compose-cli.yaml)
    -s <dbtype> - the database backend to use: goleveldb (default) or couchdb
    -l <language> - the chaincode language: golang (default) or node

Typically, one would first generate the required certificates and 
genesis block, then bring up the network. e.g.:

        byfn.sh -m generate -c mychannel
        byfn.sh -m up -c mychannel -s couchdb
        byfn.sh -m up -l node
        byfn.sh -m down -c mychannel

Taking all defaults:
        byfn.sh -m generate
        byfn.sh -m up
        byfn.sh -m down
```

#### 1.9.1 Generate Network Artifacts

``` bash
root@gerryyang:~/fabric/fabric-samples/first-network# ./byfn.sh -m generate
Generating certs and genesis block for with channel 'mychannel' and CLI timeout of '10000' seconds and CLI delay of '3' seconds
Continue (y/n)? y
proceeding ...
/root/fabric/ps_binaries/bin/cryptogen

##########################################################
##### Generate certificates using cryptogen tool #########
##########################################################
org1.example.com
org2.example.com

/root/fabric/ps_binaries/bin/configtxgen
##########################################################
#########  Generating Orderer Genesis block ##############
##########################################################
2017-11-29 15:40:01.436 CST [common/configtx/tool] main -> INFO 001 Loading configuration
2017-11-29 15:40:01.491 CST [common/configtx/tool] doOutputBlock -> INFO 002 Generating genesis block
2017-11-29 15:40:01.493 CST [common/configtx/tool] doOutputBlock -> INFO 003 Writing genesis block

#################################################################
### Generating channel configuration transaction 'channel.tx' ###
#################################################################
2017-11-29 15:40:01.509 CST [common/configtx/tool] main -> INFO 001 Loading configuration
2017-11-29 15:40:01.562 CST [common/configtx/tool] doOutputChannelCreateTx -> INFO 002 Generating new channel configtx
2017-11-29 15:40:01.563 CST [common/configtx/tool] doOutputChannelCreateTx -> INFO 003 Writing new channel tx

#################################################################
#######    Generating anchor peer update for Org1MSP   ##########
#################################################################
2017-11-29 15:40:01.578 CST [common/configtx/tool] main -> INFO 001 Loading configuration
2017-11-29 15:40:01.583 CST [common/configtx/tool] doOutputAnchorPeersUpdate -> INFO 002 Generating anchor peer update
2017-11-29 15:40:01.584 CST [common/configtx/tool] doOutputAnchorPeersUpdate -> INFO 003 Writing anchor peer update

#################################################################
#######    Generating anchor peer update for Org2MSP   ##########
#################################################################
2017-11-29 15:40:01.608 CST [common/configtx/tool] main -> INFO 001 Loading configuration
2017-11-29 15:40:01.671 CST [common/configtx/tool] doOutputAnchorPeersUpdate -> INFO 002 Generating anchor peer update
2017-11-29 15:40:01.672 CST [common/configtx/tool] doOutputAnchorPeersUpdate -> INFO 003 Writing anchor peer update

```

* 首先为网络中的各种实体生成`证书`和`相关的key`
*  `the genesis block`用于启动`the ordering service`
*  `channel`的相关配置

> This first step generates all of the certificates and keys for our various network entities, the genesis block used to bootstrap the ordering service, and a collection of configuration transactions required to configure a Channel.

#### 1.9.2 Bring Up the Network

使用如下命令启动网络。
* 编译`Golang chaincode images`，并启动对应的容器
* Go是默认的`chaincode`编程语言，如果是其他语言，通过`-l`选项来指定，比如`-l node`

> The above command will compile Golang chaincode images and spin up the corresponding containers. Go is the default chaincode language, however there is also support for Node.js chaincode. 

通过`docker ps`可以看到会创建以下的容器：

``` bash
root@ubuntu-s-1vcpu-3gb-nyc3-01:~/go/src/github.com/hyperledger/fabric/build/image/peer# docker ps
CONTAINER ID        IMAGE                        COMMAND                  CREATED             STATUS              PORTS                                              NAMES
dd1a45e2b908        hyperledger/fabric-tools     "/bin/bash -c './s..."   5 minutes ago       Up 5 minutes                                                           cli
9d0329060dbb        hyperledger/fabric-peer      "peer node start"        5 minutes ago       Up 5 minutes        0.0.0.0:8051->7051/tcp, 0.0.0.0:8053->7053/tcp     peer1.org1.example.com
5ed79df36f35        hyperledger/fabric-peer      "peer node start"        5 minutes ago       Up 5 minutes        0.0.0.0:10051->7051/tcp, 0.0.0.0:10053->7053/tcp   peer1.org2.example.com
96e1c56d4d4d        hyperledger/fabric-peer      "peer node start"        5 minutes ago       Up 5 minutes        0.0.0.0:9051->7051/tcp, 0.0.0.0:9053->7053/tcp     peer0.org2.example.com
662dc16b1bae        hyperledger/fabric-peer      "peer node start"        5 minutes ago       Up 5 minutes        0.0.0.0:7051->7051/tcp, 0.0.0.0:7053->7053/tcp     peer0.org1.example.com
168a83cc9781        hyperledger/fabric-orderer   "orderer"                5 minutes ago       Up 5 minutes        0.0.0.0:7050->7050/tcp                             orderer.example.com
```

下面是整个启动过程。
``` bash
root@ubuntu-s-1vcpu-3gb-nyc3-01:~/fabric/fabric-samples/first-network# ./byfn.sh -m up
Starting with channel 'mychannel' and CLI timeout of '10000'
Continue (y/n)? y
proceeding ...
Creating network "net_byfn" with the default driver
Creating orderer.example.com ... 
Creating peer0.org1.example.com ... 
Creating peer0.org2.example.com ... 
Creating peer1.org1.example.com ... 
Creating orderer.example.com
Creating peer0.org1.example.com
Creating peer1.org2.example.com ... 
Creating peer0.org2.example.com
Creating peer1.org1.example.com
Creating peer1.org2.example.com ... done
Creating cli ... 
Creating cli ... done

 ____    _____      _      ____    _____ 
/ ___|  |_   _|    / \    |  _ \  |_   _|
\___ \    | |     / _ \   | |_) |   | |  
 ___) |   | |    / ___ \  |  _ <    | |  
|____/    |_|   /_/   \_\ |_| \_\   |_|  

Build your first network (BYFN) end-to-end test

Channel name : mychannel
Creating channel...
CORE_PEER_TLS_ROOTCERT_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/ca.crt
CORE_PEER_TLS_KEY_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/server.key
CORE_PEER_LOCALMSPID=Org1MSP
CORE_VM_ENDPOINT=unix:///host/var/run/docker.sock
CORE_PEER_TLS_CERT_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/server.crt
CORE_PEER_TLS_ENABLED=true
CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/users/Admin@org1.example.com/msp
CORE_PEER_ID=cli
CORE_LOGGING_LEVEL=DEBUG
CORE_PEER_ADDRESS=peer0.org1.example.com:7051
2017-11-29 10:05:21.806 UTC [msp] GetLocalMSP -> DEBU 001 Returning existing local MSP
2017-11-29 10:05:21.806 UTC [msp] GetDefaultSigningIdentity -> DEBU 002 Obtaining default signing identity
2017-11-29 10:05:21.812 UTC [channelCmd] InitCmdFactory -> INFO 003 Endorser and orderer connections initialized
2017-11-29 10:05:21.812 UTC [msp] GetLocalMSP -> DEBU 004 Returning existing local MSP
2017-11-29 10:05:21.812 UTC [msp] GetDefaultSigningIdentity -> DEBU 005 Obtaining default signing identity
2017-11-29 10:05:21.812 UTC [msp] GetLocalMSP -> DEBU 006 Returning existing local MSP
2017-11-29 10:05:21.812 UTC [msp] GetDefaultSigningIdentity -> DEBU 007 Obtaining default signing identity
2017-11-29 10:05:21.812 UTC [msp/identity] Sign -> DEBU 008 Sign: plaintext: 0A88060A074F7267314D535012FC052D...53616D706C65436F6E736F727469756D 
2017-11-29 10:05:21.812 UTC [msp/identity] Sign -> DEBU 009 Sign: digest: FEFB8799995020A77E0FE798BD187D019ED1F1170313AAC6912C98334BCD8E6E 
2017-11-29 10:05:21.813 UTC [msp] GetLocalMSP -> DEBU 00a Returning existing local MSP
2017-11-29 10:05:21.813 UTC [msp] GetDefaultSigningIdentity -> DEBU 00b Obtaining default signing identity
2017-11-29 10:05:21.813 UTC [msp] GetLocalMSP -> DEBU 00c Returning existing local MSP
2017-11-29 10:05:21.813 UTC [msp] GetDefaultSigningIdentity -> DEBU 00d Obtaining default signing identity
2017-11-29 10:05:21.813 UTC [msp/identity] Sign -> DEBU 00e Sign: plaintext: 0ABF060A1508021A0608E18CFAD00522...2708F22B84FAD57236C4E076E690B09B 
2017-11-29 10:05:21.813 UTC [msp/identity] Sign -> DEBU 00f Sign: digest: 05CD6DCBEDE5A734C77A9BB5A5CEF62C1EAEDF06E69317D5F296EB0F5440A146 
2017-11-29 10:05:21.924 UTC [msp] GetLocalMSP -> DEBU 010 Returning existing local MSP
2017-11-29 10:05:21.924 UTC [msp] GetDefaultSigningIdentity -> DEBU 011 Obtaining default signing identity
2017-11-29 10:05:21.924 UTC [msp] GetLocalMSP -> DEBU 012 Returning existing local MSP
2017-11-29 10:05:21.924 UTC [msp] GetDefaultSigningIdentity -> DEBU 013 Obtaining default signing identity
2017-11-29 10:05:21.924 UTC [msp/identity] Sign -> DEBU 014 Sign: plaintext: 0ABF060A1508021A0608E18CFAD00522...61A46C0552E112080A021A0012021A00 
2017-11-29 10:05:21.924 UTC [msp/identity] Sign -> DEBU 015 Sign: digest: 21B0F42B7BD384BD2D51F1DBD6A48BEEBE225D9825EA21F8DEAE4E83C82D3AF0 
2017-11-29 10:05:21.954 UTC [channelCmd] readBlock -> DEBU 016 Got status: &{NOT_FOUND}
2017-11-29 10:05:21.954 UTC [msp] GetLocalMSP -> DEBU 017 Returning existing local MSP
2017-11-29 10:05:21.954 UTC [msp] GetDefaultSigningIdentity -> DEBU 018 Obtaining default signing identity
2017-11-29 10:05:21.975 UTC [channelCmd] InitCmdFactory -> INFO 019 Endorser and orderer connections initialized
2017-11-29 10:05:22.175 UTC [msp] GetLocalMSP -> DEBU 01a Returning existing local MSP
2017-11-29 10:05:22.175 UTC [msp] GetDefaultSigningIdentity -> DEBU 01b Obtaining default signing identity
2017-11-29 10:05:22.176 UTC [msp] GetLocalMSP -> DEBU 01c Returning existing local MSP
2017-11-29 10:05:22.176 UTC [msp] GetDefaultSigningIdentity -> DEBU 01d Obtaining default signing identity
2017-11-29 10:05:22.176 UTC [msp/identity] Sign -> DEBU 01e Sign: plaintext: 0ABF060A1508021A0608E28CFAD00522...01B5258BC89F12080A021A0012021A00 
2017-11-29 10:05:22.176 UTC [msp/identity] Sign -> DEBU 01f Sign: digest: 150D5ECB467B5FF7C4CE7AB0E6493817D2BAD481746F93EFF2DDA19D4F68CAE4 
2017-11-29 10:05:22.181 UTC [channelCmd] readBlock -> DEBU 020 Received block: 0
2017-11-29 10:05:22.181 UTC [main] main -> INFO 021 Exiting.....
===================== Channel "mychannel" is created successfully ===================== 

Having all peers join the channel...
CORE_PEER_TLS_ROOTCERT_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/ca.crt
CORE_PEER_TLS_KEY_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/server.key
CORE_PEER_LOCALMSPID=Org1MSP
CORE_VM_ENDPOINT=unix:///host/var/run/docker.sock
CORE_PEER_TLS_CERT_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/server.crt
CORE_PEER_TLS_ENABLED=true
CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/users/Admin@org1.example.com/msp
CORE_PEER_ID=cli
CORE_LOGGING_LEVEL=DEBUG
CORE_PEER_ADDRESS=peer0.org1.example.com:7051
2017-11-29 10:05:22.224 UTC [msp] GetLocalMSP -> DEBU 001 Returning existing local MSP
2017-11-29 10:05:22.224 UTC [msp] GetDefaultSigningIdentity -> DEBU 002 Obtaining default signing identity
2017-11-29 10:05:22.228 UTC [channelCmd] InitCmdFactory -> INFO 003 Endorser and orderer connections initialized
2017-11-29 10:05:22.228 UTC [msp/identity] Sign -> DEBU 004 Sign: plaintext: 0A85070A5B08011A0B08E28CFAD00510...E373C2F2BAA01A080A000A000A000A00 
2017-11-29 10:05:22.228 UTC [msp/identity] Sign -> DEBU 005 Sign: digest: D1E5130E758D5DD16F6C54822E1C3169DA7DD499ADCC82D6E08F57119C71ED39 
2017-11-29 10:05:22.284 UTC [channelCmd] executeJoin -> INFO 006 Peer joined the channel!
2017-11-29 10:05:22.284 UTC [main] main -> INFO 007 Exiting.....
===================== PEER0 joined on the channel "mychannel" ===================== 

CORE_PEER_TLS_ROOTCERT_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/ca.crt
CORE_PEER_TLS_KEY_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/server.key
CORE_PEER_LOCALMSPID=Org1MSP
CORE_VM_ENDPOINT=unix:///host/var/run/docker.sock
CORE_PEER_TLS_CERT_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/server.crt
CORE_PEER_TLS_ENABLED=true
CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/users/Admin@org1.example.com/msp
CORE_PEER_ID=cli
CORE_LOGGING_LEVEL=DEBUG
CORE_PEER_ADDRESS=peer1.org1.example.com:7051
2017-11-29 10:05:25.329 UTC [msp] GetLocalMSP -> DEBU 001 Returning existing local MSP
2017-11-29 10:05:25.329 UTC [msp] GetDefaultSigningIdentity -> DEBU 002 Obtaining default signing identity
2017-11-29 10:05:25.333 UTC [channelCmd] InitCmdFactory -> INFO 003 Endorser and orderer connections initialized
2017-11-29 10:05:25.333 UTC [msp/identity] Sign -> DEBU 004 Sign: plaintext: 0A86070A5C08011A0C08E58CFAD00510...E373C2F2BAA01A080A000A000A000A00 
2017-11-29 10:05:25.333 UTC [msp/identity] Sign -> DEBU 005 Sign: digest: 297AC357427B845F7C4C35FBAB0F15480C5FC8CB63DA241AFD0441F7334955F5 
2017-11-29 10:05:25.386 UTC [channelCmd] executeJoin -> INFO 006 Peer joined the channel!
2017-11-29 10:05:25.386 UTC [main] main -> INFO 007 Exiting.....
===================== PEER1 joined on the channel "mychannel" ===================== 

CORE_PEER_TLS_ROOTCERT_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org2.example.com/peers/peer0.org2.example.com/tls/ca.crt
CORE_PEER_TLS_KEY_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/server.key
CORE_PEER_LOCALMSPID=Org2MSP
CORE_VM_ENDPOINT=unix:///host/var/run/docker.sock
CORE_PEER_TLS_CERT_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/server.crt
CORE_PEER_TLS_ENABLED=true
CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org2.example.com/users/Admin@org2.example.com/msp
CORE_PEER_ID=cli
CORE_LOGGING_LEVEL=DEBUG
CORE_PEER_ADDRESS=peer0.org2.example.com:7051
2017-11-29 10:05:28.428 UTC [msp] GetLocalMSP -> DEBU 001 Returning existing local MSP
2017-11-29 10:05:28.428 UTC [msp] GetDefaultSigningIdentity -> DEBU 002 Obtaining default signing identity
2017-11-29 10:05:28.432 UTC [channelCmd] InitCmdFactory -> INFO 003 Endorser and orderer connections initialized
2017-11-29 10:05:28.433 UTC [msp/identity] Sign -> DEBU 004 Sign: plaintext: 0A8A070A5C08011A0C08E88CFAD00510...E373C2F2BAA01A080A000A000A000A00 
2017-11-29 10:05:28.433 UTC [msp/identity] Sign -> DEBU 005 Sign: digest: 1506F30DED5E88A2E7390AB19E6BC8263726D25D6CF6BBD1AA3ED3D817B98C6E 
2017-11-29 10:05:28.482 UTC [channelCmd] executeJoin -> INFO 006 Peer joined the channel!
2017-11-29 10:05:28.483 UTC [main] main -> INFO 007 Exiting.....
===================== PEER2 joined on the channel "mychannel" ===================== 

CORE_PEER_TLS_ROOTCERT_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org2.example.com/peers/peer0.org2.example.com/tls/ca.crt
CORE_PEER_TLS_KEY_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/server.key
CORE_PEER_LOCALMSPID=Org2MSP
CORE_VM_ENDPOINT=unix:///host/var/run/docker.sock
CORE_PEER_TLS_CERT_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/server.crt
CORE_PEER_TLS_ENABLED=true
CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org2.example.com/users/Admin@org2.example.com/msp
CORE_PEER_ID=cli
CORE_LOGGING_LEVEL=DEBUG
CORE_PEER_ADDRESS=peer1.org2.example.com:7051
2017-11-29 10:05:31.528 UTC [msp] GetLocalMSP -> DEBU 001 Returning existing local MSP
2017-11-29 10:05:31.528 UTC [msp] GetDefaultSigningIdentity -> DEBU 002 Obtaining default signing identity
2017-11-29 10:05:31.532 UTC [channelCmd] InitCmdFactory -> INFO 003 Endorser and orderer connections initialized
2017-11-29 10:05:31.533 UTC [msp/identity] Sign -> DEBU 004 Sign: plaintext: 0A8A070A5C08011A0C08EB8CFAD00510...E373C2F2BAA01A080A000A000A000A00 
2017-11-29 10:05:31.533 UTC [msp/identity] Sign -> DEBU 005 Sign: digest: 33FC4FF7EF3D306B7D8DBFD235FD8B923F0F6A5876AFD4144E96EB9937CB9951 
2017-11-29 10:05:31.590 UTC [channelCmd] executeJoin -> INFO 006 Peer joined the channel!
2017-11-29 10:05:31.590 UTC [main] main -> INFO 007 Exiting.....
===================== PEER3 joined on the channel "mychannel" ===================== 

Updating anchor peers for org1...
CORE_PEER_TLS_ROOTCERT_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/ca.crt
CORE_PEER_TLS_KEY_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/server.key
CORE_PEER_LOCALMSPID=Org1MSP
CORE_VM_ENDPOINT=unix:///host/var/run/docker.sock
CORE_PEER_TLS_CERT_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/server.crt
CORE_PEER_TLS_ENABLED=true
CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/users/Admin@org1.example.com/msp
CORE_PEER_ID=cli
CORE_LOGGING_LEVEL=DEBUG
CORE_PEER_ADDRESS=peer0.org1.example.com:7051
2017-11-29 10:05:34.638 UTC [msp] GetLocalMSP -> DEBU 001 Returning existing local MSP
2017-11-29 10:05:34.638 UTC [msp] GetDefaultSigningIdentity -> DEBU 002 Obtaining default signing identity
2017-11-29 10:05:34.643 UTC [channelCmd] InitCmdFactory -> INFO 003 Endorser and orderer connections initialized
2017-11-29 10:05:34.644 UTC [msp] GetLocalMSP -> DEBU 004 Returning existing local MSP
2017-11-29 10:05:34.644 UTC [msp] GetDefaultSigningIdentity -> DEBU 005 Obtaining default signing identity
2017-11-29 10:05:34.644 UTC [msp] GetLocalMSP -> DEBU 006 Returning existing local MSP
2017-11-29 10:05:34.644 UTC [msp] GetDefaultSigningIdentity -> DEBU 007 Obtaining default signing identity
2017-11-29 10:05:34.644 UTC [msp/identity] Sign -> DEBU 008 Sign: plaintext: 0A88060A074F7267314D535012FC052D...2A0641646D696E732A0641646D696E73 
2017-11-29 10:05:34.644 UTC [msp/identity] Sign -> DEBU 009 Sign: digest: 70F8DFE6A82603AED318AD8797A79469623382F52330588F6AFBBC74FCE00DE0 
2017-11-29 10:05:34.644 UTC [msp] GetLocalMSP -> DEBU 00a Returning existing local MSP
2017-11-29 10:05:34.644 UTC [msp] GetDefaultSigningIdentity -> DEBU 00b Obtaining default signing identity
2017-11-29 10:05:34.644 UTC [msp] GetLocalMSP -> DEBU 00c Returning existing local MSP
2017-11-29 10:05:34.644 UTC [msp] GetDefaultSigningIdentity -> DEBU 00d Obtaining default signing identity
2017-11-29 10:05:34.644 UTC [msp/identity] Sign -> DEBU 00e Sign: plaintext: 0ABF060A1508021A0608EE8CFAD00522...E621FFB54B500691FC599DAE14EAB60F 
2017-11-29 10:05:34.644 UTC [msp/identity] Sign -> DEBU 00f Sign: digest: 748A7371E6184E13866B252CCE7AF8976F3141DCD77AF989C9079C225F365707 
2017-11-29 10:05:34.683 UTC [main] main -> INFO 010 Exiting.....
===================== Anchor peers for org "Org1MSP" on "mychannel" is updated successfully ===================== 

Updating anchor peers for org2...
CORE_PEER_TLS_ROOTCERT_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org2.example.com/peers/peer0.org2.example.com/tls/ca.crt
CORE_PEER_TLS_KEY_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/server.key
CORE_PEER_LOCALMSPID=Org2MSP
CORE_VM_ENDPOINT=unix:///host/var/run/docker.sock
CORE_PEER_TLS_CERT_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/server.crt
CORE_PEER_TLS_ENABLED=true
CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org2.example.com/users/Admin@org2.example.com/msp
CORE_PEER_ID=cli
CORE_LOGGING_LEVEL=DEBUG
CORE_PEER_ADDRESS=peer0.org2.example.com:7051
2017-11-29 10:05:37.735 UTC [msp] GetLocalMSP -> DEBU 001 Returning existing local MSP
2017-11-29 10:05:37.735 UTC [msp] GetDefaultSigningIdentity -> DEBU 002 Obtaining default signing identity
2017-11-29 10:05:37.739 UTC [channelCmd] InitCmdFactory -> INFO 003 Endorser and orderer connections initialized
2017-11-29 10:05:37.740 UTC [msp] GetLocalMSP -> DEBU 004 Returning existing local MSP
2017-11-29 10:05:37.740 UTC [msp] GetDefaultSigningIdentity -> DEBU 005 Obtaining default signing identity
2017-11-29 10:05:37.740 UTC [msp] GetLocalMSP -> DEBU 006 Returning existing local MSP
2017-11-29 10:05:37.740 UTC [msp] GetDefaultSigningIdentity -> DEBU 007 Obtaining default signing identity
2017-11-29 10:05:37.740 UTC [msp/identity] Sign -> DEBU 008 Sign: plaintext: 0A8C060A074F7267324D53501280062D...2A0641646D696E732A0641646D696E73 
2017-11-29 10:05:37.740 UTC [msp/identity] Sign -> DEBU 009 Sign: digest: F49A966255D63B2CDF6C9C782902A104DD54F9009B08FF2B07BC9A36B8DACF6E 
2017-11-29 10:05:37.740 UTC [msp] GetLocalMSP -> DEBU 00a Returning existing local MSP
2017-11-29 10:05:37.740 UTC [msp] GetDefaultSigningIdentity -> DEBU 00b Obtaining default signing identity
2017-11-29 10:05:37.740 UTC [msp] GetLocalMSP -> DEBU 00c Returning existing local MSP
2017-11-29 10:05:37.740 UTC [msp] GetDefaultSigningIdentity -> DEBU 00d Obtaining default signing identity
2017-11-29 10:05:37.740 UTC [msp/identity] Sign -> DEBU 00e Sign: plaintext: 0AC3060A1508021A0608F18CFAD00522...FA54E864FC51858799222D44C08E08EF 
2017-11-29 10:05:37.740 UTC [msp/identity] Sign -> DEBU 00f Sign: digest: 268AF53B8564482B17D516FA868EAB78EB9187EC4242020664C47481AC0D7A0E 
2017-11-29 10:05:37.779 UTC [main] main -> INFO 010 Exiting.....
===================== Anchor peers for org "Org2MSP" on "mychannel" is updated successfully ===================== 

Installing chaincode on org1/peer0...
CORE_PEER_TLS_ROOTCERT_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/ca.crt
CORE_PEER_TLS_KEY_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/server.key
CORE_PEER_LOCALMSPID=Org1MSP
CORE_VM_ENDPOINT=unix:///host/var/run/docker.sock
CORE_PEER_TLS_CERT_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/server.crt
CORE_PEER_TLS_ENABLED=true
CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/users/Admin@org1.example.com/msp
CORE_PEER_ID=cli
CORE_LOGGING_LEVEL=DEBUG
CORE_PEER_ADDRESS=peer0.org1.example.com:7051
2017-11-29 10:05:40.830 UTC [msp] GetLocalMSP -> DEBU 001 Returning existing local MSP
2017-11-29 10:05:40.830 UTC [msp] GetDefaultSigningIdentity -> DEBU 002 Obtaining default signing identity
2017-11-29 10:05:40.830 UTC [chaincodeCmd] checkChaincodeCmdParams -> INFO 003 Using default escc
2017-11-29 10:05:40.830 UTC [chaincodeCmd] checkChaincodeCmdParams -> INFO 004 Using default vscc
2017-11-29 10:05:40.912 UTC [golang-platform] getCodeFromFS -> DEBU 005 getCodeFromFS github.com/hyperledger/fabric/examples/chaincode/go/chaincode_example02
2017-11-29 10:05:41.086 UTC [golang-platform] func1 -> DEBU 006 Discarding GOROOT package fmt
2017-11-29 10:05:41.086 UTC [golang-platform] func1 -> DEBU 007 Discarding provided package github.com/hyperledger/fabric/core/chaincode/shim
2017-11-29 10:05:41.086 UTC [golang-platform] func1 -> DEBU 008 Discarding provided package github.com/hyperledger/fabric/protos/peer
2017-11-29 10:05:41.086 UTC [golang-platform] func1 -> DEBU 009 Discarding GOROOT package strconv
2017-11-29 10:05:41.087 UTC [golang-platform] GetDeploymentPayload -> DEBU 00a done
2017-11-29 10:05:41.088 UTC [msp/identity] Sign -> DEBU 00b Sign: plaintext: 0A85070A5B08031A0B08F58CFAD00510...5F74FD270000FFFFCEF44F9B002C0000 
2017-11-29 10:05:41.088 UTC [msp/identity] Sign -> DEBU 00c Sign: digest: 0443DE5BF1ABD9863AC4CA23B657A4F3D61B1E5F2A6825FA13FAFB4F032C2F51 
2017-11-29 10:05:41.094 UTC [chaincodeCmd] install -> DEBU 00d Installed remotely response:<status:200 payload:"OK" > 
2017-11-29 10:05:41.094 UTC [main] main -> INFO 00e Exiting.....
===================== Chaincode is installed on remote peer PEER0 ===================== 

Install chaincode on org2/peer2...
CORE_PEER_TLS_ROOTCERT_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org2.example.com/peers/peer0.org2.example.com/tls/ca.crt
CORE_PEER_TLS_KEY_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/server.key
CORE_PEER_LOCALMSPID=Org2MSP
CORE_VM_ENDPOINT=unix:///host/var/run/docker.sock
CORE_PEER_TLS_CERT_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/server.crt
CORE_PEER_TLS_ENABLED=true
CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org2.example.com/users/Admin@org2.example.com/msp
CORE_PEER_ID=cli
CORE_LOGGING_LEVEL=DEBUG
CORE_PEER_ADDRESS=peer0.org2.example.com:7051
2017-11-29 10:05:41.138 UTC [msp] GetLocalMSP -> DEBU 001 Returning existing local MSP
2017-11-29 10:05:41.138 UTC [msp] GetDefaultSigningIdentity -> DEBU 002 Obtaining default signing identity
2017-11-29 10:05:41.138 UTC [chaincodeCmd] checkChaincodeCmdParams -> INFO 003 Using default escc
2017-11-29 10:05:41.138 UTC [chaincodeCmd] checkChaincodeCmdParams -> INFO 004 Using default vscc
2017-11-29 10:05:41.194 UTC [golang-platform] getCodeFromFS -> DEBU 005 getCodeFromFS github.com/hyperledger/fabric/examples/chaincode/go/chaincode_example02
2017-11-29 10:05:41.343 UTC [golang-platform] func1 -> DEBU 006 Discarding GOROOT package fmt
2017-11-29 10:05:41.343 UTC [golang-platform] func1 -> DEBU 007 Discarding provided package github.com/hyperledger/fabric/core/chaincode/shim
2017-11-29 10:05:41.343 UTC [golang-platform] func1 -> DEBU 008 Discarding provided package github.com/hyperledger/fabric/protos/peer
2017-11-29 10:05:41.343 UTC [golang-platform] func1 -> DEBU 009 Discarding GOROOT package strconv
2017-11-29 10:05:41.343 UTC [golang-platform] GetDeploymentPayload -> DEBU 00a done
2017-11-29 10:05:41.344 UTC [msp/identity] Sign -> DEBU 00b Sign: plaintext: 0A8A070A5C08031A0C08F58CFAD00510...5F74FD270000FFFFCEF44F9B002C0000 
2017-11-29 10:05:41.344 UTC [msp/identity] Sign -> DEBU 00c Sign: digest: 3B04CCBA10DCFE86A70D535334CF4D6CB7A8FAC3DACEA6587C008CB46D1D394A 
2017-11-29 10:05:41.350 UTC [chaincodeCmd] install -> DEBU 00d Installed remotely response:<status:200 payload:"OK" > 
2017-11-29 10:05:41.350 UTC [main] main -> INFO 00e Exiting.....
===================== Chaincode is installed on remote peer PEER2 ===================== 

Instantiating chaincode on org2/peer2...
CORE_PEER_TLS_ROOTCERT_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org2.example.com/peers/peer0.org2.example.com/tls/ca.crt
CORE_PEER_TLS_KEY_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/server.key
CORE_PEER_LOCALMSPID=Org2MSP
CORE_VM_ENDPOINT=unix:///host/var/run/docker.sock
CORE_PEER_TLS_CERT_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/server.crt
CORE_PEER_TLS_ENABLED=true
CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org2.example.com/users/Admin@org2.example.com/msp
CORE_PEER_ID=cli
CORE_LOGGING_LEVEL=DEBUG
CORE_PEER_ADDRESS=peer0.org2.example.com:7051
2017-11-29 10:05:41.451 UTC [msp] GetLocalMSP -> DEBU 001 Returning existing local MSP
2017-11-29 10:05:41.451 UTC [msp] GetDefaultSigningIdentity -> DEBU 002 Obtaining default signing identity
2017-11-29 10:05:41.459 UTC [chaincodeCmd] checkChaincodeCmdParams -> INFO 003 Using default escc
2017-11-29 10:05:41.459 UTC [chaincodeCmd] checkChaincodeCmdParams -> INFO 004 Using default vscc
2017-11-29 10:05:41.462 UTC [msp/identity] Sign -> DEBU 005 Sign: plaintext: 0A95070A6708031A0C08F58CFAD00510...324D53500A04657363630A0476736363 
2017-11-29 10:05:41.462 UTC [msp/identity] Sign -> DEBU 006 Sign: digest: 8D01287A32FD305C8FE36F722535B9BABA711D3C59DCBFFFA9A0F6EEAEB2C64B 
2017-11-29 10:06:06.389 UTC [msp/identity] Sign -> DEBU 007 Sign: plaintext: 0A95070A6708031A0C08F58CFAD00510...2769527059A0B3E12A6545A0706ACD10 
2017-11-29 10:06:06.389 UTC [msp/identity] Sign -> DEBU 008 Sign: digest: 948A8DE933C173ACF217E61AEFFDFA85024EB5B12B4CB8D74290F9A094A91B86 
2017-11-29 10:06:06.397 UTC [main] main -> INFO 009 Exiting.....
===================== Chaincode Instantiation on PEER2 on channel 'mychannel' is successful ===================== 

Querying chaincode on org1/peer0...
===================== Querying on PEER0 on channel 'mychannel'... ===================== 
CORE_PEER_TLS_ROOTCERT_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/ca.crt
CORE_PEER_TLS_KEY_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/server.key
CORE_PEER_LOCALMSPID=Org1MSP
CORE_VM_ENDPOINT=unix:///host/var/run/docker.sock
CORE_PEER_TLS_CERT_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/server.crt
CORE_PEER_TLS_ENABLED=true
CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/users/Admin@org1.example.com/msp
CORE_PEER_ID=cli
CORE_LOGGING_LEVEL=DEBUG
CORE_PEER_ADDRESS=peer0.org1.example.com:7051
Attempting to Query PEER0 ...3 secs

2017-11-29 10:06:09.526 UTC [msp] GetLocalMSP -> DEBU 001 Returning existing local MSP
2017-11-29 10:06:09.526 UTC [msp] GetDefaultSigningIdentity -> DEBU 002 Obtaining default signing identity
2017-11-29 10:06:09.526 UTC [chaincodeCmd] checkChaincodeCmdParams -> INFO 003 Using default escc
2017-11-29 10:06:09.526 UTC [chaincodeCmd] checkChaincodeCmdParams -> INFO 004 Using default vscc
2017-11-29 10:06:09.526 UTC [msp/identity] Sign -> DEBU 005 Sign: plaintext: 0A91070A6708031A0C08918DFAD00510...6D7963631A0A0A0571756572790A0161 
2017-11-29 10:06:09.526 UTC [msp/identity] Sign -> DEBU 006 Sign: digest: 4DF71D05F3610CE5528B2352E12DEE10F249CF7B356CABAA50BDFE3B9C424459 
Query Result: 100
2017-11-29 10:06:32.978 UTC [main] main -> INFO 007 Exiting.....
===================== Query on PEER0 on channel 'mychannel' is successful ===================== 
Sending invoke transaction on org1/peer0...
CORE_PEER_TLS_ROOTCERT_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/ca.crt
CORE_PEER_TLS_KEY_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/server.key
CORE_PEER_LOCALMSPID=Org1MSP
CORE_VM_ENDPOINT=unix:///host/var/run/docker.sock
CORE_PEER_TLS_CERT_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/server.crt
CORE_PEER_TLS_ENABLED=true
CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/users/Admin@org1.example.com/msp
CORE_PEER_ID=cli
CORE_LOGGING_LEVEL=DEBUG
CORE_PEER_ADDRESS=peer0.org1.example.com:7051
2017-11-29 10:06:33.036 UTC [msp] GetLocalMSP -> DEBU 001 Returning existing local MSP
2017-11-29 10:06:33.036 UTC [msp] GetDefaultSigningIdentity -> DEBU 002 Obtaining default signing identity
2017-11-29 10:06:33.042 UTC [chaincodeCmd] checkChaincodeCmdParams -> INFO 003 Using default escc
2017-11-29 10:06:33.042 UTC [chaincodeCmd] checkChaincodeCmdParams -> INFO 004 Using default vscc
2017-11-29 10:06:33.043 UTC [msp/identity] Sign -> DEBU 005 Sign: plaintext: 0A90070A6608031A0B08A98DFAD00510...696E766F6B650A01610A01620A023130 
2017-11-29 10:06:33.043 UTC [msp/identity] Sign -> DEBU 006 Sign: digest: EFD09D72F88DF15EC20CB683B8CA6DA2C5BAECA91A0417E33B1D44ABDD573C82 
2017-11-29 10:06:33.056 UTC [msp/identity] Sign -> DEBU 007 Sign: plaintext: 0A90070A6608031A0B08A98DFAD00510...74EBCA8115328710DE26FC727E182A70 
2017-11-29 10:06:33.056 UTC [msp/identity] Sign -> DEBU 008 Sign: digest: 2FDC01A0D2DCBDA4CDD3A3177015C061E7770FA1BCE3F8F54A8D36422E40A291 
2017-11-29 10:06:33.060 UTC [chaincodeCmd] chaincodeInvokeOrQuery -> DEBU 009 ESCC invoke result: version:1 response:<status:200 message:"OK" > payload:"\n \303\360\221y,w\311\265\2516;\375\322\232v~\336\307\221]\021\001\302\013Z\010\025\305>\204\247\363\022Y\nE\022\024\n\004lscc\022\014\n\n\n\004mycc\022\002\010\003\022-\n\004mycc\022%\n\007\n\001a\022\002\010\003\n\007\n\001b\022\002\010\003\032\007\n\001a\032\00290\032\010\n\001b\032\003210\032\003\010\310\001\"\013\022\004mycc\032\0031.0" endorsement:<endorser:"\n\007Org1MSP\022\200\006-----BEGIN -----\nMIICGjCCAcCgAwIBAgIRAJkhe2BpYp3vXXlKV3v/4O8wCgYIKoZIzj0EAwIwczEL\nMAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBG\ncmFuY2lzY28xGTAXBgNVBAoTEG9yZzEuZXhhbXBsZS5jb20xHDAaBgNVBAMTE2Nh\nLm9yZzEuZXhhbXBsZS5jb20wHhcNMTcxMTI5MDk1OTU1WhcNMjcxMTI3MDk1OTU1\nWjBbMQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMN\nU2FuIEZyYW5jaXNjbzEfMB0GA1UEAxMWcGVlcjAub3JnMS5leGFtcGxlLmNvbTBZ\nMBMGByqGSM49AgEGCCqGSM49AwEHA0IABKsEtI4XCUYwvEjAHtBmvVVMB2bOAuFc\nbb5S7WJfxbkcYMv9YG/6uTQwElsXAurcKNL0JHiwLpEhT2XMqLHUoD6jTTBLMA4G\nA1UdDwEB/wQEAwIHgDAMBgNVHRMBAf8EAjAAMCsGA1UdIwQkMCKAIM+kr17iChgL\nM1IICjalzAYfs/SQ49tUCM34op155j5qMAoGCCqGSM49BAMCA0gAMEUCIQDAGAE5\nhw9dhmnZWrraAYOcIowe0JzCIEWljtHTJ0UxNAIgQ1/Jr3lR6KP6tsR4Wmya+yM5\n1zKeNZo+kTO6tUa9vvE=\n-----END -----\n" signature:"0E\002!\000\212M\355\232c\033Of\374\227\306\261\340\305\265\302\352\364.\n\016\250\334\370L\0275R\006\222\n\315\002 \031\205\347\335\371\005Az\\K\232\030\251\\a\nt\353\312\201\0252\207\020\336&\374r~\030*p" > 
2017-11-29 10:06:33.060 UTC [chaincodeCmd] chaincodeInvokeOrQuery -> INFO 00a Chaincode invoke successful. result: status:200 
2017-11-29 10:06:33.061 UTC [main] main -> INFO 00b Exiting.....
===================== Invoke transaction on PEER0 on channel 'mychannel' is successful ===================== 

Installing chaincode on org2/peer3...
CORE_PEER_TLS_ROOTCERT_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org2.example.com/peers/peer0.org2.example.com/tls/ca.crt
CORE_PEER_TLS_KEY_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/server.key
CORE_PEER_LOCALMSPID=Org2MSP
CORE_VM_ENDPOINT=unix:///host/var/run/docker.sock
CORE_PEER_TLS_CERT_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/server.crt
CORE_PEER_TLS_ENABLED=true
CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org2.example.com/users/Admin@org2.example.com/msp
CORE_PEER_ID=cli
CORE_LOGGING_LEVEL=DEBUG
CORE_PEER_ADDRESS=peer1.org2.example.com:7051
2017-11-29 10:06:33.129 UTC [msp] GetLocalMSP -> DEBU 001 Returning existing local MSP
2017-11-29 10:06:33.129 UTC [msp] GetDefaultSigningIdentity -> DEBU 002 Obtaining default signing identity
2017-11-29 10:06:33.129 UTC [chaincodeCmd] checkChaincodeCmdParams -> INFO 003 Using default escc
2017-11-29 10:06:33.129 UTC [chaincodeCmd] checkChaincodeCmdParams -> INFO 004 Using default vscc
2017-11-29 10:06:33.190 UTC [golang-platform] getCodeFromFS -> DEBU 005 getCodeFromFS github.com/hyperledger/fabric/examples/chaincode/go/chaincode_example02
2017-11-29 10:06:33.343 UTC [golang-platform] func1 -> DEBU 006 Discarding GOROOT package fmt
2017-11-29 10:06:33.343 UTC [golang-platform] func1 -> DEBU 007 Discarding provided package github.com/hyperledger/fabric/core/chaincode/shim
2017-11-29 10:06:33.343 UTC [golang-platform] func1 -> DEBU 008 Discarding provided package github.com/hyperledger/fabric/protos/peer
2017-11-29 10:06:33.343 UTC [golang-platform] func1 -> DEBU 009 Discarding GOROOT package strconv
2017-11-29 10:06:33.343 UTC [golang-platform] GetDeploymentPayload -> DEBU 00a done
2017-11-29 10:06:33.345 UTC [msp/identity] Sign -> DEBU 00b Sign: plaintext: 0A8A070A5C08031A0C08A98DFAD00510...5F74FD270000FFFFCEF44F9B002C0000 
2017-11-29 10:06:33.345 UTC [msp/identity] Sign -> DEBU 00c Sign: digest: 4A1BA56C63118A9C1544E65286AFD258CBA9455A42945FABF5816D2336AA2E60 
2017-11-29 10:06:33.352 UTC [chaincodeCmd] install -> DEBU 00d Installed remotely response:<status:200 payload:"OK" > 
2017-11-29 10:06:33.352 UTC [main] main -> INFO 00e Exiting.....
===================== Chaincode is installed on remote peer PEER3 ===================== 

Querying chaincode on org2/peer3...
===================== Querying on PEER3 on channel 'mychannel'... ===================== 
CORE_PEER_TLS_ROOTCERT_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org2.example.com/peers/peer0.org2.example.com/tls/ca.crt
CORE_PEER_TLS_KEY_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/server.key
CORE_PEER_LOCALMSPID=Org2MSP
CORE_VM_ENDPOINT=unix:///host/var/run/docker.sock
CORE_PEER_TLS_CERT_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/server.crt
CORE_PEER_TLS_ENABLED=true
CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org2.example.com/users/Admin@org2.example.com/msp
CORE_PEER_ID=cli
CORE_LOGGING_LEVEL=DEBUG
CORE_PEER_ADDRESS=peer1.org2.example.com:7051
Attempting to Query PEER3 ...3 secs

2017-11-29 10:06:36.402 UTC [msp] GetLocalMSP -> DEBU 001 Returning existing local MSP
2017-11-29 10:06:36.402 UTC [msp] GetDefaultSigningIdentity -> DEBU 002 Obtaining default signing identity
2017-11-29 10:06:36.402 UTC [chaincodeCmd] checkChaincodeCmdParams -> INFO 003 Using default escc
2017-11-29 10:06:36.402 UTC [chaincodeCmd] checkChaincodeCmdParams -> INFO 004 Using default vscc
2017-11-29 10:06:36.402 UTC [msp/identity] Sign -> DEBU 005 Sign: plaintext: 0A95070A6708031A0C08AC8DFAD00510...6D7963631A0A0A0571756572790A0161 
2017-11-29 10:06:36.402 UTC [msp/identity] Sign -> DEBU 006 Sign: digest: 5F74DFA9029B81BD9CE30854D277ED18B785536546BDD3D701EB1D5726F61816 
Query Result: 90
2017-11-29 10:06:59.885 UTC [main] main -> INFO 007 Exiting.....
===================== Query on PEER3 on channel 'mychannel' is successful ===================== 

========= All GOOD, BYFN execution completed =========== 


 _____   _   _   ____   
| ____| | \ | | |  _ \  
|  _|   |  \| | | | | | 
| |___  | |\  | | |_| | 
|_____| |_| \_| |____/  

```


> You can scroll through these logs to see the various transactions. If you don’t get this result, then jump down to the Troubleshooting section and let’s see whether we can help you discover what went wrong.

#### 1.9.3 Bring Down the Network

执行如下命令：will kill your containers, remove the crypto material and four artifacts, and delete the chaincode images from your Docker Registry。

``` bash 
root@ubuntu-s-1vcpu-3gb-nyc3-01:~/fabric/fabric-samples/first-network# ./byfn.sh -m down
Stopping with channel 'mychannel' and CLI timeout of '10000'
Continue (y/n)? y
proceeding ...
WARNING: The CHANNEL_NAME variable is not set. Defaulting to a blank string.
WARNING: The DELAY variable is not set. Defaulting to a blank string.
WARNING: The TIMEOUT variable is not set. Defaulting to a blank string.
Stopping cli                    ... done
Stopping peer1.org2.example.com ... done
Stopping peer1.org1.example.com ... done
Stopping peer0.org1.example.com ... done
Stopping peer0.org2.example.com ... done
Stopping orderer.example.com    ... done
Removing cli                    ... done
Removing peer1.org2.example.com ... done
Removing peer1.org1.example.com ... done
Removing peer0.org1.example.com ... done
Removing peer0.org2.example.com ... done
Removing orderer.example.com    ... done
Removing network net_byfn
WARNING: The CHANNEL_NAME variable is not set. Defaulting to a blank string.
WARNING: The DELAY variable is not set. Defaulting to a blank string.
WARNING: The TIMEOUT variable is not set. Defaulting to a blank string.
Removing network net_byfn
WARNING: Network net_byfn not found.
3f79d93fc4d3
35664a4d2e8a
35ed37bc8923
Untagged: dev-peer1.org2.example.com-mycc-1.0-26c2ef32838554aac4f7ad6f100aca865e87959c9a126e86d764c8d01f8346ab:latest
Deleted: sha256:cec03990f463dcf131b0f37e79af51d25461388003cd6edcbfc63fd48351610e
Deleted: sha256:167c715d423dcc41797eb6951ca20872d82ea698afc2c00ee15bace5029b4b97
Deleted: sha256:70ed5cbb609f2a9434421e027e09f19510c3fa9a5e2239d1153b4fec6486d08f
Deleted: sha256:7a3caf8a6849faefa063197f60ccf27e4fa94771ecb627aa815655b4d2b33886
Deleted: sha256:8e67b51bd6da7629ab16de8845a0830bcfde683d163ceacdd609763bf73717a6
Deleted: sha256:5e26eb69b8faf1f0becd37e9ab99d7e0ad225e888e2e28d8017dc16836e2735c
Deleted: sha256:5deb9551e724bb816fd761866492e68be67fab303bcef3049db29a38f4fcb162
Untagged: dev-peer0.org1.example.com-mycc-1.0-384f11f484b9302df90b453200cfb25174305fce8f53f4e94d45ee3b6cab0ce9:latest
Deleted: sha256:11ccb0f41d784f2fa045dcd6d647dffaccf963bad9578581161e18ef2b32d543
Deleted: sha256:1e0f381c3b5e10863a2801695a26bf5c4aa98e67b37cd2fd991230dd6d57c5a7
Deleted: sha256:ae5bf7884451058cabbfb13415df9d282cb71dca6e684632191d15cc2c3639fa
Deleted: sha256:81c29c1d34e6c278e95cdc78c8c508beaecc0f38ef4a6bd4749a8dbe5907452d
Deleted: sha256:43234bc91e8ff9f354c7f68bf8539bc3bd7661fd51e8b6b7cb88bb9509454112
Deleted: sha256:bebd3c36bd07ad1fa819f35a2289e33d05e83d883f415dd0f033d6dd4ead0c29
Deleted: sha256:ea4ee9f52dd2c2006b620373fa41e17a1e1ef511c09b5b698fdd673d7d64d924
Untagged: dev-peer0.org2.example.com-mycc-1.0-15b571b3ce849066b7ec74497da3b27e54e0df1345daff3951b94245ce09c42b:latest
Deleted: sha256:eac1b69b25e01cf37dd7dc242acfa10049f378de060cd31355cff10f9721bcbf
Deleted: sha256:860e2ae2ba6372f788a0618864c675a84944345480d106d5582ac4ace3b2a935
Deleted: sha256:844cf819a22f803886a60afb23b3a1188fec70bb31e19b60e1b586bf47a2066b
Deleted: sha256:a2442f1c1241d9d990fdd7bc645c79fd529117cf9885e18f4b297a00e8cf34b0
Deleted: sha256:a24dd2fb52a1d3efebe2a1daaf7308c397dccf16057e3bead21621d1fd817997
Deleted: sha256:5e1e2b2493dd8818e5b75b5173b50c9535e55564c49a91dbbe921acf5f848620
Deleted: sha256:381dc25de27e0bdab40916e346a496c8a9e748efe5ca359c606a277e90ad046c
```

## 2 Frist network源码分析

整个例子通过`byfn.sh`脚本进行控制。

``` bash
#Create the network using docker compose
if [ "${MODE}" == "up" ]; then
  networkUp
  elif [ "${MODE}" == "down" ]; then ## Clear the network
  networkDown
  elif [ "${MODE}" == "generate" ]; then ## Generate Artifacts
  generateCerts
  replacePrivateKey
  generateChannelArtifacts
  elif [ "${MODE}" == "restart" ]; then ## Restart the network
  networkDown
  networkUp
else
  printHelp
  exit 1
fi
```

* This script will orchestrate **a sample end-to-end execution of the Hyperledger Fabric network**. 
* The end-to-end verification provisions a sample Fabric network consisting of **two organizations, each maintaining two peers, and a “solo” ordering service**.
* This verification makes use of **two fundamental tools**, which are necessary to create a functioning transactional network with digital signature validation and access control:
	* **cryptogen** - generates the x509 certificates used to identify and authenticate the various components in the network.
	* **configtxgen** - generates the requisite configuration artifacts for orderer bootstrap and channel creation.
* Each tool consumes **a configuration yaml file**, within which we specify the topology of our network (cryptogen) and the location of our certificates for various configuration operations (configtxgen).  Once the tools have been successfully run, we are able to launch our network.  More detail on the tools and the structure of the network will be provided later in this document. 

其中，涉及到的`镜像`包括：
``` yaml
docker-compose-cli.yaml
image: hyperledger/fabric-tools
working_dir: /opt/gopath/src/github.com/hyperledger/fabric/peer
command: /bin/bash -c './scripts/script.sh ${CHANNEL_NAME} ${DELAY}; sleep $TIMEOUT'

docker-compose-base.yaml
image: hyperledger/fabric-orderer
working_dir: /opt/gopath/src/github.com/hyperledger/fabric
command: orderer

peer-base.yaml
image: hyperledger/fabric-peer
working_dir: /opt/gopath/src/github.com/hyperledger/fabric/peer
command: peer node start
```

其中，`fabric-tools`镜像启动容器时指定`sleep $TIMEOUT`是为了防止容器启动后立刻退出。


### 2.0 Crypto Generator

``` bash 
# We will use the cryptogen tool to generate the cryptographic material (x509 certs)
# for our various network entities.  The certificates are based on a standard PKI
# implementation where validation is achieved by reaching a common trust anchor.
#
# Cryptogen consumes a file - ``crypto-config.yaml`` - that contains the network
# topology and allows us to generate a library of certificates for both the
# Organizations and the components that belong to those Organizations.  Each
# Organization is provisioned a unique root certificate (``ca-cert``), that binds
# specific components (peers and orderers) to that Org.  Transactions and communications
# within Fabric are signed by an entity's private key (``keystore``), and then verified
# by means of a public key (``signcerts``).  You will notice a "count" variable within
# this file.  We use this to specify the number of peers per Organization; in our
# case it's two peers per Org.  The rest of this template is extremely
# self-explanatory.
#
# After we run the tool, the certs will be parked in a folder titled ``crypto-config``.

# Generates Org certs using cryptogen tool
function generateCerts (){
  which cryptogen
  if [ "$?" -ne 0 ]; then
    echo "cryptogen tool not found. exiting"
    exit 1
  fi
  echo
  echo "##########################################################"
  echo "##### Generate certificates using cryptogen tool #########"
  echo "##########################################################"
  if [ -d "crypto-config" ]; then
    rm -Rf crypto-config
  fi
  cryptogen generate --config=./crypto-config.yaml
  if [ "$?" -ne 0 ]; then
    echo "Failed to generate certificates..."
    exit 1
  fi
  echo
}

# Using docker-compose-e2e-template.yaml, replace constants with private key file names
# generated by the cryptogen tool and output a docker-compose.yaml specific to this
# configuration
function replacePrivateKey () {
  # sed on MacOSX does not support -i flag with a null extension. We will use
  # 't' for our back-up's extension and depete it at the end of the function
  ARCH=`uname -s | grep Darwin`
  if [ "$ARCH" == "Darwin" ]; then
    OPTS="-it"
  else
    OPTS="-i"
  fi

  # Copy the template to the file that will be modified to add the private key
  cp docker-compose-e2e-template.yaml docker-compose-e2e.yaml

  # The next steps will replace the template's contents with the
  # actual values of the private key file names for the two CAs.
  CURRENT_DIR=$PWD
  cd crypto-config/peerOrganizations/org1.example.com/ca/
  PRIV_KEY=$(ls *_sk)
  cd "$CURRENT_DIR"
  sed $OPTS "s/CA1_PRIVATE_KEY/${PRIV_KEY}/g" docker-compose-e2e.yaml
  cd crypto-config/peerOrganizations/org2.example.com/ca/
  PRIV_KEY=$(ls *_sk)
  cd "$CURRENT_DIR"
  sed $OPTS "s/CA2_PRIVATE_KEY/${PRIV_KEY}/g" docker-compose-e2e.yaml
  # If MacOSX, remove the temporary backup of the docker-compose file
  if [ "$ARCH" == "Darwin" ]; then
    rm docker-compose-e2e.yamlt
  fi
}
```

### 2.1 Configuration Transaction Generator

1. Profiles 

通过`-profile`选项指定要生成的场景。

2. Organizations 

定义一些组织的索引。

3. Orderer和Application

这两个部分作为默认配置。

更详细的描述：http://hyperledger-fabric.readthedocs.io/en/latest/configtxgen.html

``` bash
# The `configtxgen tool is used to create four artifacts: orderer **bootstrap
# block**, fabric **channel configuration transaction**, and two **anchor
# peer transactions** - one for each Peer Org.
#
# The orderer block is the genesis block for the ordering service, and the
# channel transaction file is broadcast to the orderer at channel creation
# time.  The anchor peer transactions, as the name might suggest, specify each
# Org's anchor peer on this channel.
#
# Configtxgen consumes a file - ``configtx.yaml`` - that contains the definitions
# for the sample network. There are three members - one Orderer Org (``OrdererOrg``)
# and two Peer Orgs (``Org1`` & ``Org2``) each managing and maintaining two peer nodes.
# This file also specifies a consortium - ``SampleConsortium`` - consisting of our
# two Peer Orgs.  Pay specific attention to the "Profiles" section at the top of
# this file.  You will notice that we have two unique headers. One for the orderer genesis
# block - ``TwoOrgsOrdererGenesis`` - and one for our channel - ``TwoOrgsChannel``.
# These headers are important, as we will pass them in as arguments when we create
# our artifacts.  This file also contains two additional specifications that are worth
# noting.  Firstly, we specify the anchor peers for each Peer Org
# (``peer0.org1.example.com`` & ``peer0.org2.example.com``).  Secondly, we point to
# the location of the MSP directory for each member, in turn allowing us to store the
# root certificates for each Org in the orderer genesis block.  This is a critical
# concept. Now any network entity communicating with the ordering service can have
# its digital signature verified.
#
# This function will generate the crypto material and our four configuration
# artifacts, and subsequently output these files into the ``channel-artifacts``
# folder.
#
# If you receive the following warning, it can be safely ignored:
#
# [bccsp] GetDefault -> WARN 001 Before using BCCSP, please call InitFactories(). Falling back to bootBCCSP.
#
# You can ignore the logs regarding intermediate certs, we are not using them in
# this crypto implementation.

# Generate orderer genesis block, channel configuration transaction and
# anchor peer update transactions
function generateChannelArtifacts() {
  which configtxgen
  if [ "$?" -ne 0 ]; then
    echo "configtxgen tool not found. exiting"
    exit 1
  fi

  echo "##########################################################"
  echo "#########  Generating Orderer Genesis block ##############"
  echo "##########################################################"
  # Note: For some unknown reason (at least for now) the block file can't be
  # named orderer.genesis.block or the orderer will fail to launch!
  configtxgen -profile TwoOrgsOrdererGenesis -outputBlock ./channel-artifacts/genesis.block
  if [ "$?" -ne 0 ]; then
    echo "Failed to generate orderer genesis block..."
    exit 1
  fi
  echo
  echo "#################################################################"
  echo "### Generating channel configuration transaction 'channel.tx' ###"
  echo "#################################################################"
  configtxgen -profile TwoOrgsChannel -outputCreateChannelTx ./channel-artifacts/channel.tx -channelID $CHANNEL_NAME
  if [ "$?" -ne 0 ]; then
    echo "Failed to generate channel configuration transaction..."
    exit 1
  fi

  echo
  echo "#################################################################"
  echo "#######    Generating anchor peer update for Org1MSP   ##########"
  echo "#################################################################"
  configtxgen -profile TwoOrgsChannel -outputAnchorPeersUpdate ./channel-artifacts/Org1MSPanchors.tx -channelID $CHANNEL_NAME -asOrg Org1MSP
  if [ "$?" -ne 0 ]; then
    echo "Failed to generate anchor peer update for Org1MSP..."
    exit 1
  fi

  echo
  echo "#################################################################"
  echo "#######    Generating anchor peer update for Org2MSP   ##########"
  echo "#################################################################"
  configtxgen -profile TwoOrgsChannel -outputAnchorPeersUpdate \
  ./channel-artifacts/Org2MSPanchors.tx -channelID $CHANNEL_NAME -asOrg Org2MSP
  if [ "$?" -ne 0 ]; then
    echo "Failed to generate anchor peer update for Org2MSP..."
    exit 1
  fi
  echo
}
```

### 2.2 交易配置

通过`byfn.sh -m generate`会生成`channel-artifacts`和`crypto-config`两个目录信息。

``` bash
root@ubuntu-s-1vcpu-3gb-nyc3-01:~/fabric/fabric-samples/first-network# tree channel-artifacts
channel-artifacts
├── channel.tx
├── genesis.block
├── Org1MSPanchors.tx
└── Org2MSPanchors.tx

0 directories, 4 files
```

``` bash
root@ubuntu-s-1vcpu-3gb-nyc3-01:~/fabric/fabric-samples/first-network# tree crypto-config
crypto-config
├── ordererOrganizations
│   └── example.com
│       ├── ca
│       │   ├── ca.example.com-cert.pem
│       │   └── dedbb07dc124a626ab645ea749313e8f6403ee3e871f6ebd2761dfe1710d5e4e_sk
│       ├── msp
│       │   ├── admincerts
│       │   │   └── Admin@example.com-cert.pem
│       │   ├── cacerts
│       │   │   └── ca.example.com-cert.pem
│       │   └── tlscacerts
│       │       └── tlsca.example.com-cert.pem
│       ├── orderers
│       │   └── orderer.example.com
│       │       ├── msp
│       │       │   ├── admincerts
│       │       │   │   └── Admin@example.com-cert.pem
│       │       │   ├── cacerts
│       │       │   │   └── ca.example.com-cert.pem
│       │       │   ├── keystore
│       │       │   │   └── 7a5200e9a0cee2740c251960970b488d15ae9c32c4c20e0b6103b9caeb17d8de_sk
│       │       │   ├── signcerts
│       │       │   │   └── orderer.example.com-cert.pem
│       │       │   └── tlscacerts
│       │       │       └── tlsca.example.com-cert.pem
│       │       └── tls
│       │           ├── ca.crt
│       │           ├── server.crt
│       │           └── server.key
│       ├── tlsca
│       │   ├── a4cdfca22e791fe17dbeb70029a91e724bac8ccf52fe867fe68f5123b1dd536b_sk
│       │   └── tlsca.example.com-cert.pem
│       └── users
│           └── Admin@example.com
│               ├── msp
│               │   ├── admincerts
│               │   │   └── Admin@example.com-cert.pem
│               │   ├── cacerts
│               │   │   └── ca.example.com-cert.pem
│               │   ├── keystore
│               │   │   └── 3f90d72e30ff90c9fd1b95c427e776182ce9093f7d6d7c406ad9a52126b7dc3e_sk
│               │   ├── signcerts
│               │   │   └── Admin@example.com-cert.pem
│               │   └── tlscacerts
│               │       └── tlsca.example.com-cert.pem
│               └── tls
│                   ├── ca.crt
│                   ├── server.crt
│                   └── server.key
└── peerOrganizations
    ├── org1.example.com
    │   ├── ca
    │   │   ├── 7dd0158d5f173f14eebeb232213a0a57bd6a8c5bb58eefa15540435e984d69c6_sk
    │   │   └── ca.org1.example.com-cert.pem
    │   ├── msp
    │   │   ├── admincerts
    │   │   │   └── Admin@org1.example.com-cert.pem
    │   │   ├── cacerts
    │   │   │   └── ca.org1.example.com-cert.pem
    │   │   └── tlscacerts
    │   │       └── tlsca.org1.example.com-cert.pem
    │   ├── peers
    │   │   ├── peer0.org1.example.com
    │   │   │   ├── msp
    │   │   │   │   ├── admincerts
    │   │   │   │   │   └── Admin@org1.example.com-cert.pem
    │   │   │   │   ├── cacerts
    │   │   │   │   │   └── ca.org1.example.com-cert.pem
    │   │   │   │   ├── keystore
    │   │   │   │   │   └── 1cd28313f42282d57c39fed3a4365e5db4bcad5d1322dec67a092667cf3afd33_sk
    │   │   │   │   ├── signcerts
    │   │   │   │   │   └── peer0.org1.example.com-cert.pem
    │   │   │   │   └── tlscacerts
    │   │   │   │       └── tlsca.org1.example.com-cert.pem
    │   │   │   └── tls
    │   │   │       ├── ca.crt
    │   │   │       ├── server.crt
    │   │   │       └── server.key
    │   │   └── peer1.org1.example.com
    │   │       ├── msp
    │   │       │   ├── admincerts
    │   │       │   │   └── Admin@org1.example.com-cert.pem
    │   │       │   ├── cacerts
    │   │       │   │   └── ca.org1.example.com-cert.pem
    │   │       │   ├── keystore
    │   │       │   │   └── ea15490a5bf1771c8390fc318009723f5445bfd5beb832e103aab576d42b991b_sk
    │   │       │   ├── signcerts
    │   │       │   │   └── peer1.org1.example.com-cert.pem
    │   │       │   └── tlscacerts
    │   │       │       └── tlsca.org1.example.com-cert.pem
    │   │       └── tls
    │   │           ├── ca.crt
    │   │           ├── server.crt
    │   │           └── server.key
    │   ├── tlsca
    │   │   ├── 62eec200ae7b6dbcb0023f77586a7f06e36b103af95b3c95a59c5661792a8d94_sk
    │   │   └── tlsca.org1.example.com-cert.pem
    │   └── users
    │       ├── Admin@org1.example.com
    │       │   ├── msp
    │       │   │   ├── admincerts
    │       │   │   │   └── Admin@org1.example.com-cert.pem
    │       │   │   ├── cacerts
    │       │   │   │   └── ca.org1.example.com-cert.pem
    │       │   │   ├── keystore
    │       │   │   │   └── b3c7f120081b7f15d3ef246c8706aff3c4d28843a0497b08bbafa9575de9da7e_sk
    │       │   │   ├── signcerts
    │       │   │   │   └── Admin@org1.example.com-cert.pem
    │       │   │   └── tlscacerts
    │       │   │       └── tlsca.org1.example.com-cert.pem
    │       │   └── tls
    │       │       ├── ca.crt
    │       │       ├── server.crt
    │       │       └── server.key
    │       └── User1@org1.example.com
    │           ├── msp
    │           │   ├── admincerts
    │           │   │   └── User1@org1.example.com-cert.pem
    │           │   ├── cacerts
    │           │   │   └── ca.org1.example.com-cert.pem
    │           │   ├── keystore
    │           │   │   └── 11011a1ede24dabfab56881198d1088332142493937d98b7769444dcf0ffdbd9_sk
    │           │   ├── signcerts
    │           │   │   └── User1@org1.example.com-cert.pem
    │           │   └── tlscacerts
    │           │       └── tlsca.org1.example.com-cert.pem
    │           └── tls
    │               ├── ca.crt
    │               ├── server.crt
    │               └── server.key
    └── org2.example.com
        ├── ca
        │   ├── 6501055ca7fb0b419608a787c8b3ed1ebcb8d12b4c6fdf9050b076d809bba299_sk
        │   └── ca.org2.example.com-cert.pem
        ├── msp
        │   ├── admincerts
        │   │   └── Admin@org2.example.com-cert.pem
        │   ├── cacerts
        │   │   └── ca.org2.example.com-cert.pem
        │   └── tlscacerts
        │       └── tlsca.org2.example.com-cert.pem
        ├── peers
        │   ├── peer0.org2.example.com
        │   │   ├── msp
        │   │   │   ├── admincerts
        │   │   │   │   └── Admin@org2.example.com-cert.pem
        │   │   │   ├── cacerts
        │   │   │   │   └── ca.org2.example.com-cert.pem
        │   │   │   ├── keystore
        │   │   │   │   └── 8206708a382407ef0ca729b2aee989e15947ab797cd0ba81f33ee4b63771fcb1_sk
        │   │   │   ├── signcerts
        │   │   │   │   └── peer0.org2.example.com-cert.pem
        │   │   │   └── tlscacerts
        │   │   │       └── tlsca.org2.example.com-cert.pem
        │   │   └── tls
        │   │       ├── ca.crt
        │   │       ├── server.crt
        │   │       └── server.key
        │   └── peer1.org2.example.com
        │       ├── msp
        │       │   ├── admincerts
        │       │   │   └── Admin@org2.example.com-cert.pem
        │       │   ├── cacerts
        │       │   │   └── ca.org2.example.com-cert.pem
        │       │   ├── keystore
        │       │   │   └── 2922a0809d4e60291ebdca4f3bca1d7003319e2fd92098620f4444fce0f7ff83_sk
        │       │   ├── signcerts
        │       │   │   └── peer1.org2.example.com-cert.pem
        │       │   └── tlscacerts
        │       │       └── tlsca.org2.example.com-cert.pem
        │       └── tls
        │           ├── ca.crt
        │           ├── server.crt
        │           └── server.key
        ├── tlsca
        │   ├── edcad6366e510f08af0bd1fc4079b783c4b9623d67716aa9937c54c4dd5ad06a_sk
        │   └── tlsca.org2.example.com-cert.pem
        └── users
            ├── Admin@org2.example.com
            │   ├── msp
            │   │   ├── admincerts
            │   │   │   └── Admin@org2.example.com-cert.pem
            │   │   ├── cacerts
            │   │   │   └── ca.org2.example.com-cert.pem
            │   │   ├── keystore
            │   │   │   └── c29b2133baa54b5cc187aa0469a2ff363d0f5ee6494b851f84db19d23e1a5ea8_sk
            │   │   ├── signcerts
            │   │   │   └── Admin@org2.example.com-cert.pem
            │   │   └── tlscacerts
            │   │       └── tlsca.org2.example.com-cert.pem
            │   └── tls
            │       ├── ca.crt
            │       ├── server.crt
            │       └── server.key
            └── User1@org2.example.com
                ├── msp
                │   ├── admincerts
                │   │   └── User1@org2.example.com-cert.pem
                │   ├── cacerts
                │   │   └── ca.org2.example.com-cert.pem
                │   ├── keystore
                │   │   └── aad7d9bb5342f463db323a8e39c46985f247e54d354cef412897ff851e3bc2f4_sk
                │   ├── signcerts
                │   │   └── User1@org2.example.com-cert.pem
                │   └── tlscacerts
                │       └── tlsca.org2.example.com-cert.pem
                └── tls
                    ├── ca.crt
                    ├── server.crt
                    └── server.key

109 directories, 101 files
```

#### 2.2.1 configtx.yaml 

`Configtxgen`工具通过`configtx.yaml`配置，生成（`generateChannelArtifacts`方法）channel-artifacts目录下的信息。

| Name | Cmd |
| -- | --
| Generating Orderer Genesis block | configtxgen -profile TwoOrgsOrdererGenesis -outputBlock ./channel-artifacts/genesis.block
| Generating channel configuration transaction 'channel.tx' | configtxgen -profile TwoOrgsChannel -outputCreateChannelTx ./channel-artifacts/channel.tx -channelID $CHANNEL_NAME
| Generating anchor peer update for Org1MSP | configtxgen -profile TwoOrgsChannel -outputAnchorPeersUpdate ./channel-artifacts/Org1MSPanchors.tx -channelID $CHANNEL_NAME -asOrg Org1MSP
| Generating anchor peer update for Org2MSP | configtxgen -profile TwoOrgsChannel -outputAnchorPeersUpdate ./channel-artifacts/Org2MSPanchors.tx -channelID $CHANNEL_NAME -asOrg Org2MSP

通过下面`configtxgen`工具可以查看生成`genesis.block`的通道  配置信息：

``` bash
configtxgen -profile TwoOrgsOrdererGenesis -inspectBlock ./channel-artifacts/genesis.block > genesis.block.json
```

genesis.blcok 

``` json
Config for channel: testchainid at sequence 0
{
    "Channel": {
        "Values": {
 ...
```

或者通过`configtxlaor`工具查看区块的信息：

``` bash
curl -X POST --data-binary @./channel-artifacts/genesis.block http://127.0.0.1:7059/protolator/decode/common.Block
```

通过`configtxgen`工具可以查看生成`channel.tx`的配置更新信息：
``` bash
configtxgen -profile TwoOrgsChannel -inspectChannelCreateTx ./channel-artifacts/channel.tx 
```

或者通过`configtxlator`工具查看：

``` bash
curl -X POST --data-binary @./channel-artifacts/channel.tx http://127.0.0.1:7059/protolator/decode/common.Envelope
```

### 2.3 容器配置

在执行`byfn.sh`时如果没有指定`-f`配置文件，则docker-compose使用的默认配置为：

```
# use this as the default docker-compose yaml definition
COMPOSE_FILE=docker-compose-cli.yaml
#
COMPOSE_FILE_COUCH=docker-compose-couch.yaml
```

> 关于 compose 配置文件的格式，可以对应参考官方的说明。本文使用的版本号是`version: '2'`
> https://docs.docker.com/compose/compose-file/compose-file-v2


``` bash
# Generate the needed certificates, the genesis block and start the network.
function networkUp () {
  # generate artifacts if they don't exist
  if [ ! -d "crypto-config" ]; then
    generateCerts
    replacePrivateKey
    generateChannelArtifacts
  fi
  if [ "${IF_COUCHDB}" == "couchdb" ]; then
      CHANNEL_NAME=$CHANNEL_NAME TIMEOUT=$CLI_TIMEOUT DELAY=$CLI_DELAY docker-compose -f $COMPOSE_FILE -f $COMPOSE_FILE_COUCH up -d 2>&1
  else
      CHANNEL_NAME=$CHANNEL_NAME TIMEOUT=$CLI_TIMEOUT DELAY=$CLI_DELAY docker-compose -f $COMPOSE_FILE up -d 2>&1
  fi
  if [ $? -ne 0 ]; then
    echo "ERROR !!!! Unable to start network"
    docker logs -f cli
    exit 1
  fi
  docker logs -f cli
}
```

#### 2.3.1 docker-compose-cli.yaml

``` yaml
# Copyright IBM Corp. All Rights Reserved.
#
# SPDX-License-Identifier: Apache-2.0
#

version: '2'

networks:
  byfn:

services:

  orderer.example.com:
    extends:
      file:   base/docker-compose-base.yaml
      service: orderer.example.com
    container_name: orderer.example.com
    networks:
      - byfn

  peer0.org1.example.com:
    container_name: peer0.org1.example.com
    extends:
      file:  base/docker-compose-base.yaml
      service: peer0.org1.example.com
    networks:
      - byfn

  peer1.org1.example.com:
    container_name: peer1.org1.example.com
    extends:
      file:  base/docker-compose-base.yaml
      service: peer1.org1.example.com
    networks:
      - byfn

  peer0.org2.example.com:
    container_name: peer0.org2.example.com
    extends:
      file:  base/docker-compose-base.yaml
      service: peer0.org2.example.com
    networks:
      - byfn

  peer1.org2.example.com:
    container_name: peer1.org2.example.com
    extends:
      file:  base/docker-compose-base.yaml
      service: peer1.org2.example.com
    networks:
      - byfn

  cli:
    container_name: cli
    image: hyperledger/fabric-tools
    tty: true
    environment:
      - GOPATH=/opt/gopath
      - CORE_VM_ENDPOINT=unix:///host/var/run/docker.sock
      - CORE_LOGGING_LEVEL=DEBUG
      - CORE_PEER_ID=cli
      - CORE_PEER_ADDRESS=peer0.org1.example.com:7051
      - CORE_PEER_LOCALMSPID=Org1MSP
      - CORE_PEER_TLS_ENABLED=true
      - CORE_PEER_TLS_CERT_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/server.crt
      - CORE_PEER_TLS_KEY_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/server.key
      - CORE_PEER_TLS_ROOTCERT_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/ca.crt
      - CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/users/Admin@org1.example.com/msp
    working_dir: /opt/gopath/src/github.com/hyperledger/fabric/peer
    command: /bin/bash -c './scripts/script.sh ${CHANNEL_NAME} ${DELAY}; sleep $TIMEOUT'
    volumes:
        - /var/run/:/host/var/run/
        - ./../chaincode/:/opt/gopath/src/github.com/hyperledger/fabric/examples/chaincode/go
        - ./crypto-config:/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/
        - ./scripts:/opt/gopath/src/github.com/hyperledger/fabric/peer/scripts/
        - ./channel-artifacts:/opt/gopath/src/github.com/hyperledger/fabric/peer/channel-artifacts
    depends_on:
      - orderer.example.com
      - peer0.org1.example.com
      - peer1.org1.example.com
      - peer0.org2.example.com
      - peer1.org2.example.com
    networks:
      - byfn
```

#### 2.3.2 docker-compose-base.yaml

然后可以看到其依赖另一个yaml配置：base/docker-compose-base.yaml

``` yaml
# Copyright IBM Corp. All Rights Reserved.
#
# SPDX-License-Identifier: Apache-2.0
#

version: '2'

services:

  orderer.example.com:
    container_name: orderer.example.com
    image: hyperledger/fabric-orderer
    environment:
      - ORDERER_GENERAL_LOGLEVEL=debug
      - ORDERER_GENERAL_LISTENADDRESS=0.0.0.0
      - ORDERER_GENERAL_GENESISMETHOD=file
      - ORDERER_GENERAL_GENESISFILE=/var/hyperledger/orderer/orderer.genesis.block
      - ORDERER_GENERAL_LOCALMSPID=OrdererMSP
      - ORDERER_GENERAL_LOCALMSPDIR=/var/hyperledger/orderer/msp
      # enabled TLS
      - ORDERER_GENERAL_TLS_ENABLED=true
      - ORDERER_GENERAL_TLS_PRIVATEKEY=/var/hyperledger/orderer/tls/server.key
      - ORDERER_GENERAL_TLS_CERTIFICATE=/var/hyperledger/orderer/tls/server.crt
      - ORDERER_GENERAL_TLS_ROOTCAS=[/var/hyperledger/orderer/tls/ca.crt]
    working_dir: /opt/gopath/src/github.com/hyperledger/fabric
    command: orderer
    volumes:
    - ../channel-artifacts/genesis.block:/var/hyperledger/orderer/orderer.genesis.block
    - ../crypto-config/ordererOrganizations/example.com/orderers/orderer.example.com/msp:/var/hyperledger/orderer/msp
    - ../crypto-config/ordererOrganizations/example.com/orderers/orderer.example.com/tls/:/var/hyperledger/orderer/tls
    ports:
      - 7050:7050

  peer0.org1.example.com:
    container_name: peer0.org1.example.com
    extends:
      file: peer-base.yaml
      service: peer-base
    environment:
      - CORE_PEER_ID=peer0.org1.example.com
      - CORE_PEER_ADDRESS=peer0.org1.example.com:7051
      - CORE_PEER_GOSSIP_EXTERNALENDPOINT=peer0.org1.example.com:7051
      - CORE_PEER_LOCALMSPID=Org1MSP
    volumes:
        - /var/run/:/host/var/run/
        - ../crypto-config/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/msp:/etc/hyperledger/fabric/msp
        - ../crypto-config/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls:/etc/hyperledger/fabric/tls
    ports:
      - 7051:7051
      - 7053:7053

  peer1.org1.example.com:
    container_name: peer1.org1.example.com
    extends:
      file: peer-base.yaml
      service: peer-base
    environment:
      - CORE_PEER_ID=peer1.org1.example.com
      - CORE_PEER_ADDRESS=peer1.org1.example.com:7051
      - CORE_PEER_GOSSIP_EXTERNALENDPOINT=peer1.org1.example.com:7051
      - CORE_PEER_GOSSIP_BOOTSTRAP=peer0.org1.example.com:7051
      - CORE_PEER_LOCALMSPID=Org1MSP
    volumes:
        - /var/run/:/host/var/run/
        - ../crypto-config/peerOrganizations/org1.example.com/peers/peer1.org1.example.com/msp:/etc/hyperledger/fabric/msp
        - ../crypto-config/peerOrganizations/org1.example.com/peers/peer1.org1.example.com/tls:/etc/hyperledger/fabric/tls

    ports:
      - 8051:7051
      - 8053:7053

  peer0.org2.example.com:
    container_name: peer0.org2.example.com
    extends:
      file: peer-base.yaml
      service: peer-base
    environment:
      - CORE_PEER_ID=peer0.org2.example.com
      - CORE_PEER_ADDRESS=peer0.org2.example.com:7051
      - CORE_PEER_GOSSIP_EXTERNALENDPOINT=peer0.org2.example.com:7051
      - CORE_PEER_GOSSIP_BOOTSTRAP=peer0.org2.example.com:7051
      - CORE_PEER_LOCALMSPID=Org2MSP
    volumes:
        - /var/run/:/host/var/run/
        - ../crypto-config/peerOrganizations/org2.example.com/peers/peer0.org2.example.com/msp:/etc/hyperledger/fabric/msp
        - ../crypto-config/peerOrganizations/org2.example.com/peers/peer0.org2.example.com/tls:/etc/hyperledger/fabric/tls
    ports:
      - 9051:7051
      - 9053:7053

  peer1.org2.example.com:
    container_name: peer1.org2.example.com
    extends:
      file: peer-base.yaml
      service: peer-base
    environment:
      - CORE_PEER_ID=peer1.org2.example.com
      - CORE_PEER_ADDRESS=peer1.org2.example.com:7051
      - CORE_PEER_GOSSIP_EXTERNALENDPOINT=peer1.org2.example.com:7051
      - CORE_PEER_GOSSIP_BOOTSTRAP=peer1.org2.example.com:7051
      - CORE_PEER_LOCALMSPID=Org2MSP
    volumes:
        - /var/run/:/host/var/run/
        - ../crypto-config/peerOrganizations/org2.example.com/peers/peer1.org2.example.com/msp:/etc/hyperledger/fabric/msp
        - ../crypto-config/peerOrganizations/org2.example.com/peers/peer1.org2.example.com/tls:/etc/hyperledger/fabric/tls
    ports:
      - 10051:7051
      - 10053:7053
```

#### 2.3.3 peer-base.yaml

``` yaml
# Copyright IBM Corp. All Rights Reserved.
#
# SPDX-License-Identifier: Apache-2.0
#

version: '2'

services:
  peer-base:
    image: hyperledger/fabric-peer
    environment:
      - CORE_VM_ENDPOINT=unix:///host/var/run/doggcker.sock
      # the following setting starts chaincode containers on the same
      # bridge network as the peers
      # https://docs.docker.com/compose/networking/
      - CORE_VM_DOCKER_HOSTCONFIG_NETWORKMODE=${COMPOSE_PROJECT_NAME}_byfn
      #- CORE_LOGGING_LEVEL=ERROR
      - CORE_LOGGING_LEVEL=DEBUG
      - CORE_PEER_TLS_ENABLED=true
      - CORE_PEER_GOSSIP_USELEADERELECTION=true
      - CORE_PEER_GOSSIP_ORGLEADER=false
      - CORE_PEER_PROFILE_ENABLED=true
      - CORE_PEER_TLS_CERT_FILE=/etc/hyperledger/fabric/tls/server.crt
      - CORE_PEER_TLS_KEY_FILE=/etc/hyperledger/fabric/tls/server.key
      - CORE_PEER_TLS_ROOTCERT_FILE=/etc/hyperledger/fabric/tls/ca.crt
    working_dir: /opt/gopath/src/github.com/hyperledger/fabric/peer
    command: peer node start
```


### 2.4 启动容器

通过`byfn.sh up`（执行`networkUp`方法）来启动所有容器。实际执行的命令是：
``` bash
  if [ "${IF_COUCHDB}" == "couchdb" ]; then
      CHANNEL_NAME=$CHANNEL_NAME TIMEOUT=$CLI_TIMEOUT DELAY=$CLI_DELAY docker-compose -f $COMPOSE_FILE -f $COMPOSE_FILE_COUCH up -d 2>&1
  else
      CHANNEL_NAME=$CHANNEL_NAME TIMEOUT=$CLI_TIMEOUT DELAY=$CLI_DELAY docker-compose -f $COMPOSE_FILE up -d 2>&1
  fi
```

查看容器的执行结果，比如`cli`容器：
``` bash
docker logs -f cli
```

上述过程会涉及多个`容器`的启动。通过查看`docker-compse-cli.yaml`涉及的`容器`主要有：

| image | container | cmd | ports
| -- | -- | -- 
| hyperledger/fabric-orderer | orderer.example.com | orderer | 7050:7050
| hyperledger/fabric-peer | peer0.org1.example.com | peer node start | 7051:7051 7053:7053
| hyperledger/fabric-peer |  peer1.org1.example.com | peer node start | 8051:7051 8053:7053
| hyperledger/fabric-peer |  peer0.org2.example.com | peer node start | 9051:7051 9053:7053
| hyperledger/fabric-peer |  peer1.org2.example.com | peer node start | 10051:7051 10053:7053
| hyperledger/fabric-tools | cli | `/bin/bash -c './scripts/script.sh ${CHANNEL_NAME} ${DELAY}; sleep $TIMEOUT'` | NULL

#### 2.4.1 order - orderer.example.com

##### 容器启动的配置信息

``` yaml
 orderer.example.com:
    container_name: orderer.example.com
    image: hyperledger/fabric-orderer
    environment:
      - ORDERER_GENERAL_LOGLEVEL=debug
      - ORDERER_GENERAL_LISTENADDRESS=0.0.0.0
      - ORDERER_GENERAL_GENESISMETHOD=file
      - ORDERER_GENERAL_GENESISFILE=/var/hyperledger/orderer/orderer.genesis.block
      - ORDERER_GENERAL_LOCALMSPID=OrdererMSP
      - ORDERER_GENERAL_LOCALMSPDIR=/var/hyperledger/orderer/msp
      # enabled TLS
      - ORDERER_GENERAL_TLS_ENABLED=true
      - ORDERER_GENERAL_TLS_PRIVATEKEY=/var/hyperledger/orderer/tls/server.key
      - ORDERER_GENERAL_TLS_CERTIFICATE=/var/hyperledger/orderer/tls/server.crt
      - ORDERER_GENERAL_TLS_ROOTCAS=[/var/hyperledger/orderer/tls/ca.crt]
    working_dir: /opt/gopath/src/github.com/hyperledger/fabric
    command: orderer
    volumes:
    - ../channel-artifacts/genesis.block:/var/hyperledger/orderer/orderer.genesis.block
    - ../crypto-config/ordererOrganizations/example.com/orderers/orderer.example.com/msp:/var/hyperledger/orderer/msp
    - ../crypto-config/ordererOrganizations/example.com/orderers/orderer.example.com/tls/:/var/hyperledger/orderer/tls
    ports:
      - 7050:7050
```

##### 关于`Hyperledger Fabric Ordering Service`的介绍

https://github.com/hyperledger/fabric/tree/release/orderer

> The Hyperledger Fabric **ordering service** provides **an atomic broadcast ordering service for consumption by the peers**. This means that many clients can submit messages to the ordering service, and the same sequence of ordered batches will be delivered to all clients in response.

##### **Protocol definition**

* The atomic broadcast ordering protocol for Hyperledger Fabric is described in `hyperledger/fabric/protos/orderer/ab.proto`. 
* There are **two services**: the **Broadcast service** for injecting messages into the system and the **Deliver service** for receiving ordered batches from the service.

广播服务：负责收集消息
投递服务：负责接收排序结果


`ab.proto`协议内容：

``` protobuf
syntax = "proto3";

import "common/common.proto";

option go_package = "github.com/hyperledger/fabric/protos/orderer";
option java_package = "org.hyperledger.fabric.protos.orderer";

package orderer;

message BroadcastResponse {
    common.Status status = 1;
}

message SeekNewest { }

message SeekOldest { }

message SeekSpecified {
    uint64 number = 1;
}

message SeekPosition {
    oneof Type {
        SeekNewest newest = 1;
        SeekOldest oldest = 2;
        SeekSpecified specified = 3;
    }
}

// SeekInfo specifies the range of requested blocks to return
// If the start position is not found, an error is immediately returned
// Otherwise, blocks are returned until a missing block is encountered, then behavior is dictated
// by the SeekBehavior specified.  If BLOCK_UNTIL_READY is specified, the reply will block until
// the requested blocks are available, if FAIL_IF_NOT_READY is specified, the reply will return an
// error indicating that the block is not found.  To request that all blocks be returned indefinitely
// as they are created, behavior should be set to BLOCK_UNTIL_READY and the stop should be set to
// specified with a number of MAX_UINT64
message SeekInfo {
    enum SeekBehavior {
        BLOCK_UNTIL_READY = 0;
        FAIL_IF_NOT_READY = 1;
    }
    SeekPosition start = 1;    // The position to start the deliver from
    SeekPosition stop = 2;     // The position to stop the deliver
    SeekBehavior behavior = 3; // The behavior when a missing block is encountered
}

message DeliverResponse {
    oneof Type {
        common.Status status = 1;
        common.Block block = 2;
    }
}

service AtomicBroadcast {
    // broadcast receives a reply of Acknowledgement for each common.Envelope in order, indicating success or type of failure
    rpc Broadcast(stream common.Envelope) returns (stream BroadcastResponse) {}

    // deliver first requires an Envelope of type DELIVER_SEEK_INFO with Payload data as a mashaled SeekInfo message, then a stream of block replies is received.
    rpc Deliver(stream common.Envelope) returns (stream DeliverResponse) {}
}
```

##### **Service types**

* **Solo ordering service (testing)**: The solo ordering service is intended to be an extremely easy to deploy, non-production ordering service. It consists of a single process which serves all clients, so consensus is not required as there is a single central authority. There is correspondingly no high availability or scalability. This makes solo ideal for development and testing, but not for deployment.

* **Kafka-based ordering service (production)**: The Kafka-based ordering service **leverages the Kafka pub/sub system（发布/订阅） to perform the ordering**, but wraps this in the familiar `ab.proto` definition so that the peer orderer client code does not to be written specifically for Kafka. Kafka is currently the preferred choice for production deployments which demand high throughput and high availability, but do not require byzantine fault tolerance.

* **PBFT ordering service (pending)**: The PBFT（**Practical	Byzantine Fault Tolerance**）ordering service will use the Hyperledger Fabric PBFT implementation (currently under development) to order messages in a byzantine fault tolerant way.

> 补充：关于[Byzantine fault tolerance](https://en.wikipedia.org/wiki/Byzantine_fault_tolerance)的描述。

**Choosing a service type**

* In order to set a service type, the ordering service administrator needs to set the right value in the genesis block that the ordering service nodes will be bootstrapped from. Specifically, the value corresponding to the `ConsensusType` key of the Values map of the Orderer config group on the system channel should be set to either `solo` or `kafka`.
* For details on the configuration structure of channels, refer to the [Channel Configuration guide](https://github.com/hyperledger/fabric/blob/release/docs/source/configtx.rst).
* `configtxgen` is a tool that allows for the creation of a genesis block using profiles, or grouped configuration parameters — refer to the [Configuring using the connfigtxgen tool guide](https://github.com/hyperledger/fabric/blob/release/docs/source/configtxgen.rst) for more.
* The location of this block can be set using the `ORDERER_GENERAL_GENESISFILE` environment variable. As is the case with all the configuration paths for Fabric binaries, this location is relative to the path set via the `FABRIC_CFG_PATH` environment variable.

##### **Ledger types**

Because the ordering service must **allow clients to seek within the ordered batch stream**, orderers need **a backing ledger, where they maintain a local copy of past batches**. *Not all ledgers are crash fault tolerant, so care should be used when selecting a ledger for an application*. Because the orderer ledger interface is abstracted, the ledger type for a particular orderer may be selected at runtime. The following options are available:

* **File ledger (production)**: The file-based ledger stores blocks directly on the file system. The block locations on disk are 'indexed' in a lightweight **LevelDB database** by number so that clients can efficiently retrieve a block by number. This is the default, and the suggested option for production deployments.
* **RAM ledger (testing)**: The RAM ledger implementation is a simple development oriented ledger which stores batches purely in memory, with a configurable history size for retention. This ledger is not crash fault tolerant; restarting the process will reset the ledger to the genesis block.
* **JSON ledger (testing)**: The file ledger implementation is a simple development oriented ledger which stores batches as JSON encoded files on the filesystem. This is intended to make inspecting the ledger easy and to allow for crash fault tolerance. This ledger is not intended to be performant, but is intended to be simple and easy to deploy and understand.

**Choosing a ledger type**

This can be set by setting the `ORDERER_GENERAL_LEDGERTYPE` environment variable before executing the orderer binary. Acceptable values are `file (default)`, `ram`, and `json`.

##### Experimenting with the orderer service

To experiment with the orderer service you may build the orderer binary by simply typing `go build` in the `hyperledger/fabric/orderer` directory. You may then invoke the `orderer` binary with no parameters, or you can override the bind address, port, and backing ledger by setting the environment variables `ORDERER_GENERAL_LISTENADDRESS`, `ORDERER_GENERAL_LISTENPORT` and `ORDERER_GENERAL_LEDGER_TYPE` respectively.

There are sample clients in the `fabric/orderer/sample_clients` directory.

The `broadcast_timestamp` client sends a message containing the timestamp to the `Broadcast service`.
The `deliver_stdout` client prints received batches to stdout from the `Deliver interface`.
These may both be built simply by typing go build in their respective directories. Note that neither of these clients supports config (so editing the source manually to adjust address and port is required), or signing (so they can only work against channels where no ACL is enforced).

##### Profiling (Not Suggested)

Profiling the ordering service is possible through `a standard HTTP interface` [documented here](https://golang.org/pkg/net/http/pprof). The profiling service can be configured using the `orderer.yaml` file, or through `environment variables`. To enable profiling set `ORDERER_GENERAL_PROFILE_ENABLED=true`, and optionally set `ORDERER_GENERAL_PROFILE_ADDRESS` to the desired network address for the profiling service. The default address is `0.0.0.0:6060` as in the Golang documentation.

Note that failures of the profiling service, either at startup or anytime during the run, will cause the overall orderer service to fail. Therefore it is currently not recommended to enable profiling in production settings.


#### 2.4.2 peer - peer0.org1.example.com/...

##### 容器启动的配置信息

Demo中总共涉及到4个`peer`

| peer container | ports |
| -- | --
| peer0.org1.example.com | 7051:7051 7053:7053
| peer0.org2.example.com | 9051:7051 9053:7053
| peer1.org1.example.com | 8051:7051 8053:7053
| peer1.org2.example.com | 10051:7051 10053:7053

下面以`peer0.org1.example.com`为例进行说明。

``` yaml
 peer0.org1.example.com:
    container_name: peer0.org1.example.com
    extends:
      file: peer-base.yaml
      service: peer-base
    environment:
      - CORE_PEER_ID=peer0.org1.example.com
      - CORE_PEER_ADDRESS=peer0.org1.example.com:7051
      - CORE_PEER_GOSSIP_EXTERNALENDPOINT=peer0.org1.example.com:7051
      - CORE_PEER_LOCALMSPID=Org1MSP
    volumes:
        - /var/run/:/host/var/run/
        - ../crypto-config/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/msp:/etc/hyperledger/fabric/msp
        - ../crypto-config/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls:/etc/hyperledger/fabric/tls
    ports:
      - 7051:7051
      - 7053:7053

 peer-base:
    image: hyperledger/fabric-peer
    environment:
      - CORE_VM_ENDPOINT=unix:///host/var/run/docker.sock
      # the following setting starts chaincode containers on the same
      # bridge network as the peers
      # https://docs.docker.com/compose/networking/
      - CORE_VM_DOCKER_HOSTCONFIG_NETWORKMODE=${COMPOSE_PROJECT_NAME}_byfn
      #- CORE_LOGGING_LEVEL=ERROR
      - CORE_LOGGING_LEVEL=DEBUG
      - CORE_PEER_TLS_ENABLED=true
      - CORE_PEER_GOSSIP_USELEADERELECTION=true
      - CORE_PEER_GOSSIP_ORGLEADER=false
      - CORE_PEER_PROFILE_ENABLED=true
      - CORE_PEER_TLS_CERT_FILE=/etc/hyperledger/fabric/tls/server.crt
      - CORE_PEER_TLS_KEY_FILE=/etc/hyperledger/fabric/tls/server.key
      - CORE_PEER_TLS_ROOTCERT_FILE=/etc/hyperledger/fabric/tls/ca.crt
    working_dir: /opt/gopath/src/github.com/hyperledger/fabric/peer
    command: peer node start
```

额外环境变量：

- CORE_PEER_GOMAXPROCS 设置并发核数，主要用于golang的协程

#### 2.4.3 cli

`cli`容器作为测试客户端，用于初始化fabric网络，比如创建渠道，加入渠道，安装chaincode，实例化chaincode，query，以及invork等操作。

##### 容器启动的配置信息

``` yaml
container_name: cli
    image: hyperledger/fabric-tools
    tty: true
    environment:
      - GOPATH=/opt/gopath
      - CORE_VM_ENDPOINT=unix:///host/var/run/docker.sock
      - CORE_LOGGING_LEVEL=DEBUG
      - CORE_PEER_ID=cli
      - CORE_PEER_ADDRESS=peer0.org1.example.com:7051
      - CORE_PEER_LOCALMSPID=Org1MSP
      - CORE_PEER_TLS_ENABLED=true
      - CORE_PEER_TLS_CERT_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/server.crt
      - CORE_PEER_TLS_KEY_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/server.key
      - CORE_PEER_TLS_ROOTCERT_FILE=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/ca.crt
      - CORE_PEER_MSPCONFIGPATH=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/users/Admin@org1.example.com/msp
    working_dir: /opt/gopath/src/github.com/hyperledger/fabric/peer
    command: /bin/bash -c './scripts/script.sh ${CHANNEL_NAME} ${DELAY}; sleep $TIMEOUT'
    volumes:
        - /var/run/:/host/var/run/
        - ./../chaincode/:/opt/gopath/src/github.com/hyperledger/fabric/examples/chaincode/go
        - ./crypto-config:/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/
        - ./scripts:/opt/gopath/src/github.com/hyperledger/fabric/peer/scripts/
        - ./channel-artifacts:/opt/gopath/src/github.com/hyperledger/fabric/peer/channel-artifacts
    depends_on:
      - orderer.example.com
      - peer0.org1.example.com
      - peer1.org1.example.com
      - peer0.org2.example.com
      - peer1.org2.example.com
    networks:
      - byfn
```

### 2.5 编译

整个`fabric`项目可以通过[Makefile](https://github.com/hyperledger/fabric/blob/release/Makefile)来编译。通过`make -n > make_op.out`可以先看下make会执行哪些操作，便于我们理解会构建哪些模块，以及每个模块是如何构建的。

编译依赖：

apt-get update
apt-get install make
apt-get install gcc libltdl-dev

`Makefile`用法

``` tex
# -------------------------------------------------------------
# This makefile defines the following targets
#
#   - all (default) - builds all targets and runs all tests/checks
#   - checks - runs all tests/checks
#   - desk-check - runs linters and verify to test changed packages
#   - configtxgen - builds a native configtxgen binary
#   - configtxlator - builds a native configtxlator binary
#   - cryptogen  -  builds a native cryptogen binary
#   - peer - builds a native fabric peer binary
#   - orderer - builds a native fabric orderer binary
#   - release - builds release packages for the host platform
#   - release-all - builds release packages for all target platforms
#   - unit-test - runs the go-test based unit tests
#   - verify - runs unit tests for only the changed package tree
#   - test-cmd - generates a "go test" string suitable for manual customization
#   - behave - runs the behave test
#   - behave-deps - ensures pre-requisites are available for running behave manually
#   - gotools - installs go tools like golint
#   - linter - runs all code checks
#   - license - checks go sourrce files for Apache license header
#   - native - ensures all native binaries are available
#   - docker[-clean] - ensures all docker images are available[/cleaned]
#   - peer-docker[-clean] - ensures the peer container is available[/cleaned]
#   - orderer-docker[-clean] - ensures the orderer container is available[/cleaned]
#   - tools-docker[-clean] - ensures the tools container is available[/cleaned]
#   - protos - generate all protobuf artifacts based on .proto files
#   - clean - cleans the build area
#   - clean-all - superset of 'clean' that also removes persistent state
#   - dist-clean - clean release packages for all target platforms
#   - unit-test-clean - cleans unit test state (particularly from docker)
```

以`images/peer/Dockerfile.in`为例，在创建镜像时，会把编译生成的工具和配置安装在容器环境的相应位置。其中：
*  `/usr/local/bin`存放二进制程序，比如peer镜像存放的是`peer`程序。
*  `/etc/hyperledger/fabric`存放`sampleconfig`的相关内容，其中`ADD`命令会执行自动解压操作。

``` dockerfile
FROM _BASE_NS_/fabric-baseos:_BASE_TAG_
ENV FABRIC_CFG_PATH /etc/hyperledger/fabric
RUN mkdir -p /var/hyperledger/production $FABRIC_CFG_PATH
COPY payload/peer /usr/local/bin
ADD  payload/sampleconfig.tar.bz2 $FABRIC_CFG_PATH
CMD ["peer","node","start"]
```

``` bash
# payload definitions'
build/image/ccenv/payload:      build/docker/gotools/bin/protoc-gen-go \
				build/bin/chaintool \
				build/goshim.tar.bz2
build/image/javaenv/payload:    build/javashim.tar.bz2 \
				build/protos.tar.bz2 \
				settings.gradle
build/image/peer/payload:       build/docker/bin/peer \
				build/sampleconfig.tar.bz2
build/image/orderer/payload:    build/docker/bin/orderer \
				build/sampleconfig.tar.bz2
build/image/buildenv/payload:   build/gotools.tar.bz2 \
				build/docker/gotools/bin/protoc-gen-go
build/image/testenv/payload:    build/docker/bin/orderer \
				build/docker/bin/peer \
				build/sampleconfig.tar.bz2 \
				images/testenv/install-softhsm2.sh
build/image/zookeeper/payload:  images/zookeeper/docker-entrypoint.sh
build/image/kafka/payload:      images/kafka/docker-entrypoint.sh \
				images/kafka/kafka-run-class.sh
build/image/couchdb/payload:	images/couchdb/docker-entrypoint.sh \
				images/couchdb/local.ini \
				images/couchdb/vm.args
build/image/tools/payload:      build/docker/bin/cryptogen \
	                        build/docker/bin/configtxgen \
	                        build/docker/bin/configtxlator \
				build/docker/bin/peer \
				build/sampleconfig.tar.bz2
```

下面抽取了一些关键的`make`信息：

``` bash
mkdir -p build/docker/gotools/bin build/docker/gotools/obj
docker run -i --rm --user=0 -v /root/go/src/github.com/hyperledger/fabric:/opt/gopath/src/github.com/hyperledger/fabric \
	-w /opt/gopath/src/github.com/hyperledger/fabric \
	-v /root/go/src/github.com/hyperledger/fabric/build/docker/gotools:/opt/gotools \
	-w /opt/gopath/src/github.com/hyperledger/fabric/gotools \
	hyperledger/fabric-baseimage:x86_64-0.3.2 \
	make install BINDIR=/opt/gotools/bin OBJDIR=/opt/gotools/obj
	
echo "Installing chaintool"
mkdir -p build/bin
curl -fL https://nexus.hyperledger.org/content/repositories/releases/org/hyperledger/fabric/hyperledger-fabric/chaintool-1.0.0/hyperledger-fabric-chaintool-1.0.0.jar > build/bin/chaintool
chmod +x build/bin/chaintool

echo "Creating build/goshim.tar.bz2"
tar -jhc -C /root/go/src \
	github.com/hyperledger/fabric/bccsp/aesopts.go \
	github.com/hyperledger/fabric/bccsp/bccsp.go \
	github.com/hyperledger/fabric/bccsp/bccsp_test.go \
	github.com/hyperledger/fabric/bccsp/ecdsaopts.go \
	... \
	github.com/hyperledger/fabric/vendor/gopkg.in/yaml.v2/yamlprivateh.go > build/goshim.tar.bz2

mkdir -p build/image/ccenv/payload
cp build/docker/gotools/bin/protoc-gen-go build/bin/chaintool build/goshim.tar.bz2 build/image/ccenv/payload
cat images/ccenv/Dockerfile.in \
	| sed -e 's/_BASE_NS_/hyperledger/g' \
	| sed -e 's/_NS_/hyperledger/g' \
	| sed -e 's/_BASE_TAG_/x86_64-0.3.2/g' \
	| sed -e 's/_TAG_/x86_64-1.0.5/g' \
	> build/image/ccenv/Dockerfile

# -----------------------------------
# 创建镜像 hyperledger/fabric-ccenv
# -----------------------------------
echo "Building docker ccenv-image"
docker build  -t hyperledger/fabric-ccenv build/image/ccenv
docker tag hyperledger/fabric-ccenv hyperledger/fabric-ccenv:x86_64-1.0.5
touch build/image/ccenv/.dummy-x86_64-1.0.5

echo "Creating build/javashim.tar.bz2"
tar -jc core/chaincode/shim/java/.gitignore \
	core/chaincode/shim/java/build.gradle \
	core/chaincode/shim/java/javabuild.sh \
	core/chaincode/shim/java/src/test/java/org/hyperledger/fabric/shim/ledger/CompositeKeyTest.java > build/javashim.tar.bz2
	
echo "Creating build/protos.tar.bz2"
tar -jc common/tools/protolator/testprotos/sample.proto \
	core/comm/testdata/grpc/test.proto \
	examples/chaincode/go/utxo/util/dah.proto \
	protos/peer/transaction.proto > build/protos.tar.bz2

mkdir -p build/image/javaenv/payload
cp build/javashim.tar.bz2 build/protos.tar.bz2 settings.gradle build/image/javaenv/payload
cat images/javaenv/Dockerfile.in \
	| sed -e 's/_BASE_NS_/hyperledger/g' \
	| sed -e 's/_NS_/hyperledger/g' \
	| sed -e 's/_BASE_TAG_/x86_64-0.3.2/g' \
	| sed -e 's/_TAG_/x86_64-1.0.5/g' \
	> build/image/javaenv/Dockerfile

# -----------------------------------
# 创建镜像 hyperledger/fabric-javaenv
# -----------------------------------
echo "Building docker javaenv-image"	
docker build  -t hyperledger/fabric-javaenv build/image/javaenv
docker tag hyperledger/fabric-javaenv hyperledger/fabric-javaenv:x86_64-1.0.5
touch build/image/javaenv/.dummy-x86_64-1.0.5

# 创建 build/bin/peer
mkdir -p build/bin
echo "build/bin/peer"
CGO_CFLAGS=" " 
GOBIN=/root/go/src/github.com/hyperledger/fabric/build/bin 
go install -tags "" -ldflags "-X github.com/hyperledger/fabric/common/metadata.Version=1.0.5 -X github.com/hyperledger/fabric/common/metadata.BaseVersion=0.3.2 -X github.com/hyperledger/fabric/common/metadata.BaseDockerLabel=org.hyperledger.fabric -X github.com/hyperledger/fabric/common/metadata.DockerNamespace=hyperledger -X github.com/hyperledger/fabric/common/metadata.BaseDockerNamespace=hyperledger" github.com/hyperledger/fabric/peer
echo "Binary available as build/bin/peer"
touch build/bin/peer

# 创建 build/bin/orderer
mkdir -p build/bin
echo "build/bin/orderer"
CGO_CFLAGS=" " 
GOBIN=/root/go/src/github.com/hyperledger/fabric/build/bin 
go install -tags "" -ldflags "-X github.com/hyperledger/fabric/common/metadata.Version=1.0.5 -X github.com/hyperledger/fabric/common/metadata.BaseVersion=0.3.2 -X github.com/hyperledger/fabric/common/metadata.BaseDockerLabel=org.hyperledger.fabric -X github.com/hyperledger/fabric/common/metadata.DockerNamespace=hyperledger -X github.com/hyperledger/fabric/common/metadata.BaseDockerNamespace=hyperledger" github.com/hyperledger/fabric/orderer
echo "Binary available as build/bin/orderer"
touch build/bin/orderer

# 创建 build/bin/configtxgen
mkdir -p build/bin
echo "build/bin/configtxgen"
CGO_CFLAGS=" " 
GOBIN=/root/go/src/github.com/hyperledger/fabric/build/bin 
go install -tags "nopkcs11" -ldflags "-X github.com/hyperledger/fabric/common/configtx/tool/configtxgen/metadata.Version=1.0.5" github.com/hyperledger/fabric/common/configtx/tool/configtxgen
echo "Binary available as build/bin/configtxgen"
touch build/bin/configtxgen

# 创建 build/bin/cryptogen
mkdir -p build/bin
echo "build/bin/cryptogen"
CGO_CFLAGS=" " 
GOBIN=/root/go/src/github.com/hyperledger/fabric/build/bin 
go install -tags "" -ldflags "-X github.com/hyperledger/fabric/common/tools/cryptogen/metadata.Version=1.0.5" github.com/hyperledger/fabric/common/tools/cryptogen
echo "Binary available as build/bin/cryptogen"
touch build/bin/cryptogen

# 创建 build/bin/configtxlator
mkdir -p build/bin
echo "build/bin/configtxlator"
CGO_CFLAGS=" " 
GOBIN=/root/go/src/github.com/hyperledger/fabric/build/bin 
go install -tags "" -ldflags "-X github.com/hyperledger/fabric/common/tools/configtxlator/metadata.Version=1.0.5" github.com/hyperledger/fabric/common/tools/configtxlator
echo "Binary available as build/bin/configtxlator"
touch build/bin/configtxlator

# -----------------------------------
# 创建镜像 hyperledger/fabric-peer
# -----------------------------------
echo "Building build/docker/bin/peer"
mkdir -p build/docker/bin build/docker/peer/pkg
# ...
touch build/docker/bin/peer
(cd sampleconfig && tar -jc *) > build/sampleconfig.tar.bz2
mkdir -p build/image/peer/payload
cp build/docker/bin/peer build/sampleconfig.tar.bz2 build/image/peer/payload
cat images/peer/Dockerfile.in \
	| sed -e 's/_BASE_NS_/hyperledger/g' \
	| sed -e 's/_NS_/hyperledger/g' \
	| sed -e 's/_BASE_TAG_/x86_64-0.3.2/g' \
	| sed -e 's/_TAG_/x86_64-1.0.5/g' \
	> build/image/peer/Dockerfile

# 使用build/image/peer目录下的Dockerfile编译
echo "Building docker peer-image"
docker build  -t hyperledger/fabric-peer build/image/peer
docker tag hyperledger/fabric-peer hyperledger/fabric-peer:x86_64-1.0.5
touch build/image/peer/.dummy-x86_64-1.0.5

# -----------------------------------
# 创建镜像 hyperledger/fabric-orderer
# -----------------------------------
echo "Building build/docker/bin/orderer"
mkdir -p build/docker/bin build/docker/orderer/pkg
# ...
touch build/docker/bin/orderer
echo "Building docker orderer-image"
docker build  -t hyperledger/fabric-orderer build/image/orderer
docker tag hyperledger/fabric-orderer hyperledger/fabric-orderer:x86_64-1.0.5
touch build/image/orderer/.dummy-x86_64-1.0.5

# -----------------------------------
# 创建镜像 hyperledger/fabric-buildenv
# -----------------------------------
echo "Building docker buildenv-image"
docker build  -t hyperledger/fabric-buildenv build/image/buildenv
docker tag hyperledger/fabric-buildenv hyperledger/fabric-buildenv:x86_64-1.0.5
touch build/image/buildenv/.dummy-x86_64-1.0.5

# -----------------------------------
# 创建镜像 hyperledger/fabric-testenv 
# -----------------------------------
echo "Building docker testenv-image"

# -----------------------------------
# 创建镜像 hyperledger/fabric-zookeeper
# -----------------------------------
echo "Building docker zookeeper-image"

# -----------------------------------
# 创建镜像 hyperledger/fabric-kafka
# -----------------------------------
echo "Building docker kafka-image"

# -----------------------------------
# 创建镜像 hyperledger/fabric-couchdb
# -----------------------------------
echo "Building docker couchdb-image"

# -----------------------------------
# 创建镜像 hyperledger/fabric-tools
# -----------------------------------
echo "Building docker tools-image"


scripts/check_license.sh
scripts/check_spelling.sh
echo "LINT: Running code checks.."

docker run -i --rm --user=0 -v /root/go/src/github.com/hyperledger/fabric:/opt/gopath/src/github.com/hyperledger/fabric -w /opt/gopath/src/github.com/hyperledger/fabric hyperledger/fabric-buildenv:x86_64-1.0.5 ./scripts/golinter.sh

cd unit-test && docker-compose down
cd unit-test && docker-compose up --abort-on-container-exit --force-recreate && docker-compose down

echo "build/bin/block-listener"
echo "Binary available as build/bin/block-listener"
touch build/bin/block-listener
echo "Running behave tests"
cd bddtests; behave
```

编译后会生成一个`build`子目录，保存了编译后信息：

``` bash
root@ubuntu-s-1vcpu-3gb-nyc3-01:~/go/src/github.com/hyperledger/fabric/build# du -csh *
96M     bin
316M    docker
696K    goshim.tar.bz2
15M     gotools.tar.bz2
198M    image
20K     javashim.tar.bz2
20K     protos.tar.bz2
12K     sampleconfig.tar.bz2
624M    total

root@ubuntu-s-1vcpu-3gb-nyc3-01:~/go/src/github.com/hyperledger/fabric/build/bin# ls -rtlh
total 96M
-rwxr-xr-x 1 root root  16M Dec  7 09:10 chaintool
-rwxr-xr-x 1 root root  23M Dec  7 10:09 peer
-rwxr-xr-x 1 root root  20M Dec  7 10:09 orderer
-rwxr-xr-x 1 root root  15M Dec  7 10:09 configtxgen
-rwxr-xr-x 1 root root 7.7M Dec  7 10:09 cryptogen
-rwxr-xr-x 1 root root  16M Dec  7 10:10 configtxlator

root@ubuntu-s-1vcpu-3gb-nyc3-01:~/go/src/github.com/hyperledger/fabric/build/image# ls -rtlh
total 40K
drwxr-xr-x 3 root root 4.0K Dec  7 09:10 ccenv
drwxr-xr-x 3 root root 4.0K Dec  7 09:11 javaenv
drwxr-xr-x 3 root root 4.0K Dec  7 09:17 peer
drwxr-xr-x 3 root root 4.0K Dec  7 09:18 orderer
drwxr-xr-x 3 root root 4.0K Dec  7 09:18 buildenv
drwxr-xr-x 3 root root 4.0K Dec  7 09:19 testenv
drwxr-xr-x 3 root root 4.0K Dec  7 09:19 zookeeper
drwxr-xr-x 3 root root 4.0K Dec  7 09:19 kafka
drwxr-xr-x 3 root root 4.0K Dec  7 09:21 couchdb
drwxr-xr-x 3 root root 4.0K Dec  7 09:22 tools
```

通过`docker images`可以看到生成的镜像信息。


### 2.6 源码解析

fabric的整体代码结构说明：

https://github.com/yeasy/hyperledger_code_fabric/blob/master/overview.md

实现 fabric 功能的核心代码，包括：

> bccsp 包：实现对加解密算法和机制的支持。
common 包：一些通用的模块；
core 包：大部分核心实现代码都在本包下。其它包的代码封装上层接口，最终调用本包内代码；
events 包：支持 event 框架；
examples 包：包括一些示例的 chaincode 代码；
gossip 包：实现 gossip 协议；
msp 包：Member Service Provider 包；
order 包：order 服务相关的入口和框架代码；
peer 包：peer 的入口和框架代码；
protos 包：包括各种协议和消息的 protobuf 定义文件和生成的 go 文件。

#### 2.6.1 first-network/script/script.sh

`cli`容器启动后会做哪些事情。通过[script.sh](https://github.com/hyperledger/fabric-samples/blob/release/first-network/scripts/script.sh)脚本，可以了解到整个区块链的交互过程。

> `command: /bin/bash -c './scripts/script.sh ${CHANNEL_NAME} ${DELAY}; sleep $TIMEOUT'`

在`script.sh`脚本中，会调用`peer`程序，具体命令含义如下。

| BIN | CMD | SUBCMD | Jobs
| -- | -- | -- | --
| peer | node | start;status
| peer | channel | create | 1, sendCreateChainTransaction（broadcast）-> order:7050 
| | | | 2, getGenesisBlock（deliver） 
| | | fetch
| | | join | 1, getJoinCCSpec
| | | | 2, ProcessProposal（endorser）
| | | list
| | | update | 1, broadcastClient.Send(sCtxEnv)
| peer | chaincode | install | 1, EndorserClient
| | | instantiate | 1, EndorserClient, instantiate the command via Endorser
| | | invoke | endorserClient
| | | package
| | | query | endorserClient
| | | signpackage
| | | upgrade
| peer | logging | getlevel
| | | setlevel
| | | revertlevels
| peer | version

在`script.sh`脚本中，主要执行以下几个操作（都是调用`peer`程序完成的，需要传递不同的环境变量）：

``` bash
## Create channel
echo "Creating channel..."
createChannel

## Join all the peers to the channel
echo "Having all peers join the channel..."
joinChannel

## Set the anchor peers for each org in the channel
echo "Updating anchor peers for org1..."
updateAnchorPeers 0
echo "Updating anchor peers for org2..."
updateAnchorPeers 2

## Install chaincode on Peer0/Org1 and Peer2/Org2
echo "Installing chaincode on org1/peer0..."
installChaincode 0
echo "Install chaincode on org2/peer2..."
installChaincode 2

#Instantiate chaincode on Peer2/Org2
echo "Instantiating chaincode on org2/peer2..."
instantiateChaincode 2

#Query on chaincode on Peer0/Org1
echo "Querying chaincode on org1/peer0..."
chaincodeQuery 0 100

#Invoke on chaincode on Peer0/Org1
echo "Sending invoke transaction on org1/peer0..."
chaincodeInvoke 0

## Install chaincode on Peer3/Org2
echo "Installing chaincode on org2/peer3..."
installChaincode 3

#Query on chaincode on Peer3/Org2, check if the result is 90
echo "Querying chaincode on org2/peer3..."
chaincodeQuery 3 90
```

##### (1) createChannel

`setGlobals`方法会分别设置四个`peer`的几个环境变量的信息。包括：

| ENV | VALUE |
| -- | --
| CORE_PEER_LOCALMSPID | Org1MS ; Org2MSP
| CORE_PEER_TLS_ROOTCERT_FILE | /opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/tls/ca.crt
| CORE_PEER_MSPCONFIGPATH | /opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/peerOrganizations/org1.example.com/users/Admin@org1.example.com/msp
| CORE_PEER_ADDRESS | peer0.org1.example.com:7051 ; peer1.org1.example.com:7051 ; peer0.org2.example.com:7051 ; peer1.org2.example.com:7051

> 通过 `env |grep CORE` 可查看。

``` bash
createChannel() {
  setGlobals 0

  if [ -z "$CORE_PEER_TLS_ENABLED" -o "$CORE_PEER_TLS_ENABLED" = "false" ]; then
		peer channel create -o orderer.example.com:7050 -c $CHANNEL_NAME -f ./channel-artifacts/channel.tx >&log.txt
	else
		peer channel create -o orderer.example.com:7050 -c $CHANNEL_NAME -f ./channel-artifacts/channel.tx --tls $CORE_PEER_TLS_ENABLED --cafile $ORDERER_CA >&log.txt
	fi
	res=$?
	cat log.txt
	verifyResult $res "Channel creation failed"
	echo "===================== Channel \"$CHANNEL_NAME\" is created successfully ===================== "
	echo
}
```

在不考虑TLS安全验证的情况下，实际执行如下命令：

> `peer channel create -o orderer.example.com:7050 -c $CHANNEL_NAME -f ./channel-artifacts/channel.tx >&log.txt`

其中，
* `-c $CHANNEL_NAME `：指定要创建的`channel`的名字，默认是`mychannel`
* `-f ./channel-artifacts/channel.tx`：通过`configtxgen`工具生成的配置文件

``` tex
Flags:
  -c, --channelID string   In case of a newChain command, the channel ID to create.
  -f, --file string        Configuration transaction file generated by a tool such as configtxgen for submitting to orderer
  -h, --help               help for create
  -t, --timeout int        Channel creation timeout (default 5)

Global Flags:
      --cafile string              Path to file containing PEM-encoded trusted certificate(s) for the ordering endpoint
      --logging-level string       Default logging level and overrides, see core.yaml for full syntax
  -o, --orderer string             Ordering service endpoint
      --test.coverprofile string   Done (default "coverage.cov")
      --tls                        Use TLS when communicating with the orderer endpoint
  -v, --version                    Display current version of fabric peer server
```

##### (2) joinChannel

> `peer channel join -b $CHANNEL_NAME.block  >&log.txt`

##### (3) updateAnchorPeers

> `peer channel update -o orderer.example.com:7050 -c $CHANNEL_NAME -f ./channel-artifacts/${CORE_PEER_LOCALMSPID}anchors.tx >&log.txt`

**Anchor peer vs. Leading peer**

Anchors peers encoded into config update transaction in order to enable communication between peers of different organizations and discover all active participants of the channel.

Leaders and anchor peers are not very related terms in the context of Hyperledger Fabric.

**Anchor peers** - used to advertise peers from different organization to eventually build up shared membership view of all peers in the channels from all organizations.

**Leaders** - responsible to maintain highly available connection with ordering service to guarantee cluster of peers will be able to make a progress and receive new blocks.

There is no election for anchor peers, if a anchor peer crash, how to select a peer to take over the work?

Once network is bootstrapped and peers has build membership view, the failure on anchor peer won't cause a lot of damage. But yes, operators will have to take care to maintain anchor peers highly available in general. Also you can always run config update transaction with new anchor peer endpoints.

[Anchor Peer](http://hyperledger-fabric.readthedocs.io/en/release/glossary.html#anchor-peer)
[what-is-anchor-peer-in-fabric](https://stackoverflow.com/questions/45632859/what-is-anchor-peer-in-fabric)
[what's the difference between anchor peer and a gossip leading peer?](https://stackoverflow.com/questions/46904681/whats-the-difference-between-anchor-peer-and-a-gossip-leading-peer)
[How to configure multiple anchor peers](https://stackoverflow.com/questions/45610253/how-to-configure-multiple-anchor-peers)
https://stackoverflow.com/questions/46638813/in-examples-e2e-cli-sample-demo-all-peers-are-default-anchor-peers-no-matter-h

##### (4) installChaincode 

> `peer chaincode install -n mycc -v 1.0 -p github.com/hyperledger/fabric/examples/chaincode/go/chaincode_example02 >&log.txt`

``` tex
Flags:
  -c, --ctor string      Constructor message for the chaincode in JSON format (default "{}")
  -l, --lang string      Language the chaincode is written in (default "golang")
  -n, --name string      Name of the chaincode
  -p, --path string      Path to chaincode
  -v, --version string   Version of the chaincode specified in install/instantiate/upgrade commands
```

##### (5) instantiateChaincode 

> `peer chaincode instantiate -o orderer.example.com:7050 -C $CHANNEL_NAME -n mycc -v 1.0 -c '{"Args":["init","a","100","b","200"]}' -P "OR	('Org1MSP.member','Org2MSP.member')" >&log.txt`

``` tex
Flags:
  -C, --channelID string   The channel on which this command should be executed (default "testchainid")
  -c, --ctor string        Constructor message for the chaincode in JSON format (default "{}")
  -E, --escc string        The name of the endorsement system chaincode to be used for this chaincode
  -l, --lang string        Language the chaincode is written in (default "golang")
  -n, --name string        Name of the chaincode
  -P, --policy string      The endorsement policy associated to this chaincode
  -v, --version string     Version of the chaincode specified in install/instantiate/upgrade commands
  -V, --vscc string        The name of the verification system chaincode to be used for this chaincode
```

##### (6) chaincodeQuery 

> `peer chaincode query -C $CHANNEL_NAME -n mycc -c '{"Args":["query","a"]}' >&log.txt`

``` tex
Flags:
  -C, --channelID string   The channel on which this command should be executed (default "testchainid")
  -c, --ctor string        Constructor message for the chaincode in JSON format (default "{}")
  -x, --hex                If true, output the query value byte array in hexadecimal. Incompatible with --raw
  -n, --name string        Name of the chaincode
  -r, --raw                If true, output the query value as raw bytes, otherwise format as a printable string
  -t, --tid string         Name of a custom ID generation algorithm (hashing and decoding) e.g. sha256base64
```

##### (7) chaincodeInvoke

> `peer chaincode invoke -o orderer.example.com:7050 -C $CHANNEL_NAME -n mycc -c '{"Args":["invoke","a","b","10"]}' >&log.txt`

``` tex
Flags:
  -C, --channelID string   The channel on which this command should be executed (default "testchainid")
  -c, --ctor string        Constructor message for the chaincode in JSON format (default "{}")
  -n, --name string        Name of the chaincode
```


#### 2.6.2 protos

Protobuf 格式的数据结构和消息协议。都在同一个 protos 包内。

这里面是所有基本的数据结构（message）定义和 GRPC 的服务（service）接口声明。

所有的 .proto 文件是 protobuf 格式的声明文件，.pb.go 文件是基于 .proto 文件生成的 go 语言的类文件。

生成的结构体，一般都至少默认支持 4 个默认生成的方法。

* Reset()：重置结构体。
* String() string：返回代表对象的字符串。
* ProtoMessage()：协议消息。
* Descriptor([]byte, []int)：描述信息。

https://github.com/yeasy/hyperledger_code_fabric/blob/master/protos/README.md

#### 2.6.3 peer

源码路径：https://github.com/hyperledger/fabric/tree/release/peer

主命令模块。

作为服务端时候，支持 node 子命令；作为命令行时候，支持 chaincode、channel 等子命令。

作为命令行时候，会维持一个`ChaincodeCmdFactory`结构。

``` go
type ChaincodeCmdFactory struct {
	EndorserClient  pb.EndorserClient
	Signer          msp.SigningIdentity
	BroadcastClient common.BroadcastClient
}
```

其中：

* EndorserClient 是跟 peer.address 指定地址通信的 grpc 通道；
* Signer 为 LocalMSP 中的默认签名实体；
* BroadcastClient 是连接到通过 -o 指定的 orderer 服务的 grpc 通道。

`peer`的用法：
``` bash
root@ubuntu-s-1vcpu-3gb-nyc3-01:~/go/src/github.com/hyperledger/fabric/build/bin# ./peer 
Usage:
  peer [flags]
  peer [command]

Available Commands:
  chaincode   Operate a chaincode: install|instantiate|invoke|package|query|signpackage|upgrade.
  channel     Operate a channel: create|fetch|join|list|update.
  logging     Log levels: getlevel|setlevel|revertlevels.
  node        Operate a peer node: start|status.
  version     Print fabric peer version.

Flags:
  -h, --help                       help for peer
      --logging-level string       Default logging level and overrides, see core.yaml for full syntax
      --test.coverprofile string   Done (default "coverage.cov")
  -v, --version                    Display current version of fabric peer server

Use "peer [command] --help" for more information about a command.
```

#####  1 node

`node`代码中的一些关键实现：

* [cobra](https://github.com/spf13/cobra)实现CLI命令行解析。执行`peer node start`，其中`node`是一级命令，而`start`是`node`的子命令（即二级命令），其中，`Command`的用法可以参考[cobra Command](https://godoc.org/github.com/spf13/cobra#Command)，`flag`的用法可以参考[pflag](https://github.com/spf13/pflag)。

``` go
type Command struct {

    // The *Run functions are executed in the following order:
    //   * PersistentPreRun()
    //   * PreRun()
    //   * Run()
    //   * PostRun()
    //   * PersistentPostRun()
    // All functions get the same args, the arguments after the command name.
   
    // ... 
}
```

* [viper](https://github.com/spf13/viper)用于读取配置，这里是通过环境变量的方式来获取配置。例如，环境变量在`yaml`中是通过`CORE_LOGGING_LEVEL=DEBUG`的方式指定。

> 注意环境变量的格式要求，the ENV variable is IN ALL CAPS. When you explicitly provide the ENV variable name, it does not automatically add the prefix.  参考[Working with Environment Variables](https://github.com/spf13/viper#working-with-environment-variables)。

* 日志功能。通过`logger.Info`和`logger.Infof`（格式化）两种方式输出日志。`fabric/common/flogging`日志模块封装了[go-logging](https://github.com/op/go-logging)，`go-logging`主要用于设置日志级别和日志格式。[日志级别](https://github.com/op/go-logging/blob/master/level.go)分为：

``` go
// Log levels.
const (
	CRITICAL Level = iota
	ERROR
	WARNING
	NOTICE
	INFO
	DEBUG
)
```
关于日志格式说明可以参考[这里](https://godoc.org/github.com/op/go-logging)。

> %{id}        Sequence number for log message (uint64).
%{pid}       Process id (int)
%{time}      Time when log occurred (time.Time)
%{level}     Log level (Level)
%{module}    Module (string)
%{program}   Basename of os.Args[0] (string)
%{message}   Message (string)
%{longfile}  Full file name and line number: /a/b/c/d.go:23
%{shortfile} Final file name element and line number: d.go:23
%{callpath}  Callpath like main.a.b.c...c  "..." meaning recursive call ~. meaning truncated path
%{color}     ANSI color based on log level

建议：在测试环节，将[core.yaml](https://github.com/hyperledger/fabric/blob/release/sampleconfig/core.yaml)中logging.peer的日志级别从`info`修改为`debug`，因为代码中很多debug的日志，便于查看更多的调试信息。

* 如果单独运行`github.com/hyperledger/fabric/build/bin`目录下的`peer`程序（比如，`peer node start`），由于没有设置环境变量，默认会读取`src/github.com/hyperledger/fabric/sampleconfig`路径下的`core.yaml`配置文件。

``` go
// https://github.com/hyperledger/fabric/blob/release/peer/main.go
func main() {
    // ...
	err := common.InitConfig(cmdRoot)
	if err != nil { // Handle errors reading the config file
		panic(fmt.Errorf("Fatal error when initializing %s config : %s\n", cmdRoot, err))
	}
    // ...
}

// common.InitConfig->InitViper->GetDevConfigDir
// https://github.com/hyperledger/fabric/blob/release/core/config/config.go
const OfficialPath = "/etc/hyperledger/fabric"

//----------------------------------------------------------------------------------
// GetDevConfigDir()
//----------------------------------------------------------------------------------
// Returns the path to the default configuration that is maintained with the source
// tree.  Only valid to call from a test/development context.
//----------------------------------------------------------------------------------
func GetDevConfigDir() (string, error) {
	gopath := os.Getenv("GOPATH")
	if gopath == "" {
		return "", fmt.Errorf("GOPATH not set")
	}

	for _, p := range filepath.SplitList(gopath) {
		devPath := filepath.Join(p, "src/github.com/hyperledger/fabric/sampleconfig")
		if !dirExists(devPath) {
			continue
		}

		return devPath, nil
	}

	return "", fmt.Errorf("DevConfigDir not found in %s", gopath)
}
```

* 在`Makefile`中，通过`go install -ldflags "-X a=b"`的方式，可以通过外部环境变量初始化[代码中的常量](https://github.com/hyperledger/fabric/blob/release/common/metadata/metadata.go)[（详细介绍）](https://blog.cloudflare.com/setting-go-variables-at-compile-time/)。
``` bash
go install -tags "" -ldflags "-X github.com/hyperledger/fabric/common/metadata.Version=1.0.5 -X github.com/hyperledger/fabric/common/metadata.BaseVersion=0.3.2 -X github.com/hyperledger/fabric/common/metadata.BaseDockerLabel=org.hyperledger.fabric -X github.com/hyperledger/fabric/common/metadata.DockerNamespace=hyperledger -X github.com/hyperledger/fabric/common/metadata.BaseDockerNamespace=hyperledger" github.com/hyperledger/fabric/peer
```

* 关于`ledger`的创建（每个`peer`都会维护一份`ledger`的存储），默认使用[goleveldb](https://github.com/syndtr/goleveldb)作为存储。在`core.yaml`配置中指定了`ledger`存储的位置（`peer.fileSystemPath: /var/hyperledger/production`）对于存储`ledger`的目录需要由权限控制，如果直接对`ledger`数据进行修改，可能导致`peer`读写`ledger`操作失败。

``` bash
# core.yaml
# Path on the file system where peer will store data (eg ledger). This
# location must be access control protected to prevent unintended
# modification that might corrupt the peer operations.
fileSystemPath: /var/hyperledger/production

root@ubuntu-s-1vcpu-3gb-nyc3-01:/var/hyperledger/production# tree
.
├── chaincodes
├── ledgersData
│   ├── chains
│   │   └── index
│   │       ├── 000010.log
│   │       ├── CURRENT
│   │       ├── LOCK
│   │       ├── LOG
│   │       └── MANIFEST-000011
│   ├── historyLeveldb
│   │   ├── 000010.log
│   │   ├── CURRENT
│   │   ├── LOCK
│   │   ├── LOG
│   │   └── MANIFEST-000011
│   ├── ledgerProvider
│   │   ├── 000010.log
│   │   ├── CURRENT
│   │   ├── LOCK
│   │   ├── LOG
│   │   └── MANIFEST-000011
│   └── stateLeveldb
│       ├── 000010.log
│       ├── CURRENT
│       ├── LOCK
│       ├── LOG
│       └── MANIFEST-000011
└── peer.pid

7 directories, 21 files
```

通过`docker exec -it  peer0.org1.example.com bash`命令进入其中一个peer容器，可以看到创建账本的信息：

```
root@45eb4f0d9e94:/var/hyperledger/production/ledgersData# ls
chains  historyLeveldb  ledgerProvider
root@45eb4f0d9e94:/var/hyperledger/production/ledgersData# cd ledgerProvider/
root@45eb4f0d9e94:/var/hyperledger/production/ledgersData/ledgerProvider# ls
000001.log  CURRENT  LOCK  LOG  MANIFEST-000000
```


在`serve`函数中调用`ledgermgmt.Initialize()`会完成对`PeerLedger`和`PeerLedgerProvider`两个`Interface对象`的创建。

``` go
// core\ledger\ledgermgmt\ledger_mgmt.go
func initialize() {
	logger.Info("Initializing ledger mgmt")
	lock.Lock()
	defer lock.Unlock()
	initialized = true
	openedLedgers = make(map[string]ledger.PeerLedger)
	provider, err := kvledger.NewProvider()
	if err != nil {
		panic(fmt.Errorf("Error in instantiating ledger provider: %s", err))
	}
	ledgerProvider = provider
	logger.Info("ledger mgmt initialized")
}
```

``` go
var openedLedgers map[string]ledger.PeerLedger
var ledgerProvider ledger.PeerLedgerProvider

// PeerLedgerProvider provides handle to ledger instances
type PeerLedgerProvider interface {
	// Create creates a new ledger with the given genesis block.
	// This function guarantees that the creation of ledger and committing the genesis block would an atomic action
	// The chain id retrieved from the genesis block is treated as a ledger id
	Create(genesisBlock *common.Block) (PeerLedger, error)
	// Open opens an already created ledger
	Open(ledgerID string) (PeerLedger, error)
	// Exists tells whether the ledger with given id exists
	Exists(ledgerID string) (bool, error)
	// List lists the ids of the existing ledgers
	List() ([]string, error)
	// Close closes the PeerLedgerProvider
	Close()
}

// PeerLedger differs from the OrdererLedger in that PeerLedger locally maintain a bitmask
// that tells apart valid transactions from invalid ones
type PeerLedger interface {
	commonledger.Ledger
	// GetTransactionByID retrieves a transaction by id
	GetTransactionByID(txID string) (*peer.ProcessedTransaction, error)
	// GetBlockByHash returns a block given it's hash
	GetBlockByHash(blockHash []byte) (*common.Block, error)
	// GetBlockByTxID returns a block which contains a transaction
	GetBlockByTxID(txID string) (*common.Block, error)
	// GetTxValidationCodeByTxID returns reason code of transaction validation
	GetTxValidationCodeByTxID(txID string) (peer.TxValidationCode, error)
	// NewTxSimulator gives handle to a transaction simulator.
	// A client can obtain more than one 'TxSimulator's for parallel execution.
	// Any snapshoting/synchronization should be performed at the implementation level if required
	NewTxSimulator() (TxSimulator, error)
	// NewQueryExecutor gives handle to a query executor.
	// A client can obtain more than one 'QueryExecutor's for parallel execution.
	// Any synchronization should be performed at the implementation level if required
	NewQueryExecutor() (QueryExecutor, error)
	// NewHistoryQueryExecutor gives handle to a history query executor.
	// A client can obtain more than one 'HistoryQueryExecutor's for parallel execution.
	// Any synchronization should be performed at the implementation level if required
	NewHistoryQueryExecutor() (HistoryQueryExecutor, error)
	//Prune prunes the blocks/transactions that satisfy the given policy
	Prune(policy commonledger.PrunePolicy) error
}
```

`Provider`是对`PeerLedgerProvider`接口的实现。在[NewProvider()](github.com/hyperledger/fabric/blob/release/core/ledger/kvledger/kv_ledger_provider.go)方法中会生成一个`provider`。

``` go
// core/ledger/kvledger/kv_ledger_provider.go
// Provider implements interface ledger.PeerLedgerProvider
type Provider struct {
	idStore            *idStore
	blockStoreProvider blkstorage.BlockStoreProvider
	vdbProvider        statedb.VersionedDBProvider
	historydbProvider  historydb.HistoryDBProvider
}

// NewProvider instantiates a new Provider.
// This is not thread-safe and assumed to be synchronized be the caller
func NewProvider() (ledger.PeerLedgerProvider, error) {

	logger.Info("Initializing ledger provider")

	// Initialize the ID store (inventory of chainIds/ledgerIds)
	idStore := openIDStore(ledgerconfig.GetLedgerProviderPath())

	// Initialize the block storage
	attrsToIndex := []blkstorage.IndexableAttr{
		blkstorage.IndexableAttrBlockHash,
		blkstorage.IndexableAttrBlockNum,
		blkstorage.IndexableAttrTxID,
		blkstorage.IndexableAttrBlockNumTranNum,
		blkstorage.IndexableAttrBlockTxID,
		blkstorage.IndexableAttrTxValidationCode,
	}
	indexConfig := &blkstorage.IndexConfig{AttrsToIndex: attrsToIndex}
	blockStoreProvider := fsblkstorage.NewProvider(
		fsblkstorage.NewConf(ledgerconfig.GetBlockStorePath(), ledgerconfig.GetMaxBlockfileSize()),
		indexConfig)

	// Initialize the versioned database (state database)
	var vdbProvider statedb.VersionedDBProvider
	if !ledgerconfig.IsCouchDBEnabled() {
		logger.Debug("Constructing leveldb VersionedDBProvider")
		vdbProvider = stateleveldb.NewVersionedDBProvider()
	} else {
		logger.Debug("Constructing CouchDB VersionedDBProvider")
		var err error
		vdbProvider, err = statecouchdb.NewVersionedDBProvider()
		if err != nil {
			return nil, err
		}
	}

	// Initialize the history database (index for history of values by key)
	var historydbProvider historydb.HistoryDBProvider
	historydbProvider = historyleveldb.NewHistoryDBProvider()

	logger.Info("ledger provider Initialized")
	provider := &Provider{idStore, blockStoreProvider, vdbProvider, historydbProvider}
	provider.recoverUnderConstructionLedger()
	return provider, nil
}
```

通过封装的`leveldbhelper`操作。
``` go
// path: /var/hyperledger/production/ledgersData/ledgerProvider
idStore := openIDStore(ledgerconfig.GetLedgerProviderPath())

// https://github.com/hyperledger/fabric/blob/release/core/ledger/kvledger/kv_ledger_provider.go
func openIDStore(path string) *idStore {
	db := leveldbhelper.CreateDB(&leveldbhelper.Conf{DBPath: path})
	db.Open()
	return &idStore{db}
}
```

`leveldbhelper`里面实际调用的是`goleveldb`提供的接口。
``` go
// https://github.com/hyperledger/fabric/blob/release/common/ledger/util/leveldbhelper/leveldb_helper.go
// CreateDB constructs a `DB`
func CreateDB(conf *Conf) *DB {
	readOpts := &opt.ReadOptions{}
	writeOptsNoSync := &opt.WriteOptions{}
	writeOptsSync := &opt.WriteOptions{}
	writeOptsSync.Sync = true

	return &DB{
		conf:            conf,
		dbState:         closed,
		readOpts:        readOpts,
		writeOptsNoSync: writeOptsNoSync,
		writeOptsSync:   writeOptsSync}
}

type dbState int32

const (
	closed dbState = iota
	opened
)

// DB - a wrapper on an actual store
type DB struct {
	conf    *Conf
	db      *leveldb.DB
	dbState dbState
	mux     sync.Mutex

	readOpts        *opt.ReadOptions
	writeOptsNoSync *opt.WriteOptions
	writeOptsSync   *opt.WriteOptions
}

// Open opens the underlying db
func (dbInst *DB) Open() {
	dbInst.mux.Lock()
	defer dbInst.mux.Unlock()
	if dbInst.dbState == opened {
		return
	}
	dbOpts := &opt.Options{}
	dbPath := dbInst.conf.DBPath
	var err error
	var dirEmpty bool
	if dirEmpty, err = util.CreateDirIfMissing(dbPath); err != nil {
		panic(fmt.Sprintf("Error while trying to create dir if missing: %s", err))
	}
	dbOpts.ErrorIfMissing = !dirEmpty
	if dbInst.db, err = leveldb.OpenFile(dbPath, dbOpts); err != nil {
		panic(fmt.Sprintf("Error while trying to open DB: %s", err))
	}
	dbInst.dbState = opened
}
```

关于`goleveldb`的用法参考[这里](https://github.com/syndtr/goleveldb)。

``` go
// The returned DB instance is safe for concurrent use. Which mean that all
// DB's methods may be called concurrently from multiple goroutine.
db, err := leveldb.OpenFile("path/to/db", nil)
...
defer db.Close()
...
```

* EventHubServer

在`core.yaml`中的默认配置为：
``` yaml
# EventHub related configuration
 events:
     # The address that the Event service will be enabled on the peer
     address: 0.0.0.0:7053

     # total number of events that could be buffered without blocking send
     buffersize: 100

     # timeout duration for producer to send an event.
     # if < 0, if buffer full, unblocks immediately and not send
     # if 0, if buffer full, will block and guarantee the event will be sent out
     # if > 0, if buffer full, blocks till timeout
     timeout: 10ms
```

初始化`EventHub`的代码：

``` go
// peer/node/start.go
//TODO - do we need different SSL material for events ?
ehubGrpcServer, err := createEventHubServer(secureConfig)
if err != nil {
	grpclog.Fatalf("Failed to create ehub server: %v", err)
}

func createEventHubServer(secureConfig comm.SecureServerConfig) (comm.GRPCServer, error) {
	var lis net.Listener
	var err error
	lis, err = net.Listen("tcp", viper.GetString("peer.events.address"))
	if err != nil {
		return nil, fmt.Errorf("failed to listen: %v", err)
	}

	grpcServer, err := comm.NewGRPCServerFromListener(lis, secureConfig)
	if err != nil {
		logger.Errorf("Failed to return new GRPC server: %s", err)
		return nil, err
	}
	ehServer := producer.NewEventsServer(
		uint(viper.GetInt("peer.events.buffersize")),
		viper.GetDuration("peer.events.timeout"))

	pb.RegisterEventsServer(grpcServer.Server(), ehServer)
	return grpcServer, nil
}
```

import "google.golang.org/grpc"
https://godoc.org/google.golang.org/grpc



执行输出如下：
``` bash
2017-12-08 09:55:44.304 UTC [start.go:143] [nodeCmd] serve -> DEBU 024 [gerry] before createEventHubServer
2017-12-08 09:55:44.305 UTC [events.go:262] [eventhub_producer] AddEventType -> DEBU 025 Registering BLOCK
2017-12-08 09:55:44.305 UTC [events.go:262] [eventhub_producer] AddEventType -> DEBU 026 Registering CHAINCODE
2017-12-08 09:55:44.305 UTC [events.go:262] [eventhub_producer] AddEventType -> DEBU 027 Registering REJECTION
2017-12-08 09:55:44.305 UTC [events.go:262] [eventhub_producer] AddEventType -> DEBU 028 Registering REGISTER
2017-12-08 09:55:44.305 UTC [start.go:149] [nodeCmd] serve -> DEBU 029 [gerry] after createEventHubServer
```

``` go
// events/producer/events.go
//AddEventType supported event
func AddEventType(eventType pb.EventType) error {
	gEventProcessor.Lock()
	logger.Debugf("Registering %s", pb.EventType_name[int32(eventType)])
	if _, ok := gEventProcessor.eventConsumers[eventType]; ok {
		gEventProcessor.Unlock()
		return fmt.Errorf("event type exists %s", pb.EventType_name[int32(eventType)])
	}

	switch eventType {
	case pb.EventType_BLOCK:
		gEventProcessor.eventConsumers[eventType] = &genericHandlerList{handlers: make(map[*handler]bool)}
	case pb.EventType_CHAINCODE:
		gEventProcessor.eventConsumers[eventType] = &chaincodeHandlerList{handlers: make(map[string]map[string]map[*handler]bool)}
	case pb.EventType_REJECTION:
		gEventProcessor.eventConsumers[eventType] = &genericHandlerList{handlers: make(map[*handler]bool)}
	}
	gEventProcessor.Unlock()

	return nil
}

//eventProcessor has a map of event type to handlers interested in that
//event type. start() kicks of the event processor where it waits for Events
//from producers. We could easily generalize the one event handling loop to one
//per handlerMap if necessary.
//
type eventProcessor struct {
	sync.RWMutex
	eventConsumers map[pb.EventType]handlerList

	//we could generalize this with mutiple channels each with its own size
	eventChannel chan *pb.Event

	//timeout duration for producer to send an event.
	//if < 0, if buffer full, unblocks immediately and not send
	//if 0, if buffer full, will block and guarantee the event will be sent out
	//if > 0, if buffer full, blocks till timeout
	timeout time.Duration
}

//global eventProcessor singleton created by initializeEvents. Openchain producers
//send events simply over a reentrant static method
var gEventProcessor *eventProcessor
```

* ChaincodeServer

以新的协程启动运行。
``` go
go ccSrv.Start()
```

``` bash
2017-12-08 09:55:44.305 UTC [start.go:155] [nodeCmd] serve -> DEBU 02a [gerry] before createChaincodeServer
2017-12-08 09:55:44.305 UTC [start.go:311] [nodeCmd] createChaincodeServer -> WARN 02b peer.chaincodeListenAddress is not set, use peer.listenAddress 0.0.0.0:7051
2017-12-08 09:55:44.305 UTC [start.go:157] [nodeCmd] serve -> DEBU 02c [gerry] after createChaincodeServer
2017-12-08 09:55:44.305 UTC [start.go:159] [nodeCmd] serve -> DEBU 02d [gerry] before registerChaincodeSupport
2017-12-08 09:55:44.306 UTC [start.go:352] [nodeCmd] registerChaincodeSupport -> DEBU 02e Chaincode startup timeout value set to 5m0s
2017-12-08 09:55:44.306 UTC [chaincode_support.go:152] [chaincode] NewChaincodeSupport -> INFO 02f Chaincode support using peerAddress: 0.0.0.0:7051
2017-12-08 09:55:44.307 UTC [chaincode_support.go:178] [chaincode] NewChaincodeSupport -> DEBU 030 Turn off keepalive(value 0)
2017-12-08 09:55:44.307 UTC [chaincode_support.go:189] [chaincode] NewChaincodeSupport -> DEBU 031 Setting execute timeout value to 30s
2017-12-08 09:55:44.307 UTC [chaincode_support.go:213] [chaincode] getLogLevelFromViper -> DEBU 032 CORE_CHAINCODE_LEVEL set to level debug
2017-12-08 09:55:44.307 UTC [chaincode_support.go:213] [chaincode] getLogLevelFromViper -> DEBU 033 CORE_CHAINCODE_SHIM set to level debug
2017-12-08 09:55:44.308 UTC [sysccapi.go:87] [sccapi] RegisterSysCC -> INFO 034 system chaincode cscc(github.com/hyperledger/fabric/core/scc/cscc) registered
2017-12-08 09:55:44.308 UTC [sysccapi.go:87] [sccapi] RegisterSysCC -> INFO 035 system chaincode lscc(github.com/hyperledger/fabric/core/scc/lscc) registered
2017-12-08 09:55:44.308 UTC [sysccapi.go:87] [sccapi] RegisterSysCC -> INFO 036 system chaincode escc(github.com/hyperledger/fabric/core/scc/escc) registered
2017-12-08 09:55:44.308 UTC [sysccapi.go:87] [sccapi] RegisterSysCC -> INFO 037 system chaincode vscc(github.com/hyperledger/fabric/core/scc/vscc) registered
2017-12-08 09:55:44.309 UTC [sysccapi.go:87] [sccapi] RegisterSysCC -> INFO 038 system chaincode qscc(github.com/hyperledger/fabric/core/chaincode/qscc) registered
2017-12-08 09:55:44.309 UTC [start.go:161] [nodeCmd] serve -> DEBU 039 [gerry] after registerChaincodeSupport
2017-12-08 09:55:44.309 UTC [start.go:163] [nodeCmd] serve -> DEBU 03a [gerry] before go ccSrv.Start()
2017-12-08 09:55:44.309 UTC [start.go:165] [nodeCmd] serve -> DEBU 03b [gerry] after go ccSrv.Start()
```

* Running peer

``` bash
2017-12-08 09:55:44.309 UTC [start.go:167] [nodeCmd] serve -> DEBU 03c Running peer
2017-12-08 09:55:44.309 UTC [start.go:170] [nodeCmd] serve -> DEBU 03d [gerry] before RegisterAdminServer
2017-12-08 09:55:44.309 UTC [start.go:172] [nodeCmd] serve -> DEBU 03e [gerry] after RegisterAdminServer
2017-12-08 09:55:44.309 UTC [start.go:175] [nodeCmd] serve -> DEBU 03f [gerry] before RegisterEndorserServer
2017-12-08 09:55:44.309 UTC [mgmt.go:149] [msp] GetLocalMSP -> DEBU 040 Returning existing local MSP
2017-12-08 09:55:44.309 UTC [start.go:178] [nodeCmd] serve -> DEBU 041 [gerry] after RegisterEndorserServer
```

* gossip component
`core.yaml`

``` bash
2017-12-08 09:55:44.309 UTC [start.go:181] [nodeCmd] serve -> DEBU 042 [gerry] before Initialize gossip component
2017-12-08 09:55:44.310 UTC [mgmt.go:149] [msp] GetLocalMSP -> DEBU 043 Returning existing local MSP
2017-12-08 09:55:44.310 UTC [mspimpl.go:369] [msp] GetDefaultSigningIdentity -> DEBU 044 Obtaining default signing identity
2017-12-08 09:55:44.310 UTC [gossip_service.go:144] [gossip/service] func1 -> INFO 045 Initialize gossip with endpoint 0.0.0.0:7051 and bootstrap set [127.0.0.1:7051]
2017-12-08 09:55:44.310 UTC [mgmt.go:149] [msp] GetLocalMSP -> DEBU 046 Returning existing local MSP
2017-12-08 09:55:44.310 UTC [mspimpl.go:407] [msp] DeserializeIdentity -> INFO 047 Obtaining identity
2017-12-08 09:55:44.311 UTC [identities.go:42] [msp/identity] newIdentity -> DEBU 048 Creating identity instance for ID -----BEGIN CERTIFICATE-----
MIICNjCCAd2gAwIBAgIRAMnf9/dmV9RvCCVw9pZQUfUwCgYIKoZIzj0EAwIwgYEx
CzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1TYW4g
RnJhbmNpc2NvMRkwFwYDVQQKExBvcmcxLmV4YW1wbGUuY29tMQwwCgYDVQQLEwND
T1AxHDAaBgNVBAMTE2NhLm9yZzEuZXhhbXBsZS5jb20wHhcNMTcxMTEyMTM0MTEx
WhcNMjcxMTEwMTM0MTExWjBpMQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZv
cm5pYTEWMBQGA1UEBxMNU2FuIEZyYW5jaXNjbzEMMAoGA1UECxMDQ09QMR8wHQYD
VQQDExZwZWVyMC5vcmcxLmV4YW1wbGUuY29tMFkwEwYHKoZIzj0CAQYIKoZIzj0D
AQcDQgAEZ8S4V71OBJpyMIVZdwYdFXAckItrpvSrCf0HQg40WW9XSoOOO76I+Umf
EkmTlIJXP7/AyRRSRU38oI8Ivtu4M6NNMEswDgYDVR0PAQH/BAQDAgeAMAwGA1Ud
EwEB/wQCMAAwKwYDVR0jBCQwIoAginORIhnPEFZUhXm6eWBkm7K7Zc8R4/z7LW4H
ossDlCswCgYIKoZIzj0EAwIDRwAwRAIgVikIUZzgfuFsGLQHWJUVJCU7pDaETkaz
PzFgsCiLxUACICgzJYlW7nvZxP7b6tbeu3t8mrhMXQs956mD4+BoKuNI
-----END CERTIFICATE-----
2017-12-08 09:55:44.311 UTC [mgmt.go:149] [msp] GetLocalMSP -> DEBU 049 Returning existing local MSP
2017-12-08 09:55:44.311 UTC [mspimpl.go:391] [msp] Validate -> DEBU 04a MSP DEFAULT validating identity
2017-12-08 09:55:44.311 UTC [mspimpl.go:559] [msp] getCertificationChain -> DEBU 04b MSP DEFAULT getting certification chain
2017-12-08 09:55:44.316 UTC [mgmt.go:149] [msp] GetLocalMSP -> DEBU 04c Returning existing local MSP
2017-12-08 09:55:44.316 UTC [mspimpl.go:407] [msp] DeserializeIdentity -> INFO 04d Obtaining identity
2017-12-08 09:55:44.316 UTC [identities.go:42] [msp/identity] newIdentity -> DEBU 04e Creating identity instance for ID -----BEGIN CERTIFICATE-----
MIICNjCCAd2gAwIBAgIRAMnf9/dmV9RvCCVw9pZQUfUwCgYIKoZIzj0EAwIwgYEx
CzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1TYW4g
RnJhbmNpc2NvMRkwFwYDVQQKExBvcmcxLmV4YW1wbGUuY29tMQwwCgYDVQQLEwND
T1AxHDAaBgNVBAMTE2NhLm9yZzEuZXhhbXBsZS5jb20wHhcNMTcxMTEyMTM0MTEx
WhcNMjcxMTEwMTM0MTExWjBpMQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZv
cm5pYTEWMBQGA1UEBxMNU2FuIEZyYW5jaXNjbzEMMAoGA1UECxMDQ09QMR8wHQYD
VQQDExZwZWVyMC5vcmcxLmV4YW1wbGUuY29tMFkwEwYHKoZIzj0CAQYIKoZIzj0D
AQcDQgAEZ8S4V71OBJpyMIVZdwYdFXAckItrpvSrCf0HQg40WW9XSoOOO76I+Umf
EkmTlIJXP7/AyRRSRU38oI8Ivtu4M6NNMEswDgYDVR0PAQH/BAQDAgeAMAwGA1Ud
EwEB/wQCMAAwKwYDVR0jBCQwIoAginORIhnPEFZUhXm6eWBkm7K7Zc8R4/z7LW4H
ossDlCswCgYIKoZIzj0EAwIDRwAwRAIgVikIUZzgfuFsGLQHWJUVJCU7pDaETkaz
PzFgsCiLxUACICgzJYlW7nvZxP7b6tbeu3t8mrhMXQs956mD4+BoKuNI
-----END CERTIFICATE-----
2017-12-08 09:55:44.318 UTC [discovery_impl.go:134] [gossip/discovery] NewDiscoveryService -> INFO 04f Started { [] [219 222 126 206 254 133 230 98 76 31 29 178 226 8 67 63 195 139 101 87 11 142 70 12 0 160 87 17 119 246 200 234] 0.0.0.0:7051} incTime is 1512726944317318910
2017-12-08 09:55:44.318 UTC [gossip_impl.go:128] [gossip/gossip] NewGossipService -> INFO 050 Creating gossip service with self membership of { [] [219 222 126 206 254 133 230 98 76 31 29 178 226 8 67 63 195 139 101 87 11 142 70 12 0 160 87 17 119 246 200 234] 0.0.0.0:7051}
2017-12-08 09:55:44.318 UTC [mgmt.go:149] [msp] GetLocalMSP -> DEBU 051 Returning existing local MSP

...

2017-12-08 09:55:44.319 UTC [mspimpl.go:369] [msp] GetDefaultSigningIdentity -> DEBU 058 Obtaining default signing identity
2017-12-08 09:55:44.319 UTC [identities.go:208] [msp/identity] Sign -> DEBU 059 Sign: plaintext: 1801AA01D5060A20DBDE7ECEFE85E662...0A2D2D2D2D2D454E44202D2D2D2D2D0A 
2017-12-08 09:55:44.319 UTC [identities.go:210] [msp/identity] Sign -> DEBU 05a Sign: digest: BEB7CF68B202F01F308D265568CFFD934EB26A13CEE0705EF5FD9810F8D109CA 
2017-12-08 09:55:44.320 UTC [gossip_impl.go:133] [gossip/gossip] NewGossipService -> WARN 05b External endpoint is empty, peer will not be accessible outside of its organization
2017-12-08 09:55:44.320 UTC [start.go:218] [nodeCmd] serve -> DEBU 05c [gerry] after Initialize gossip component
```

* initialize system chaincodes

初始化系统chaincode。

``` bash
...
2017-12-08 09:55:44.340 UTC [sysccapi.go:136] [sccapi] deploySysCC -> INFO 1ac system chaincode qscc/(github.com/hyperledger/fabric/core/chaincode/qscc) deployed
2017-12-08 09:55:44.340 UTC [start.go:83] [nodeCmd] initSysCCs -> INFO 1ad Deployed system chaincodess
2017-12-08 09:55:44.340 UTC [start.go:223] [nodeCmd] serve -> DEBU 1ae [gerry] after initSysCCs
```

* peer.Initialize
TODO

``` go
// src\github.com\hyperledger\fabric\peer\node\start.go
// this brings up all the chains (including testchainid)
peer.Initialize(func(cid string) {
	logger.Debugf("Deploying system CC, for chain <%s>", cid)
	scc.DeploySysCCs(cid)
})
	
// src\github.com\hyperledger\fabric\core\peer\peer.go
// Initialize sets up any chains that the peer has from the persistence. This
// function should be called at the start up when the ledger and gossip
// ready
func Initialize(init func(string)) {
	chainInitializer = init

    // 结构说明如下
	var cb *common.Block
    
    // 结构说明如下
	var ledger ledger.PeerLedger
	
	// 初始化ledger一次
	// src\github.com\hyperledger\fabric\core\ledger\ledgermgmt\ledger_mgmt.go
	ledgermgmt.Initialize()
	
	// GetLedgerIDs returns the ids of the ledgers created
	ledgerIds, err := ledgermgmt.GetLedgerIDs()
	if err != nil {
		panic(fmt.Errorf("Error in initializing ledgermgmt: %s", err))
	}
	
	for _, cid := range ledgerIds {
		peerLogger.Infof("Loading chain %s", cid)
		if ledger, err = ledgermgmt.OpenLedger(cid); err != nil {
			peerLogger.Warningf("Failed to load ledger %s(%s)", cid, err)
			peerLogger.Debugf("Error while loading ledger %s with message %s. We continue to the next ledger rather than abort.", cid, err)
			continue
		}
		if cb, err = getCurrConfigBlockFromLedger(ledger); err != nil {
			peerLogger.Warningf("Failed to find config block on ledger %s(%s)", cid, err)
			peerLogger.Debugf("Error while looking for config block on ledger %s with message %s. We continue to the next ledger rather than abort.", cid, err)
			continue
		}
		// Create a chain if we get a valid ledger with config block
		if err = createChain(cid, ledger, cb); err != nil {
			peerLogger.Warningf("Failed to load chain %s(%s)", cid, err)
			peerLogger.Debugf("Error reloading chain %s with message %s. We continue to the next chain rather than abort.", cid, err)
			continue
		}

		InitChain(cid)
	}
}
```

order和peer之间同步的`block`结构：

``` go
// src\github.com\hyperledger\fabric\protos\common\common.pb.go
// This is finalized block structure to be shared among the orderer and peer
// Note that the BlockHeader chains to the previous BlockHeader, and the BlockData hash is embedded
// in the BlockHeader.  This makes it natural and obvious that the Data is included in the hash, but
// the Metadata is not.
type Block struct {
	Header   *BlockHeader   `protobuf:"bytes,1,opt,name=header" json:"header,omitempty"`
	Data     *BlockData     `protobuf:"bytes,2,opt,name=data" json:"data,omitempty"`
	Metadata *BlockMetadata `protobuf:"bytes,3,opt,name=metadata" json:"metadata,omitempty"`
}
```

peer的ledger接口：（peer的ledger与order的ledger作用不同，peer的ledger主要是用于对交易的校验）

``` go
// src\github.com\hyperledger\fabric\core\ledger\ledger_interface.go

// PeerLedgerProvider provides handle to ledger instances
type PeerLedgerProvider interface {
	// Create creates a new ledger with the given genesis block.
	// This function guarantees that the creation of ledger and committing the genesis block would an atomic action
	// The chain id retrieved from the genesis block is treated as a ledger id
	Create(genesisBlock *common.Block) (PeerLedger, error)
	// Open opens an already created ledger
	Open(ledgerID string) (PeerLedger, error)
	// Exists tells whether the ledger with given id exists
	Exists(ledgerID string) (bool, error)
	// List lists the ids of the existing ledgers
	List() ([]string, error)
	// Close closes the PeerLedgerProvider
	Close()
}

// PeerLedger differs from the OrdererLedger in that PeerLedger locally maintain a bitmask
// that tells apart valid transactions from invalid ones
type PeerLedger interface {
	commonledger.Ledger
	// GetTransactionByID retrieves a transaction by id
	GetTransactionByID(txID string) (*peer.ProcessedTransaction, error)
	// GetBlockByHash returns a block given it's hash
	GetBlockByHash(blockHash []byte) (*common.Block, error)
	// GetBlockByTxID returns a block which contains a transaction
	GetBlockByTxID(txID string) (*common.Block, error)
	// GetTxValidationCodeByTxID returns reason code of transaction validation
	GetTxValidationCodeByTxID(txID string) (peer.TxValidationCode, error)
	// NewTxSimulator gives handle to a transaction simulator.
	// A client can obtain more than one 'TxSimulator's for parallel execution.
	// Any snapshoting/synchronization should be performed at the implementation level if required
	NewTxSimulator() (TxSimulator, error)
	// NewQueryExecutor gives handle to a query executor.
	// A client can obtain more than one 'QueryExecutor's for parallel execution.
	// Any synchronization should be performed at the implementation level if required
	NewQueryExecutor() (QueryExecutor, error)
	// NewHistoryQueryExecutor gives handle to a history query executor.
	// A client can obtain more than one 'HistoryQueryExecutor's for parallel execution.
	// Any synchronization should be performed at the implementation level if required
	NewHistoryQueryExecutor() (HistoryQueryExecutor, error)
	//Prune prunes the blocks/transactions that satisfy the given policy
	Prune(policy commonledger.PrunePolicy) error
}
```

``` go
// src\github.com\hyperledger\fabric\core\ledger\ledgermgmt\ledger_mgmt.go

var openedLedgers map[string]ledger.PeerLedger
var ledgerProvider ledger.PeerLedgerProvider
var lock sync.Mutex
var initialized bool
var once sync.Once

// Initialize initializes ledgermgmt
func Initialize() {
	once.Do(func() {
		initialize()
	})
}

func initialize() {
	logger.Info("Initializing ledger mgmt")
	lock.Lock()
	defer lock.Unlock()
	
	initialized = true
	openedLedgers = make(map[string]ledger.PeerLedger)

    // 不是线程安全
	provider, err := kvledger.NewProvider()
	if err != nil {
		panic(fmt.Errorf("Error in instantiating ledger provider: %s", err))
	}
	
	ledgerProvider = provider
	logger.Info("ledger mgmt initialized")
}

// src\github.com\hyperledger\fabric\core\ledger\kvledger\kv_ledger_provider.go
// NewProvider instantiates a new Provider.
// This is not thread-safe and assumed to be synchronized be the caller
func NewProvider() (ledger.PeerLedgerProvider, error) {

	logger.Info("Initializing ledger provider")

	// Initialize the ID store (inventory of chainIds/ledgerIds)
	logger.Debugf("[gerry] ledgerconfig.GetLedgerProviderPath: %s", ledgerconfig.GetLedgerProviderPath());
	idStore := openIDStore(ledgerconfig.GetLedgerProviderPath())

	// Initialize the block storage
	attrsToIndex := []blkstorage.IndexableAttr{
		blkstorage.IndexableAttrBlockHash,
		blkstorage.IndexableAttrBlockNum,
		blkstorage.IndexableAttrTxID,
		blkstorage.IndexableAttrBlockNumTranNum,
		blkstorage.IndexableAttrBlockTxID,
		blkstorage.IndexableAttrTxValidationCode,
	}
	indexConfig := &blkstorage.IndexConfig{AttrsToIndex: attrsToIndex}
	blockStoreProvider := fsblkstorage.NewProvider(
		fsblkstorage.NewConf(ledgerconfig.GetBlockStorePath(), ledgerconfig.GetMaxBlockfileSize()),
		indexConfig)

	// Initialize the versioned database (state database)
	var vdbProvider statedb.VersionedDBProvider
	if !ledgerconfig.IsCouchDBEnabled() {
		logger.Debug("Constructing leveldb VersionedDBProvider")
		vdbProvider = stateleveldb.NewVersionedDBProvider()
	} else {
		logger.Debug("Constructing CouchDB VersionedDBProvider")
		var err error
		vdbProvider, err = statecouchdb.NewVersionedDBProvider()
		if err != nil {
			return nil, err
		}
	}

	// Initialize the history database (index for history of values by key)
	var historydbProvider historydb.HistoryDBProvider
	historydbProvider = historyleveldb.NewHistoryDBProvider()

	logger.Info("ledger provider Initialized")
	provider := &Provider{idStore, blockStoreProvider, vdbProvider, historydbProvider}
	provider.recoverUnderConstructionLedger()
	return provider, nil
}
```

``` go
// src\github.com\hyperledger\fabric\core\ledger\kvledger\kv_ledger_provider.go
// Provider implements interface ledger.PeerLedgerProvider
type Provider struct {
	idStore            *idStore
	blockStoreProvider blkstorage.BlockStoreProvider
	vdbProvider        statedb.VersionedDBProvider
	historydbProvider  historydb.HistoryDBProvider
}
```

ledgerProvider数据对应的接口

``` go
// src\github.com\hyperledger\fabric\core\ledger\kvledger\kv_ledger_provider.go
type idStore struct {
	db *leveldbhelper.DB
}

// src\github.com\hyperledger\fabric\common\ledger\util\leveldbhelper\leveldb_helper.go
// DB - a wrapper on an actual store
type DB struct {
	conf    *Conf
	db      *leveldb.DB
	dbState dbState
	mux     sync.Mutex

	readOpts        *opt.ReadOptions
	writeOptsNoSync *opt.WriteOptions
	writeOptsSync   *opt.WriteOptions
}
```

chains数据对应的接口

``` go
// src\github.com\hyperledger\fabric\common\ledger\blkstorage\blockstorage.go
// BlockStoreProvider provides an handle to a BlockStore
type BlockStoreProvider interface {
	CreateBlockStore(ledgerid string) (BlockStore, error)
	OpenBlockStore(ledgerid string) (BlockStore, error)
	Exists(ledgerid string) (bool, error)
	List() ([]string, error)
	Close()
}
```

stateDB数据对应的接口

``` go
// src\github.com\hyperledger\fabric\core\ledger\kvledger\txmgmt\statedb\statedb.go
// VersionedDBProvider provides an instance of an versioned DB
type VersionedDBProvider interface {
	// GetDBHandle returns a handle to a VersionedDB
	GetDBHandle(id string) (VersionedDB, error)
	// Close closes all the VersionedDB instances and releases any resources held by VersionedDBProvider
	Close()
}

// VersionedDB lists methods that a db is supposed to implement
type VersionedDB interface {
	// GetState gets the value for given namespace and key. For a chaincode, the namespace corresponds to the chaincodeId
	GetState(namespace string, key string) (*VersionedValue, error)
	// GetStateMultipleKeys gets the values for multiple keys in a single call
	GetStateMultipleKeys(namespace string, keys []string) ([]*VersionedValue, error)
	// GetStateRangeScanIterator returns an iterator that contains all the key-values between given key ranges.
	// startKey is inclusive
	// endKey is exclusive
	// The returned ResultsIterator contains results of type *VersionedKV
	GetStateRangeScanIterator(namespace string, startKey string, endKey string) (ResultsIterator, error)
	// ExecuteQuery executes the given query and returns an iterator that contains results of type *VersionedKV.
	ExecuteQuery(namespace, query string) (ResultsIterator, error)
	// ApplyUpdates applies the batch to the underlying db.
	// height is the height of the highest transaction in the Batch that
	// a state db implementation is expected to ues as a save point
	ApplyUpdates(batch *UpdateBatch, height *version.Height) error
	// GetLatestSavePoint returns the height of the highest transaction upto which
	// the state db is consistent
	GetLatestSavePoint() (*version.Height, error)
	// ValidateKey tests whether the key is supported by the db implementation.
	// For instance, leveldb supports any bytes for the key while the couchdb supports only valid utf-8 string
	ValidateKey(key string) error
	// Open opens the db
	Open() error
	// Close closes the db
	Close()
}
```

historyLeveldb数据：

``` go
// src\github.com\hyperledger\fabric\common\ledger\util\leveldbhelper\leveldb_provider.go
// Provider enables to use a single leveldb as multiple logical leveldbs
type Provider struct {
	db        *DB
	dbHandles map[string]*DBHandle
	mux       sync.Mutex
}

// DBHandle is an handle to a named db
type DBHandle struct {
	dbName string
	db     *DB
}
```

数据恢复（基于上面四种数据）
``` go
provider := &Provider{idStore, blockStoreProvider, vdbProvider, historydbProvider}

// 数据恢复
provider.recoverUnderConstructionLedger()
```

``` go
// src\github.com\hyperledger\fabric\core\ledger\kvledger\kv_ledger_provider.go
// recoverUnderConstructionLedger checks whether the under construction flag is set - this would be the case
// if a crash had happened during creation of ledger and the ledger creation could have been left in intermediate
// state. Recovery checks if the ledger was created and the genesis block was committed successfully then it completes
// the last step of adding the ledger id to the list of created ledgers. Else, it clears the under construction flag
func (provider *Provider) recoverUnderConstructionLedger() {
	logger.Debugf("Recovering under construction ledger")
	ledgerID, err := provider.idStore.getUnderConstructionFlag()
	panicOnErr(err, "Error while checking whether the under construction flag is set")
	if ledgerID == "" {
		logger.Debugf("No under construction ledger found. Quitting recovery")
		return
	}
	logger.Infof("ledger [%s] found as under construction", ledgerID)
	ledger, err := provider.openInternal(ledgerID)
	panicOnErr(err, "Error while opening under construction ledger [%s]", ledgerID)
	bcInfo, err := ledger.GetBlockchainInfo()
	panicOnErr(err, "Error while getting blockchain info for the under construction ledger [%s]", ledgerID)
	ledger.Close()

	switch bcInfo.Height {
	case 0:
		logger.Infof("Genesis block was not committed. Hence, the peer ledger not created. unsetting the under construction flag")
		panicOnErr(provider.runCleanup(ledgerID), "Error while running cleanup for ledger id [%s]", ledgerID)
		panicOnErr(provider.idStore.unsetUnderConstructionFlag(), "Error while unsetting under construction flag")
	case 1:
		logger.Infof("Genesis block was committed. Hence, marking the peer ledger as created")
		genesisBlock, err := ledger.GetBlockByNumber(0)
		panicOnErr(err, "Error while retrieving genesis block from blockchain for ledger [%s]", ledgerID)
		panicOnErr(provider.idStore.createLedgerID(ledgerID, genesisBlock), "Error while adding ledgerID [%s] to created list", ledgerID)
	default:
		panic(fmt.Errorf(
			"Data inconsistency: under construction flag is set for ledger [%s] while the height of the blockchain is [%d]",
			ledgerID, bcInfo.Height))
	}
	return
}
```

TODO2

-----

``` bash
2017-12-08 09:55:44.340 UTC [start.go:226] [nodeCmd] serve -> DEBU 1af [gerry] before peer.Initialize
2017-12-08 09:55:44.340 UTC [start.go:231] [nodeCmd] serve -> DEBU 1b0 [gerry] after peer.Initialize
2017-12-08 09:55:44.340 UTC [start.go:233] [nodeCmd] serve -> INFO 1b1 Starting peer with ID=[name:"jdoe" ], network ID=[dev], address=[0.0.0.0:7051]
2017-12-08 09:55:44.340 UTC [start.go:278] [nodeCmd] serve -> INFO 1b2 Started peer with ID=[name:"jdoe" ], network ID=[dev], address=[0.0.0.0:7051]
2017-12-08 09:55:44.341 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1b3 Module 'configvalues/msp' logger enabled for log level 'DEBUG'
2017-12-08 09:55:44.341 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1b4 Module 'msp' logger enabled for log level 'DEBUG'
2017-12-08 09:55:44.341 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1b5 Module 'msp/identity' logger enabled for log level 'DEBUG'
2017-12-08 09:55:44.341 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1b6 Module 'peer/gossip/sa' logger enabled for log level 'DEBUG'
2017-12-08 09:55:44.341 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1b7 Module 'gossip/gossip' logger enabled for log level 'DEBUG'
2017-12-08 09:55:44.341 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1b8 Module 'gossip/state' logger enabled for log level 'DEBUG'
2017-12-08 09:55:44.341 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1b9 Module 'gossip/election' logger enabled for log level 'DEBUG'
2017-12-08 09:55:44.341 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1ba Module 'gossip/discovery' logger enabled for log level 'DEBUG'
2017-12-08 09:55:44.341 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1bb Module 'gossip/service' logger enabled for log level 'DEBUG'
2017-12-08 09:55:44.341 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1bc Module 'peer/gossip/mcs' logger enabled for log level 'DEBUG'
2017-12-08 09:55:44.341 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1bd Module 'gossip/pull' logger enabled for log level 'DEBUG'
2017-12-08 09:55:44.341 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1be Module 'gossip/comm' logger enabled for log level 'DEBUG'
2017-12-08 09:55:44.341 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1bf Module 'kvledger' logger enabled for log level 'DEBUG'
2017-12-08 09:55:44.341 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1c0 Module 'ledgermgmt' logger enabled for log level 'DEBUG'
2017-12-08 09:55:44.341 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1c1 Module 'kvledger.util' logger enabled for log level 'DEBUG'
2017-12-08 09:55:44.342 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1c2 Module 'cauthdsl' logger enabled for log level 'DEBUG'
2017-12-08 09:55:44.342 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1c3 Module 'policies' logger enabled for log level 'DEBUG'
2017-12-08 09:55:44.342 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1c4 Module 'grpc' logger enabled for log level 'DEBUG'
...
```

* flogging.SetPeerStartupModulesMap()

``` go
// common/flogging/logging.go
// SetPeerStartupModulesMap saves the modules and their log levels.
// this function should only be called at the end of peer startup.
func SetPeerStartupModulesMap() {
	lock.Lock()
	defer lock.Unlock()

	once.Do(func() {
		peerStartModules = make(map[string]string)
		for k, v := range modules {
			peerStartModules[k] = v
		}
	})
}
```


##### 2 channel

`channel`代码中的一些关键实现：


``` go
// ChannelCmdFactory holds the clients used by ChannelCmdFactory
type ChannelCmdFactory struct {
	EndorserClient   pb.EndorserClient
	Signer           msp.SigningIdentity
	BroadcastClient  common.BroadcastClient
	DeliverClient    deliverClientIntf
	BroadcastFactory BroadcastClientFactory
}
```

``` go
// protos/common.pb.go

/*It is generated from these files:
	common/common.proto
	common/configtx.proto
	common/configuration.proto
	common/ledger.proto
	common/policies.proto
	*/

// This is finalized block structure to be shared among the orderer and peer
// Note that the BlockHeader chains to the previous BlockHeader, and the BlockData hash is embedded
// in the BlockHeader.  This makes it natural and obvious that the Data is included in the hash, but
// the Metadata is not.
type Block struct {
	Header   *BlockHeader   `protobuf:"bytes,1,opt,name=header" json:"header,omitempty"`
	Data     *BlockData     `protobuf:"bytes,2,opt,name=data" json:"data,omitempty"`
	Metadata *BlockMetadata `protobuf:"bytes,3,opt,name=metadata" json:"metadata,omitempty"`
}

// Envelope wraps a Payload with a signature so that the message may be authenticated
type Envelope struct {
	// A marshaled Payload
	Payload []byte `protobuf:"bytes,1,opt,name=payload,proto3" json:"payload,omitempty"`
	// A signature by the creator specified in the Payload header
	Signature []byte `protobuf:"bytes,2,opt,name=signature,proto3" json:"signature,omitempty"`
}
```

``` go
// peer/common/ordererclient.go

type BroadcastClient interface {
	//Send data to orderer
	Send(env *cb.Envelope) error
	Close() error
}
```

``` go
// peer/channel/create.go
func createCmd(cf *ChannelCmdFactory) *cobra.Command {
	createCmd := &cobra.Command{
		Use:   "create",
		Short: createCmdDescription,
		Long:  createCmdDescription,
		RunE: func(cmd *cobra.Command, args []string) error {
			return create(cmd, args, cf)
		},
	}
	flagList := []string{
		"channelID",
		"file",
		"timeout",
	}
	attachFlags(createCmd, flagList)

	return createCmd
}

func create(cmd *cobra.Command, args []string, cf *ChannelCmdFactory) error {
	//the global chainID filled by the "-c" command
	if chainID == common.UndefinedParamValue {
		return errors.New("Must supply channel ID")
	}

	var err error
	if cf == nil {
		cf, err = InitCmdFactory(EndorserNotRequired/*false*/, OrdererRequired/*true*/)
		if err != nil {
			return err
		}
	}
	return executeCreate(cf)
}

func executeCreate(cf *ChannelCmdFactory) error {
	var err error

	if err = sendCreateChainTransaction(cf); err != nil {
		return err
	}

	var block *cb.Block
	if block, err = getGenesisBlock(cf); err != nil {
		return err
	}

	b, err := proto.Marshal(block)
	if err != nil {
		return err
	}

	file := chainID + ".block"
	if err = ioutil.WriteFile(file, b, 0644); err != nil {
		return err
	}

	return nil
}
```

``` go
// peer/channel/channel.go
// InitCmdFactory init the ChannelCmdFactory with clients to endorser and orderer according to params
func InitCmdFactory(isEndorserRequired EndorserRequirement, isOrdererRequired OrdererRequirement) (*ChannelCmdFactory, error) {
	var err error

	cmdFact := &ChannelCmdFactory{}

	cmdFact.Signer, err = common.GetDefaultSignerFnc()
	if err != nil {
		return nil, fmt.Errorf("Error getting default signer: %s", err)
	}

	cmdFact.BroadcastFactory = func() (common.BroadcastClient, error) {
		return common.GetBroadcastClientFnc(orderingEndpoint, tls, caFile)
	}

	//for join and list, we need the endorser as well
	if isEndorserRequired {
		cmdFact.EndorserClient, err = common.GetEndorserClientFnc()
		if err != nil {
			return nil, fmt.Errorf("Error getting endorser client %s: %s", channelFuncName, err)
		}
	}

	//for create and fetch, we need the orderer as well
	if isOrdererRequired {
		if len(strings.Split(orderingEndpoint, ":")) != 2 {
			return nil, fmt.Errorf("Ordering service endpoint %s is not valid or missing", orderingEndpoint)
		}

		var opts []grpc.DialOption
		// check for TLS
		if tls {
			if caFile != "" {
				creds, err := credentials.NewClientTLSFromFile(caFile, "")
				if err != nil {
					return nil, fmt.Errorf("Error connecting to %s due to %s", orderingEndpoint, err)
				}
				opts = append(opts, grpc.WithTransportCredentials(creds))
			}
		} else {
			opts = append(opts, grpc.WithInsecure())
		}
		conn, err := grpc.Dial(orderingEndpoint, opts...)
		if err != nil {
			return nil, err
		}

		client, err := ab.NewAtomicBroadcastClient(conn).Deliver(context.TODO())
		if err != nil {
			return nil, fmt.Errorf("Error connecting due to  %s", err)
		}

		cmdFact.DeliverClient = newDeliverClient(conn, client, chainID)
	}
	logger.Infof("Endorser and orderer connections initialized")
	return cmdFact, nil
}
```

##### 3 chaincode

TODO

`peer`模块中的关键代码：

``` go
func startCmd() *cobra.Command {
	// Set the flags on the node start command.
	flags := nodeStartCmd.Flags()
	flags.BoolVarP(&chaincodeDevMode, "peer-chaincodedev", "", false,
		"Whether peer in chaincode development mode")
	flags.BoolVarP(&peerDefaultChain, "peer-defaultchain", "", false,
		"Whether to start peer with chain testchainid")
	flags.StringVarP(&orderingEndpoint, "orderer", "o", "orderer:7050", "Ordering service endpoint")

	return nodeStartCmd
}

var nodeStartCmd = &cobra.Command{
	Use:   "start",
	Short: "Starts the node.",
	Long:  `Starts a node that interacts with the network.`,
	RunE: func(cmd *cobra.Command, args []string) error {
		return serve(args)
	},
}
```

实际执行的函数是：

``` go
func serve(args []string) error {
	logger.Infof("Starting %s", version.GetInfo())
	ledgermgmt.Initialize()
	// Parameter overrides must be processed before any parameters are
	// cached. Failures to cache cause the server to terminate immediately.
	if chaincodeDevMode {
		logger.Info("Running in chaincode development mode")
		logger.Info("Disable loading validity system chaincode")

		viper.Set("chaincode.mode", chaincode.DevModeUserRunsChaincode)

	}

	if err := peer.CacheConfiguration(); err != nil {
		return err
	}

	peerEndpoint, err := peer.GetPeerEndpoint()
	if err != nil {
		err = fmt.Errorf("Failed to get Peer Endpoint: %s", err)
		return err
	}

	listenAddr := viper.GetString("peer.listenAddress")

	secureConfig, err := peer.GetSecureConfig()
	if err != nil {
		logger.Fatalf("Error loading secure config for peer (%s)", err)
	}
	peerServer, err := peer.CreatePeerServer(listenAddr, secureConfig)
	if err != nil {
		logger.Fatalf("Failed to create peer server (%s)", err)
	}

	if secureConfig.UseTLS {
		logger.Info("Starting peer with TLS enabled")
		// set up CA support
		caSupport := comm.GetCASupport()
		caSupport.ServerRootCAs = secureConfig.ServerRootCAs
	}

	//TODO - do we need different SSL material for events ?
	ehubGrpcServer, err := createEventHubServer(secureConfig)
	if err != nil {
		grpclog.Fatalf("Failed to create ehub server: %v", err)
	}

	// enable the cache of chaincode info
	ccprovider.EnableCCInfoCache()

	ccSrv, ccEpFunc := createChaincodeServer(peerServer, listenAddr)
	registerChaincodeSupport(ccSrv.Server(), ccEpFunc)
	go ccSrv.Start()

	logger.Debugf("Running peer")

	// Register the Admin server
	pb.RegisterAdminServer(peerServer.Server(), core.NewAdminServer())

	// Register the Endorser server
	serverEndorser := endorser.NewEndorserServer()
	pb.RegisterEndorserServer(peerServer.Server(), serverEndorser)

	// Initialize gossip component
	bootstrap := viper.GetStringSlice("peer.gossip.bootstrap")

	serializedIdentity, err := mgmt.GetLocalSigningIdentityOrPanic().Serialize()
	if err != nil {
		logger.Panicf("Failed serializing self identity: %v", err)
	}

	messageCryptoService := peergossip.NewMCS(
		peer.NewChannelPolicyManagerGetter(),
		localmsp.NewSigner(),
		mgmt.NewDeserializersManager())
	secAdv := peergossip.NewSecurityAdvisor(mgmt.NewDeserializersManager())

	// callback function for secure dial options for gossip service
	secureDialOpts := func() []grpc.DialOption {
		var dialOpts []grpc.DialOption
		// set max send/recv msg sizes
		dialOpts = append(dialOpts, grpc.WithDefaultCallOptions(grpc.MaxCallRecvMsgSize(comm.MaxRecvMsgSize()),
			grpc.MaxCallSendMsgSize(comm.MaxSendMsgSize())))
		// set the keepalive options
		dialOpts = append(dialOpts, comm.ClientKeepaliveOptions()...)

		if comm.TLSEnabled() {
			tlsCert := peerServer.ServerCertificate()
			dialOpts = append(dialOpts, grpc.WithTransportCredentials(comm.GetCASupport().GetPeerCredentials(tlsCert)))
		} else {
			dialOpts = append(dialOpts, grpc.WithInsecure())
		}
		return dialOpts
	}
	err = service.InitGossipService(serializedIdentity, peerEndpoint.Address, peerServer.Server(),
		messageCryptoService, secAdv, secureDialOpts, bootstrap...)
	if err != nil {
		return err
	}
	defer service.GetGossipService().Stop()

	//initialize system chaincodes
	initSysCCs()

	//this brings up all the chains (including testchainid)
	peer.Initialize(func(cid string) {
		logger.Debugf("Deploying system CC, for chain <%s>", cid)
		scc.DeploySysCCs(cid)
	})

	logger.Infof("Starting peer with ID=[%s], network ID=[%s], address=[%s]",
		peerEndpoint.Id, viper.GetString("peer.networkId"), peerEndpoint.Address)

	// Start the grpc server. Done in a goroutine so we can deploy the
	// genesis block if needed.
	serve := make(chan error)

	sigs := make(chan os.Signal, 1)
	signal.Notify(sigs, syscall.SIGINT, syscall.SIGTERM)
	go func() {
		sig := <-sigs
		logger.Debugf("sig: %s", sig)
		serve <- nil
	}()

	go func() {
		var grpcErr error
		if grpcErr = peerServer.Start(); grpcErr != nil {
			grpcErr = fmt.Errorf("grpc server exited with error: %s", grpcErr)
		} else {
			logger.Info("peer server exited")
		}
		serve <- grpcErr
	}()

	if err := writePid(config.GetPath("peer.fileSystemPath")+"/peer.pid", os.Getpid()); err != nil {
		return err
	}

	// Start the event hub server
	if ehubGrpcServer != nil {
		go ehubGrpcServer.Start()
	}

	// Start profiling http endpoint if enabled
	if viper.GetBool("peer.profile.enabled") {
		go func() {
			profileListenAddress := viper.GetString("peer.profile.listenAddress")
			logger.Infof("Starting profiling server with listenAddress = %s", profileListenAddress)
			if profileErr := http.ListenAndServe(profileListenAddress, nil); profileErr != nil {
				logger.Errorf("Error starting profiler: %s", profileErr)
			}
		}()
	}

	logger.Infof("Started peer with ID=[%s], network ID=[%s], address=[%s]",
		peerEndpoint.Id, viper.GetString("peer.networkId"), peerEndpoint.Address)

	// set the logging level for specific modules defined via environment
	// variables or core.yaml
	overrideLogModules := []string{"msp", "gossip", "ledger", "cauthdsl", "policies", "grpc"}
	for _, module := range overrideLogModules {
		err = common.SetLogLevelFromViper(module)
		if err != nil {
			logger.Warningf("Error setting log level for module '%s': %s", module, err.Error())
		}
	}

	flogging.SetPeerStartupModulesMap()

	// Block until grpc server exits
	return <-serve
}
```

#### 2.6.4 order

TODO


### 2.7 node sdk

[Installing Node.js via package manage](https://nodejs.org/en/download/package-manager/)

#### ubuntu

apt-get update

apt-get install npm

curl -sL https://deb.nodesource.com/setup_8.x | sudo -E bash -

sudo apt-get install -y nodejs

#### centos

https://nodejs.org/en/download/package-manager/#enterprise-linux-and-fedora

curl --silent --location https://rpm.nodesource.com/setup_8.x | sudo bash -

sudo yum -y install nodejs

#### install fabric client api

npm install fabric-client

npm install fabric-ca-client

npm install grpc

#### debug

**调试技巧**：使用`supervisor`实现动态修改js代码，当`supervisor`发现js代码变动后会重新启动nodejs。

npm install -g supervisor

supervisor app.js

安装问题：

1. https://stackoverflow.com/questions/19936850/nodejs-error-node-js810-var-cwd-process-cwd

2. 
npm rebuild

#### some tools

[Examples of sequential, concurrent and parallel requests in node.js](https://gist.github.com/montanaflynn/cb349fd109b561c35d6c8500471cdb39)


### 2.8 fabric-sdk-go

https://github.com/hyperledger/fabric-sdk-go

https://github.com/securekey/fabric-examples/tree/master/fabric-cli/



### 2.9 DB

#### 2.9.1 CouchDB

安装：
http://docs.couchdb.org/en/latest/install/unix.html#installation-using-the-apache-couchdb-convenience-binary-packages

API：
http://docs.couchdb.org/en/latest/api/index.html

测试：
http://45.55.51.66:35611/_utils/
存储数据的目录：在Dockerfile中指定
VOLUME ["/opt/couchdb/data"]

#### 2.9.2 LevelDB

接口说明：
https://godoc.org/github.com/syndtr/goleveldb/leveldb


### 2.10 Order

#### peer join操作如何选择和哪个order通信

peer在执行peer join操作时，会调用cscc系统链的JoinChain操作，完成PeerLedger的创建，需要建立和order的通信。order的地址信息通过下面配置获取：
``` go
ordererAddresses := configtxManager.ChannelConfig().OrdererAddresses()
```
有两个地方会调用到`createChain`操作。一个是正常peer join时，一个是peer启动恢复PeerLedger时。
``` go
// src\github.com\hyperledger\fabric\core\peer\peer.go
// createChain creates a new chain object and insert it into the chains
func createChain(cid string, ledger ledger.PeerLedger, cb *common.Block) error {
    // ...
	ordererAddresses := configtxManager.ChannelConfig().OrdererAddresses()
	if len(ordererAddresses) == 0 {
		return errors.New("No ordering service endpoint provided in configuration block")
	}
	service.GetGossipService().InitializeChannel(cs.ChainID(), c, ordererAddresses)
	// ...
}
```

order的地址信息可以配置多个，peer通过洗牌算法会随机选取一个order通信。

``` go
// https://golang.org/pkg/math/rand/#Perm
// src\github.com\hyperledger\fabric\core\comm\producer.go

// NewConnection creates a new connection.
// Returns the connection, the endpoint selected, nil on success.
// Returns nil, "", error on failure
func (cp *connProducer) NewConnection() (*grpc.ClientConn, string, error) {
	cp.RLock()
	defer cp.RUnlock()

	endpoints := shuffle(cp.endpoints)
	for _, endpoint := range endpoints {
		conn, err := cp.connect(endpoint) // 随机选取一个order创建连接
		if err != nil {
			logger.Error("Failed connecting to", endpoint, ", error:", err)
			continue
		}
		return conn, endpoint, nil
	}
	return nil, "", fmt.Errorf("Could not connect to any of the endpoints: %v", endpoints)
}

func shuffle(a []string) []string {
	n := len(a)
	returnedSlice := make([]string, n)
	rand.Seed(time.Now().UnixNano())
	indices := rand.Perm(n)
	for i, idx := range indices {
		returnedSlice[i] = a[idx]
	}
	return returnedSlice
}
```

[Fabric/源码分析]Order服务原理浅析（v1.0）
https://www.jianshu.com/p/bd00f24a4771



### 2.11  kafka

配置目录：/opt/kafka （通过src\github.com\hyperledger\fabric\images\kafka\Dockerfile.in可知）
日志目录：/tmp/kafka-logs

关于`KAFKA_UNCLEAN_LEADER_ELECTION_ENABLE=false`的说明：

> * 这就需要在可用性和一致性当中作出一个简单的抉择。如果一定要等待ISR中的replica“活”过来，那不可用的时间就可能会相对较长。而且如果ISR中所有的replica都无法“活”过来了，或者数据丢失了，这个partition将永远不可用。选择第一个“活”过来的replica作为leader,而这个replica不是ISR中的replica,那即使它并不保障已经包含了所有已commit的消息，它也会成为leader而作为consumer的数据源。默认情况下，Kafka采用第二种策略，即unclean.leader.election.enable=true，也可以将此参数设置为false来启用第一种策略。
http://www.importnew.com/25247.html
* unclean.leader.election.enable = false — Data consistency is key in a blockchain environment. We cannot have a channel leader chosen outside of the in-sync replica set, or we run the risk of overwriting the offsets that the previous leader produced, and —as a result— rewrite the blockchain that the orderers produce.
http://hyperledger-fabric.readthedocs.io/en/latest/kafka.html

Kafka系统配置：
/opt/kafka/config/server.properties
```
############################# Log Retention Policy #############################

# The following configurations control the disposal of log segments. The policy can
# be set to delete segments after a period of time, or after a given size has accumulated.
# A segment will be deleted whenever *either* of these criteria are met. Deletion always happens
# from the end of the log.

# The minimum age of a log file to be eligible for deletion
log.retention.hours=168

# A size-based retention policy for logs. Segments are pruned from the log as long as the remaining
# segments don't drop below log.retention.bytes.
#log.retention.bytes=1073741824

# The maximum size of a log segment file. When this size is reached a new log segment will be created.
log.segment.bytes=1073741824

# The interval at which log segments are checked to see if they can be deleted according
# to the retention policies
log.retention.check.interval.ms=300000
```


## 3 数据同步

1，在使用kafka共识下，验证多个order间如何保证cut block一致性（通过bathSize,batchTimeout以及TTC-x）
2，验证peer节点之间gossip的两种同步机制，static leader peer和dynamic leader selection的数据一致性

## ledger落盘策略

### order

``` go
// BlockStore - an interface for persisting and retrieving blocks
// An implementation of this interface is expected to take an argument
// of type `IndexConfig` which configures the block store on what items should be indexed
type BlockStore interface {
	AddBlock(block *common.Block) error
	GetBlockchainInfo() (*common.BlockchainInfo, error)
	RetrieveBlocks(startNum uint64) (ledger.ResultsIterator, error)
	RetrieveBlockByHash(blockHash []byte) (*common.Block, error)
	RetrieveBlockByNumber(blockNum uint64) (*common.Block, error) // blockNum of  math.MaxUint64 will return last block
	RetrieveTxByID(txID string) (*common.Envelope, error)
	RetrieveTxByBlockNumTranNum(blockNum uint64, tranNum uint64) (*common.Envelope, error)
	RetrieveBlockByTxID(txID string) (*common.Block, error)
	RetrieveTxValidationCodeByTxID(txID string) (peer.TxValidationCode, error)
	Shutdown()
}
```

``` go
// src\github.com\hyperledger\fabric\common\ledger\blkstorage\fsblkstorage\fs_blockstore.go
// AddBlock adds a new block
func (store *fsBlockStore) AddBlock(block *common.Block) error {
	return store.fileMgr.addBlock(block)
}
```

``` go
// src\github.com\hyperledger\fabric\common\ledger\blkstorage\fsblkstorage\blockfile_mgr.go
func (mgr *blockfileMgr) addBlock(block *common.Block) error {
	if block.Header.Number != mgr.getBlockchainInfo().Height {
		return fmt.Errorf("Block number should have been %d but was %d", mgr.getBlockchainInfo().Height, block.Header.Number)
	}
	blockBytes, info, err := serializeBlock(block)
	if err != nil {
		return fmt.Errorf("Error while serializing block: %s", err)
	}
	blockHash := block.Header.Hash()
	//Get the location / offset where each transaction starts in the block and where the block ends
	txOffsets := info.txOffsets
	currentOffset := mgr.cpInfo.latestFileChunksize
	if err != nil {
		return fmt.Errorf("Error while serializing block: %s", err)
	}
	blockBytesLen := len(blockBytes)
	blockBytesEncodedLen := proto.EncodeVarint(uint64(blockBytesLen))
	totalBytesToAppend := blockBytesLen + len(blockBytesEncodedLen)

	//Determine if we need to start a new file since the size of this block
	//exceeds the amount of space left in the current file
	if currentOffset+totalBytesToAppend > mgr.conf.maxBlockfileSize {
		mgr.moveToNextFile()
		currentOffset = 0
	}
	//append blockBytesEncodedLen to the file
	err = mgr.currentFileWriter.append(blockBytesEncodedLen, false)
	if err == nil {
		//append the actual block bytes to the file
		err = mgr.currentFileWriter.append(blockBytes, true)
	}
	if err != nil {
		truncateErr := mgr.currentFileWriter.truncateFile(mgr.cpInfo.latestFileChunksize)
		if truncateErr != nil {
			panic(fmt.Sprintf("Could not truncate current file to known size after an error during block append: %s", err))
		}
		return fmt.Errorf("Error while appending block to file: %s", err)
	}

	//Update the checkpoint info with the results of adding the new block
	currentCPInfo := mgr.cpInfo
	newCPInfo := &checkpointInfo{
		latestFileChunkSuffixNum: currentCPInfo.latestFileChunkSuffixNum,
		latestFileChunksize:      currentCPInfo.latestFileChunksize + totalBytesToAppend,
		isChainEmpty:             false,
		lastBlockNumber:          block.Header.Number}
	//save the checkpoint information in the database
	if err = mgr.saveCurrentInfo(newCPInfo, false); err != nil {
		truncateErr := mgr.currentFileWriter.truncateFile(currentCPInfo.latestFileChunksize)
		if truncateErr != nil {
			panic(fmt.Sprintf("Error in truncating current file to known size after an error in saving checkpoint info: %s", err))
		}
		return fmt.Errorf("Error while saving current file info to db: %s", err)
	}

	//Index block file location pointer updated with file suffex and offset for the new block
	blockFLP := &fileLocPointer{fileSuffixNum: newCPInfo.latestFileChunkSuffixNum}
	blockFLP.offset = currentOffset
	// shift the txoffset because we prepend length of bytes before block bytes
	for _, txOffset := range txOffsets {
		txOffset.loc.offset += len(blockBytesEncodedLen)
	}
	//save the index in the database
	mgr.index.indexBlock(&blockIdxInfo{
		blockNum: block.Header.Number, blockHash: blockHash,
		flp: blockFLP, txOffsets: txOffsets, metadata: block.Metadata})

	//update the checkpoint info (for storage) and the blockchain info (for APIs) in the manager
	mgr.updateCheckpoint(newCPInfo)
	mgr.updateBlockchainInfo(blockHash, block)
	return nil
}
```

``` go
func (w *blockfileWriter) append(b []byte, sync bool) error {
	_, err := w.file.Write(b)
	if err != nil {
		return err
	}
	if sync {
		return w.file.Sync()
	}
	return nil
}
```

https://golang.org/pkg/os/#File.Sync

TODO

## 4 blockchain-explorer

https://github.com/hyperledger/blockchain-explorer

mysql -u<username> -p < db/fabricexplorer.sql
./start.sh

http://45.55.51.66:8080/

![blockchain-explorer.png](https://github.com/gerryyang/wcdj/blob/master/fabric-annotated/pic/blockchain-explorer.png?raw=true)

创世块：

![blockchain-explorer_genisis.png](https://github.com/gerryyang/wcdj/blob/master/fabric-annotated/pic/blockchain-explorer_genisis.png?raw=true)

对应的数据库信息：

![blockchain-explorer-db.png](https://github.com/gerryyang/wcdj/blob/master/fabric-annotated/pic/blockchain-explorer-db.png?raw=true)


## 5 链码（Chaincode/智能合约）

* 用户链码
* 系统链码

用户只需要关注`Init()`和`Invoke()`函数的实现。
``` go
// src\github.com\hyperledger\fabric\core\chaincode\shim\interfaces.go
// Chaincode interface must be implemented by all chaincodes. The fabric runs
// the transactions by calling these functions as specified.
type Chaincode interface {
	// Init is called during Instantiate transaction after the chaincode container
	// has been established for the first time, allowing the chaincode to
	// initialize its internal data
	Init(stub ChaincodeStubInterface) pb.Response

	// Invoke is called to update or query the ledger in a proposal transaction.
	// Updated state variables are not committed to the ledger until the
	// transaction is committed.
	Invoke(stub ChaincodeStubInterface) pb.Response
}
```

在其中利用`shim.ChaincodeStubInterface`结构，实现跟账本的交互逻辑，功能包括，对账本进行操作，读取交易参数，调用其他链码等，链码开发者最好能够熟练使用这些API。
``` go
// ChaincodeStubInterface is used by deployable chaincode apps to access and
// modify their ledgers
type ChaincodeStubInterface interface {
	// GetArgs returns the arguments intended for the chaincode Init and Invoke
	// as an array of byte arrays.
	GetArgs() [][]byte

	// GetStringArgs returns the arguments intended for the chaincode Init and
	// Invoke as a string array. Only use GetStringArgs if the client passes
	// arguments intended to be used as strings.
	GetStringArgs() []string

	// GetFunctionAndParameters returns the first argument as the function
	// name and the rest of the arguments as parameters in a string array.
	// Only use GetFunctionAndParameters if the client passes arguments intended
	// to be used as strings.
	GetFunctionAndParameters() (string, []string)
    // ...
}
```

这些API主要分为4类。

### 账本状态交互API

* GetState
* PutState
* DelState
* GetStateByRange
* GetStateByPartialCompositeKey
* GetHistoryForKey
* GetQueryResult

### 交易信息相关API

* GetTxID
* GetTxTimestamp
* GetBinding
* GetSignedProposal
* GetCreator
* GetTransient

### 参数读取API

* GetArgs
* GetArgsSlice
* GetFunctionAndParameters
* GetStringArgs

### 其他API

* CreateCompositeKey
* SplitCompositeKey
* InvokeChaincode
* SetEvent

## 6 性能测试

[Performance Traffic Engine - PTE](https://github.com/hyperledger/fabric/tree/release/test/tools/PTE) uses [Hyperledger Fabric Client (HFC) Node SDK](https://fabric-sdk-node.github.io/index.html) to interact with a Hyperledger Fabric network.

TODO

## 7 Hyperledger Composer

https://github.com/hyperledger/composer
https://hyperledger.github.io/composer/introduction/introduction.html
https://hyperledger.github.io/composer/tutorials/tutorials.html

https://hyperledger.github.io/composer/introduction/key-concepts.html
https://hyperledger.github.io/composer/introduction/solution-architecture.html

INSTALL
https://hyperledger.github.io/composer/installing/installing-index.html
https://hyperledger.github.io/composer/installing/installing-prereqs.html
https://hyperledger.github.io/composer/installing/development-tools.html
https://hyperledger.github.io/composer/tutorials/developer-tutorial.html

https://hyperledger.github.io/composer/tutorials/playground-tutorial.html

http://duktape.org/guide.html#programming
http://duktape.org/guide.html#gettingstarted

https://stackoverflow.com/questions/48398274/what-different-in-chaincode-in-golang-and-composer-create-bna-file

[Getting Started with Blockchain Development](https://blog.selman.org/2017/07/08/getting-started-with-blockchain-development/)


安装时的问题：

https://askubuntu.com/questions/223237/unable-to-correct-problems-you-have-held-broken-packages

https://techoverflow.net/2012/10/07/how-to-fix-npm-cannot-find-module-graceful-fs-error/
https://github.com/kelektiv/node.bcrypt.js/issues/153

npm config set unsafe-perm true
http://ethan.logdown.com/posts/2015/06/23/sh-1-node-gyp-permission-denied
https://docs.npmjs.com/misc/config#unsafe-perm
https://github.com/nodejs/node-gyp/issues/454

https://www.npmjs.com/package/root-check
https://github.com/yeoman/yo/issues/348

https://stackoverflow.com/questions/9023672/how-do-i-resolve-cannot-find-module-error-using-node-js

https://stackoverflow.com/questions/47179869/need-to-replace-localhost3000-with-realip3000-in-composer-rest-server

composer-runtime-hlfv1

https://github.com/olebedev/go-duktape/tree/v3

Youtube

[Introduction to Fabric Composer](https://www.youtube.com/watch?v=fdFUsrsv5iw)

测试：

``` bash
$ composer-playground -p 8089
```


开发语言：

Hyperledger Composer Modeling Language
https://hyperledger.github.io/composer/reference/cto_language.html
Transaction Processor Functions
https://hyperledger.github.io/composer/reference/js_scripts.html
Hyperledger Composer Access Control Language
https://hyperledger.github.io/composer/reference/acl_language.html



## 8 需要解决的问题

1. fabric共识机制目前没有实现pbft,只实现solo,kafka共识算法
2. 读写机制,存在热点问题
3. 修改world state后台数据库levelDB的数据,仍然可以达成共识,数据不可篡改性受质疑
4. 没有私钥的密钥管理,加密数据和私钥存储在一起,没有真正实现数据安全
5. fabric做了模块分拆,但是整体性能受限于单机性能,单链tps无法超越1000


## 9 一些验证

### overlay网络，chaincode容器启动失败

因为chaincode容器是以域名的方式访问peer节点（-peer.address=peer0.org1.example.com:7051），overlay网络需要设置DNS配置，否则访问会失败。

``` 
[root@10 fabcar-one-goleveldb]# docker inspect 91b1e6bd49f1
[
    {
        "Id": "91b1e6bd49f1157bb7eb421d75c9a55c91809ddf124d0d77d16ba7084c66626a",
        "Created": "2018-02-27T09:35:08.180627381Z",
        "Path": "chaincode",
        "Args": [
            "-peer.address=peer0.org1.example.com:7051"
        ],
        "State": {
            "Status": "running",
            "Running": true,
            "Paused": false,
            "Restarting": false,
            "OOMKilled": false,
            "Dead": false,
            "Pid": 23353,
            "ExitCode": 0,
            "Error": "",
            "StartedAt": "2018-02-27T09:35:08.441815983Z",
            "FinishedAt": "0001-01-01T00:00:00Z"
        },
        "Image": "sha256:1c895e589e09852f85a2e1e0ba722d7b51495add4bdd44ceedc92d4ec3ae4d50",
        "ResolvConfPath": "/var/lib/docker/containers/91b1e6bd49f1157bb7eb421d75c9a55c91809ddf124d0d77d16ba7084c66626a/resolv.conf",
        "HostnamePath": "/var/lib/docker/containers/91b1e6bd49f1157bb7eb421d75c9a55c91809ddf124d0d77d16ba7084c66626a/hostname",
        "HostsPath": "/var/lib/docker/containers/91b1e6bd49f1157bb7eb421d75c9a55c91809ddf124d0d77d16ba7084c66626a/hosts",
        "LogPath": "/var/lib/docker/containers/91b1e6bd49f1157bb7eb421d75c9a55c91809ddf124d0d77d16ba7084c66626a/91b1e6bd49f1157bb7eb421d75c9a55c91809ddf124d0d77d16ba7084c66626a-json.log",
        "Name": "/dev-peer0.org1.example.com-fabcar-1.0",
        "RestartCount": 0,
        "Driver": "overlay",
        "Platform": "linux",
        ...
```

### instantiation是否需要满足背书策略

验证测试不需要。特殊逻辑

```
root@ubuntu-s-1vcpu-3gb-nyc3-01:~# docker inspect --format={{.LogPath}} peer0.org1.example.com | xargs grep "Signature set is of size"    

# instantiate操作，只请求了一个peer节点，不满足AND背书策略，但是commit成功                                                                       
{"log":"\u001b[36m2018-02-28 06:52:57.706 UTC [validator_onevalidsignature.go:457] [vscc] deduplicateIdentity -\u003e DEBU 412\u001b[0m Signature set is of size 1 out of 1 endorsement(s)\n","stream":"stderr","time":"2018-02-28T06:52:57.706780243Z"}

# invork操作，需要满足AND背书策略，才能commit成功
{"log":"\u001b[36m2018-02-28 07:05:08.378 UTC [validator_onevalidsignature.go:457] [vscc] deduplicateIdentity -\u003e DEBU 600\u001b[0m Signature set is of size 4 out of 4 endorsement(s)\n","stream":"stderr","time":"2018-02-28T07:05:08.378978783Z"}
```

If your chaincode requires arguments be passed to the init method, then you will need to provide the appropriate key/vals and reinitialize the state. This is not the recommended practice, because the upgrade submitter could arbitrarily rewrite the world state. Instead, consider editing the source code to remove the argument dependency, or start with a chaincode that does not require args upon instantiation.

http://hyperledger-fabric.readthedocs.io/en/latest/channel_update.html


### commit逻辑

gossip时的commit

``` golang
// fabric_v1.0.4\gossip\state\state.go

func (s *GossipStateProviderImpl) deliverPayloads() {
	defer s.done.Done()

	for {
		select {
		// Wait for notification that next seq has arrived
		case <-s.payloads.Ready():
			logger.Debugf("Ready to transfer payloads to the ledger, next sequence number is = [%d]", s.payloads.Next())
			// Collect all subsequent payloads
			for payload := s.payloads.Pop(); payload != nil; payload = s.payloads.Pop() {
				rawBlock := &common.Block{}
				if err := pb.Unmarshal(payload.Data, rawBlock); err != nil {
					logger.Errorf("Error getting block with seqNum = %d due to (%s)...dropping block", payload.SeqNum, err)
					continue
				}
				if rawBlock.Data == nil || rawBlock.Header == nil {
					logger.Errorf("Block with claimed sequence %d has no header (%v) or data (%v)",
						payload.SeqNum, rawBlock.Header, rawBlock.Data)
					continue
				}
				logger.Debug("New block with claimed sequence number ", payload.SeqNum, " transactions num ", len(rawBlock.Data.Data))
				if err := s.commitBlock(rawBlock); err != nil {
					logger.Panicf("Cannot commit block to the ledger due to %s", err)
				}
			}
		case <-s.stopCh:
			s.stopCh <- struct{}{}
			logger.Debug("State provider has been stoped, finishing to push new blocks.")
			return
		}
	}
}
```

### txid重复检查

两个阶段：
1，endorser时，ProcessProposal
2，commit时，在VSCC检查前，会通过账本检查是否存在重复的txid。因为背书时ledger还没有记录

endorser时：
``` go
// fabric_v1.0.4\core\endorser\endorser.go
// func (e *Endorser) ProcessProposal(ctx context.Context, signedProp *pb.SignedProposal) (*pb.ProposalResponse, error)

	if chainID != "" {
		// here we handle uniqueness check and ACLs for proposals targeting a chain
		lgr := peer.GetLedger(chainID)
		if lgr == nil {
			return nil, fmt.Errorf("failure while looking up the ledger %s", chainID)
		}
		if _, err := lgr.GetTransactionByID(txid); err == nil {
		    // txid重复，则报错
			return nil, fmt.Errorf("Duplicate transaction found [%s]. Creator [%x]. [%s]", txid, shdr.Creator, err)
		}

		// check ACL only for application chaincodes; ACLs
		// for system chaincodes are checked elsewhere
		if !syscc.IsSysCC(hdrExt.ChaincodeId.Name) {
			// check that the proposal complies with the channel's writers
			if err = e.checkACL(signedProp, chdr, shdr, hdrExt); err != nil {
				return &pb.ProposalResponse{Response: &pb.Response{Status: 500, Message: err.Error()}}, err
			}
		}
	}
```

commit时：
``` go
// fabric_v1.0.4\core\committer\txvalidator\validator.go
// func (v *txValidator) Validate(block *common.Block) error

if common.HeaderType(chdr.Type) == common.HeaderType_ENDORSER_TRANSACTION {
	// Check duplicate transactions
	txID := chdr.TxId
	if _, err := v.support.Ledger().GetTransactionByID(txID); err == nil {
		logger.Error("Duplicate transaction found, ", txID, ", skipping")
		txsfltr.SetFlag(tIdx, peer.TxValidationCode_DUPLICATE_TXID)
		continue
	}

	// Validate tx with vscc and policy
	logger.Debug("Validating transaction vscc tx validate")
	err, cde := v.vscc.VSCCValidateTx(payload, d, env)
	if err != nil {
		txID := txID
		logger.Errorf("VSCCValidateTx for transaction txId = %s returned error %s", txID, err)
		switch err.(type) {
		case *VSCCExecutionFailureError:
			return err
		case *VSCCInfoLookupFailureError:
			return err
		default:
			txsfltr.SetFlag(tIdx, cde)
			continue
		}
	}

	invokeCC, upgradeCC, err := v.getTxCCInstance(payload)
	if err != nil {
		logger.Errorf("Get chaincode instance from transaction txId = %s returned error %s", txID, err)
		txsfltr.SetFlag(tIdx, peer.TxValidationCode_INVALID_OTHER_REASON)
		continue
	}
	txsChaincodeNames[tIdx] = invokeCC
	if upgradeCC != nil {
		logger.Infof("Find chaincode upgrade transaction for chaincode %s on chain %s with new version %s", upgradeCC.ChaincodeName, upgradeCC.ChainID, upgradeCC.ChaincodeVersion)
		txsUpgradedChaincodes[tIdx] = upgradeCC
	}
}
```
1.1.0-alpha版本，新增了一个对当前即将写入账本的区块中是否存在重复txid的检查，因为此时还没有写账本，在账本中查不到。

``` go				
// fabric-v1.1.0-alpha\core\committer\txvalidator\validator.go
// func (v *txValidator) Validate(block *common.Block) error

// if we operate with this capability, we mark invalid any transaction that has a txid
// which is equal to that of a previous tx in this block
if v.support.Capabilities().ForbidDuplicateTXIdInBlock() {
	markTXIdDuplicates(txidArray, txsfltr)
}
```

### 在Fabric 1.0.4版本和1.1.x版本中，LevelDB, CouchDB修改wordstate后，Fabric如何处理？

修改wordstate数据，包括几种情况。
情况1：修改版本号
情况2：修改数据
情况3：修改版本号和数据

Fabric在提交账本阶段，先检查背书策略，然后进行MVCC检查。若检查失败，则分别返回`ENDORSEMENT_POLICY_FAILURE`错误和`MVCC_READ_CONFLICT`错误。

背书策略检查，在执行`VSCCValidateTx`函数中，调用`GetActionFromEnvelope`从Envelope结构中获取第一个Action的Payload(读写集)。

因此，若第一个Action是篡改后的背书结果，且在满足背书检查和MVCC检查的情况下，会被正确提交。

``` golang
// GetActionFromEnvelope extracts a ChaincodeAction message from a serialized Envelope
func GetActionFromEnvelope(envBytes []byte) (*peer.ChaincodeAction, error) {
	env, err := GetEnvelopeFromBlock(envBytes)
	if err != nil {
		return nil, err
	}

	payl, err := GetPayload(env)
	if err != nil {
		return nil, err
	}

	tx, err := GetTransaction(payl.Data)
	if err != nil {
		return nil, err
	}

	if len(tx.Actions) == 0 {
		return nil, fmt.Errorf("At least one TransactionAction is required")
	}

	_, respPayload, err := GetPayloads(tx.Actions[0]) // 第一个Action的Payload
	return respPayload, err
}
```

例子：
Org1：peer0, peer1
Org2: peer0, peer1

AND策略：
200, 100    假设200是第一个Action，则commit返回ENDORSEMENT_POLICY_FAILURE错误
100, 100

200, 100    假设100是第一个Action，则commit返回成功
100, 100


## 关键日志

### peer启动日志

```
2018-02-24 06:46:27.811 UTC [main.go:114] [main] main -> INFO 001 [gerry] mspMgrConfigDir[/etc/hyperledger/msp/peer/]
2018-02-24 06:46:28.007 UTC [main.go:54] [main] func1 -> INFO 002 [gerry] logging_level: debug
2018-02-24 06:46:28.008 UTC [start.go:89] [nodeCmd] serve -> INFO 003 Starting peer:
 Version: 1.0.5
 Go version: go1.7.5
 OS/Arch: linux/amd64
 Chaincode:
  Base Image Version: 0.3.2
  Base Docker Namespace: hyperledger
  Base Docker Label: org.hyperledger.fabric
  Docker Namespace: hyperledger

2018-02-24 06:46:28.008 UTC [start.go:91] [nodeCmd] serve -> DEBU 004 [gerry] before ledgermgmt.Initialize
2018-02-24 06:46:28.008 UTC [ledger_mgmt.go:49] [ledgermgmt] func1 -> DEBU 005 [gerry] to initialize
2018-02-24 06:46:28.008 UTC [ledger_mgmt.go:55] [ledgermgmt] initialize -> INFO 006 Initializing ledger mgmt
2018-02-24 06:46:28.008 UTC [kv_ledger_provider.go:64] [kvledger] NewProvider -> INFO 007 Initializing ledger provider
2018-02-24 06:46:28.008 UTC [kv_ledger_provider.go:67] [kvledger] NewProvider -> DEBU 008 [gerry] ledgerconfig.GetLedgerProviderPath: /var/hyperledger/production/ledgersData/ledgerProvider
2018-02-24 06:46:28.008 UTC [ioutil.go:37] [kvledger.util] CreateDirIfMissing -> DEBU 009 CreateDirIfMissing [/var/hyperledger/production/ledgersData/ledgerProvider/]
2018-02-24 06:46:28.008 UTC [ioutil.go:97] [kvledger.util] logDirStatus -> DEBU 00a Before creating dir - [/var/hyperledger/production/ledgersData/ledgerProvider/] does not exist
2018-02-24 06:46:28.009 UTC [ioutil.go:95] [kvledger.util] logDirStatus -> DEBU 00b After creating dir - [/var/hyperledger/production/ledgersData/ledgerProvider/] exists
2018-02-24 06:46:28.030 UTC [ioutil.go:37] [kvledger.util] CreateDirIfMissing -> DEBU 00c CreateDirIfMissing [/var/hyperledger/production/ledgersData/chains/index/]
2018-02-24 06:46:28.031 UTC [ioutil.go:97] [kvledger.util] logDirStatus -> DEBU 00d Before creating dir - [/var/hyperledger/production/ledgersData/chains/index/] does not exist
2018-02-24 06:46:28.031 UTC [ioutil.go:95] [kvledger.util] logDirStatus -> DEBU 00e After creating dir - [/var/hyperledger/production/ledgersData/chains/index/] exists
2018-02-24 06:46:28.041 UTC [kv_ledger_provider.go:87] [kvledger] NewProvider -> DEBU 00f Constructing leveldb VersionedDBProvider
2018-02-24 06:46:28.041 UTC [stateleveldb.go:45] [stateleveldb] NewVersionedDBProvider -> DEBU 010 constructing VersionedDBProvider dbPath=/var/hyperledger/production/ledgersData/stateLeveldb
2018-02-24 06:46:28.041 UTC [ioutil.go:37] [kvledger.util] CreateDirIfMissing -> DEBU 011 CreateDirIfMissing [/var/hyperledger/production/ledgersData/stateLeveldb/]
2018-02-24 06:46:28.041 UTC [ioutil.go:97] [kvledger.util] logDirStatus -> DEBU 012 Before creating dir - [/var/hyperledger/production/ledgersData/stateLeveldb/] does not exist
2018-02-24 06:46:28.042 UTC [ioutil.go:95] [kvledger.util] logDirStatus -> DEBU 013 After creating dir - [/var/hyperledger/production/ledgersData/stateLeveldb/] exists
2018-02-24 06:46:28.050 UTC [historyleveldb.go:46] [historyleveldb] NewHistoryDBProvider -> DEBU 014 constructing HistoryDBProvider dbPath=/var/hyperledger/production/ledgersData/historyLeveldb
2018-02-24 06:46:28.050 UTC [ioutil.go:37] [kvledger.util] CreateDirIfMissing -> DEBU 015 CreateDirIfMissing [/var/hyperledger/production/ledgersData/historyLeveldb/]
2018-02-24 06:46:28.051 UTC [ioutil.go:97] [kvledger.util] logDirStatus -> DEBU 016 Before creating dir - [/var/hyperledger/production/ledgersData/historyLeveldb/] does not exist
2018-02-24 06:46:28.051 UTC [ioutil.go:95] [kvledger.util] logDirStatus -> DEBU 017 After creating dir - [/var/hyperledger/production/ledgersData/historyLeveldb/] exists
2018-02-24 06:46:28.053 UTC [kv_ledger_provider.go:102] [kvledger] NewProvider -> INFO 018 ledger provider Initialized
2018-02-24 06:46:28.053 UTC [kv_ledger_provider.go:208] [kvledger] recoverUnderConstructionLedger -> DEBU 019 Recovering under construction ledger
2018-02-24 06:46:28.053 UTC [kv_ledger_provider.go:212] [kvledger] recoverUnderConstructionLedger -> DEBU 01a No under construction ledger found. Quitting recovery
2018-02-24 06:46:28.053 UTC [ledger_mgmt.go:65] [ledgermgmt] initialize -> INFO 01b ledger mgmt initialized
2018-02-24 06:46:28.053 UTC [start.go:93] [nodeCmd] serve -> DEBU 01c [gerry] after ledgermgmt.Initialize
2018-02-24 06:46:28.053 UTC [start.go:106] [nodeCmd] serve -> DEBU 01d [gerry] before peer.CacheConfiguration
2018-02-24 06:46:28.054 UTC [start.go:110] [nodeCmd] serve -> DEBU 01e [gerry] after peer.CacheConfiguration
2018-02-24 06:46:28.054 UTC [start.go:112] [nodeCmd] serve -> DEBU 01f [gerry] before peer.GetPeerEndpoint
2018-02-24 06:46:28.054 UTC [start.go:118] [nodeCmd] serve -> DEBU 020 [gerry] after peer.GetPeerEndpoint
2018-02-24 06:46:28.055 UTC [start.go:122] [nodeCmd] serve -> DEBU 021 [gerry] before peer.GetSecureConfig
2018-02-24 06:46:28.055 UTC [start.go:127] [nodeCmd] serve -> DEBU 022 [gerry] after peer.GetSecureConfig
2018-02-24 06:46:28.055 UTC [start.go:129] [nodeCmd] serve -> DEBU 023 [gerry] before peer.CreatePeerServer
2018-02-24 06:46:28.055 UTC [start.go:134] [nodeCmd] serve -> DEBU 024 [gerry] after peer.CreatePeerServer
2018-02-24 06:46:28.056 UTC [start.go:143] [nodeCmd] serve -> DEBU 025 [gerry] before createEventHubServer
2018-02-24 06:46:28.057 UTC [events.go:262] [eventhub_producer] AddEventType -> DEBU 026 Registering BLOCK
2018-02-24 06:46:28.058 UTC [events.go:262] [eventhub_producer] AddEventType -> DEBU 027 Registering CHAINCODE
2018-02-24 06:46:28.058 UTC [events.go:262] [eventhub_producer] AddEventType -> DEBU 028 Registering REJECTION
2018-02-24 06:46:28.058 UTC [events.go:262] [eventhub_producer] AddEventType -> DEBU 029 Registering REGISTER
2018-02-24 06:46:28.059 UTC [start.go:149] [nodeCmd] serve -> DEBU 02a [gerry] after createEventHubServer
2018-02-24 06:46:28.059 UTC [start.go:155] [nodeCmd] serve -> DEBU 02b [gerry] before createChaincodeServer
2018-02-24 06:46:28.059 UTC [start.go:311] [nodeCmd] createChaincodeServer -> WARN 02c peer.chaincodeListenAddress is not set, use peer.listenAddress 0.0.0.0:7051
2018-02-24 06:46:28.059 UTC [start.go:157] [nodeCmd] serve -> DEBU 02d [gerry] after createChaincodeServer
2018-02-24 06:46:28.059 UTC [start.go:159] [nodeCmd] serve -> DEBU 02e [gerry] before registerChaincodeSupport
2018-02-24 06:46:28.060 UTC [start.go:352] [nodeCmd] registerChaincodeSupport -> DEBU 02f Chaincode startup timeout value set to 5m0s
2018-02-24 06:46:28.060 UTC [chaincode_support.go:152] [chaincode] NewChaincodeSupport -> INFO 030 Chaincode support using peerAddress: peer0.org1.example.com:7051
2018-02-24 06:46:28.061 UTC [chaincode_support.go:178] [chaincode] NewChaincodeSupport -> DEBU 031 Turn off keepalive(value 0)
2018-02-24 06:46:28.061 UTC [chaincode_support.go:189] [chaincode] NewChaincodeSupport -> DEBU 032 Setting execute timeout value to 30s
2018-02-24 06:46:28.062 UTC [chaincode_support.go:213] [chaincode] getLogLevelFromViper -> DEBU 033 CORE_CHAINCODE_LEVEL set to level debug
2018-02-24 06:46:28.062 UTC [chaincode_support.go:213] [chaincode] getLogLevelFromViper -> DEBU 034 CORE_CHAINCODE_SHIM set to level debug
2018-02-24 06:46:28.062 UTC [sysccapi.go:77] [sccapi] RegisterSysCC -> DEBU 035 [gerry] syscc.Name[cscc] syscc.Path[github.com/hyperledger/fabric/core/scc/cscc] syscc.Chaincode[&{<nil>}]
2018-02-24 06:46:28.062 UTC [sysccapi.go:88] [sccapi] RegisterSysCC -> INFO 036 system chaincode cscc(github.com/hyperledger/fabric/core/scc/cscc) registered
2018-02-24 06:46:28.063 UTC [sysccapi.go:77] [sccapi] RegisterSysCC -> DEBU 037 [gerry] syscc.Name[lscc] syscc.Path[github.com/hyperledger/fabric/core/scc/lscc] syscc.Chaincode[&{<nil> <nil>}]
2018-02-24 06:46:28.063 UTC [sysccapi.go:88] [sccapi] RegisterSysCC -> INFO 038 system chaincode lscc(github.com/hyperledger/fabric/core/scc/lscc) registered
2018-02-24 06:46:28.063 UTC [sysccapi.go:77] [sccapi] RegisterSysCC -> DEBU 039 [gerry] syscc.Name[escc] syscc.Path[github.com/hyperledger/fabric/core/scc/escc] syscc.Chaincode[&{}]
2018-02-24 06:46:28.063 UTC [sysccapi.go:88] [sccapi] RegisterSysCC -> INFO 03a system chaincode escc(github.com/hyperledger/fabric/core/scc/escc) registered
2018-02-24 06:46:28.064 UTC [sysccapi.go:77] [sccapi] RegisterSysCC -> DEBU 03b [gerry] syscc.Name[vscc] syscc.Path[github.com/hyperledger/fabric/core/scc/vscc] syscc.Chaincode[&{<nil>}]
2018-02-24 06:46:28.064 UTC [sysccapi.go:88] [sccapi] RegisterSysCC -> INFO 03c system chaincode vscc(github.com/hyperledger/fabric/core/scc/vscc) registered
2018-02-24 06:46:28.064 UTC [sysccapi.go:77] [sccapi] RegisterSysCC -> DEBU 03d [gerry] syscc.Name[qscc] syscc.Path[github.com/hyperledger/fabric/core/chaincode/qscc] syscc.Chaincode[&{<nil>}]
2018-02-24 06:46:28.064 UTC [sysccapi.go:88] [sccapi] RegisterSysCC -> INFO 03e system chaincode qscc(github.com/hyperledger/fabric/core/chaincode/qscc) registered
2018-02-24 06:46:28.065 UTC [start.go:161] [nodeCmd] serve -> DEBU 03f [gerry] after registerChaincodeSupport
2018-02-24 06:46:28.065 UTC [start.go:163] [nodeCmd] serve -> DEBU 040 [gerry] before go ccSrv.Start()
2018-02-24 06:46:28.065 UTC [start.go:165] [nodeCmd] serve -> DEBU 041 [gerry] after go ccSrv.Start()
2018-02-24 06:46:28.065 UTC [start.go:167] [nodeCmd] serve -> DEBU 042 Running peer
2018-02-24 06:46:28.065 UTC [start.go:170] [nodeCmd] serve -> DEBU 043 [gerry] before RegisterAdminServer
2018-02-24 06:46:28.065 UTC [start.go:172] [nodeCmd] serve -> DEBU 044 [gerry] after RegisterAdminServer
2018-02-24 06:46:28.066 UTC [start.go:175] [nodeCmd] serve -> DEBU 045 [gerry] before RegisterEndorserServer
2018-02-24 06:46:28.071 UTC [mgmt.go:149] [msp] GetLocalMSP -> DEBU 046 Returning existing local MSP
2018-02-24 06:46:28.071 UTC [start.go:178] [nodeCmd] serve -> DEBU 047 [gerry] after RegisterEndorserServer
2018-02-24 06:46:28.071 UTC [start.go:181] [nodeCmd] serve -> DEBU 048 [gerry] before Initialize gossip component
2018-02-24 06:46:28.071 UTC [mgmt.go:149] [msp] GetLocalMSP -> DEBU 049 Returning existing local MSP
2018-02-24 06:46:28.071 UTC [mspimpl.go:369] [msp] GetDefaultSigningIdentity -> DEBU 04a Obtaining default signing identity
2018-02-24 06:46:28.071 UTC [gossip_service.go:144] [gossip/service] func1 -> INFO 04b Initialize gossip with endpoint peer0.org1.example.com:7051 and bootstrap set [127.0.0.1:7051]
2018-02-24 06:46:28.072 UTC [mgmt.go:149] [msp] GetLocalMSP -> DEBU 04c Returning existing local MSP
2018-02-24 06:46:28.072 UTC [mspimpl.go:407] [msp] DeserializeIdentity -> INFO 04d Obtaining identity
2018-02-24 06:46:28.072 UTC [identities.go:42] [msp/identity] newIdentity -> DEBU 04e Creating identity instance for ID -----BEGIN CERTIFICATE-----
MIICGjCCAcCgAwIBAgIRAIm9LgLGuAZxcn/6thnwMD0wCgYIKoZIzj0EAwIwczEL
MAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBG
cmFuY2lzY28xGTAXBgNVBAoTEG9yZzEuZXhhbXBsZS5jb20xHDAaBgNVBAMTE2Nh
Lm9yZzEuZXhhbXBsZS5jb20wHhcNMTgwMjA3MDAxNTE0WhcNMjgwMjA1MDAxNTE0
WjBbMQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMN
U2FuIEZyYW5jaXNjbzEfMB0GA1UEAxMWcGVlcjAub3JnMS5leGFtcGxlLmNvbTBZ
MBMGByqGSM49AgEGCCqGSM49AwEHA0IABBuNYucbhnE7/3HhXvMK6YwxadS3gNpZ
dwiKXv97B4FVyNyl48sL8CYDWmlRcqmJSKn2qT+XQvVGLJo8adNy9vmjTTBLMA4G
A1UdDwEB/wQEAwIHgDAMBgNVHRMBAf8EAjAAMCsGA1UdIwQkMCKAIE/GEECU3zlV
yBJOiVtCbanzSzeWbPYj1znNyxDCRmBNMAoGCCqGSM49BAMCA0gAMEUCIQClLFp9
mNOG5mfV8b+QHD3h7BoLAyVSWjyGvTjdouXmwwIgF6O3QZ9y5VT5wEiqIz4gpftf
VzjGEYBCqQeZY618tcg=
-----END CERTIFICATE-----
2018-02-24 06:46:28.072 UTC [mgmt.go:149] [msp] GetLocalMSP -> DEBU 04f Returning existing local MSP
2018-02-24 06:46:28.072 UTC [mspimpl.go:391] [msp] Validate -> DEBU 050 MSP Org1MSP validating identity
2018-02-24 06:46:28.074 UTC [mgmt.go:149] [msp] GetLocalMSP -> DEBU 051 Returning existing local MSP
2018-02-24 06:46:28.074 UTC [mspimpl.go:407] [msp] DeserializeIdentity -> INFO 052 Obtaining identity
2018-02-24 06:46:28.074 UTC [identities.go:42] [msp/identity] newIdentity -> DEBU 053 Creating identity instance for ID -----BEGIN CERTIFICATE-----
MIICGjCCAcCgAwIBAgIRAIm9LgLGuAZxcn/6thnwMD0wCgYIKoZIzj0EAwIwczEL
MAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBG
cmFuY2lzY28xGTAXBgNVBAoTEG9yZzEuZXhhbXBsZS5jb20xHDAaBgNVBAMTE2Nh
Lm9yZzEuZXhhbXBsZS5jb20wHhcNMTgwMjA3MDAxNTE0WhcNMjgwMjA1MDAxNTE0
WjBbMQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMN
U2FuIEZyYW5jaXNjbzEfMB0GA1UEAxMWcGVlcjAub3JnMS5leGFtcGxlLmNvbTBZ
MBMGByqGSM49AgEGCCqGSM49AwEHA0IABBuNYucbhnE7/3HhXvMK6YwxadS3gNpZ
dwiKXv97B4FVyNyl48sL8CYDWmlRcqmJSKn2qT+XQvVGLJo8adNy9vmjTTBLMA4G
A1UdDwEB/wQEAwIHgDAMBgNVHRMBAf8EAjAAMCsGA1UdIwQkMCKAIE/GEECU3zlV
yBJOiVtCbanzSzeWbPYj1znNyxDCRmBNMAoGCCqGSM49BAMCA0gAMEUCIQClLFp9
mNOG5mfV8b+QHD3h7BoLAyVSWjyGvTjdouXmwwIgF6O3QZ9y5VT5wEiqIz4gpftf
VzjGEYBCqQeZY618tcg=
-----END CERTIFICATE-----
2018-02-24 06:46:28.074 UTC [discovery_impl.go:134] [gossip/discovery] NewDiscoveryService -> INFO 054 Started {peer0.org1.example.com:7051 [] [247 212 74 233 214 221 137 240 5 4 5 188 1 58 210 40 15 201 157 222 27 224 197 253 80 28 99 229 129 151 119 252] peer0.org1.example.com:7051} incTime is 1519454788074505139
2018-02-24 06:46:28.074 UTC [gossip_impl.go:128] [gossip/gossip] NewGossipService -> INFO 055 Creating gossip service with self membership of {peer0.org1.example.com:7051 [] [247 212 74 233 214 221 137 240 5 4 5 188 1 58 210 40 15 201 157 222 27 224 197 253 80 28 99 229 129 151 119 252] peer0.org1.example.com:7051}
2018-02-24 06:46:28.074 UTC [mgmt.go:149] [msp] GetLocalMSP -> DEBU 056 Returning existing local MSP
2018-02-24 06:46:28.074 UTC [mspimpl.go:407] [msp] DeserializeIdentity -> INFO 057 Obtaining identity
2018-02-24 06:46:28.075 UTC [identities.go:42] [msp/identity] newIdentity -> DEBU 058 Creating identity instance for ID -----BEGIN CERTIFICATE-----
MIICGjCCAcCgAwIBAgIRAIm9LgLGuAZxcn/6thnwMD0wCgYIKoZIzj0EAwIwczEL
MAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBG
cmFuY2lzY28xGTAXBgNVBAoTEG9yZzEuZXhhbXBsZS5jb20xHDAaBgNVBAMTE2Nh
Lm9yZzEuZXhhbXBsZS5jb20wHhcNMTgwMjA3MDAxNTE0WhcNMjgwMjA1MDAxNTE0
WjBbMQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMN
U2FuIEZyYW5jaXNjbzEfMB0GA1UEAxMWcGVlcjAub3JnMS5leGFtcGxlLmNvbTBZ
MBMGByqGSM49AgEGCCqGSM49AwEHA0IABBuNYucbhnE7/3HhXvMK6YwxadS3gNpZ
dwiKXv97B4FVyNyl48sL8CYDWmlRcqmJSKn2qT+XQvVGLJo8adNy9vmjTTBLMA4G
A1UdDwEB/wQEAwIHgDAMBgNVHRMBAf8EAjAAMCsGA1UdIwQkMCKAIE/GEECU3zlV
yBJOiVtCbanzSzeWbPYj1znNyxDCRmBNMAoGCCqGSM49BAMCA0gAMEUCIQClLFp9
mNOG5mfV8b+QHD3h7BoLAyVSWjyGvTjdouXmwwIgF6O3QZ9y5VT5wEiqIz4gpftf
VzjGEYBCqQeZY618tcg=
-----END CERTIFICATE-----
2018-02-24 06:46:28.075 UTC [mgmt.go:149] [msp] GetLocalMSP -> DEBU 059 Returning existing local MSP
2018-02-24 06:46:28.075 UTC [mspimpl.go:391] [msp] Validate -> DEBU 05a MSP Org1MSP validating identity
2018-02-24 06:46:28.090 UTC [events.go:220] [eventhub_producer] start -> INFO 05b Event processor started
2018-02-24 06:46:28.090 UTC [discovery_impl.go:733] [gossip/discovery] periodicalSendAlive -> DEBU 05c Sleeping 5s
2018-02-24 06:46:28.098 UTC [discovery_impl.go:616] [gossip/discovery] periodicalReconnectToDead -> DEBU 05d Sleeping 25s
2018-02-24 06:46:28.099 UTC [mgmt.go:149] [msp] GetLocalMSP -> DEBU 05e Returning existing local MSP
2018-02-24 06:46:28.099 UTC [mspimpl.go:369] [msp] GetDefaultSigningIdentity -> DEBU 05f Obtaining default signing identity
2018-02-24 06:46:28.099 UTC [identities.go:208] [msp/identity] Sign -> DEBU 060 Sign: plaintext: 1801AA01B1060A20F7D44AE9D6DD89F0...0A2D2D2D2D2D454E44202D2D2D2D2D0A 
2018-02-24 06:46:28.099 UTC [identities.go:210] [msp/identity] Sign -> DEBU 061 Sign: digest: 4ADABA01E373998BE704A2766FA9A666B1B3F4E617DED116826EE155DD535C52 
2018-02-24 06:46:28.099 UTC [start.go:218] [nodeCmd] serve -> DEBU 062 [gerry] after Initialize gossip component
2018-02-24 06:46:28.099 UTC [start.go:221] [nodeCmd] serve -> DEBU 063 [gerry] before initSysCCs
2018-02-24 06:46:28.099 UTC [ccprovider.go:363] [ccprovider] NewCCContext -> DEBU 064 NewCCCC (chain=,chaincode=cscc,version=1.0.5,txid=f6773fbb-b591-4a32-931b-b3a104bed193,syscc=true,proposal=0x0,canname=cscc:1.0.5
2018-02-24 06:46:28.100 UTC [chaincode_support.go:441] [chaincode] launchAndWaitForRegister -> DEBU 065 chaincode cscc:1.0.5 is being launched
2018-02-24 06:46:28.100 UTC [chaincode_support.go:406] [chaincode] getArgsAndEnv -> DEBU 066 Executable is chaincode
2018-02-24 06:46:28.100 UTC [chaincode_support.go:407] [chaincode] getArgsAndEnv -> DEBU 067 Args [chaincode -peer.address=peer0.org1.example.com:7051]
2018-02-24 06:46:28.102 UTC [chaincode_support.go:464] [chaincode] launchAndWaitForRegister -> DEBU 068 start container: cscc:1.0.5(networkid:dev,peerid:peer0.org1.example.com)
2018-02-24 06:46:28.102 UTC [chaincode_support.go:465] [chaincode] launchAndWaitForRegister -> DEBU 069 start container with args: chaincode -peer.address=peer0.org1.example.com:7051
2018-02-24 06:46:28.102 UTC [chaincode_support.go:466] [chaincode] launchAndWaitForRegister -> DEBU 06a start container with env:
        CORE_CHAINCODE_ID_NAME=cscc:1.0.5
        CORE_PEER_TLS_ENABLED=false
        CORE_CHAINCODE_LOGGING_LEVEL=debug
        CORE_CHAINCODE_LOGGING_SHIM=debug
        CORE_CHAINCODE_LOGGING_FORMAT=%{color}%{time:2006-01-02 15:04:05.000 MST} [%{module}] %{shortfunc} -> %{level:.4s} %{id:03x}%{color:reset} %{message}
2018-02-24 06:46:28.102 UTC [controller.go:91] [container] lockContainer -> DEBU 06b waiting for container(cscc-1.0.5) lock
2018-02-24 06:46:28.102 UTC [controller.go:93] [container] lockContainer -> DEBU 06c got container (cscc-1.0.5) lock
2018-02-24 06:46:28.102 UTC [inproccontroller.go:79] [inproccontroller] getInstance -> DEBU 06d chaincode instance created for cscc-1.0.5
2018-02-24 06:46:28.102 UTC [controller.go:104] [container] unlockContainer -> DEBU 06e container lock deleted(cscc-1.0.5)
2018-02-24 06:46:28.102 UTC [gossip_impl.go:331] [gossip/gossip] start -> INFO 06f Gossip instance peer0.org1.example.com:7051 started
2018-02-24 06:46:28.102 UTC [discovery_impl.go:153] [gossip/discovery] Connect -> DEBU 070 Skipping connecting to myself
2018-02-24 06:46:28.102 UTC [inproccontroller.go:131] [inproccontroller] func2 -> DEBU 071 chaincode-support started for  cscc-1.0.5
2018-02-24 06:46:28.102 UTC [handler.go:397] [chaincode] HandleChaincodeStream -> DEBU 072 Current context deadline = 0001-01-01 00:00:00 +0000 UTC, ok = false
2018-02-24 06:46:28.102 UTC [mgmt.go:149] [msp] GetLocalMSP -> DEBU 073 Returning existing local MSP
2018-02-24 06:46:28.102 UTC [gossip_impl.go:303] [gossip/gossip] syncDiscovery -> DEBU 074 Entering discovery sync with interval 4s
2018-02-24 06:46:28.102 UTC [mgmt.go:149] [msp] GetLocalMSP -> DEBU 075 Returning existing local MSP
2018-02-24 06:46:28.102 UTC [mspimpl.go:369] [msp] GetDefaultSigningIdentity -> DEBU 076 Obtaining default signing identity
2018-02-24 06:46:28.102 UTC [identities.go:208] [msp/identity] Sign -> DEBU 077 Sign: plaintext: 18012ADE060A3F0A1B70656572302E6F...0A2D2D2D2D2D454E44202D2D2D2D2D0A 
2018-02-24 06:46:28.102 UTC [identities.go:210] [msp/identity] Sign -> DEBU 078 Sign: digest: CC8DE0872E083C722C318A13630A23E43CF9F4EFA7E9FFA7FA01559F74143BB7 
2018-02-24 06:46:28.103 UTC [mgmt.go:149] [msp] GetLocalMSP -> DEBU 079 Returning existing local MSP
2018-02-24 06:46:28.103 UTC [mspimpl.go:369] [msp] GetDefaultSigningIdentity -> DEBU 07a Obtaining default signing identity
2018-02-24 06:46:28.103 UTC [identities.go:206] [msp/identity] Sign -> DEBU 07b Sign: plaintext: 0A1B70656572302E6F7267312E6578616D706C652E636F6D3A37303531 
2018-02-24 06:46:28.103 UTC [identities.go:210] [msp/identity] Sign -> DEBU 07c Sign: digest: A785FC0B61B5045A71C0FA6B50D12EB1E6F0CDB152C6F79E6575060F54022160 
2018-02-24 06:46:28.103 UTC [inproccontroller.go:113] [inproccontroller] func1 -> DEBU 07d chaincode started for cscc-1.0.5
2018-02-24 06:46:28.103 UTC [chaincode.go:199] [shim] StartInProc -> DEBU 07e in proc [chaincode -peer.address=peer0.org1.example.com:7051]
2018-02-24 06:46:28.103 UTC [chaincode.go:214] [shim] StartInProc -> DEBU 07f starting chat with peer using name=cscc:1.0.5
2018-02-24 06:46:28.103 UTC [chaincode.go:252] [shim] chatWithPeer -> DEBU 080 Registering.. sending REGISTER
2018-02-24 06:46:28.103 UTC [handler.go:336] [chaincode] processStream -> DEBU 081 []Received message REGISTER from shim
2018-02-24 06:46:28.115 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 082 []Fabric side Handling ChaincodeMessage of type: REGISTER in state created
2018-02-24 06:46:28.115 UTC [handler.go:510] [chaincode] beforeRegisterEvent -> DEBU 083 Received REGISTER in state created
2018-02-24 06:46:28.115 UTC [chaincode_support.go:291] [chaincode] registerHandler -> DEBU 084 registered handler complete for chaincode cscc:1.0.5
2018-02-24 06:46:28.115 UTC [handler.go:536] [chaincode] beforeRegisterEvent -> DEBU 085 Got REGISTER for chaincodeID = name:"cscc:1.0.5" , sending back REGISTERED
2018-02-24 06:46:28.116 UTC [handler.go:484] [chaincode] notifyDuringStartup -> DEBU 086 Notifying during startup
2018-02-24 06:46:28.116 UTC [chaincode.go:297] [shim] func1 -> DEBU 087 []Received message REGISTERED from shim
2018-02-24 06:46:28.116 UTC [handler.go:778] [shim] handleMessage -> DEBU 088 []Handling ChaincodeMessage of type: REGISTERED(state:created)
2018-02-24 06:46:28.117 UTC [handler.go:199] [shim] beforeRegistered -> DEBU 089 Received REGISTERED, ready for invocations
2018-02-24 06:46:28.117 UTC [chaincode_support.go:452] [chaincode] func1 -> DEBU 08a chaincode cscc:1.0.5 launch seq completed
2018-02-24 06:46:28.118 UTC [handler.go:1371] [chaincode] ready -> DEBU 08b sending READY
2018-02-24 06:46:28.118 UTC [handler.go:1350] [chaincode] setChaincodeProposal -> DEBU 08c Setting chaincode proposal context...
2018-02-24 06:46:28.118 UTC [handler.go:357] [chaincode] processStream -> DEBU 08d [f6773fbb]Move state message READY
2018-02-24 06:46:28.118 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 08e [f6773fbb]Fabric side Handling ChaincodeMessage of type: READY in state established
2018-02-24 06:46:28.118 UTC [handler.go:1332] [chaincode] enterReadyState -> DEBU 08f [f6773fbb]Entered state ready
2018-02-24 06:46:28.118 UTC [handler.go:551] [chaincode] notify -> DEBU 090 notifying Txid:f6773fbb-b591-4a32-931b-b3a104bed193
2018-02-24 06:46:28.118 UTC [handler.go:377] [chaincode] processStream -> DEBU 091 [f6773fbb]sending state message READY
2018-02-24 06:46:28.118 UTC [chaincode.go:297] [shim] func1 -> DEBU 092 [f6773fbb]Received message READY from shim
2018-02-24 06:46:28.118 UTC [handler.go:778] [shim] handleMessage -> DEBU 093 [f6773fbb]Handling ChaincodeMessage of type: READY(state:established)
2018-02-24 06:46:28.118 UTC [chaincode_support.go:681] [chaincode] Launch -> DEBU 094 sending init completed
2018-02-24 06:46:28.118 UTC [chaincode_support.go:684] [chaincode] Launch -> DEBU 095 LaunchChaincode complete
2018-02-24 06:46:28.118 UTC [chaincode_support.go:720] [chaincode] Execute -> DEBU 096 Entry
2018-02-24 06:46:28.118 UTC [chaincode_support.go:723] [chaincode] Execute -> DEBU 097 chaincode canonical name: cscc:1.0.5
2018-02-24 06:46:28.118 UTC [handler.go:1436] [chaincode] sendExecuteMessage -> DEBU 098 [f6773fbb]Inside sendExecuteMessage. Message INIT
2018-02-24 06:46:28.118 UTC [handler.go:1350] [chaincode] setChaincodeProposal -> DEBU 099 Setting chaincode proposal context...
2018-02-24 06:46:28.118 UTC [handler.go:1444] [chaincode] sendExecuteMessage -> DEBU 09a [f6773fbb]sendExecuteMsg trigger event INIT
2018-02-24 06:46:28.118 UTC [handler.go:357] [chaincode] processStream -> DEBU 09b [f6773fbb]Move state message INIT
2018-02-24 06:46:28.118 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 09c [f6773fbb]Fabric side Handling ChaincodeMessage of type: INIT in state ready
2018-02-24 06:46:28.118 UTC [handler.go:1417] [chaincode] filterError -> DEBU 09d Ignoring NoTransitionError: no transition
2018-02-24 06:46:28.118 UTC [handler.go:377] [chaincode] processStream -> DEBU 09e [f6773fbb]sending state message INIT
2018-02-24 06:46:28.118 UTC [chaincode.go:297] [shim] func1 -> DEBU 09f [f6773fbb]Received message INIT from shim
2018-02-24 06:46:28.118 UTC [handler.go:778] [shim] handleMessage -> DEBU 0a0 [f6773fbb]Handling ChaincodeMessage of type: INIT(state:ready)
2018-02-24 06:46:28.118 UTC [handler.go:265] [shim] beforeInit -> DEBU 0a1 Entered state ready
2018-02-24 06:46:28.118 UTC [handler.go:271] [shim] beforeInit -> DEBU 0a2 [f6773fbb]Received INIT, initializing chaincode
2018-02-24 06:46:28.118 UTC [configure.go:62] [cscc] Init -> INFO 0a3 Init CSCC
2018-02-24 06:46:28.118 UTC [mgmt.go:149] [msp] GetLocalMSP -> DEBU 0a4 Returning existing local MSP
2018-02-24 06:46:28.118 UTC [handler.go:243] [shim] func1 -> DEBU 0a5 [f6773fbb]Init get response status: 200
2018-02-24 06:46:28.119 UTC [handler.go:259] [shim] func1 -> DEBU 0a6 [f6773fbb]Init succeeded. Sending COMPLETED
2018-02-24 06:46:28.119 UTC [chaincode.go:304] [shim] func1 -> DEBU 0a7 [f6773fbb]Move state message COMPLETED
2018-02-24 06:46:28.119 UTC [handler.go:778] [shim] handleMessage -> DEBU 0a8 [f6773fbb]Handling ChaincodeMessage of type: COMPLETED(state:ready)
2018-02-24 06:46:28.119 UTC [chaincode.go:320] [shim] func1 -> DEBU 0a9 [f6773fbb]send state message COMPLETED
2018-02-24 06:46:28.119 UTC [handler.go:336] [chaincode] processStream -> DEBU 0aa [f6773fbb]Received message COMPLETED from shim
2018-02-24 06:46:28.119 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 0ab [f6773fbb]Fabric side Handling ChaincodeMessage of type: COMPLETED in state ready
2018-02-24 06:46:28.119 UTC [handler.go:1392] [chaincode] HandleMessage -> DEBU 0ac [f6773fbb-b591-4a32-931b-b3a104bed193]HandleMessage- COMPLETED. Notify
2018-02-24 06:46:28.119 UTC [handler.go:551] [chaincode] notify -> DEBU 0ad notifying Txid:f6773fbb-b591-4a32-931b-b3a104bed193
2018-02-24 06:46:28.119 UTC [chaincode_support.go:751] [chaincode] Execute -> DEBU 0ae Exit
2018-02-24 06:46:28.119 UTC [sysccapi.go:137] [sccapi] deploySysCC -> INFO 0af system chaincode cscc/(github.com/hyperledger/fabric/core/scc/cscc) deployed
2018-02-24 06:46:28.119 UTC [ccprovider.go:363] [ccprovider] NewCCContext -> DEBU 0b0 NewCCCC (chain=,chaincode=lscc,version=1.0.5,txid=1311e88f-8631-45ae-b49a-26216a55eb2a,syscc=true,proposal=0x0,canname=lscc:1.0.5
2018-02-24 06:46:28.119 UTC [chaincode_support.go:441] [chaincode] launchAndWaitForRegister -> DEBU 0b1 chaincode lscc:1.0.5 is being launched
2018-02-24 06:46:28.119 UTC [chaincode_support.go:406] [chaincode] getArgsAndEnv -> DEBU 0b2 Executable is chaincode
2018-02-24 06:46:28.119 UTC [chaincode_support.go:407] [chaincode] getArgsAndEnv -> DEBU 0b3 Args [chaincode -peer.address=peer0.org1.example.com:7051]
2018-02-24 06:46:28.119 UTC [chaincode_support.go:464] [chaincode] launchAndWaitForRegister -> DEBU 0b4 start container: lscc:1.0.5(networkid:dev,peerid:peer0.org1.example.com)
2018-02-24 06:46:28.119 UTC [chaincode_support.go:465] [chaincode] launchAndWaitForRegister -> DEBU 0b5 start container with args: chaincode -peer.address=peer0.org1.example.com:7051
2018-02-24 06:46:28.119 UTC [chaincode_support.go:466] [chaincode] launchAndWaitForRegister -> DEBU 0b6 start container with env:
        CORE_CHAINCODE_ID_NAME=lscc:1.0.5
        CORE_PEER_TLS_ENABLED=false
        CORE_CHAINCODE_LOGGING_LEVEL=debug
        CORE_CHAINCODE_LOGGING_SHIM=debug
        CORE_CHAINCODE_LOGGING_FORMAT=%{color}%{time:2006-01-02 15:04:05.000 MST} [%{module}] %{shortfunc} -> %{level:.4s} %{id:03x}%{color:reset} %{message}
2018-02-24 06:46:28.119 UTC [controller.go:91] [container] lockContainer -> DEBU 0b7 waiting for container(lscc-1.0.5) lock
2018-02-24 06:46:28.119 UTC [controller.go:93] [container] lockContainer -> DEBU 0b8 got container (lscc-1.0.5) lock
2018-02-24 06:46:28.119 UTC [inproccontroller.go:79] [inproccontroller] getInstance -> DEBU 0b9 chaincode instance created for lscc-1.0.5
2018-02-24 06:46:28.119 UTC [controller.go:104] [container] unlockContainer -> DEBU 0ba container lock deleted(lscc-1.0.5)
2018-02-24 06:46:28.119 UTC [inproccontroller.go:131] [inproccontroller] func2 -> DEBU 0bb chaincode-support started for  lscc-1.0.5
2018-02-24 06:46:28.119 UTC [handler.go:397] [chaincode] HandleChaincodeStream -> DEBU 0bc Current context deadline = 0001-01-01 00:00:00 +0000 UTC, ok = false
2018-02-24 06:46:28.120 UTC [mgmt.go:149] [msp] GetLocalMSP -> DEBU 0bd Returning existing local MSP
2018-02-24 06:46:28.120 UTC [inproccontroller.go:113] [inproccontroller] func1 -> DEBU 0be chaincode started for lscc-1.0.5
2018-02-24 06:46:28.120 UTC [chaincode.go:199] [shim] StartInProc -> DEBU 0bf in proc [chaincode -peer.address=peer0.org1.example.com:7051]
2018-02-24 06:46:28.120 UTC [chaincode.go:214] [shim] StartInProc -> DEBU 0c0 starting chat with peer using name=lscc:1.0.5
2018-02-24 06:46:28.120 UTC [chaincode.go:252] [shim] chatWithPeer -> DEBU 0c1 Registering.. sending REGISTER
2018-02-24 06:46:28.120 UTC [handler.go:336] [chaincode] processStream -> DEBU 0c2 []Received message REGISTER from shim
2018-02-24 06:46:28.120 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 0c3 []Fabric side Handling ChaincodeMessage of type: REGISTER in state created
2018-02-24 06:46:28.120 UTC [handler.go:510] [chaincode] beforeRegisterEvent -> DEBU 0c4 Received REGISTER in state created
2018-02-24 06:46:28.120 UTC [chaincode_support.go:291] [chaincode] registerHandler -> DEBU 0c5 registered handler complete for chaincode lscc:1.0.5
2018-02-24 06:46:28.120 UTC [handler.go:536] [chaincode] beforeRegisterEvent -> DEBU 0c6 Got REGISTER for chaincodeID = name:"lscc:1.0.5" , sending back REGISTERED
2018-02-24 06:46:28.120 UTC [handler.go:484] [chaincode] notifyDuringStartup -> DEBU 0c7 Notifying during startup
2018-02-24 06:46:28.120 UTC [chaincode.go:297] [shim] func1 -> DEBU 0c8 []Received message REGISTERED from shim
2018-02-24 06:46:28.120 UTC [handler.go:778] [shim] handleMessage -> DEBU 0c9 []Handling ChaincodeMessage of type: REGISTERED(state:created)
2018-02-24 06:46:28.121 UTC [handler.go:199] [shim] beforeRegistered -> DEBU 0ca Received REGISTERED, ready for invocations
2018-02-24 06:46:28.121 UTC [chaincode_support.go:452] [chaincode] func1 -> DEBU 0cb chaincode lscc:1.0.5 launch seq completed
2018-02-24 06:46:28.121 UTC [handler.go:1371] [chaincode] ready -> DEBU 0cc sending READY
2018-02-24 06:46:28.121 UTC [handler.go:1350] [chaincode] setChaincodeProposal -> DEBU 0cd Setting chaincode proposal context...
2018-02-24 06:46:28.122 UTC [handler.go:357] [chaincode] processStream -> DEBU 0ce [1311e88f]Move state message READY
2018-02-24 06:46:28.122 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 0cf [1311e88f]Fabric side Handling ChaincodeMessage of type: READY in state established
2018-02-24 06:46:28.122 UTC [handler.go:1332] [chaincode] enterReadyState -> DEBU 0d0 [1311e88f]Entered state ready
2018-02-24 06:46:28.122 UTC [handler.go:551] [chaincode] notify -> DEBU 0d1 notifying Txid:1311e88f-8631-45ae-b49a-26216a55eb2a
2018-02-24 06:46:28.122 UTC [handler.go:377] [chaincode] processStream -> DEBU 0d2 [1311e88f]sending state message READY
2018-02-24 06:46:28.122 UTC [chaincode.go:297] [shim] func1 -> DEBU 0d3 [1311e88f]Received message READY from shim
2018-02-24 06:46:28.122 UTC [handler.go:778] [shim] handleMessage -> DEBU 0d4 [1311e88f]Handling ChaincodeMessage of type: READY(state:established)
2018-02-24 06:46:28.122 UTC [chaincode_support.go:681] [chaincode] Launch -> DEBU 0d5 sending init completed
2018-02-24 06:46:28.122 UTC [chaincode_support.go:684] [chaincode] Launch -> DEBU 0d6 LaunchChaincode complete
2018-02-24 06:46:28.122 UTC [chaincode_support.go:720] [chaincode] Execute -> DEBU 0d7 Entry
2018-02-24 06:46:28.122 UTC [chaincode_support.go:723] [chaincode] Execute -> DEBU 0d8 chaincode canonical name: lscc:1.0.5
2018-02-24 06:46:28.122 UTC [handler.go:1436] [chaincode] sendExecuteMessage -> DEBU 0d9 [1311e88f]Inside sendExecuteMessage. Message INIT
2018-02-24 06:46:28.122 UTC [handler.go:1350] [chaincode] setChaincodeProposal -> DEBU 0da Setting chaincode proposal context...
2018-02-24 06:46:28.122 UTC [handler.go:1444] [chaincode] sendExecuteMessage -> DEBU 0db [1311e88f]sendExecuteMsg trigger event INIT
2018-02-24 06:46:28.123 UTC [handler.go:357] [chaincode] processStream -> DEBU 0dc [1311e88f]Move state message INIT
2018-02-24 06:46:28.123 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 0dd [1311e88f]Fabric side Handling ChaincodeMessage of type: INIT in state ready
2018-02-24 06:46:28.123 UTC [handler.go:1417] [chaincode] filterError -> DEBU 0de Ignoring NoTransitionError: no transition
2018-02-24 06:46:28.123 UTC [handler.go:377] [chaincode] processStream -> DEBU 0df [1311e88f]sending state message INIT
2018-02-24 06:46:28.123 UTC [chaincode.go:297] [shim] func1 -> DEBU 0e0 [1311e88f]Received message INIT from shim
2018-02-24 06:46:28.123 UTC [handler.go:778] [shim] handleMessage -> DEBU 0e1 [1311e88f]Handling ChaincodeMessage of type: INIT(state:ready)
2018-02-24 06:46:28.123 UTC [handler.go:265] [shim] beforeInit -> DEBU 0e2 Entered state ready
2018-02-24 06:46:28.123 UTC [handler.go:271] [shim] beforeInit -> DEBU 0e3 [1311e88f]Received INIT, initializing chaincode
2018-02-24 06:46:28.123 UTC [mgmt.go:149] [msp] GetLocalMSP -> DEBU 0e4 Returning existing local MSP
2018-02-24 06:46:28.123 UTC [handler.go:243] [shim] func1 -> DEBU 0e5 [1311e88f]Init get response status: 200
2018-02-24 06:46:28.123 UTC [handler.go:259] [shim] func1 -> DEBU 0e6 [1311e88f]Init succeeded. Sending COMPLETED
2018-02-24 06:46:28.123 UTC [chaincode.go:304] [shim] func1 -> DEBU 0e7 [1311e88f]Move state message COMPLETED
2018-02-24 06:46:28.123 UTC [handler.go:778] [shim] handleMessage -> DEBU 0e8 [1311e88f]Handling ChaincodeMessage of type: COMPLETED(state:ready)
2018-02-24 06:46:28.123 UTC [chaincode.go:320] [shim] func1 -> DEBU 0e9 [1311e88f]send state message COMPLETED
2018-02-24 06:46:28.124 UTC [handler.go:336] [chaincode] processStream -> DEBU 0ea [1311e88f]Received message COMPLETED from shim
2018-02-24 06:46:28.124 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 0eb [1311e88f]Fabric side Handling ChaincodeMessage of type: COMPLETED in state ready
2018-02-24 06:46:28.124 UTC [handler.go:1392] [chaincode] HandleMessage -> DEBU 0ec [1311e88f-8631-45ae-b49a-26216a55eb2a]HandleMessage- COMPLETED. Notify
2018-02-24 06:46:28.124 UTC [handler.go:551] [chaincode] notify -> DEBU 0ed notifying Txid:1311e88f-8631-45ae-b49a-26216a55eb2a
2018-02-24 06:46:28.125 UTC [chaincode_support.go:751] [chaincode] Execute -> DEBU 0ee Exit
2018-02-24 06:46:28.125 UTC [sysccapi.go:137] [sccapi] deploySysCC -> INFO 0ef system chaincode lscc/(github.com/hyperledger/fabric/core/scc/lscc) deployed
2018-02-24 06:46:28.126 UTC [ccprovider.go:363] [ccprovider] NewCCContext -> DEBU 0f0 NewCCCC (chain=,chaincode=escc,version=1.0.5,txid=5e59eba4-ea9f-4233-80ed-ee211d4284f2,syscc=true,proposal=0x0,canname=escc:1.0.5
2018-02-24 06:46:28.127 UTC [chaincode_support.go:441] [chaincode] launchAndWaitForRegister -> DEBU 0f1 chaincode escc:1.0.5 is being launched
2018-02-24 06:46:28.127 UTC [chaincode_support.go:406] [chaincode] getArgsAndEnv -> DEBU 0f2 Executable is chaincode
2018-02-24 06:46:28.127 UTC [chaincode_support.go:407] [chaincode] getArgsAndEnv -> DEBU 0f3 Args [chaincode -peer.address=peer0.org1.example.com:7051]
2018-02-24 06:46:28.127 UTC [chaincode_support.go:464] [chaincode] launchAndWaitForRegister -> DEBU 0f4 start container: escc:1.0.5(networkid:dev,peerid:peer0.org1.example.com)
2018-02-24 06:46:28.127 UTC [chaincode_support.go:465] [chaincode] launchAndWaitForRegister -> DEBU 0f5 start container with args: chaincode -peer.address=peer0.org1.example.com:7051
2018-02-24 06:46:28.127 UTC [chaincode_support.go:466] [chaincode] launchAndWaitForRegister -> DEBU 0f6 start container with env:
        CORE_CHAINCODE_ID_NAME=escc:1.0.5
        CORE_PEER_TLS_ENABLED=false
        CORE_CHAINCODE_LOGGING_LEVEL=debug
        CORE_CHAINCODE_LOGGING_SHIM=debug
        CORE_CHAINCODE_LOGGING_FORMAT=%{color}%{time:2006-01-02 15:04:05.000 MST} [%{module}] %{shortfunc} -> %{level:.4s} %{id:03x}%{color:reset} %{message}
2018-02-24 06:46:28.127 UTC [controller.go:91] [container] lockContainer -> DEBU 0f7 waiting for container(escc-1.0.5) lock
2018-02-24 06:46:28.127 UTC [controller.go:93] [container] lockContainer -> DEBU 0f8 got container (escc-1.0.5) lock
2018-02-24 06:46:28.127 UTC [inproccontroller.go:79] [inproccontroller] getInstance -> DEBU 0f9 chaincode instance created for escc-1.0.5
2018-02-24 06:46:28.127 UTC [controller.go:104] [container] unlockContainer -> DEBU 0fa container lock deleted(escc-1.0.5)
2018-02-24 06:46:28.127 UTC [inproccontroller.go:131] [inproccontroller] func2 -> DEBU 0fb chaincode-support started for  escc-1.0.5
2018-02-24 06:46:28.127 UTC [handler.go:397] [chaincode] HandleChaincodeStream -> DEBU 0fc Current context deadline = 0001-01-01 00:00:00 +0000 UTC, ok = false
2018-02-24 06:46:28.128 UTC [mgmt.go:149] [msp] GetLocalMSP -> DEBU 0fd Returning existing local MSP
2018-02-24 06:46:28.128 UTC [inproccontroller.go:113] [inproccontroller] func1 -> DEBU 0fe chaincode started for escc-1.0.5
2018-02-24 06:46:28.128 UTC [chaincode.go:199] [shim] StartInProc -> DEBU 0ff in proc [chaincode -peer.address=peer0.org1.example.com:7051]
2018-02-24 06:46:28.129 UTC [chaincode.go:214] [shim] StartInProc -> DEBU 100 starting chat with peer using name=escc:1.0.5
2018-02-24 06:46:28.129 UTC [chaincode.go:252] [shim] chatWithPeer -> DEBU 101 Registering.. sending REGISTER
2018-02-24 06:46:28.129 UTC [handler.go:336] [chaincode] processStream -> DEBU 102 []Received message REGISTER from shim
2018-02-24 06:46:28.130 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 103 []Fabric side Handling ChaincodeMessage of type: REGISTER in state created
2018-02-24 06:46:28.130 UTC [handler.go:510] [chaincode] beforeRegisterEvent -> DEBU 104 Received REGISTER in state created
2018-02-24 06:46:28.130 UTC [chaincode_support.go:291] [chaincode] registerHandler -> DEBU 105 registered handler complete for chaincode escc:1.0.5
2018-02-24 06:46:28.130 UTC [handler.go:536] [chaincode] beforeRegisterEvent -> DEBU 106 Got REGISTER for chaincodeID = name:"escc:1.0.5" , sending back REGISTERED
2018-02-24 06:46:28.130 UTC [handler.go:484] [chaincode] notifyDuringStartup -> DEBU 107 Notifying during startup
2018-02-24 06:46:28.131 UTC [chaincode.go:297] [shim] func1 -> DEBU 108 []Received message REGISTERED from shim
2018-02-24 06:46:28.131 UTC [handler.go:778] [shim] handleMessage -> DEBU 109 []Handling ChaincodeMessage of type: REGISTERED(state:created)
2018-02-24 06:46:28.131 UTC [handler.go:199] [shim] beforeRegistered -> DEBU 10a Received REGISTERED, ready for invocations
2018-02-24 06:46:28.131 UTC [chaincode_support.go:452] [chaincode] func1 -> DEBU 10b chaincode escc:1.0.5 launch seq completed
2018-02-24 06:46:28.131 UTC [handler.go:1371] [chaincode] ready -> DEBU 10c sending READY
2018-02-24 06:46:28.131 UTC [handler.go:1350] [chaincode] setChaincodeProposal -> DEBU 10d Setting chaincode proposal context...
2018-02-24 06:46:28.131 UTC [handler.go:357] [chaincode] processStream -> DEBU 10e [5e59eba4]Move state message READY
2018-02-24 06:46:28.131 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 10f [5e59eba4]Fabric side Handling ChaincodeMessage of type: READY in state established
2018-02-24 06:46:28.131 UTC [handler.go:1332] [chaincode] enterReadyState -> DEBU 110 [5e59eba4]Entered state ready
2018-02-24 06:46:28.131 UTC [handler.go:551] [chaincode] notify -> DEBU 111 notifying Txid:5e59eba4-ea9f-4233-80ed-ee211d4284f2
2018-02-24 06:46:28.131 UTC [handler.go:377] [chaincode] processStream -> DEBU 112 [5e59eba4]sending state message READY
2018-02-24 06:46:28.131 UTC [chaincode.go:297] [shim] func1 -> DEBU 113 [5e59eba4]Received message READY from shim
2018-02-24 06:46:28.132 UTC [handler.go:778] [shim] handleMessage -> DEBU 114 [5e59eba4]Handling ChaincodeMessage of type: READY(state:established)
2018-02-24 06:46:28.132 UTC [chaincode_support.go:681] [chaincode] Launch -> DEBU 115 sending init completed
2018-02-24 06:46:28.132 UTC [chaincode_support.go:684] [chaincode] Launch -> DEBU 116 LaunchChaincode complete
2018-02-24 06:46:28.132 UTC [chaincode_support.go:720] [chaincode] Execute -> DEBU 117 Entry
2018-02-24 06:46:28.132 UTC [chaincode_support.go:723] [chaincode] Execute -> DEBU 118 chaincode canonical name: escc:1.0.5
2018-02-24 06:46:28.133 UTC [handler.go:1436] [chaincode] sendExecuteMessage -> DEBU 119 [5e59eba4]Inside sendExecuteMessage. Message INIT
2018-02-24 06:46:28.133 UTC [handler.go:1350] [chaincode] setChaincodeProposal -> DEBU 11a Setting chaincode proposal context...
2018-02-24 06:46:28.133 UTC [handler.go:1444] [chaincode] sendExecuteMessage -> DEBU 11b [5e59eba4]sendExecuteMsg trigger event INIT
2018-02-24 06:46:28.134 UTC [handler.go:357] [chaincode] processStream -> DEBU 11c [5e59eba4]Move state message INIT
2018-02-24 06:46:28.134 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 11d [5e59eba4]Fabric side Handling ChaincodeMessage of type: INIT in state ready
2018-02-24 06:46:28.134 UTC [handler.go:1417] [chaincode] filterError -> DEBU 11e Ignoring NoTransitionError: no transition
2018-02-24 06:46:28.134 UTC [handler.go:377] [chaincode] processStream -> DEBU 11f [5e59eba4]sending state message INIT
2018-02-24 06:46:28.134 UTC [chaincode.go:297] [shim] func1 -> DEBU 120 [5e59eba4]Received message INIT from shim
2018-02-24 06:46:28.134 UTC [handler.go:778] [shim] handleMessage -> DEBU 121 [5e59eba4]Handling ChaincodeMessage of type: INIT(state:ready)
2018-02-24 06:46:28.134 UTC [handler.go:265] [shim] beforeInit -> DEBU 122 Entered state ready
2018-02-24 06:46:28.134 UTC [handler.go:271] [shim] beforeInit -> DEBU 123 [5e59eba4]Received INIT, initializing chaincode
2018-02-24 06:46:28.134 UTC [endorser_onevalidsignature.go:40] [escc] Init -> INFO 124 Successfully initialized ESCC
2018-02-24 06:46:28.134 UTC [handler.go:243] [shim] func1 -> DEBU 125 [5e59eba4]Init get response status: 200
2018-02-24 06:46:28.134 UTC [handler.go:259] [shim] func1 -> DEBU 126 [5e59eba4]Init succeeded. Sending COMPLETED
2018-02-24 06:46:28.134 UTC [chaincode.go:304] [shim] func1 -> DEBU 127 [5e59eba4]Move state message COMPLETED
2018-02-24 06:46:28.135 UTC [handler.go:778] [shim] handleMessage -> DEBU 128 [5e59eba4]Handling ChaincodeMessage of type: COMPLETED(state:ready)
2018-02-24 06:46:28.135 UTC [chaincode.go:320] [shim] func1 -> DEBU 129 [5e59eba4]send state message COMPLETED
2018-02-24 06:46:28.135 UTC [handler.go:336] [chaincode] processStream -> DEBU 12a [5e59eba4]Received message COMPLETED from shim
2018-02-24 06:46:28.135 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 12b [5e59eba4]Fabric side Handling ChaincodeMessage of type: COMPLETED in state ready
2018-02-24 06:46:28.135 UTC [handler.go:1392] [chaincode] HandleMessage -> DEBU 12c [5e59eba4-ea9f-4233-80ed-ee211d4284f2]HandleMessage- COMPLETED. Notify
2018-02-24 06:46:28.135 UTC [handler.go:551] [chaincode] notify -> DEBU 12d notifying Txid:5e59eba4-ea9f-4233-80ed-ee211d4284f2
2018-02-24 06:46:28.135 UTC [chaincode_support.go:751] [chaincode] Execute -> DEBU 12e Exit
2018-02-24 06:46:28.135 UTC [sysccapi.go:137] [sccapi] deploySysCC -> INFO 12f system chaincode escc/(github.com/hyperledger/fabric/core/scc/escc) deployed
2018-02-24 06:46:28.136 UTC [ccprovider.go:363] [ccprovider] NewCCContext -> DEBU 130 NewCCCC (chain=,chaincode=vscc,version=1.0.5,txid=4ca1a077-efc1-4a6f-834e-9a2909afb472,syscc=true,proposal=0x0,canname=vscc:1.0.5
2018-02-24 06:46:28.138 UTC [chaincode_support.go:441] [chaincode] launchAndWaitForRegister -> DEBU 131 chaincode vscc:1.0.5 is being launched
2018-02-24 06:46:28.138 UTC [chaincode_support.go:406] [chaincode] getArgsAndEnv -> DEBU 132 Executable is chaincode
2018-02-24 06:46:28.139 UTC [chaincode_support.go:407] [chaincode] getArgsAndEnv -> DEBU 133 Args [chaincode -peer.address=peer0.org1.example.com:7051]
2018-02-24 06:46:28.139 UTC [chaincode_support.go:464] [chaincode] launchAndWaitForRegister -> DEBU 134 start container: vscc:1.0.5(networkid:dev,peerid:peer0.org1.example.com)
2018-02-24 06:46:28.139 UTC [chaincode_support.go:465] [chaincode] launchAndWaitForRegister -> DEBU 135 start container with args: chaincode -peer.address=peer0.org1.example.com:7051
2018-02-24 06:46:28.139 UTC [chaincode_support.go:466] [chaincode] launchAndWaitForRegister -> DEBU 136 start container with env:
        CORE_CHAINCODE_ID_NAME=vscc:1.0.5
        CORE_PEER_TLS_ENABLED=false
        CORE_CHAINCODE_LOGGING_LEVEL=debug
        CORE_CHAINCODE_LOGGING_SHIM=debug
        CORE_CHAINCODE_LOGGING_FORMAT=%{color}%{time:2006-01-02 15:04:05.000 MST} [%{module}] %{shortfunc} -> %{level:.4s} %{id:03x}%{color:reset} %{message}
2018-02-24 06:46:28.139 UTC [controller.go:91] [container] lockContainer -> DEBU 137 waiting for container(vscc-1.0.5) lock
2018-02-24 06:46:28.139 UTC [controller.go:93] [container] lockContainer -> DEBU 138 got container (vscc-1.0.5) lock
2018-02-24 06:46:28.139 UTC [inproccontroller.go:79] [inproccontroller] getInstance -> DEBU 139 chaincode instance created for vscc-1.0.5
2018-02-24 06:46:28.139 UTC [controller.go:104] [container] unlockContainer -> DEBU 13a container lock deleted(vscc-1.0.5)
2018-02-24 06:46:28.139 UTC [inproccontroller.go:131] [inproccontroller] func2 -> DEBU 13b chaincode-support started for  vscc-1.0.5
2018-02-24 06:46:28.139 UTC [handler.go:397] [chaincode] HandleChaincodeStream -> DEBU 13c Current context deadline = 0001-01-01 00:00:00 +0000 UTC, ok = false
2018-02-24 06:46:28.140 UTC [mgmt.go:149] [msp] GetLocalMSP -> DEBU 13d Returning existing local MSP
2018-02-24 06:46:28.140 UTC [inproccontroller.go:113] [inproccontroller] func1 -> DEBU 13e chaincode started for vscc-1.0.5
2018-02-24 06:46:28.140 UTC [chaincode.go:199] [shim] StartInProc -> DEBU 13f in proc [chaincode -peer.address=peer0.org1.example.com:7051]
2018-02-24 06:46:28.140 UTC [chaincode.go:214] [shim] StartInProc -> DEBU 140 starting chat with peer using name=vscc:1.0.5
2018-02-24 06:46:28.140 UTC [chaincode.go:252] [shim] chatWithPeer -> DEBU 141 Registering.. sending REGISTER
2018-02-24 06:46:28.140 UTC [handler.go:336] [chaincode] processStream -> DEBU 142 []Received message REGISTER from shim
2018-02-24 06:46:28.140 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 143 []Fabric side Handling ChaincodeMessage of type: REGISTER in state created
2018-02-24 06:46:28.140 UTC [handler.go:510] [chaincode] beforeRegisterEvent -> DEBU 144 Received REGISTER in state created
2018-02-24 06:46:28.140 UTC [chaincode_support.go:291] [chaincode] registerHandler -> DEBU 145 registered handler complete for chaincode vscc:1.0.5
2018-02-24 06:46:28.140 UTC [handler.go:536] [chaincode] beforeRegisterEvent -> DEBU 146 Got REGISTER for chaincodeID = name:"vscc:1.0.5" , sending back REGISTERED
2018-02-24 06:46:28.141 UTC [handler.go:484] [chaincode] notifyDuringStartup -> DEBU 147 Notifying during startup
2018-02-24 06:46:28.141 UTC [chaincode.go:297] [shim] func1 -> DEBU 148 []Received message REGISTERED from shim
2018-02-24 06:46:28.141 UTC [handler.go:778] [shim] handleMessage -> DEBU 149 []Handling ChaincodeMessage of type: REGISTERED(state:created)
2018-02-24 06:46:28.141 UTC [handler.go:199] [shim] beforeRegistered -> DEBU 14a Received REGISTERED, ready for invocations
2018-02-24 06:46:28.141 UTC [chaincode_support.go:452] [chaincode] func1 -> DEBU 14b chaincode vscc:1.0.5 launch seq completed
2018-02-24 06:46:28.141 UTC [handler.go:1371] [chaincode] ready -> DEBU 14c sending READY
2018-02-24 06:46:28.141 UTC [handler.go:1350] [chaincode] setChaincodeProposal -> DEBU 14d Setting chaincode proposal context...
2018-02-24 06:46:28.141 UTC [handler.go:357] [chaincode] processStream -> DEBU 14e [4ca1a077]Move state message READY
2018-02-24 06:46:28.141 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 14f [4ca1a077]Fabric side Handling ChaincodeMessage of type: READY in state established
2018-02-24 06:46:28.141 UTC [handler.go:1332] [chaincode] enterReadyState -> DEBU 150 [4ca1a077]Entered state ready
2018-02-24 06:46:28.141 UTC [handler.go:551] [chaincode] notify -> DEBU 151 notifying Txid:4ca1a077-efc1-4a6f-834e-9a2909afb472
2018-02-24 06:46:28.141 UTC [handler.go:377] [chaincode] processStream -> DEBU 152 [4ca1a077]sending state message READY
2018-02-24 06:46:28.142 UTC [chaincode.go:297] [shim] func1 -> DEBU 153 [4ca1a077]Received message READY from shim
2018-02-24 06:46:28.142 UTC [handler.go:778] [shim] handleMessage -> DEBU 154 [4ca1a077]Handling ChaincodeMessage of type: READY(state:established)
2018-02-24 06:46:28.142 UTC [chaincode_support.go:681] [chaincode] Launch -> DEBU 155 sending init completed
2018-02-24 06:46:28.142 UTC [chaincode_support.go:684] [chaincode] Launch -> DEBU 156 LaunchChaincode complete
2018-02-24 06:46:28.142 UTC [chaincode_support.go:720] [chaincode] Execute -> DEBU 157 Entry
2018-02-24 06:46:28.142 UTC [chaincode_support.go:723] [chaincode] Execute -> DEBU 158 chaincode canonical name: vscc:1.0.5
2018-02-24 06:46:28.142 UTC [handler.go:1436] [chaincode] sendExecuteMessage -> DEBU 159 [4ca1a077]Inside sendExecuteMessage. Message INIT
2018-02-24 06:46:28.142 UTC [handler.go:1350] [chaincode] setChaincodeProposal -> DEBU 15a Setting chaincode proposal context...
2018-02-24 06:46:28.142 UTC [handler.go:1444] [chaincode] sendExecuteMessage -> DEBU 15b [4ca1a077]sendExecuteMsg trigger event INIT
2018-02-24 06:46:28.143 UTC [handler.go:357] [chaincode] processStream -> DEBU 15c [4ca1a077]Move state message INIT
2018-02-24 06:46:28.143 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 15d [4ca1a077]Fabric side Handling ChaincodeMessage of type: INIT in state ready
2018-02-24 06:46:28.143 UTC [handler.go:1417] [chaincode] filterError -> DEBU 15e Ignoring NoTransitionError: no transition
2018-02-24 06:46:28.143 UTC [handler.go:377] [chaincode] processStream -> DEBU 15f [4ca1a077]sending state message INIT
2018-02-24 06:46:28.143 UTC [chaincode.go:297] [shim] func1 -> DEBU 160 [4ca1a077]Received message INIT from shim
2018-02-24 06:46:28.147 UTC [handler.go:778] [shim] handleMessage -> DEBU 161 [4ca1a077]Handling ChaincodeMessage of type: INIT(state:ready)
2018-02-24 06:46:28.147 UTC [handler.go:265] [shim] beforeInit -> DEBU 162 Entered state ready
2018-02-24 06:46:28.147 UTC [handler.go:271] [shim] beforeInit -> DEBU 163 [4ca1a077]Received INIT, initializing chaincode
2018-02-24 06:46:28.147 UTC [handler.go:243] [shim] func1 -> DEBU 164 [4ca1a077]Init get response status: 200
2018-02-24 06:46:28.147 UTC [handler.go:259] [shim] func1 -> DEBU 165 [4ca1a077]Init succeeded. Sending COMPLETED
2018-02-24 06:46:28.147 UTC [chaincode.go:304] [shim] func1 -> DEBU 166 [4ca1a077]Move state message COMPLETED
2018-02-24 06:46:28.148 UTC [handler.go:778] [shim] handleMessage -> DEBU 167 [4ca1a077]Handling ChaincodeMessage of type: COMPLETED(state:ready)
2018-02-24 06:46:28.148 UTC [chaincode.go:320] [shim] func1 -> DEBU 168 [4ca1a077]send state message COMPLETED
2018-02-24 06:46:28.148 UTC [handler.go:336] [chaincode] processStream -> DEBU 169 [4ca1a077]Received message COMPLETED from shim
2018-02-24 06:46:28.148 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 16a [4ca1a077]Fabric side Handling ChaincodeMessage of type: COMPLETED in state ready
2018-02-24 06:46:28.148 UTC [handler.go:1392] [chaincode] HandleMessage -> DEBU 16b [4ca1a077-efc1-4a6f-834e-9a2909afb472]HandleMessage- COMPLETED. Notify
2018-02-24 06:46:28.148 UTC [handler.go:551] [chaincode] notify -> DEBU 16c notifying Txid:4ca1a077-efc1-4a6f-834e-9a2909afb472
2018-02-24 06:46:28.148 UTC [chaincode_support.go:751] [chaincode] Execute -> DEBU 16d Exit
2018-02-24 06:46:28.148 UTC [sysccapi.go:137] [sccapi] deploySysCC -> INFO 16e system chaincode vscc/(github.com/hyperledger/fabric/core/scc/vscc) deployed
2018-02-24 06:46:28.149 UTC [ccprovider.go:363] [ccprovider] NewCCContext -> DEBU 16f NewCCCC (chain=,chaincode=qscc,version=1.0.5,txid=c1a427b0-2e8f-4d6c-ac48-86ec466ce175,syscc=true,proposal=0x0,canname=qscc:1.0.5
2018-02-24 06:46:28.149 UTC [chaincode_support.go:441] [chaincode] launchAndWaitForRegister -> DEBU 170 chaincode qscc:1.0.5 is being launched
2018-02-24 06:46:28.149 UTC [chaincode_support.go:406] [chaincode] getArgsAndEnv -> DEBU 171 Executable is chaincode
2018-02-24 06:46:28.149 UTC [chaincode_support.go:407] [chaincode] getArgsAndEnv -> DEBU 172 Args [chaincode -peer.address=peer0.org1.example.com:7051]
2018-02-24 06:46:28.149 UTC [chaincode_support.go:464] [chaincode] launchAndWaitForRegister -> DEBU 173 start container: qscc:1.0.5(networkid:dev,peerid:peer0.org1.example.com)
2018-02-24 06:46:28.149 UTC [chaincode_support.go:465] [chaincode] launchAndWaitForRegister -> DEBU 174 start container with args: chaincode -peer.address=peer0.org1.example.com:7051
2018-02-24 06:46:28.149 UTC [chaincode_support.go:466] [chaincode] launchAndWaitForRegister -> DEBU 175 start container with env:
        CORE_CHAINCODE_ID_NAME=qscc:1.0.5
        CORE_PEER_TLS_ENABLED=false
        CORE_CHAINCODE_LOGGING_LEVEL=debug
        CORE_CHAINCODE_LOGGING_SHIM=debug
        CORE_CHAINCODE_LOGGING_FORMAT=%{color}%{time:2006-01-02 15:04:05.000 MST} [%{module}] %{shortfunc} -> %{level:.4s} %{id:03x}%{color:reset} %{message}
2018-02-24 06:46:28.150 UTC [controller.go:91] [container] lockContainer -> DEBU 176 waiting for container(qscc-1.0.5) lock
2018-02-24 06:46:28.150 UTC [controller.go:93] [container] lockContainer -> DEBU 177 got container (qscc-1.0.5) lock
2018-02-24 06:46:28.150 UTC [inproccontroller.go:79] [inproccontroller] getInstance -> DEBU 178 chaincode instance created for qscc-1.0.5
2018-02-24 06:46:28.150 UTC [controller.go:104] [container] unlockContainer -> DEBU 179 container lock deleted(qscc-1.0.5)
2018-02-24 06:46:28.150 UTC [inproccontroller.go:131] [inproccontroller] func2 -> DEBU 17a chaincode-support started for  qscc-1.0.5
2018-02-24 06:46:28.150 UTC [handler.go:397] [chaincode] HandleChaincodeStream -> DEBU 17b Current context deadline = 0001-01-01 00:00:00 +0000 UTC, ok = false
2018-02-24 06:46:28.150 UTC [mgmt.go:149] [msp] GetLocalMSP -> DEBU 17c Returning existing local MSP
2018-02-24 06:46:28.150 UTC [inproccontroller.go:113] [inproccontroller] func1 -> DEBU 17d chaincode started for qscc-1.0.5
2018-02-24 06:46:28.150 UTC [chaincode.go:199] [shim] StartInProc -> DEBU 17e in proc [chaincode -peer.address=peer0.org1.example.com:7051]
2018-02-24 06:46:28.151 UTC [chaincode.go:214] [shim] StartInProc -> DEBU 17f starting chat with peer using name=qscc:1.0.5
2018-02-24 06:46:28.151 UTC [chaincode.go:252] [shim] chatWithPeer -> DEBU 180 Registering.. sending REGISTER
2018-02-24 06:46:28.151 UTC [handler.go:336] [chaincode] processStream -> DEBU 181 []Received message REGISTER from shim
2018-02-24 06:46:28.153 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 182 []Fabric side Handling ChaincodeMessage of type: REGISTER in state created
2018-02-24 06:46:28.153 UTC [handler.go:510] [chaincode] beforeRegisterEvent -> DEBU 183 Received REGISTER in state created
2018-02-24 06:46:28.153 UTC [chaincode_support.go:291] [chaincode] registerHandler -> DEBU 184 registered handler complete for chaincode qscc:1.0.5
2018-02-24 06:46:28.153 UTC [handler.go:536] [chaincode] beforeRegisterEvent -> DEBU 185 Got REGISTER for chaincodeID = name:"qscc:1.0.5" , sending back REGISTERED
2018-02-24 06:46:28.153 UTC [handler.go:484] [chaincode] notifyDuringStartup -> DEBU 186 Notifying during startup
2018-02-24 06:46:28.153 UTC [chaincode.go:297] [shim] func1 -> DEBU 187 []Received message REGISTERED from shim
2018-02-24 06:46:28.153 UTC [handler.go:778] [shim] handleMessage -> DEBU 188 []Handling ChaincodeMessage of type: REGISTERED(state:created)
2018-02-24 06:46:28.153 UTC [handler.go:199] [shim] beforeRegistered -> DEBU 189 Received REGISTERED, ready for invocations
2018-02-24 06:46:28.153 UTC [chaincode_support.go:452] [chaincode] func1 -> DEBU 18a chaincode qscc:1.0.5 launch seq completed
2018-02-24 06:46:28.153 UTC [handler.go:1371] [chaincode] ready -> DEBU 18b sending READY
2018-02-24 06:46:28.154 UTC [handler.go:1350] [chaincode] setChaincodeProposal -> DEBU 18c Setting chaincode proposal context...
2018-02-24 06:46:28.154 UTC [handler.go:357] [chaincode] processStream -> DEBU 18d [c1a427b0]Move state message READY
2018-02-24 06:46:28.154 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 18e [c1a427b0]Fabric side Handling ChaincodeMessage of type: READY in state established
2018-02-24 06:46:28.154 UTC [handler.go:1332] [chaincode] enterReadyState -> DEBU 18f [c1a427b0]Entered state ready
2018-02-24 06:46:28.154 UTC [handler.go:551] [chaincode] notify -> DEBU 190 notifying Txid:c1a427b0-2e8f-4d6c-ac48-86ec466ce175
2018-02-24 06:46:28.154 UTC [handler.go:377] [chaincode] processStream -> DEBU 191 [c1a427b0]sending state message READY
2018-02-24 06:46:28.154 UTC [chaincode.go:297] [shim] func1 -> DEBU 192 [c1a427b0]Received message READY from shim
2018-02-24 06:46:28.154 UTC [handler.go:778] [shim] handleMessage -> DEBU 193 [c1a427b0]Handling ChaincodeMessage of type: READY(state:established)
2018-02-24 06:46:28.154 UTC [chaincode_support.go:681] [chaincode] Launch -> DEBU 194 sending init completed
2018-02-24 06:46:28.154 UTC [chaincode_support.go:684] [chaincode] Launch -> DEBU 195 LaunchChaincode complete
2018-02-24 06:46:28.154 UTC [chaincode_support.go:720] [chaincode] Execute -> DEBU 196 Entry
2018-02-24 06:46:28.154 UTC [chaincode_support.go:723] [chaincode] Execute -> DEBU 197 chaincode canonical name: qscc:1.0.5
2018-02-24 06:46:28.154 UTC [handler.go:1436] [chaincode] sendExecuteMessage -> DEBU 198 [c1a427b0]Inside sendExecuteMessage. Message INIT
2018-02-24 06:46:28.155 UTC [handler.go:1350] [chaincode] setChaincodeProposal -> DEBU 199 Setting chaincode proposal context...
2018-02-24 06:46:28.155 UTC [handler.go:1444] [chaincode] sendExecuteMessage -> DEBU 19a [c1a427b0]sendExecuteMsg trigger event INIT
2018-02-24 06:46:28.155 UTC [handler.go:357] [chaincode] processStream -> DEBU 19b [c1a427b0]Move state message INIT
2018-02-24 06:46:28.155 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 19c [c1a427b0]Fabric side Handling ChaincodeMessage of type: INIT in state ready
2018-02-24 06:46:28.155 UTC [handler.go:1417] [chaincode] filterError -> DEBU 19d Ignoring NoTransitionError: no transition
2018-02-24 06:46:28.155 UTC [handler.go:377] [chaincode] processStream -> DEBU 19e [c1a427b0]sending state message INIT
2018-02-24 06:46:28.155 UTC [chaincode.go:297] [shim] func1 -> DEBU 19f [c1a427b0]Received message INIT from shim
2018-02-24 06:46:28.155 UTC [handler.go:778] [shim] handleMessage -> DEBU 1a0 [c1a427b0]Handling ChaincodeMessage of type: INIT(state:ready)
2018-02-24 06:46:28.155 UTC [handler.go:265] [shim] beforeInit -> DEBU 1a1 Entered state ready
2018-02-24 06:46:28.155 UTC [handler.go:271] [shim] beforeInit -> DEBU 1a2 [c1a427b0]Received INIT, initializing chaincode
2018-02-24 06:46:28.155 UTC [query.go:59] [qscc] Init -> INFO 1a3 Init QSCC
2018-02-24 06:46:28.156 UTC [mgmt.go:149] [msp] GetLocalMSP -> DEBU 1a4 Returning existing local MSP
2018-02-24 06:46:28.156 UTC [handler.go:243] [shim] func1 -> DEBU 1a5 [c1a427b0]Init get response status: 200
2018-02-24 06:46:28.156 UTC [handler.go:259] [shim] func1 -> DEBU 1a6 [c1a427b0]Init succeeded. Sending COMPLETED
2018-02-24 06:46:28.157 UTC [chaincode.go:304] [shim] func1 -> DEBU 1a7 [c1a427b0]Move state message COMPLETED
2018-02-24 06:46:28.158 UTC [handler.go:778] [shim] handleMessage -> DEBU 1a8 [c1a427b0]Handling ChaincodeMessage of type: COMPLETED(state:ready)
2018-02-24 06:46:28.158 UTC [chaincode.go:320] [shim] func1 -> DEBU 1a9 [c1a427b0]send state message COMPLETED
2018-02-24 06:46:28.158 UTC [handler.go:336] [chaincode] processStream -> DEBU 1aa [c1a427b0]Received message COMPLETED from shim
2018-02-24 06:46:28.158 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 1ab [c1a427b0]Fabric side Handling ChaincodeMessage of type: COMPLETED in state ready
2018-02-24 06:46:28.158 UTC [handler.go:1392] [chaincode] HandleMessage -> DEBU 1ac [c1a427b0-2e8f-4d6c-ac48-86ec466ce175]HandleMessage- COMPLETED. Notify
2018-02-24 06:46:28.158 UTC [handler.go:551] [chaincode] notify -> DEBU 1ad notifying Txid:c1a427b0-2e8f-4d6c-ac48-86ec466ce175
2018-02-24 06:46:28.158 UTC [chaincode_support.go:751] [chaincode] Execute -> DEBU 1ae Exit
2018-02-24 06:46:28.158 UTC [sysccapi.go:137] [sccapi] deploySysCC -> INFO 1af system chaincode qscc/(github.com/hyperledger/fabric/core/chaincode/qscc) deployed
2018-02-24 06:46:28.158 UTC [start.go:83] [nodeCmd] initSysCCs -> INFO 1b0 Deployed system chaincodess
2018-02-24 06:46:28.158 UTC [start.go:223] [nodeCmd] serve -> DEBU 1b1 [gerry] after initSysCCs
2018-02-24 06:46:28.158 UTC [start.go:226] [nodeCmd] serve -> DEBU 1b2 [gerry] before peer.Initialize
2018-02-24 06:46:28.158 UTC [peer.go:103] [peer] Initialize -> DEBU 1b3 [gerry] ledgerIds[[]]
2018-02-24 06:46:28.158 UTC [start.go:231] [nodeCmd] serve -> DEBU 1b4 [gerry] after peer.Initialize
2018-02-24 06:46:28.159 UTC [start.go:234] [nodeCmd] serve -> INFO 1b5 Starting peer with ID=[name:"peer0.org1.example.com" ], network ID=[dev], address=[peer0.org1.example.com:7051]
2018-02-24 06:46:28.160 UTC [start.go:279] [nodeCmd] serve -> INFO 1b6 Started peer with ID=[name:"peer0.org1.example.com" ], network ID=[dev], address=[peer0.org1.example.com:7051]
2018-02-24 06:46:28.161 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1b7 Module 'msp/identity' logger enabled for log level 'WARNING'
2018-02-24 06:46:28.161 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1b8 Module 'msp' logger enabled for log level 'WARNING'
2018-02-24 06:46:28.161 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1b9 Module 'configvalues/msp' logger enabled for log level 'WARNING'
2018-02-24 06:46:28.161 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1ba Module 'peer/gossip/mcs' logger enabled for log level 'WARNING'
2018-02-24 06:46:28.161 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1bb Module 'gossip/pull' logger enabled for log level 'WARNING'
2018-02-24 06:46:28.163 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1bc Module 'gossip/state' logger enabled for log level 'WARNING'
2018-02-24 06:46:28.163 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1bd Module 'gossip/service' logger enabled for log level 'WARNING'
2018-02-24 06:46:28.163 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1be Module 'peer/gossip/sa' logger enabled for log level 'WARNING'
2018-02-24 06:46:28.163 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1bf Module 'gossip/election' logger enabled for log level 'WARNING'
2018-02-24 06:46:28.163 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1c0 Module 'gossip/gossip' logger enabled for log level 'WARNING'
2018-02-24 06:46:28.163 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1c1 Module 'gossip/comm' logger enabled for log level 'WARNING'
2018-02-24 06:46:28.163 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1c2 Module 'gossip/discovery' logger enabled for log level 'WARNING'
2018-02-24 06:46:28.163 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1c3 Module 'ledgermgmt' logger enabled for log level 'DEBUG'
2018-02-24 06:46:28.163 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1c4 Module 'kvledger.util' logger enabled for log level 'DEBUG'
2018-02-24 06:46:28.163 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1c5 Module 'kvledger' logger enabled for log level 'DEBUG'
2018-02-24 06:46:28.163 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1c6 Module 'cauthdsl' logger enabled for log level 'WARNING'
2018-02-24 06:46:28.163 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1c7 Module 'policies' logger enabled for log level 'WARNING'
2018-02-24 06:46:28.164 UTC [logging.go:121] [flogging] setModuleLevel -> DEBU 1c8 Module 'grpc' logger enabled for log level 'ERROR'
2018-02-24 06:46:28.164 UTC [start.go:271] [nodeCmd] func5 -> INFO 1c9 Starting profiling server with listenAddress = 0.0.0.0:6060
```

### order启动日志

```
2018-02-24 06:57:41.234 UTC [orderer/main][*/opt/gopath/src/github.com/hyperledger/fabric/orderer/main.go:68] main -> INFO 001 Starting orderer:
 Version: 1.0.5
 Go version: go1.7.5
 OS/Arch: linux/amd64
2018-02-24 06:57:41.235 UTC [orderer/localconfig][*/opt/gopath/src/github.com/hyperledger/fabric/orderer/localconfig/config.go:224] Load -> DEBU 002 [gerry] configName[orderer]
2018-02-24 06:57:41.244 UTC [orderer/localconfig][*/opt/gopath/src/github.com/hyperledger/fabric/orderer/localconfig/config.go:351] completeInitialization -> INFO 003 Kafka.Version unset, setting to {[0 9 0 1]}
2018-02-24 06:57:41.244 UTC [bccsp_sw] /opt/gopath/src/github.com/hyperledger/fabric/bccsp/sw/fileks.go:424 openKeyStore -> DEBU 004 KeyStore opened at [/etc/hyperledger/msp/orderer/msp/keystore]...done
2018-02-24 06:57:41.244 UTC [bccsp] /opt/gopath/src/github.com/hyperledger/fabric/bccsp/factory/factory.go:90 initBCCSP -> DEBU 005 Initialize BCCSP [SW]
2018-02-24 06:57:41.244 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/configbuilder.go:69 getPemMaterialFromDir -> DEBU 006 Reading directory /etc/hyperledger/msp/orderer/msp/signcerts
2018-02-24 06:57:41.244 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/configbuilder.go:88 getPemMaterialFromDir -> DEBU 007 Inspecting file /etc/hyperledger/msp/orderer/msp/signcerts/orderer.example.com-cert.pem
2018-02-24 06:57:41.244 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/configbuilder.go:69 getPemMaterialFromDir -> DEBU 008 Reading directory /etc/hyperledger/msp/orderer/msp/cacerts
2018-02-24 06:57:41.244 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/configbuilder.go:88 getPemMaterialFromDir -> DEBU 009 Inspecting file /etc/hyperledger/msp/orderer/msp/cacerts/ca.example.com-cert.pem
2018-02-24 06:57:41.244 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/configbuilder.go:69 getPemMaterialFromDir -> DEBU 00a Reading directory /etc/hyperledger/msp/orderer/msp/admincerts
2018-02-24 06:57:41.244 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/configbuilder.go:88 getPemMaterialFromDir -> DEBU 00b Inspecting file /etc/hyperledger/msp/orderer/msp/admincerts/Admin@example.com-cert.pem
2018-02-24 06:57:41.244 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/configbuilder.go:69 getPemMaterialFromDir -> DEBU 00c Reading directory /etc/hyperledger/msp/orderer/msp/intermediatecerts
2018-02-24 06:57:41.244 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/configbuilder.go:186 getMspConfig -> DEBU 00d Intermediate certs folder not found at [/etc/hyperledger/msp/orderer/msp/intermediatecerts]. Skipping. [stat /etc/hyperledger/msp/orderer/msp/intermediatecerts: no such file or directory]
2018-02-24 06:57:41.244 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/configbuilder.go:69 getPemMaterialFromDir -> DEBU 00e Reading directory /etc/hyperledger/msp/orderer/msp/tlscacerts
2018-02-24 06:57:41.244 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/configbuilder.go:88 getPemMaterialFromDir -> DEBU 00f Inspecting file /etc/hyperledger/msp/orderer/msp/tlscacerts/tlsca.example.com-cert.pem
2018-02-24 06:57:41.245 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/configbuilder.go:69 getPemMaterialFromDir -> DEBU 010 Reading directory /etc/hyperledger/msp/orderer/msp/tlsintermediatecerts
2018-02-24 06:57:41.245 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/configbuilder.go:200 getMspConfig -> DEBU 011 TLS intermediate certs folder not found at [/etc/hyperledger/msp/orderer/msp/tlsintermediatecerts]. Skipping. [stat /etc/hyperledger/msp/orderer/msp/tlsintermediatecerts: no such file or directory]
2018-02-24 06:57:41.245 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/configbuilder.go:69 getPemMaterialFromDir -> DEBU 012 Reading directory /etc/hyperledger/msp/orderer/msp/crls
2018-02-24 06:57:41.245 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/configbuilder.go:210 getMspConfig -> DEBU 013 crls folder not found at [/etc/hyperledger/msp/orderer/msp/crls]. Skipping. [stat /etc/hyperledger/msp/orderer/msp/crls: no such file or directory]
2018-02-24 06:57:41.245 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/configbuilder.go:250 getMspConfig -> DEBU 014 MSP configuration file not found at [/etc/hyperledger/msp/orderer/msp/config.yaml]: [stat /etc/hyperledger/msp/orderer/msp/config.yaml: no such file or directory]
2018-02-24 06:57:41.245 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:80 NewBccspMsp -> DEBU 015 Creating BCCSP-based MSP instance
2018-02-24 06:57:41.245 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mgmt/mgmt.go:147 GetLocalMSP -> DEBU 016 Created new local MSP
2018-02-24 06:57:41.245 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:291 Setup -> DEBU 017 Setting up MSP instance OrdererMSP
2018-02-24 06:57:41.245 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:42 newIdentity -> DEBU 018 Creating identity instance for ID -----BEGIN CERTIFICATE-----
MIICLjCCAdWgAwIBAgIQGs4mTvrxNqT6hf0D1qRvXDAKBggqhkjOPQQDAjBpMQsw
CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy
YW5jaXNjbzEUMBIGA1UEChMLZXhhbXBsZS5jb20xFzAVBgNVBAMTDmNhLmV4YW1w
bGUuY29tMB4XDTE4MDIwNzAwMTUxNFoXDTI4MDIwNTAwMTUxNFowaTELMAkGA1UE
BhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBGcmFuY2lz
Y28xFDASBgNVBAoTC2V4YW1wbGUuY29tMRcwFQYDVQQDEw5jYS5leGFtcGxlLmNv
bTBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABOt3TKRas6HwSSStk07X9EnFSs8b
Z7ssJTRsCWUUnioBg0gWjSDR4TeG/06bIBsJHBiWl9CpcRKvto2gYG7CDr6jXzBd
MA4GA1UdDwEB/wQEAwIBpjAPBgNVHSUECDAGBgRVHSUAMA8GA1UdEwEB/wQFMAMB
Af8wKQYDVR0OBCIEIOdJRe0m8ofllu1MFlSBoDeCGmW5l4hQR0bZk+FFyT47MAoG
CCqGSM49BAMCA0cAMEQCIDBFGOkb6cLs6uvVu5ojOHAAWuiNhqVDEnx4BRdDqq+7
AiBfyVTGIlPPzh7264+aLADIN3iicTQLPewjGFoTPnKkSw==
-----END CERTIFICATE-----
2018-02-24 06:57:41.282 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:42 newIdentity -> DEBU 019 Creating identity instance for ID -----BEGIN CERTIFICATE-----
MIICCTCCAbCgAwIBAgIQFSTZJid1AoQWlVlHj2QU5TAKBggqhkjOPQQDAjBpMQsw
CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy
YW5jaXNjbzEUMBIGA1UEChMLZXhhbXBsZS5jb20xFzAVBgNVBAMTDmNhLmV4YW1w
bGUuY29tMB4XDTE4MDIwNzAwMTUxNFoXDTI4MDIwNTAwMTUxNFowVjELMAkGA1UE
BhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBGcmFuY2lz
Y28xGjAYBgNVBAMMEUFkbWluQGV4YW1wbGUuY29tMFkwEwYHKoZIzj0CAQYIKoZI
zj0DAQcDQgAEZqbHme3U0crsOjR5T+KA8V7WtpDeXBp/CGK29EuGabFtuz1PnCvs
F/XHdmicFjXM+2AroXyiDC/qn9r98qFuTaNNMEswDgYDVR0PAQH/BAQDAgeAMAwG
A1UdEwEB/wQCMAAwKwYDVR0jBCQwIoAg50lF7Sbyh+WW7UwWVIGgN4IaZbmXiFBH
RtmT4UXJPjswCgYIKoZIzj0EAwIDRwAwRAIgEOzFDDnlaQdbFOXSvkVqzBN6CR96
hSlrtYtjoptufVoCID7wWJdGkqtl9NPL9WuGUgflnFVd05LoVTyX5W2iq8CM
-----END CERTIFICATE-----
2018-02-24 06:57:41.290 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:42 newIdentity -> DEBU 01a Creating identity instance for ID -----BEGIN CERTIFICATE-----
MIICCTCCAbCgAwIBAgIQFSTZJid1AoQWlVlHj2QU5TAKBggqhkjOPQQDAjBpMQsw
CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy
YW5jaXNjbzEUMBIGA1UEChMLZXhhbXBsZS5jb20xFzAVBgNVBAMTDmNhLmV4YW1w
bGUuY29tMB4XDTE4MDIwNzAwMTUxNFoXDTI4MDIwNTAwMTUxNFowVjELMAkGA1UE
BhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBGcmFuY2lz
Y28xGjAYBgNVBAMMEUFkbWluQGV4YW1wbGUuY29tMFkwEwYHKoZIzj0CAQYIKoZI
zj0DAQcDQgAEZqbHme3U0crsOjR5T+KA8V7WtpDeXBp/CGK29EuGabFtuz1PnCvs
F/XHdmicFjXM+2AroXyiDC/qn9r98qFuTaNNMEswDgYDVR0PAQH/BAQDAgeAMAwG
A1UdEwEB/wQCMAAwKwYDVR0jBCQwIoAg50lF7Sbyh+WW7UwWVIGgN4IaZbmXiFBH
RtmT4UXJPjswCgYIKoZIzj0EAwIDRwAwRAIgEOzFDDnlaQdbFOXSvkVqzBN6CR96
hSlrtYtjoptufVoCID7wWJdGkqtl9NPL9WuGUgflnFVd05LoVTyX5W2iq8CM
-----END CERTIFICATE-----
2018-02-24 06:57:41.290 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:42 newIdentity -> DEBU 01b Creating identity instance for ID -----BEGIN CERTIFICATE-----
MIICDTCCAbOgAwIBAgIRANcwn2Ut52MzGhJoMMvngb0wCgYIKoZIzj0EAwIwaTEL
MAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBG
cmFuY2lzY28xFDASBgNVBAoTC2V4YW1wbGUuY29tMRcwFQYDVQQDEw5jYS5leGFt
cGxlLmNvbTAeFw0xODAyMDcwMDE1MTRaFw0yODAyMDUwMDE1MTRaMFgxCzAJBgNV
BAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1TYW4gRnJhbmNp
c2NvMRwwGgYDVQQDExNvcmRlcmVyLmV4YW1wbGUuY29tMFkwEwYHKoZIzj0CAQYI
KoZIzj0DAQcDQgAEaYgpFFX79dOrNRz5tc4XT7hwUhRYesd2qIHDf3XDtknwpy/d
AwAmT9fx9gKI84hSsOtK3KD2UvFiB+L4zcaTXKNNMEswDgYDVR0PAQH/BAQDAgeA
MAwGA1UdEwEB/wQCMAAwKwYDVR0jBCQwIoAg50lF7Sbyh+WW7UwWVIGgN4IaZbmX
iFBHRtmT4UXJPjswCgYIKoZIzj0EAwIDSAAwRQIhALafi1AF5kO9MOdv1NlRYXhs
LUb85nV+FqAMj+ibRmG6AiA4EX8AzWEMDblTgK05gFWY00Y2W7Pp1BC7sYnHViNV
hA==
-----END CERTIFICATE-----
2018-02-24 06:57:41.291 UTC [bccsp_sw] /opt/gopath/src/github.com/hyperledger/fabric/bccsp/sw/fileks.go:329 loadPrivateKey -> DEBU 01c Loading private key [b12d8ca582dd0b6ad76a95b05e2b84a384be91ef51a525f040efa98913ccbda8] at [/etc/hyperledger/msp/orderer/msp/keystore/b12d8ca582dd0b6ad76a95b05e2b84a384be91ef51a525f040efa98913ccbda8_sk]...
2018-02-24 06:57:41.295 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:42 newIdentity -> DEBU 01d Creating identity instance for ID -----BEGIN CERTIFICATE-----
MIICDTCCAbOgAwIBAgIRANcwn2Ut52MzGhJoMMvngb0wCgYIKoZIzj0EAwIwaTEL
MAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBG
cmFuY2lzY28xFDASBgNVBAoTC2V4YW1wbGUuY29tMRcwFQYDVQQDEw5jYS5leGFt
cGxlLmNvbTAeFw0xODAyMDcwMDE1MTRaFw0yODAyMDUwMDE1MTRaMFgxCzAJBgNV
BAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1TYW4gRnJhbmNp
c2NvMRwwGgYDVQQDExNvcmRlcmVyLmV4YW1wbGUuY29tMFkwEwYHKoZIzj0CAQYI
KoZIzj0DAQcDQgAEaYgpFFX79dOrNRz5tc4XT7hwUhRYesd2qIHDf3XDtknwpy/d
AwAmT9fx9gKI84hSsOtK3KD2UvFiB+L4zcaTXKNNMEswDgYDVR0PAQH/BAQDAgeA
MAwGA1UdEwEB/wQCMAAwKwYDVR0jBCQwIoAg50lF7Sbyh+WW7UwWVIGgN4IaZbmX
iFBHRtmT4UXJPjswCgYIKoZIzj0EAwIDSAAwRQIhALafi1AF5kO9MOdv1NlRYXhs
LUb85nV+FqAMj+ibRmG6AiA4EX8AzWEMDblTgK05gFWY00Y2W7Pp1BC7sYnHViNV
hA==
-----END CERTIFICATE-----
2018-02-24 06:57:41.296 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:391 Validate -> DEBU 01e MSP OrdererMSP validating identity
2018-02-24 06:57:41.296 UTC [orderer/main] /opt/gopath/src/github.com/hyperledger/fabric/orderer/util.go:41 createLedgerFactory -> DEBU 01f Ledger dir: /var/hyperledger/production/orderer
2018-02-24 06:57:41.296 UTC [kvledger.util] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/util/ioutil.go:37 CreateDirIfMissing -> DEBU 020 CreateDirIfMissing [/var/hyperledger/production/orderer/index/]
2018-02-24 06:57:41.296 UTC [kvledger.util] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/util/ioutil.go:97 logDirStatus -> DEBU 021 Before creating dir - [/var/hyperledger/production/orderer/index/] does not exist
2018-02-24 06:57:41.296 UTC [kvledger.util] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/util/ioutil.go:95 logDirStatus -> DEBU 022 After creating dir - [/var/hyperledger/production/orderer/index/] exists
2018-02-24 06:57:41.309 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_mgr.go:100 newBlockfileMgr -> DEBU 023 newBlockfileMgr() initializing file-based block storage for ledger: testchainid 
2018-02-24 06:57:41.309 UTC [kvledger.util] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/util/ioutil.go:37 CreateDirIfMissing -> DEBU 024 CreateDirIfMissing [/var/hyperledger/production/orderer/chains/testchainid/]
2018-02-24 06:57:41.309 UTC [kvledger.util] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/util/ioutil.go:97 logDirStatus -> DEBU 025 Before creating dir - [/var/hyperledger/production/orderer/chains/testchainid/] does not exist
2018-02-24 06:57:41.309 UTC [kvledger.util] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/util/ioutil.go:95 logDirStatus -> DEBU 026 After creating dir - [/var/hyperledger/production/orderer/chains/testchainid/] exists
2018-02-24 06:57:41.309 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_mgr.go:118 newBlockfileMgr -> INFO 027 Getting block information from block storage
2018-02-24 06:57:41.309 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_helper.go:24 constructCheckpointInfoFromBlockFiles -> DEBU 028 Retrieving checkpoint info from block files
2018-02-24 06:57:41.309 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_helper.go:81 retrieveLastFileSuffix -> DEBU 029 retrieveLastFileSuffix()
2018-02-24 06:57:41.309 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_helper.go:102 retrieveLastFileSuffix -> DEBU 02a retrieveLastFileSuffix() - biggestFileNum = -1
2018-02-24 06:57:41.309 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_helper.go:37 constructCheckpointInfoFromBlockFiles -> DEBU 02b Last file number found = -1
2018-02-24 06:57:41.309 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_helper.go:41 constructCheckpointInfoFromBlockFiles -> DEBU 02c No block file found
2018-02-24 06:57:41.309 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_mgr.go:122 newBlockfileMgr -> DEBU 02d Info constructed by scanning the blocks dir = (*fsblkstorage.checkpointInfo)(0xc420364800)(latestFileChunkSuffixNum=[0], latestFileChunksize=[0], isChainEmpty=[true], lastBlockNumber=[0])
2018-02-24 06:57:41.311 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockindex.go:72 newBlockIndex -> DEBU 02e newBlockIndex() - indexItems:[[BlockNum]]
2018-02-24 06:57:41.312 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockindex.go:98 indexBlock -> DEBU 02f Indexing block [blockNum=0, blockHash=[]byte{0xe3, 0xe1, 0x6c, 0xb2, 0xfa, 0x99, 0x8e, 0x7b, 0x12, 0x83, 0x28, 0xf, 0x42, 0xb1, 0x93, 0x60, 0x6d, 0xf3, 0xeb, 0xc8, 0x28, 0x80, 0xb, 0x34, 0x77, 0x9a, 0xde, 0x8d, 0xb3, 0x2b, 0xd8, 0x22} txOffsets=
txId=1a28426b6b932c2ff7c2828018ee3307d1a222286486fd70b24c09f5abb8b618 locPointer=offset=38, bytesLength=9031
]
2018-02-24 06:57:41.313 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_mgr.go:430 updateCheckpoint -> DEBU 030 Broadcasting about update checkpointInfo: latestFileChunkSuffixNum=[0], latestFileChunksize=[9074], isChainEmpty=[false], lastBlockNumber=[0]
2018-02-24 06:57:41.313 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_mgr.go:454 retrieveBlockByNumber -> DEBU 031 retrieveBlockByNumber() - blockNum = [0]
2018-02-24 06:57:41.313 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:66 newBlockfileStream -> DEBU 032 newBlockfileStream(): filePath=[/var/hyperledger/production/orderer/chains/testchainid/blockfile_000000], startOffset=[0]
2018-02-24 06:57:41.313 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:114 nextBlockBytesAndPlacementInfo -> DEBU 033 Remaining bytes=[9074], Going to peek [8] bytes
2018-02-24 06:57:41.313 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:147 nextBlockBytesAndPlacementInfo -> DEBU 034 Returning blockbytes - length=[9072], placementInfo={fileNum=[0], startOffset=[0], bytesOffset=[2]}
2018-02-24 06:57:41.313 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_mgr.go:454 retrieveBlockByNumber -> DEBU 035 retrieveBlockByNumber() - blockNum = [0]
2018-02-24 06:57:41.313 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:66 newBlockfileStream -> DEBU 036 newBlockfileStream(): filePath=[/var/hyperledger/production/orderer/chains/testchainid/blockfile_000000], startOffset=[0]
2018-02-24 06:57:41.313 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:114 nextBlockBytesAndPlacementInfo -> DEBU 037 Remaining bytes=[9074], Going to peek [8] bytes
2018-02-24 06:57:41.313 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:147 nextBlockBytesAndPlacementInfo -> DEBU 038 Returning blockbytes - length=[9072], placementInfo={fileNum=[0], startOffset=[0], bytesOffset=[2]}
2018-02-24 06:57:41.314 UTC [common/config] /opt/gopath/src/github.com/hyperledger/fabric/common/config/standardvalues.go:41 NewStandardValues -> DEBU 039 Initializing protos for *config.ChannelProtos
2018-02-24 06:57:41.314 UTC [common/config] /opt/gopath/src/github.com/hyperledger/fabric/common/config/standardvalues.go:79 initializeProtosStruct -> DEBU 03a Processing field: HashingAlgorithm
2018-02-24 06:57:41.314 UTC [common/config] /opt/gopath/src/github.com/hyperledger/fabric/common/config/standardvalues.go:79 initializeProtosStruct -> DEBU 03b Processing field: BlockDataHashingStructure
2018-02-24 06:57:41.314 UTC [common/config] /opt/gopath/src/github.com/hyperledger/fabric/common/config/standardvalues.go:79 initializeProtosStruct -> DEBU 03c Processing field: OrdererAddresses
2018-02-24 06:57:41.314 UTC [common/config] /opt/gopath/src/github.com/hyperledger/fabric/common/config/standardvalues.go:79 initializeProtosStruct -> DEBU 03d Processing field: Consortium
2018-02-24 06:57:41.314 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 03e Adding to config map: [Groups] /Channel
2018-02-24 06:57:41.314 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 03f Adding to config map: [Groups] /Channel/Consortiums
2018-02-24 06:57:41.314 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 040 Adding to config map: [Groups] /Channel/Consortiums/SampleConsortium
2018-02-24 06:57:41.314 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 041 Adding to config map: [Groups] /Channel/Consortiums/SampleConsortium/Org1MSP
2018-02-24 06:57:41.314 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 042 Adding to config map: [Values] /Channel/Consortiums/SampleConsortium/Org1MSP/MSP
2018-02-24 06:57:41.314 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 043 Adding to config map: [Policy] /Channel/Consortiums/SampleConsortium/Org1MSP/Readers
2018-02-24 06:57:41.314 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 044 Adding to config map: [Policy] /Channel/Consortiums/SampleConsortium/Org1MSP/Writers
2018-02-24 06:57:41.314 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 045 Adding to config map: [Policy] /Channel/Consortiums/SampleConsortium/Org1MSP/Admins
2018-02-24 06:57:41.314 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 046 Adding to config map: [Groups] /Channel/Consortiums/SampleConsortium/Org2MSP
2018-02-24 06:57:41.314 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 047 Adding to config map: [Values] /Channel/Consortiums/SampleConsortium/Org2MSP/MSP
2018-02-24 06:57:41.314 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 048 Adding to config map: [Policy] /Channel/Consortiums/SampleConsortium/Org2MSP/Admins
2018-02-24 06:57:41.317 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 049 Adding to config map: [Policy] /Channel/Consortiums/SampleConsortium/Org2MSP/Readers
2018-02-24 06:57:41.317 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 04a Adding to config map: [Policy] /Channel/Consortiums/SampleConsortium/Org2MSP/Writers
2018-02-24 06:57:41.317 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 04b Adding to config map: [Values] /Channel/Consortiums/SampleConsortium/ChannelCreationPolicy
2018-02-24 06:57:41.317 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 04c Adding to config map: [Policy] /Channel/Consortiums/Admins
2018-02-24 06:57:41.317 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 04d Adding to config map: [Groups] /Channel/Orderer
2018-02-24 06:57:41.317 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 04e Adding to config map: [Groups] /Channel/Orderer/OrdererOrg
2018-02-24 06:57:41.317 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 04f Adding to config map: [Values] /Channel/Orderer/OrdererOrg/MSP
2018-02-24 06:57:41.317 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 050 Adding to config map: [Policy] /Channel/Orderer/OrdererOrg/Admins
2018-02-24 06:57:41.317 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 051 Adding to config map: [Policy] /Channel/Orderer/OrdererOrg/Readers
2018-02-24 06:57:41.318 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 052 Adding to config map: [Policy] /Channel/Orderer/OrdererOrg/Writers
2018-02-24 06:57:41.318 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 053 Adding to config map: [Values] /Channel/Orderer/BatchSize
2018-02-24 06:57:41.318 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 054 Adding to config map: [Values] /Channel/Orderer/BatchTimeout
2018-02-24 06:57:41.318 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 055 Adding to config map: [Values] /Channel/Orderer/ChannelRestrictions
2018-02-24 06:57:41.318 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 056 Adding to config map: [Values] /Channel/Orderer/ConsensusType
2018-02-24 06:57:41.318 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 057 Adding to config map: [Policy] /Channel/Orderer/Writers
2018-02-24 06:57:41.318 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 058 Adding to config map: [Policy] /Channel/Orderer/Admins
2018-02-24 06:57:41.318 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 059 Adding to config map: [Policy] /Channel/Orderer/BlockValidation
2018-02-24 06:57:41.318 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 05a Adding to config map: [Policy] /Channel/Orderer/Readers
2018-02-24 06:57:41.318 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 05b Adding to config map: [Values] /Channel/HashingAlgorithm
2018-02-24 06:57:41.318 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 05c Adding to config map: [Values] /Channel/BlockDataHashingStructure
2018-02-24 06:57:41.318 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 05d Adding to config map: [Values] /Channel/OrdererAddresses
2018-02-24 06:57:41.318 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 05e Adding to config map: [Policy] /Channel/Writers
2018-02-24 06:57:41.318 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 05f Adding to config map: [Policy] /Channel/Admins
2018-02-24 06:57:41.318 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/configmap.go:74 addToMap -> DEBU 060 Adding to config map: [Policy] /Channel/Readers
2018-02-24 06:57:41.318 UTC [common/configtx] /opt/gopath/src/github.com/hyperledger/fabric/common/configtx/config.go:269 processConfig -> DEBU 061 Beginning new config for channel testchainid
2018-02-24 06:57:41.318 UTC [common/config] /opt/gopath/src/github.com/hyperledger/fabric/common/config/standardvalues.go:41 NewStandardValues -> DEBU 062 Initializing protos for *config.ChannelProtos
2018-02-24 06:57:41.318 UTC [common/config] /opt/gopath/src/github.com/hyperledger/fabric/common/config/standardvalues.go:79 initializeProtosStruct -> DEBU 063 Processing field: HashingAlgorithm
2018-02-24 06:57:41.318 UTC [common/config] /opt/gopath/src/github.com/hyperledger/fabric/common/config/standardvalues.go:79 initializeProtosStruct -> DEBU 064 Processing field: BlockDataHashingStructure
2018-02-24 06:57:41.318 UTC [common/config] /opt/gopath/src/github.com/hyperledger/fabric/common/config/standardvalues.go:79 initializeProtosStruct -> DEBU 065 Processing field: OrdererAddresses
2018-02-24 06:57:41.318 UTC [common/config] /opt/gopath/src/github.com/hyperledger/fabric/common/config/standardvalues.go:79 initializeProtosStruct -> DEBU 066 Processing field: Consortium
2018-02-24 06:57:41.318 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:374 ProposePolicy -> DEBU 067 Proposed new policy Readers for Channel
2018-02-24 06:57:41.318 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:374 ProposePolicy -> DEBU 068 Proposed new policy Writers for Channel
2018-02-24 06:57:41.318 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:374 ProposePolicy -> DEBU 069 Proposed new policy Admins for Channel
2018-02-24 06:57:41.318 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:374 ProposePolicy -> DEBU 06a Proposed new policy Admins for Consortiums
2018-02-24 06:57:41.318 UTC [common/config] /opt/gopath/src/github.com/hyperledger/fabric/common/config/standardvalues.go:41 NewStandardValues -> DEBU 06b Initializing protos for *config.ConsortiumProtos
2018-02-24 06:57:41.318 UTC [common/config] /opt/gopath/src/github.com/hyperledger/fabric/common/config/standardvalues.go:79 initializeProtosStruct -> DEBU 06c Processing field: ChannelCreationPolicy
2018-02-24 06:57:41.318 UTC [common/config] /opt/gopath/src/github.com/hyperledger/fabric/common/config/standardvalues.go:41 NewStandardValues -> DEBU 06d Initializing protos for *config.OrganizationProtos
2018-02-24 06:57:41.318 UTC [common/config] /opt/gopath/src/github.com/hyperledger/fabric/common/config/standardvalues.go:79 initializeProtosStruct -> DEBU 06e Processing field: MSP
2018-02-24 06:57:41.318 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:374 ProposePolicy -> DEBU 06f Proposed new policy Readers for Org1MSP
2018-02-24 06:57:41.318 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:374 ProposePolicy -> DEBU 070 Proposed new policy Writers for Org1MSP
2018-02-24 06:57:41.318 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:374 ProposePolicy -> DEBU 071 Proposed new policy Admins for Org1MSP
2018-02-24 06:57:41.318 UTC [common/config] /opt/gopath/src/github.com/hyperledger/fabric/common/config/standardvalues.go:41 NewStandardValues -> DEBU 072 Initializing protos for *config.OrganizationProtos
2018-02-24 06:57:41.318 UTC [common/config] /opt/gopath/src/github.com/hyperledger/fabric/common/config/standardvalues.go:79 initializeProtosStruct -> DEBU 073 Processing field: MSP
2018-02-24 06:57:41.318 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:374 ProposePolicy -> DEBU 074 Proposed new policy Admins for Org2MSP
2018-02-24 06:57:41.318 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:374 ProposePolicy -> DEBU 075 Proposed new policy Readers for Org2MSP
2018-02-24 06:57:41.318 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:374 ProposePolicy -> DEBU 076 Proposed new policy Writers for Org2MSP
2018-02-24 06:57:41.318 UTC [common/config] /opt/gopath/src/github.com/hyperledger/fabric/common/config/standardvalues.go:41 NewStandardValues -> DEBU 077 Initializing protos for *config.OrdererProtos
2018-02-24 06:57:41.318 UTC [common/config] /opt/gopath/src/github.com/hyperledger/fabric/common/config/standardvalues.go:79 initializeProtosStruct -> DEBU 078 Processing field: ConsensusType
2018-02-24 06:57:41.318 UTC [common/config] /opt/gopath/src/github.com/hyperledger/fabric/common/config/standardvalues.go:79 initializeProtosStruct -> DEBU 079 Processing field: BatchSize
2018-02-24 06:57:41.318 UTC [common/config] /opt/gopath/src/github.com/hyperledger/fabric/common/config/standardvalues.go:79 initializeProtosStruct -> DEBU 07a Processing field: BatchTimeout
2018-02-24 06:57:41.318 UTC [common/config] /opt/gopath/src/github.com/hyperledger/fabric/common/config/standardvalues.go:79 initializeProtosStruct -> DEBU 07b Processing field: KafkaBrokers
2018-02-24 06:57:41.318 UTC [common/config] /opt/gopath/src/github.com/hyperledger/fabric/common/config/standardvalues.go:79 initializeProtosStruct -> DEBU 07c Processing field: ChannelRestrictions
2018-02-24 06:57:41.318 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:374 ProposePolicy -> DEBU 07d Proposed new policy Admins for Orderer
2018-02-24 06:57:41.318 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:374 ProposePolicy -> DEBU 07e Proposed new policy BlockValidation for Orderer
2018-02-24 06:57:41.318 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:374 ProposePolicy -> DEBU 07f Proposed new policy Readers for Orderer
2018-02-24 06:57:41.318 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:374 ProposePolicy -> DEBU 080 Proposed new policy Writers for Orderer
2018-02-24 06:57:41.318 UTC [common/config] /opt/gopath/src/github.com/hyperledger/fabric/common/config/standardvalues.go:41 NewStandardValues -> DEBU 081 Initializing protos for *config.OrganizationProtos
2018-02-24 06:57:41.318 UTC [common/config] /opt/gopath/src/github.com/hyperledger/fabric/common/config/standardvalues.go:79 initializeProtosStruct -> DEBU 082 Processing field: MSP
2018-02-24 06:57:41.318 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:374 ProposePolicy -> DEBU 083 Proposed new policy Admins for OrdererOrg
2018-02-24 06:57:41.318 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:374 ProposePolicy -> DEBU 084 Proposed new policy Readers for OrdererOrg
2018-02-24 06:57:41.318 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:374 ProposePolicy -> DEBU 085 Proposed new policy Writers for OrdererOrg
2018-02-24 06:57:41.318 UTC [common/config] /opt/gopath/src/github.com/hyperledger/fabric/common/config/organization.go:112 validateMSP -> DEBU 086 Setting up MSP for org Org1MSP
2018-02-24 06:57:41.318 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:80 NewBccspMsp -> DEBU 087 Creating BCCSP-based MSP instance
2018-02-24 06:57:41.318 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:291 Setup -> DEBU 088 Setting up MSP instance Org1MSP
2018-02-24 06:57:41.319 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:42 newIdentity -> DEBU 089 Creating identity instance for ID -----BEGIN CERTIFICATE-----
MIICQjCCAemgAwIBAgIQALfJgAr0WuoYrndT1JgYtzAKBggqhkjOPQQDAjBzMQsw
CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy
YW5jaXNjbzEZMBcGA1UEChMQb3JnMS5leGFtcGxlLmNvbTEcMBoGA1UEAxMTY2Eu
b3JnMS5leGFtcGxlLmNvbTAeFw0xODAyMDcwMDE1MTRaFw0yODAyMDUwMDE1MTRa
MHMxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1T
YW4gRnJhbmNpc2NvMRkwFwYDVQQKExBvcmcxLmV4YW1wbGUuY29tMRwwGgYDVQQD
ExNjYS5vcmcxLmV4YW1wbGUuY29tMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE
C5w60CovhKqoP2s7Jq3+BiBN9eObo9C+i+bjtVZKwe5YTxdC83hS05pSKlWUOIAR
+mKI/KAbxmWbJRgobxJStaNfMF0wDgYDVR0PAQH/BAQDAgGmMA8GA1UdJQQIMAYG
BFUdJQAwDwYDVR0TAQH/BAUwAwEB/zApBgNVHQ4EIgQgT8YQQJTfOVXIEk6JW0Jt
qfNLN5Zs9iPXOc3LEMJGYE0wCgYIKoZIzj0EAwIDRwAwRAIgKTAMHtnSyWKL4nE2
gQkWxIGlep8L7BL9bv4uXvsnYtMCIBTgITu+ZCJ/TyPYYM8JNuu/KosnHAuf4/gT
sFb9Jvlg
-----END CERTIFICATE-----
2018-02-24 06:57:41.326 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:42 newIdentity -> DEBU 08a Creating identity instance for ID -----BEGIN CERTIFICATE-----
MIICGTCCAb+gAwIBAgIQU2EEaekZq1LsI6bZEdL9rzAKBggqhkjOPQQDAjBzMQsw
CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy
YW5jaXNjbzEZMBcGA1UEChMQb3JnMS5leGFtcGxlLmNvbTEcMBoGA1UEAxMTY2Eu
b3JnMS5leGFtcGxlLmNvbTAeFw0xODAyMDcwMDE1MTRaFw0yODAyMDUwMDE1MTRa
MFsxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1T
YW4gRnJhbmNpc2NvMR8wHQYDVQQDDBZBZG1pbkBvcmcxLmV4YW1wbGUuY29tMFkw
EwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE01kUw4VY/YsrMtL8eZxZchnBuzZNPOfj
JN97M78wxDGWVVnHDhrMFemCJ0r3MLJw2J/AJ869y9HdUrDWX0lo+6NNMEswDgYD
VR0PAQH/BAQDAgeAMAwGA1UdEwEB/wQCMAAwKwYDVR0jBCQwIoAgT8YQQJTfOVXI
Ek6JW0JtqfNLN5Zs9iPXOc3LEMJGYE0wCgYIKoZIzj0EAwIDSAAwRQIhAJpvtr6J
EKkSikx802ESGmwdxejmU2HT4vjySUluYCS7AiBAO7bP3X2sIFYCF/A94bBjfcQR
XNvYJqPJ+Y/zLYo2PQ==
-----END CERTIFICATE-----
2018-02-24 06:57:41.327 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:42 newIdentity -> DEBU 08b Creating identity instance for ID -----BEGIN CERTIFICATE-----
MIICGTCCAb+gAwIBAgIQU2EEaekZq1LsI6bZEdL9rzAKBggqhkjOPQQDAjBzMQsw
CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy
YW5jaXNjbzEZMBcGA1UEChMQb3JnMS5leGFtcGxlLmNvbTEcMBoGA1UEAxMTY2Eu
b3JnMS5leGFtcGxlLmNvbTAeFw0xODAyMDcwMDE1MTRaFw0yODAyMDUwMDE1MTRa
MFsxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1T
YW4gRnJhbmNpc2NvMR8wHQYDVQQDDBZBZG1pbkBvcmcxLmV4YW1wbGUuY29tMFkw
EwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE01kUw4VY/YsrMtL8eZxZchnBuzZNPOfj
JN97M78wxDGWVVnHDhrMFemCJ0r3MLJw2J/AJ869y9HdUrDWX0lo+6NNMEswDgYD
VR0PAQH/BAQDAgeAMAwGA1UdEwEB/wQCMAAwKwYDVR0jBCQwIoAgT8YQQJTfOVXI
Ek6JW0JtqfNLN5Zs9iPXOc3LEMJGYE0wCgYIKoZIzj0EAwIDSAAwRQIhAJpvtr6J
EKkSikx802ESGmwdxejmU2HT4vjySUluYCS7AiBAO7bP3X2sIFYCF/A94bBjfcQR
XNvYJqPJ+Y/zLYo2PQ==
-----END CERTIFICATE-----
2018-02-24 06:57:41.329 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:391 Validate -> DEBU 08c MSP Org1MSP validating identity
2018-02-24 06:57:41.329 UTC [common/config] /opt/gopath/src/github.com/hyperledger/fabric/common/config/organization.go:112 validateMSP -> DEBU 08d Setting up MSP for org Org2MSP
2018-02-24 06:57:41.329 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:80 NewBccspMsp -> DEBU 08e Creating BCCSP-based MSP instance
2018-02-24 06:57:41.329 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:291 Setup -> DEBU 08f Setting up MSP instance Org2MSP
2018-02-24 06:57:41.329 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:42 newIdentity -> DEBU 090 Creating identity instance for ID -----BEGIN CERTIFICATE-----
MIICQzCCAeqgAwIBAgIRALWMX2SDad2vYSXcpdECVhQwCgYIKoZIzj0EAwIwczEL
MAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBG
cmFuY2lzY28xGTAXBgNVBAoTEG9yZzIuZXhhbXBsZS5jb20xHDAaBgNVBAMTE2Nh
Lm9yZzIuZXhhbXBsZS5jb20wHhcNMTgwMjA3MDAxNTE0WhcNMjgwMjA1MDAxNTE0
WjBzMQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMN
U2FuIEZyYW5jaXNjbzEZMBcGA1UEChMQb3JnMi5leGFtcGxlLmNvbTEcMBoGA1UE
AxMTY2Eub3JnMi5leGFtcGxlLmNvbTBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IA
BDk6FBv4eN3z1ykrrtNe64armBlpqBPI9QYqkHXe8lEXK4AoH+m4lQmTXy4HyUL9
XTYdlq+S/oVFf13kXN106GijXzBdMA4GA1UdDwEB/wQEAwIBpjAPBgNVHSUECDAG
BgRVHSUAMA8GA1UdEwEB/wQFMAMBAf8wKQYDVR0OBCIEILTo1yHrU1t5U1CIOl2t
mpWeq3CbAdpAmURXUES4Db6WMAoGCCqGSM49BAMCA0cAMEQCIH8DP9cyGC/QKI3T
N22O6dm4+R2LyMZ8qzhOZpnjwKzQAiAjCHSE0LIoW6m3VyP65mbhU7fyqd8j642E
ML/i57mI8w==
-----END CERTIFICATE-----
2018-02-24 06:57:41.330 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:42 newIdentity -> DEBU 091 Creating identity instance for ID -----BEGIN CERTIFICATE-----
MIICGTCCAcCgAwIBAgIRALaPHpESgV/iTEWMsdLDjs8wCgYIKoZIzj0EAwIwczEL
MAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBG
cmFuY2lzY28xGTAXBgNVBAoTEG9yZzIuZXhhbXBsZS5jb20xHDAaBgNVBAMTE2Nh
Lm9yZzIuZXhhbXBsZS5jb20wHhcNMTgwMjA3MDAxNTE0WhcNMjgwMjA1MDAxNTE0
WjBbMQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMN
U2FuIEZyYW5jaXNjbzEfMB0GA1UEAwwWQWRtaW5Ab3JnMi5leGFtcGxlLmNvbTBZ
MBMGByqGSM49AgEGCCqGSM49AwEHA0IABDz0mzjZ4KG7+P3rfb2vmkNHArPSN2sk
T2pVS2/Wk76pcrTaIbIBbSB5w5UZhHXUNtcJ/rkkK3J/etiZIZrAUhWjTTBLMA4G
A1UdDwEB/wQEAwIHgDAMBgNVHRMBAf8EAjAAMCsGA1UdIwQkMCKAILTo1yHrU1t5
U1CIOl2tmpWeq3CbAdpAmURXUES4Db6WMAoGCCqGSM49BAMCA0cAMEQCIGNqjFVl
AiJVSNkGgeKoIWnIbZQxSykwbdjs562J2eUcAiBbwZ+LqcVlzvlT/hMEuaF0K3i6
CpAf56SYwUSa19Sk7g==
-----END CERTIFICATE-----
2018-02-24 06:57:41.330 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:42 newIdentity -> DEBU 092 Creating identity instance for ID -----BEGIN CERTIFICATE-----
MIICGTCCAcCgAwIBAgIRALaPHpESgV/iTEWMsdLDjs8wCgYIKoZIzj0EAwIwczEL
MAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBG
cmFuY2lzY28xGTAXBgNVBAoTEG9yZzIuZXhhbXBsZS5jb20xHDAaBgNVBAMTE2Nh
Lm9yZzIuZXhhbXBsZS5jb20wHhcNMTgwMjA3MDAxNTE0WhcNMjgwMjA1MDAxNTE0
WjBbMQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMN
U2FuIEZyYW5jaXNjbzEfMB0GA1UEAwwWQWRtaW5Ab3JnMi5leGFtcGxlLmNvbTBZ
MBMGByqGSM49AgEGCCqGSM49AwEHA0IABDz0mzjZ4KG7+P3rfb2vmkNHArPSN2sk
T2pVS2/Wk76pcrTaIbIBbSB5w5UZhHXUNtcJ/rkkK3J/etiZIZrAUhWjTTBLMA4G
A1UdDwEB/wQEAwIHgDAMBgNVHRMBAf8EAjAAMCsGA1UdIwQkMCKAILTo1yHrU1t5
U1CIOl2tmpWeq3CbAdpAmURXUES4Db6WMAoGCCqGSM49BAMCA0cAMEQCIGNqjFVl
AiJVSNkGgeKoIWnIbZQxSykwbdjs562J2eUcAiBbwZ+LqcVlzvlT/hMEuaF0K3i6
CpAf56SYwUSa19Sk7g==
-----END CERTIFICATE-----
2018-02-24 06:57:41.330 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:391 Validate -> DEBU 093 MSP Org2MSP validating identity
2018-02-24 06:57:41.330 UTC [common/config] /opt/gopath/src/github.com/hyperledger/fabric/common/config/organization.go:112 validateMSP -> DEBU 094 Setting up MSP for org OrdererOrg
2018-02-24 06:57:41.331 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:80 NewBccspMsp -> DEBU 095 Creating BCCSP-based MSP instance
2018-02-24 06:57:41.331 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:291 Setup -> DEBU 096 Setting up MSP instance OrdererMSP
2018-02-24 06:57:41.331 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:42 newIdentity -> DEBU 097 Creating identity instance for ID -----BEGIN CERTIFICATE-----
MIICLjCCAdWgAwIBAgIQGs4mTvrxNqT6hf0D1qRvXDAKBggqhkjOPQQDAjBpMQsw
CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy
YW5jaXNjbzEUMBIGA1UEChMLZXhhbXBsZS5jb20xFzAVBgNVBAMTDmNhLmV4YW1w
bGUuY29tMB4XDTE4MDIwNzAwMTUxNFoXDTI4MDIwNTAwMTUxNFowaTELMAkGA1UE
BhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBGcmFuY2lz
Y28xFDASBgNVBAoTC2V4YW1wbGUuY29tMRcwFQYDVQQDEw5jYS5leGFtcGxlLmNv
bTBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABOt3TKRas6HwSSStk07X9EnFSs8b
Z7ssJTRsCWUUnioBg0gWjSDR4TeG/06bIBsJHBiWl9CpcRKvto2gYG7CDr6jXzBd
MA4GA1UdDwEB/wQEAwIBpjAPBgNVHSUECDAGBgRVHSUAMA8GA1UdEwEB/wQFMAMB
Af8wKQYDVR0OBCIEIOdJRe0m8ofllu1MFlSBoDeCGmW5l4hQR0bZk+FFyT47MAoG
CCqGSM49BAMCA0cAMEQCIDBFGOkb6cLs6uvVu5ojOHAAWuiNhqVDEnx4BRdDqq+7
AiBfyVTGIlPPzh7264+aLADIN3iicTQLPewjGFoTPnKkSw==
-----END CERTIFICATE-----
2018-02-24 06:57:41.372 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:42 newIdentity -> DEBU 098 Creating identity instance for ID -----BEGIN CERTIFICATE-----
MIICCTCCAbCgAwIBAgIQFSTZJid1AoQWlVlHj2QU5TAKBggqhkjOPQQDAjBpMQsw
CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy
YW5jaXNjbzEUMBIGA1UEChMLZXhhbXBsZS5jb20xFzAVBgNVBAMTDmNhLmV4YW1w
bGUuY29tMB4XDTE4MDIwNzAwMTUxNFoXDTI4MDIwNTAwMTUxNFowVjELMAkGA1UE
BhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBGcmFuY2lz
Y28xGjAYBgNVBAMMEUFkbWluQGV4YW1wbGUuY29tMFkwEwYHKoZIzj0CAQYIKoZI
zj0DAQcDQgAEZqbHme3U0crsOjR5T+KA8V7WtpDeXBp/CGK29EuGabFtuz1PnCvs
F/XHdmicFjXM+2AroXyiDC/qn9r98qFuTaNNMEswDgYDVR0PAQH/BAQDAgeAMAwG
A1UdEwEB/wQCMAAwKwYDVR0jBCQwIoAg50lF7Sbyh+WW7UwWVIGgN4IaZbmXiFBH
RtmT4UXJPjswCgYIKoZIzj0EAwIDRwAwRAIgEOzFDDnlaQdbFOXSvkVqzBN6CR96
hSlrtYtjoptufVoCID7wWJdGkqtl9NPL9WuGUgflnFVd05LoVTyX5W2iq8CM
-----END CERTIFICATE-----
2018-02-24 06:57:41.373 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:42 newIdentity -> DEBU 099 Creating identity instance for ID -----BEGIN CERTIFICATE-----
MIICCTCCAbCgAwIBAgIQFSTZJid1AoQWlVlHj2QU5TAKBggqhkjOPQQDAjBpMQsw
CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy
YW5jaXNjbzEUMBIGA1UEChMLZXhhbXBsZS5jb20xFzAVBgNVBAMTDmNhLmV4YW1w
bGUuY29tMB4XDTE4MDIwNzAwMTUxNFoXDTI4MDIwNTAwMTUxNFowVjELMAkGA1UE
BhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExFjAUBgNVBAcTDVNhbiBGcmFuY2lz
Y28xGjAYBgNVBAMMEUFkbWluQGV4YW1wbGUuY29tMFkwEwYHKoZIzj0CAQYIKoZI
zj0DAQcDQgAEZqbHme3U0crsOjR5T+KA8V7WtpDeXBp/CGK29EuGabFtuz1PnCvs
F/XHdmicFjXM+2AroXyiDC/qn9r98qFuTaNNMEswDgYDVR0PAQH/BAQDAgeAMAwG
A1UdEwEB/wQCMAAwKwYDVR0jBCQwIoAg50lF7Sbyh+WW7UwWVIGgN4IaZbmXiFBH
RtmT4UXJPjswCgYIKoZIzj0EAwIDRwAwRAIgEOzFDDnlaQdbFOXSvkVqzBN6CR96
hSlrtYtjoptufVoCID7wWJdGkqtl9NPL9WuGUgflnFVd05LoVTyX5W2iq8CM
-----END CERTIFICATE-----
2018-02-24 06:57:41.373 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:391 Validate -> DEBU 09a MSP OrdererMSP validating identity
2018-02-24 06:57:41.373 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspmgrimpl.go:60 Setup -> DEBU 09b Setting up the MSP manager (3 msps)
2018-02-24 06:57:41.373 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspmgrimpl.go:76 Setup -> DEBU 09c MSP manager setup complete, setup 3 msps
2018-02-24 06:57:41.373 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 09d Returning policy Readers for evaluation
2018-02-24 06:57:41.373 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:285 CommitProposals -> DEBU 09e In commit adding relative sub-policy Org1MSP/Readers to SampleConsortium
2018-02-24 06:57:41.373 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 09f Returning policy Writers for evaluation
2018-02-24 06:57:41.373 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:285 CommitProposals -> DEBU 0a0 In commit adding relative sub-policy Org1MSP/Writers to SampleConsortium
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0a1 Returning policy Admins for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:285 CommitProposals -> DEBU 0a2 In commit adding relative sub-policy Org1MSP/Admins to SampleConsortium
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0a3 Returning policy Readers for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:285 CommitProposals -> DEBU 0a4 In commit adding relative sub-policy Org2MSP/Readers to SampleConsortium
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0a5 Returning policy Writers for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:285 CommitProposals -> DEBU 0a6 In commit adding relative sub-policy Org2MSP/Writers to SampleConsortium
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0a7 Returning policy Admins for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:285 CommitProposals -> DEBU 0a8 In commit adding relative sub-policy Org2MSP/Admins to SampleConsortium
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0a9 Returning policy Org1MSP/Readers for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:285 CommitProposals -> DEBU 0aa In commit adding relative sub-policy SampleConsortium/Org1MSP/Readers to Consortiums
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0ab Returning policy Org1MSP/Writers for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:285 CommitProposals -> DEBU 0ac In commit adding relative sub-policy SampleConsortium/Org1MSP/Writers to Consortiums
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0ad Returning policy Org1MSP/Admins for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:285 CommitProposals -> DEBU 0ae In commit adding relative sub-policy SampleConsortium/Org1MSP/Admins to Consortiums
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0af Returning policy Org2MSP/Readers for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:285 CommitProposals -> DEBU 0b0 In commit adding relative sub-policy SampleConsortium/Org2MSP/Readers to Consortiums
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0b1 Returning policy Org2MSP/Writers for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:285 CommitProposals -> DEBU 0b2 In commit adding relative sub-policy SampleConsortium/Org2MSP/Writers to Consortiums
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0b3 Returning policy Org2MSP/Admins for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:285 CommitProposals -> DEBU 0b4 In commit adding relative sub-policy SampleConsortium/Org2MSP/Admins to Consortiums
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0b5 Returning policy Admins for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:285 CommitProposals -> DEBU 0b6 In commit adding relative sub-policy OrdererOrg/Admins to Orderer
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0b7 Returning policy Readers for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:285 CommitProposals -> DEBU 0b8 In commit adding relative sub-policy OrdererOrg/Readers to Orderer
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0b9 Returning policy Writers for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:285 CommitProposals -> DEBU 0ba In commit adding relative sub-policy OrdererOrg/Writers to Orderer
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0bb Returning policy Admins for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0bc Returning policy Writers for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0bd Returning policy Readers for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0be Returning policy Writers for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0bf Returning policy Admins for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:285 CommitProposals -> DEBU 0c0 In commit adding relative sub-policy Consortiums/Admins to Channel
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0c1 Returning policy SampleConsortium/Org1MSP/Readers for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:285 CommitProposals -> DEBU 0c2 In commit adding relative sub-policy Consortiums/SampleConsortium/Org1MSP/Readers to Channel
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0c3 Returning policy SampleConsortium/Org1MSP/Writers for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:285 CommitProposals -> DEBU 0c4 In commit adding relative sub-policy Consortiums/SampleConsortium/Org1MSP/Writers to Channel
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0c5 Returning policy SampleConsortium/Org1MSP/Admins for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:285 CommitProposals -> DEBU 0c6 In commit adding relative sub-policy Consortiums/SampleConsortium/Org1MSP/Admins to Channel
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0c7 Returning policy SampleConsortium/Org2MSP/Readers for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:285 CommitProposals -> DEBU 0c8 In commit adding relative sub-policy Consortiums/SampleConsortium/Org2MSP/Readers to Channel
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0c9 Returning policy SampleConsortium/Org2MSP/Writers for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:285 CommitProposals -> DEBU 0ca In commit adding relative sub-policy Consortiums/SampleConsortium/Org2MSP/Writers to Channel
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0cb Returning policy SampleConsortium/Org2MSP/Admins for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:285 CommitProposals -> DEBU 0cc In commit adding relative sub-policy Consortiums/SampleConsortium/Org2MSP/Admins to Channel
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0cd Returning policy Admins for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:285 CommitProposals -> DEBU 0ce In commit adding relative sub-policy Orderer/Admins to Channel
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0cf Returning policy BlockValidation for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:285 CommitProposals -> DEBU 0d0 In commit adding relative sub-policy Orderer/BlockValidation to Channel
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0d1 Returning policy Readers for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:285 CommitProposals -> DEBU 0d2 In commit adding relative sub-policy Orderer/Readers to Channel
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0d3 Returning policy Writers for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:285 CommitProposals -> DEBU 0d4 In commit adding relative sub-policy Orderer/Writers to Channel
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0d5 Returning policy OrdererOrg/Admins for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:285 CommitProposals -> DEBU 0d6 In commit adding relative sub-policy Orderer/OrdererOrg/Admins to Channel
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0d7 Returning policy OrdererOrg/Readers for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:285 CommitProposals -> DEBU 0d8 In commit adding relative sub-policy Orderer/OrdererOrg/Readers to Channel
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0d9 Returning policy OrdererOrg/Writers for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:285 CommitProposals -> DEBU 0da In commit adding relative sub-policy Orderer/OrdererOrg/Writers to Channel
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:221 GetPolicy -> DEBU 0db Returning dummy reject all policy because Readers could not be found in /Consortiums/Readers
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0dc Returning policy Readers for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:221 GetPolicy -> DEBU 0dd Returning dummy reject all policy because Writers could not be found in /Consortiums/Writers
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0de Returning policy Writers for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0df Returning policy Admins for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0e0 Returning policy Admins for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0e1 Returning policy Readers for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:303 CommitProposals -> DEBU 0e2 As expected, current configuration has policy '/Channel/Readers'
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0e3 Returning policy Writers for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:303 CommitProposals -> DEBU 0e4 As expected, current configuration has policy '/Channel/Writers'
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 0e5 Returning policy Orderer/BlockValidation for evaluation
2018-02-24 06:57:41.374 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:326 CommitProposals -> DEBU 0e6 As expected, current configuration has policy '/Channel/Orderer/BlockValidation'
2018-02-24 06:57:41.374 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_mgr.go:454 retrieveBlockByNumber -> DEBU 0e7 retrieveBlockByNumber() - blockNum = [0]
2018-02-24 06:57:41.374 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:66 newBlockfileStream -> DEBU 0e8 newBlockfileStream(): filePath=[/var/hyperledger/production/orderer/chains/testchainid/blockfile_000000], startOffset=[0]
2018-02-24 06:57:41.374 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:114 nextBlockBytesAndPlacementInfo -> DEBU 0e9 Remaining bytes=[9074], Going to peek [8] bytes
2018-02-24 06:57:41.374 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:147 nextBlockBytesAndPlacementInfo -> DEBU 0ea Returning blockbytes - length=[9072], placementInfo={fileNum=[0], startOffset=[0], bytesOffset=[2]}
2018-02-24 06:57:41.374 UTC [orderer/multichain] /opt/gopath/src/github.com/hyperledger/fabric/orderer/multichain/chainsupport.go:157 newChainSupport -> DEBU 0eb [channel: testchainid] Retrieved metadata for tip of chain (blockNumber=0, lastConfig=0, lastConfigSeq=0): 
2018-02-24 06:57:41.375 UTC [orderer/multichain] /opt/gopath/src/github.com/hyperledger/fabric/orderer/multichain/manager.go:114 NewManagerImpl -> INFO 0ec Starting with system channel testchainid and orderer type solo
2018-02-24 06:57:41.375 UTC [orderer/main] /opt/gopath/src/github.com/hyperledger/fabric/orderer/main.go:78 main -> INFO 0ed Beginning to serve requests

### create channel （order 日志）

{"log":"\u001b[36m2018-02-24 07:01:47.093 UTC [orderer/main] /opt/gopath/src/github.com/hyperledger/fabric/orderer/server.go:86 Deliver -\u003e DEBU 0ee\u001b[0m Starting new Deliver handler\n","stream":"stderr","time":"2018-02-24T07:01:47.093362982Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.093 UTC [orderer/common/deliver] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/deliver/deliver.go:73 Handle -\u003e DEBU 0ef\u001b[0m Starting new deliver loop\n","stream":"stderr","time":"2018-02-24T07:01:47.097351665Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.096 UTC [orderer/common/deliver] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/deliver/deliver.go:75 Handle -\u003e DEBU 0f0\u001b[0m Attempting to read seek info message\n","stream":"stderr","time":"2018-02-24T07:01:47.097374355Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.104 UTC [orderer/main] /opt/gopath/src/github.com/hyperledger/fabric/orderer/server.go:74 Broadcast -\u003e DEBU 0f1\u001b[0m Starting new Broadcast handler\n","stream":"stderr","time":"2018-02-24T07:01:47.105841466Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.104 UTC [orderer/common/broadcast] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/broadcast/broadcast.go:77 Handle -\u003e DEBU 0f2\u001b[0m Starting new broadcast loop\n","stream":"stderr","time":"2018-02-24T07:01:47.105899688Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.104 UTC [orderer/common/broadcast] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/broadcast/broadcast.go:107 Handle -\u003e DEBU 0f3\u001b[0m Preprocessing CONFIG_UPDATE\n","stream":"stderr","time":"2018-02-24T07:01:47.12442146Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.104 UTC [orderer/configupdate] /opt/gopath/src/github.com/hyperledger/fabric/orderer/configupdate/configupdate.go:116 Process -\u003e DEBU 0f4\u001b[0m Processing channel creation request for channel mychannel\n","stream":"stderr","time":"2018-02-24T07:01:47.124459538Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.105 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mgmt/mgmt.go:149 GetLocalMSP -\u003e DEBU 0f5\u001b[0m Returning existing local MSP\n","stream":"stderr","time":"2018-02-24T07:01:47.124479258Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.105 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:369 GetDefaultSigningIdentity -\u003e DEBU 0f6\u001b[0m Obtaining default signing identity\n","stream":"stderr","time":"2018-02-24T07:01:47.124484581Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.107 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mgmt/mgmt.go:149 GetLocalMSP -\u003e DEBU 0f7\u001b[0m Returning existing local MSP\n","stream":"stderr","time":"2018-02-24T07:01:47.124491774Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.107 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:369 GetDefaultSigningIdentity -\u003e DEBU 0f8\u001b[0m Obtaining default signing identity\n","stream":"stderr","time":"2018-02-24T07:01:47.124496366Z"}
...
config和police的日志

{"log":"\u001b[36m2018-02-24 07:01:47.413 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -\u003e DEBU 891\u001b[0m Returning policy Orderer/BlockValidation for evaluation\n","stream":"stderr","time":"2018-02-24T07:01:47.461191737Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.413 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:326 CommitProposals -\u003e DEBU 892\u001b[0m As expected, current configuration has policy '/Channel/Orderer/BlockValidation'\n","stream":"stderr","time":"2018-02-24T07:01:47.461195979Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.413 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_mgr.go:100 newBlockfileMgr -\u003e DEBU 893\u001b[0m newBlockfileMgr() initializing file-based block storage for ledger: mychannel \n","stream":"stderr","time":"2018-02-24T07:01:47.461200041Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.413 UTC [kvledger.util] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/util/ioutil.go:37 CreateDirIfMissing -\u003e DEBU 894\u001b[0m CreateDirIfMissing [/var/hyperledger/production/orderer/chains/mychannel/]\n","stream":"stderr","time":"2018-02-24T07:01:47.461204179Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.413 UTC [kvledger.util] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/util/ioutil.go:97 logDirStatus -\u003e DEBU 895\u001b[0m Before creating dir - [/var/hyperledger/production/orderer/chains/mychannel/] does not exist\n","stream":"stderr","time":"2018-02-24T07:01:47.461208284Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.413 UTC [kvledger.util] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/util/ioutil.go:95 logDirStatus -\u003e DEBU 896\u001b[0m After creating dir - [/var/hyperledger/production/orderer/chains/mychannel/] exists\n","stream":"stderr","time":"2018-02-24T07:01:47.46121233Z"}
{"log":"2018-02-24 07:01:47.413 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_mgr.go:118 newBlockfileMgr -\u003e INFO 897\u001b[0m Getting block information from block storage\n","stream":"stderr","time":"2018-02-24T07:01:47.46121637Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.413 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_helper.go:24 constructCheckpointInfoFromBlockFiles -\u003e DEBU 898\u001b[0m Retrieving checkpoint info from block files\n","stream":"stderr","time":"2018-02-24T07:01:47.461220688Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.413 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_helper.go:81 retrieveLastFileSuffix -\u003e DEBU 899\u001b[0m retrieveLastFileSuffix()\n","stream":"stderr","time":"2018-02-24T07:01:47.461225176Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.413 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_helper.go:102 retrieveLastFileSuffix -\u003e DEBU 89a\u001b[0m retrieveLastFileSuffix() - biggestFileNum = -1\n","stream":"stderr","time":"2018-02-24T07:01:47.4612414Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.413 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_helper.go:37 constructCheckpointInfoFromBlockFiles -\u003e DEBU 89b\u001b[0m Last file number found = -1\n","stream":"stderr","time":"2018-02-24T07:01:47.461246028Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.413 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_helper.go:41 constructCheckpointInfoFromBlockFiles -\u003e DEBU 89c\u001b[0m No block file found\n","stream":"stderr","time":"2018-02-24T07:01:47.461250168Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.413 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_mgr.go:122 newBlockfileMgr -\u003e DEBU 89d\u001b[0m Info constructed by scanning the blocks dir = (*fsblkstorage.checkpointInfo)(0xc42098b520)(latestFileChunkSuffixNum=[0], latestFileChunksize=[0], isChainEmpty=[true], lastBlockNumber=[0])\n","stream":"stderr","time":"2018-02-24T07:01:47.461254273Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.415 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockindex.go:72 newBlockIndex -\u003e DEBU 89e\u001b[0m newBlockIndex() - indexItems:[[BlockNum]]\n","stream":"stderr","time":"2018-02-24T07:01:47.46126389Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.415 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockindex.go:98 indexBlock -\u003e DEBU 89f\u001b[0m Indexing block [blockNum=0, blockHash=[]byte{0xbb, 0xd1, 0x65, 0xb8, 0x6, 0x6d, 0x5c, 0xf2, 0x70, 0x43, 0xaf, 0x62, 0x5e, 0x7e, 0x78, 0x90, 0x62, 0x37, 0x29, 0x90, 0x44, 0xf, 0xdf, 0x3b, 0x38, 0x14, 0x12, 0x5a, 0xf5, 0x17, 0x12, 0xba} txOffsets=\n","stream":"stderr","time":"2018-02-24T07:01:47.461268917Z"}
{"log":"txId= locPointer=offset=38, bytesLength=11869\n","stream":"stderr","time":"2018-02-24T07:01:47.46127352Z"}
{"log":"]\n","stream":"stderr","time":"2018-02-24T07:01:47.461277114Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.416 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_mgr.go:430 updateCheckpoint -\u003e DEBU 8a0\u001b[0m Broadcasting about update checkpointInfo: latestFileChunkSuffixNum=[0], latestFileChunksize=[11912], isChainEmpty=[false], lastBlockNumber=[0]\n","stream":"stderr","time":"2018-02-24T07:01:47.461280557Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.416 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_mgr.go:454 retrieveBlockByNumber -\u003e DEBU 8a1\u001b[0m retrieveBlockByNumber() - blockNum = [0]\n","stream":"stderr","time":"2018-02-24T07:01:47.461284773Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.416 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:66 newBlockfileStream -\u003e DEBU 8a2\u001b[0m newBlockfileStream(): filePath=[/var/hyperledger/production/orderer/chains/mychannel/blockfile_000000], startOffset=[0]\n","stream":"stderr","time":"2018-02-24T07:01:47.46128892Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.416 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:114 nextBlockBytesAndPlacementInfo -\u003e DEBU 8a3\u001b[0m Remaining bytes=[11912], Going to peek [8] bytes\n","stream":"stderr","time":"2018-02-24T07:01:47.461293133Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.417 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:147 nextBlockBytesAndPlacementInfo -\u003e DEBU 8a4\u001b[0m Returning blockbytes - length=[11910], placementInfo={fileNum=[0], startOffset=[0], bytesOffset=[2]}\n","stream":"stderr","time":"2018-02-24T07:01:47.461297222Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.417 UTC [orderer/multichain] /opt/gopath/src/github.com/hyperledger/fabric/orderer/multichain/chainsupport.go:157 newChainSupport -\u003e DEBU 8a5\u001b[0m [channel: mychannel] Retrieved metadata for tip of chain (blockNumber=0, lastConfig=0, lastConfigSeq=1): \n","stream":"stderr","time":"2018-02-24T07:01:47.46130142Z"}
{"log":"2018-02-24 07:01:47.417 UTC [orderer/multichain] /opt/gopath/src/github.com/hyperledger/fabric/orderer/multichain/manager.go:182 newChain -\u003e INFO 8a6\u001b[0m Created and starting new chain mychannel\n","stream":"stderr","time":"2018-02-24T07:01:47.461317636Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.417 UTC [orderer/multichain] /opt/gopath/src/github.com/hyperledger/fabric/orderer/multichain/chainsupport.go:228 addBlockSignature -\u003e DEBU 8a7\u001b[0m \u0026{ledgerResources:0xc4202dd900 chain:0xc4202ddd20 cutter:0xc4202dfc70 filters:0xc4202dd960 signer:0x128b4a8 lastConfig:0 lastConfigSeq:0}\n","stream":"stderr","time":"2018-02-24T07:01:47.461322844Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.417 UTC [orderer/multichain] /opt/gopath/src/github.com/hyperledger/fabric/orderer/multichain/chainsupport.go:229 addBlockSignature -\u003e DEBU 8a8\u001b[0m \u0026{}\n","stream":"stderr","time":"2018-02-24T07:01:47.461331108Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.417 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mgmt/mgmt.go:149 GetLocalMSP -\u003e DEBU 8a9\u001b[0m Returning existing local MSP\n","stream":"stderr","time":"2018-02-24T07:01:47.461335402Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.417 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:369 GetDefaultSigningIdentity -\u003e DEBU 8aa\u001b[0m Obtaining default signing identity\n","stream":"stderr","time":"2018-02-24T07:01:47.461339411Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.417 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mgmt/mgmt.go:149 GetLocalMSP -\u003e DEBU 8ab\u001b[0m Returning existing local MSP\n","stream":"stderr","time":"2018-02-24T07:01:47.461343317Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.417 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:369 GetDefaultSigningIdentity -\u003e DEBU 8ac\u001b[0m Obtaining default signing identity\n","stream":"stderr","time":"2018-02-24T07:01:47.461347183Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.417 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:208 Sign -\u003e DEBU 8ad\u001b[0m Sign: plaintext: 0AFF050A0A4F7264657265724D535012...2FF57759E35DDA9A07DB0915D8DF6C3F \n","stream":"stderr","time":"2018-02-24T07:01:47.461351122Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.417 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:210 Sign -\u003e DEBU 8ae\u001b[0m Sign: digest: 8807E23A6F5F8BDEDBE78C1CE36A600175C2AA442C490A2396AC87518320993D \n","stream":"stderr","time":"2018-02-24T07:01:47.461355313Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.417 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mgmt/mgmt.go:149 GetLocalMSP -\u003e DEBU 8af\u001b[0m Returning existing local MSP\n","stream":"stderr","time":"2018-02-24T07:01:47.461359357Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.417 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:369 GetDefaultSigningIdentity -\u003e DEBU 8b0\u001b[0m Obtaining default signing identity\n","stream":"stderr","time":"2018-02-24T07:01:47.461363213Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.417 UTC [orderer/multichain] /opt/gopath/src/github.com/hyperledger/fabric/orderer/multichain/chainsupport.go:262 addLastConfigSignature -\u003e DEBU 8b1\u001b[0m [channel: testchainid] About to write block, setting its LAST_CONFIG to 0\n","stream":"stderr","time":"2018-02-24T07:01:47.461369213Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.417 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mgmt/mgmt.go:149 GetLocalMSP -\u003e DEBU 8b2\u001b[0m Returning existing local MSP\n","stream":"stderr","time":"2018-02-24T07:01:47.46137348Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.417 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:369 GetDefaultSigningIdentity -\u003e DEBU 8b3\u001b[0m Obtaining default signing identity\n","stream":"stderr","time":"2018-02-24T07:01:47.461377367Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.418 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:208 Sign -\u003e DEBU 8b4\u001b[0m Sign: plaintext: 0AFF050A0A4F7264657265724D535012...2FF57759E35DDA9A07DB0915D8DF6C3F \n","stream":"stderr","time":"2018-02-24T07:01:47.46138134Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.418 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:210 Sign -\u003e DEBU 8b5\u001b[0m Sign: digest: B925919A4D1E342B34C7CFC0983669B7DE92825445EED39981708F3BF8A41C97 \n","stream":"stderr","time":"2018-02-24T07:01:47.461397624Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.419 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockindex.go:98 indexBlock -\u003e DEBU 8b6\u001b[0m Indexing block [blockNum=1, blockHash=[]byte{0x2b, 0x9, 0x95, 0xf8, 0xe5, 0x2e, 0x4a, 0xbd, 0x93, 0x37, 0x31, 0xc2, 0xc1, 0x8e, 0xba, 0xf7, 0x87, 0x42, 0x9a, 0x25, 0x11, 0x37, 0x86, 0x78, 0xf6, 0x7d, 0x36, 0x98, 0x59, 0x52, 0x2f, 0x89} txOffsets=\n","stream":"stderr","time":"2018-02-24T07:01:47.461401973Z"}
{"log":"txId= locPointer=offset=70, bytesLength=12774\n","stream":"stderr","time":"2018-02-24T07:01:47.46141028Z"}
{"log":"]\n","stream":"stderr","time":"2018-02-24T07:01:47.461414073Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.419 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_mgr.go:430 updateCheckpoint -\u003e DEBU 8b7\u001b[0m Broadcasting about update checkpointInfo: latestFileChunkSuffixNum=[0], latestFileChunksize=[23673], isChainEmpty=[false], lastBlockNumber=[1]\n","stream":"stderr","time":"2018-02-24T07:01:47.46141745Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.420 UTC [orderer/multichain] /opt/gopath/src/github.com/hyperledger/fabric/orderer/multichain/chainsupport.go:289 WriteBlock -\u003e DEBU 8b8\u001b[0m [channel: testchainid] Wrote block 1\n","stream":"stderr","time":"2018-02-24T07:01:47.461421804Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.567 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -\u003e DEBU 8b9\u001b[0m Returning policy Readers for evaluation\n","stream":"stderr","time":"2018-02-24T07:01:47.568625577Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.567 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:58 func1 -\u003e DEBU 8ba\u001b[0m 0xc420a5d3a0 gate 1519455707567149138 evaluation starts\n","stream":"stderr","time":"2018-02-24T07:01:47.568677288Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.567 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:83 func2 -\u003e DEBU 8bb\u001b[0m 0xc420a5d3a0 signed by 0 principal evaluation starts (used [false])\n","stream":"stderr","time":"2018-02-24T07:01:47.568684353Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.567 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:91 func2 -\u003e DEBU 8bc\u001b[0m 0xc420a5d3a0 processing identity 0 with bytes of 0a074f7267314d53501280062d2d2d2d2d424547494e202d2d2d2d2d0a4d4949434754434341622b6741774942416749515532454561656b5a71314c734936625a45644c39727a414b42676771686b6a4f50515144416a427a4d5173770a435159445651514745774a56557a45544d4245474131554543424d4b5132467361575a76636d3570595445574d4251474131554542784d4e5532467549455a790a5957356a61584e6a627a455a4d4263474131554543684d5162334a6e4d53356c654746746347786c4c6d4e76625445634d426f474131554541784d54593245750a62334a6e4d53356c654746746347786c4c6d4e7662544165467730784f4441794d4463774d4445314d545261467730794f4441794d4455774d4445314d5452610a4d467378437a414a42674e5642415954416c56544d524d77455159445651514945777044595778705a6d3979626d6c684d5259774641594456515148457731540a59573467526e4a68626d4e7063324e764d523877485159445651514444425a425a473170626b4276636d63784c6d56345957317762475575593239744d466b770a457759484b6f5a497a6a3043415159494b6f5a497a6a3044415163445167414530316b55773456592f5973724d744c38655a785a63686e42757a5a4e504f666a0a4a4e39374d3738777844475756566e484468724d46656d434a3072334d4c4a77324a2f414a383639793948645572445758306c6f2b364e4e4d457377446759440a565230504151482f42415144416765414d41774741315564457745422f7751434d4141774b7759445652306a42435177496f416754385951514a54664f5658490a456b364a57304a7471664e4c4e355a73396950584f63334c454d4a4759453077436759494b6f5a497a6a3045417749445341417752514968414a70767472364a0a454b6b53696b783830324553476d776478656a6d5532485434766a7953556c7559435337416942414f3762503358327349465943462f41393462426a666351520a584e76594a71504a2b592f7a4c596f3250513d3d0a2d2d2d2d2d454e44202d2d2d2d2d0a\n","stream":"stderr","time":"2018-02-24T07:01:47.568690596Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.567 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:42 newIdentity -\u003e DEBU 8bd\u001b[0m Creating identity instance for ID -----BEGIN CERTIFICATE-----\n","stream":"stderr","time":"2018-02-24T07:01:47.568698863Z"}
{"log":"MIICGTCCAb+gAwIBAgIQU2EEaekZq1LsI6bZEdL9rzAKBggqhkjOPQQDAjBzMQsw\n","stream":"stderr","time":"2018-02-24T07:01:47.568721467Z"}
{"log":"CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy\n","stream":"stderr","time":"2018-02-24T07:01:47.568727634Z"}
{"log":"YW5jaXNjbzEZMBcGA1UEChMQb3JnMS5leGFtcGxlLmNvbTEcMBoGA1UEAxMTY2Eu\n","stream":"stderr","time":"2018-02-24T07:01:47.568732345Z"}
{"log":"b3JnMS5leGFtcGxlLmNvbTAeFw0xODAyMDcwMDE1MTRaFw0yODAyMDUwMDE1MTRa\n","stream":"stderr","time":"2018-02-24T07:01:47.568736097Z"}
{"log":"MFsxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1T\n","stream":"stderr","time":"2018-02-24T07:01:47.56873973Z"}
{"log":"YW4gRnJhbmNpc2NvMR8wHQYDVQQDDBZBZG1pbkBvcmcxLmV4YW1wbGUuY29tMFkw\n","stream":"stderr","time":"2018-02-24T07:01:47.568743442Z"}
{"log":"EwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE01kUw4VY/YsrMtL8eZxZchnBuzZNPOfj\n","stream":"stderr","time":"2018-02-24T07:01:47.568747139Z"}
{"log":"JN97M78wxDGWVVnHDhrMFemCJ0r3MLJw2J/AJ869y9HdUrDWX0lo+6NNMEswDgYD\n","stream":"stderr","time":"2018-02-24T07:01:47.56875112Z"}
{"log":"VR0PAQH/BAQDAgeAMAwGA1UdEwEB/wQCMAAwKwYDVR0jBCQwIoAgT8YQQJTfOVXI\n","stream":"stderr","time":"2018-02-24T07:01:47.568762928Z"}
{"log":"Ek6JW0JtqfNLN5Zs9iPXOc3LEMJGYE0wCgYIKoZIzj0EAwIDSAAwRQIhAJpvtr6J\n","stream":"stderr","time":"2018-02-24T07:01:47.568767576Z"}
{"log":"EKkSikx802ESGmwdxejmU2HT4vjySUluYCS7AiBAO7bP3X2sIFYCF/A94bBjfcQR\n","stream":"stderr","time":"2018-02-24T07:01:47.568771408Z"}
{"log":"XNvYJqPJ+Y/zLYo2PQ==\n","stream":"stderr","time":"2018-02-24T07:01:47.568775039Z"}
{"log":"-----END CERTIFICATE-----\n","stream":"stderr","time":"2018-02-24T07:01:47.568778705Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.567 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:489 SatisfiesPrincipal -\u003e DEBU 8be\u001b[0m Checking if identity satisfies MEMBER role for Org1MSP\n","stream":"stderr","time":"2018-02-24T07:01:47.568782288Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.567 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:391 Validate -\u003e DEBU 8bf\u001b[0m MSP Org1MSP validating identity\n","stream":"stderr","time":"2018-02-24T07:01:47.56878649Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.568 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:103 func2 -\u003e DEBU 8c0\u001b[0m 0xc420a5d3a0 principal matched by identity 0\n","stream":"stderr","time":"2018-02-24T07:01:47.568790553Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.568 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:129 Verify -\u003e DEBU 8c1\u001b[0m Verify: digest = 00000000  7a dd 9f 3d 7b cb 1e 75  46 c5 e5 eb da 7a 70 44  |z..={..uF....zpD|\n","stream":"stderr","time":"2018-02-24T07:01:47.568794645Z"}
{"log":"00000010  da e8 ee 2a 70 09 fe 07  fe f4 e0 10 28 a0 b5 ae  |...*p.......(...|\n","stream":"stderr","time":"2018-02-24T07:01:47.56879952Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.568 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:130 Verify -\u003e DEBU 8c2\u001b[0m Verify: sig = 00000000  30 45 02 21 00 fc 0a 76  44 ae 5d da 95 1f 5f 77  |0E.!...vD.]..._w|\n","stream":"stderr","time":"2018-02-24T07:01:47.568803436Z"}
{"log":"00000010  e7 d8 0c e2 3e 56 91 e6  59 a9 0c 7a 46 21 f7 a9  |....\u003eV..Y..zF!..|\n","stream":"stderr","time":"2018-02-24T07:01:47.568807579Z"}
{"log":"00000020  75 be 1a ff f1 02 20 64  0f 4e e2 7c 58 fd c5 9c  |u..... d.N.|X...|\n","stream":"stderr","time":"2018-02-24T07:01:47.56881149Z"}
{"log":"00000030  45 e4 89 c7 9e 4f d5 ba  9f 7e d2 d4 15 67 f5 1f  |E....O...~...g..|\n","stream":"stderr","time":"2018-02-24T07:01:47.568815219Z"}
{"log":"00000040  75 9c 4f 50 cf 31 7f                              |u.OP.1.|\n","stream":"stderr","time":"2018-02-24T07:01:47.568818902Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.568 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:109 func2 -\u003e DEBU 8c3\u001b[0m 0xc420a5d3a0 principal evaluation succeeds for identity 0\n","stream":"stderr","time":"2018-02-24T07:01:47.568827268Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.568 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:70 func1 -\u003e DEBU 8c4\u001b[0m 0xc420a5d3a0 gate 1519455707567149138 evaluation succeeds\n","stream":"stderr","time":"2018-02-24T07:01:47.569351579Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.568 UTC [orderer/common/sigfilter] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/sigfilter/sigfilter.go:69 Apply -\u003e DEBU 8c5\u001b[0m Forwarding validly signed message for policy \u0026{%!s(*common.ImplicitMetaPolicy=\u0026{Readers 0}) %!s(int=1) [%!s(*policies.implicitMetaPolicy=\u0026{0xc4209ae100 1 [0xc420a5d0f8 0xc420a5d1b8]}) %!s(*policies.implicitMetaPolicy=\u0026{0xc4209af0e0 1 [0xc420a5d230]})]}\n","stream":"stderr","time":"2018-02-24T07:01:47.569364757Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.569 UTC [orderer/common/deliver] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/deliver/deliver.go:141 Handle -\u003e DEBU 8c6\u001b[0m [channel: mychannel] Received seekInfo (0xc420973c60) start:\u003cspecified:\u003c\u003e \u003e stop:\u003cspecified:\u003c\u003e \u003e \n","stream":"stderr","time":"2018-02-24T07:01:47.569370199Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.569 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_mgr.go:454 retrieveBlockByNumber -\u003e DEBU 8c7\u001b[0m retrieveBlockByNumber() - blockNum = [0]\n","stream":"stderr","time":"2018-02-24T07:01:47.579738664Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.569 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:66 newBlockfileStream -\u003e DEBU 8c8\u001b[0m newBlockfileStream(): filePath=[/var/hyperledger/production/orderer/chains/mychannel/blockfile_000000], startOffset=[0]\n","stream":"stderr","time":"2018-02-24T07:01:47.579761733Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.569 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:114 nextBlockBytesAndPlacementInfo -\u003e DEBU 8c9\u001b[0m Remaining bytes=[11912], Going to peek [8] bytes\n","stream":"stderr","time":"2018-02-24T07:01:47.579767953Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.571 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:147 nextBlockBytesAndPlacementInfo -\u003e DEBU 8ca\u001b[0m Returning blockbytes - length=[11910], placementInfo={fileNum=[0], startOffset=[0], bytesOffset=[2]}\n","stream":"stderr","time":"2018-02-24T07:01:47.579772433Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.571 UTC [orderer/common/deliver] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/deliver/deliver.go:191 Handle -\u003e DEBU 8cb\u001b[0m [channel: mychannel] Delivering block for (0xc420973c60)\n","stream":"stderr","time":"2018-02-24T07:01:47.579776873Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.579 UTC [orderer/common/deliver] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/deliver/deliver.go:208 Handle -\u003e DEBU 8cc\u001b[0m [channel: mychannel] Done delivering for (0xc420973c60), waiting for new SeekInfo\n","stream":"stderr","time":"2018-02-24T07:01:47.59065682Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.579 UTC [orderer/common/deliver] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/deliver/deliver.go:75 Handle -\u003e DEBU 8cd\u001b[0m Attempting to read seek info message\n","stream":"stderr","time":"2018-02-24T07:01:47.590709701Z"}
{"log":"\u001b[33m2018-02-24 07:01:47.579 UTC [orderer/common/deliver] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/deliver/deliver.go:83 Handle -\u003e WARN 8ce\u001b[0m Error reading from stream: rpc error: code = Canceled desc = context canceled\n","stream":"stderr","time":"2018-02-24T07:01:47.608070326Z"}
{"log":"\u001b[36m2018-02-24 07:01:47.583 UTC [orderer/main] /opt/gopath/src/github.com/hyperledger/fabric/orderer/server.go:91 func1 -\u003e DEBU 8cf\u001b[0m Closing Deliver stream\n","stream":"stderr","time":"2018-02-24T07:01:47.608112811Z"}
```

### join channel (peer 日志)

```
2018-02-24 07:15:47.476 UTC [endorser.go:373] [endorser] ProcessProposal -> DEBU 1ca Entry
2018-02-24 07:15:47.480 UTC [msgvalidation.go:79] [protoutils] ValidateProposalMessage -> DEBU 1cb ValidateProposalMessage starts for signed proposal 0xc4202b1380
2018-02-24 07:15:47.480 UTC [msgvalidation.go:215] [protoutils] validateChannelHeader -> DEBU 1cc validateChannelHeader info: header type 1
2018-02-24 07:15:47.480 UTC [msgvalidation.go:142] [protoutils] checkSignatureFromCreator -> DEBU 1cd checkSignatureFromCreator starts
2018-02-24 07:15:47.483 UTC [msgvalidation.go:160] [protoutils] checkSignatureFromCreator -> DEBU 1ce checkSignatureFromCreator info: creator is &{Org1MSP 3dc70c07d3096e889a28bca219d07b0d858baede9b7d60760c9bf5a76017c9a1}
2018-02-24 07:15:47.483 UTC [msgvalidation.go:168] [protoutils] checkSignatureFromCreator -> DEBU 1cf checkSignatureFromCreator info: creator is valid
2018-02-24 07:15:47.484 UTC [msgvalidation.go:176] [protoutils] checkSignatureFromCreator -> DEBU 1d0 checkSignatureFromCreator exists successfully
2018-02-24 07:15:47.484 UTC [msgvalidation.go:39] [protoutils] validateChaincodeProposalMessage -> DEBU 1d1 validateChaincodeProposalMessage starts for proposal 0xc4201a5ea0, header 0xc4202b1410
2018-02-24 07:15:47.484 UTC [msgvalidation.go:51] [protoutils] validateChaincodeProposalMessage -> DEBU 1d2 validateChaincodeProposalMessage info: header extension references chaincode name:"cscc" 
2018-02-24 07:15:47.484 UTC [endorser.go:412] [endorser] ProcessProposal -> DEBU 1d3 processing txid: 72427d9ece8288e07287264351927092d178edf727eea249b65fa393d953aab0
2018-02-24 07:15:47.484 UTC [endorser.go:441] [endorser] ProcessProposal -> DEBU 1d4 [gerry] to get TxSimulator and HistoryQueryExecutor
2018-02-24 07:15:47.484 UTC [endorser.go:472] [endorser] ProcessProposal -> DEBU 1d5 [gerry] 1 -- simulate
2018-02-24 07:15:47.484 UTC [endorser.go:211] [endorser] simulateProposal -> DEBU 1d6 Entry - txid: 72427d9ece8288e07287264351927092d178edf727eea249b65fa393d953aab0 channel id: 
2018-02-24 07:15:47.484 UTC [endorser.go:99] [endorser] callChaincode -> DEBU 1d7 Entry - txid: 72427d9ece8288e07287264351927092d178edf727eea249b65fa393d953aab0 channel id:  version: 1.0.5
2018-02-24 07:15:47.484 UTC [ccprovider.go:363] [ccprovider] NewCCContext -> DEBU 1d8 NewCCCC (chain=,chaincode=cscc,version=1.0.5,txid=72427d9ece8288e07287264351927092d178edf727eea249b65fa393d953aab0,syscc=true,proposal=0xc4201a5ea0,canname=cscc:1.0.5
2018-02-24 07:15:47.484 UTC [chaincode_support.go:585] [chaincode] Launch -> DEBU 1d9 chaincode is running(no need to launch) : cscc:1.0.5
2018-02-24 07:15:47.484 UTC [chaincode_support.go:720] [chaincode] Execute -> DEBU 1da Entry
2018-02-24 07:15:47.484 UTC [chaincode_support.go:723] [chaincode] Execute -> DEBU 1db chaincode canonical name: cscc:1.0.5
2018-02-24 07:15:47.485 UTC [handler.go:1436] [chaincode] sendExecuteMessage -> DEBU 1dc [72427d9e]Inside sendExecuteMessage. Message TRANSACTION
2018-02-24 07:15:47.485 UTC [handler.go:1350] [chaincode] setChaincodeProposal -> DEBU 1dd Setting chaincode proposal context...
2018-02-24 07:15:47.485 UTC [handler.go:1352] [chaincode] setChaincodeProposal -> DEBU 1de Proposal different from nil. Creating chaincode proposal context...
2018-02-24 07:15:47.485 UTC [handler.go:1444] [chaincode] sendExecuteMessage -> DEBU 1df [72427d9e]sendExecuteMsg trigger event TRANSACTION
2018-02-24 07:15:47.485 UTC [handler.go:357] [chaincode] processStream -> DEBU 1e0 [72427d9e]Move state message TRANSACTION
2018-02-24 07:15:47.486 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 1e1 [72427d9e]Fabric side Handling ChaincodeMessage of type: TRANSACTION in state ready
2018-02-24 07:15:47.486 UTC [handler.go:1417] [chaincode] filterError -> DEBU 1e2 Ignoring NoTransitionError: no transition
2018-02-24 07:15:47.486 UTC [handler.go:377] [chaincode] processStream -> DEBU 1e3 [72427d9e]sending state message TRANSACTION
2018-02-24 07:15:47.486 UTC [chaincode.go:297] [shim] func1 -> DEBU 1e4 [72427d9e]Received message TRANSACTION from shim
2018-02-24 07:15:47.486 UTC [handler.go:778] [shim] handleMessage -> DEBU 1e5 [72427d9e]Handling ChaincodeMessage of type: TRANSACTION(state:ready)
2018-02-24 07:15:47.486 UTC [handler.go:338] [shim] beforeTransaction -> DEBU 1e6 [72427d9e]Received TRANSACTION, invoking transaction on chaincode(Src:ready, Dst:ready)
2018-02-24 07:15:47.487 UTC [configure.go:97] [cscc] Invoke -> DEBU 1e7 Invoke function: JoinChain
2018-02-24 07:15:47.488 UTC [ledger_mgmt.go:82] [ledgermgmt] CreateLedger -> INFO 1e8 Creating ledger [mychannel] with genesis block
2018-02-24 07:15:47.491 UTC [blockfile_mgr.go:100] [fsblkstorage] newBlockfileMgr -> DEBU 1e9 newBlockfileMgr() initializing file-based block storage for ledger: mychannel 
2018-02-24 07:15:47.491 UTC [ioutil.go:37] [kvledger.util] CreateDirIfMissing -> DEBU 1ea CreateDirIfMissing [/var/hyperledger/production/ledgersData/chains/chains/mychannel/]
2018-02-24 07:15:47.491 UTC [ioutil.go:97] [kvledger.util] logDirStatus -> DEBU 1eb Before creating dir - [/var/hyperledger/production/ledgersData/chains/chains/mychannel/] does not exist
2018-02-24 07:15:47.492 UTC [ioutil.go:95] [kvledger.util] logDirStatus -> DEBU 1ec After creating dir - [/var/hyperledger/production/ledgersData/chains/chains/mychannel/] exists
2018-02-24 07:15:47.492 UTC [blockfile_mgr.go:118] [fsblkstorage] newBlockfileMgr -> INFO 1ed Getting block information from block storage
2018-02-24 07:15:47.492 UTC [blockfile_helper.go:24] [fsblkstorage] constructCheckpointInfoFromBlockFiles -> DEBU 1ee Retrieving checkpoint info from block files
2018-02-24 07:15:47.492 UTC [blockfile_helper.go:81] [fsblkstorage] retrieveLastFileSuffix -> DEBU 1ef retrieveLastFileSuffix()
2018-02-24 07:15:47.492 UTC [blockfile_helper.go:102] [fsblkstorage] retrieveLastFileSuffix -> DEBU 1f0 retrieveLastFileSuffix() - biggestFileNum = -1
2018-02-24 07:15:47.492 UTC [blockfile_helper.go:37] [fsblkstorage] constructCheckpointInfoFromBlockFiles -> DEBU 1f1 Last file number found = -1
2018-02-24 07:15:47.492 UTC [blockfile_helper.go:41] [fsblkstorage] constructCheckpointInfoFromBlockFiles -> DEBU 1f2 No block file found
2018-02-24 07:15:47.492 UTC [blockfile_mgr.go:122] [fsblkstorage] newBlockfileMgr -> DEBU 1f3 Info constructed by scanning the blocks dir = (*fsblkstorage.checkpointInfo)(0xc420134ba0)(latestFileChunkSuffixNum=[0], latestFileChunksize=[0], isChainEmpty=[true], lastBlockNumber=[0])
2018-02-24 07:15:47.493 UTC [blockindex.go:72] [fsblkstorage] newBlockIndex -> DEBU 1f4 newBlockIndex() - indexItems:[[BlockHash BlockNum TxID BlockNumTranNum BlockTxID TxValidationCode]]
2018-02-24 07:15:47.494 UTC [kv_ledger.go:51] [kvledger] newKVLedger -> DEBU 1f5 Creating KVLedger ledgerID=mychannel: 
2018-02-24 07:15:47.494 UTC [kv_ledger.go:72] [kvledger] recoverDBs -> DEBU 1f6 Entering recoverDB()
2018-02-24 07:15:47.494 UTC [kv_ledger.go:76] [kvledger] recoverDBs -> DEBU 1f7 Block storage is empty.
2018-02-24 07:15:47.494 UTC [kv_ledger.go:215] [kvledger] Commit -> DEBU 1f8 Channel [mychannel]: Validating block [0]
2018-02-24 07:15:47.494 UTC [lockbased_txmgr.go:72] [lockbasedtxmgr] ValidateAndPrepare -> DEBU 1f9 Validating new block with num trans = [1]
2018-02-24 07:15:47.494 UTC [state_based_validator.go:78] [statevalidator] ValidateAndPrepareBatch -> DEBU 1fa New block arrived for validation:&common.Block{Header:(*common.BlockHeader)(0xc4203353c0), Data:(*common.BlockData)(0xc4202ae2e0), Metadata:(*common.BlockMetadata)(0xc4202ae320)}, doMVCCValidation=true
2018-02-24 07:15:47.494 UTC [state_based_validator.go:80] [statevalidator] ValidateAndPrepareBatch -> DEBU 1fb Validating a block with [1] transactions
2018-02-24 07:15:47.494 UTC [state_based_validator.go:118] [statevalidator] ValidateAndPrepareBatch -> DEBU 1fc Skipping mvcc validation for Block [0] Transaction index [0] because, the transaction type is [CONFIG]
2018-02-24 07:15:47.494 UTC [kv_ledger.go:221] [kvledger] Commit -> DEBU 1fd Channel [mychannel]: Committing block [0] to storage
2018-02-24 07:15:47.495 UTC [blockindex.go:98] [fsblkstorage] indexBlock -> DEBU 1fe Indexing block [blockNum=0, blockHash=[]byte{0xbb, 0xd1, 0x65, 0xb8, 0x6, 0x6d, 0x5c, 0xf2, 0x70, 0x43, 0xaf, 0x62, 0x5e, 0x7e, 0x78, 0x90, 0x62, 0x37, 0x29, 0x90, 0x44, 0xf, 0xdf, 0x3b, 0x38, 0x14, 0x12, 0x5a, 0xf5, 0x17, 0x12, 0xba} txOffsets=
txId= locPointer=offset=38, bytesLength=11869
]
2018-02-24 07:15:47.495 UTC [blockindex.go:122] [fsblkstorage] indexBlock -> DEBU 1ff Adding txLoc [fileSuffixNum=0, offset=38, bytesLength=11869] for tx ID: [] to index
2018-02-24 07:15:47.495 UTC [blockindex.go:135] [fsblkstorage] indexBlock -> DEBU 200 Adding txLoc [fileSuffixNum=0, offset=38, bytesLength=11869] for tx number:[0] ID: [] to blockNumTranNum index
2018-02-24 07:15:47.496 UTC [blockfile_mgr.go:430] [fsblkstorage] updateCheckpoint -> DEBU 201 Broadcasting about update checkpointInfo: latestFileChunkSuffixNum=[0], latestFileChunksize=[11913], isChainEmpty=[false], lastBlockNumber=[0]
2018-02-24 07:15:47.496 UTC [kv_ledger.go:225] [kvledger] Commit -> INFO 202 Channel [mychannel]: Created block [0] with 1 transaction(s)
2018-02-24 07:15:47.496 UTC [kv_ledger.go:227] [kvledger] Commit -> DEBU 203 Channel [mychannel]: Committing block [0] transactions to state database
2018-02-24 07:15:47.496 UTC [lockbased_txmgr.go:89] [lockbasedtxmgr] Commit -> DEBU 204 Committing updates to state database
2018-02-24 07:15:47.496 UTC [lockbased_txmgr.go:92] [lockbasedtxmgr] Commit -> DEBU 205 Write lock acquired for committing updates to state database
2018-02-24 07:15:47.497 UTC [lockbased_txmgr.go:101] [lockbasedtxmgr] Commit -> DEBU 206 Updates committed to state database
2018-02-24 07:15:47.497 UTC [kv_ledger.go:234] [kvledger] Commit -> DEBU 207 Channel [mychannel]: Committing block [0] transactions to history database
2018-02-24 07:15:47.497 UTC [historyleveldb.go:93] [historyleveldb] Commit -> DEBU 208 Channel [mychannel]: Updating history database for blockNo [0] with [1] transactions
2018-02-24 07:15:47.497 UTC [historyleveldb.go:162] [historyleveldb] Commit -> DEBU 209 Skipping transaction [0] since it is not an endorsement transaction
2018-02-24 07:15:47.498 UTC [historyleveldb.go:177] [historyleveldb] Commit -> DEBU 20a Channel [mychannel]: Updates committed to history database for blockNo [0]
2018-02-24 07:15:47.499 UTC [ledger_mgmt.go:89] [ledgermgmt] CreateLedger -> INFO 20b Created ledger [mychannel] with genesis block
2018-02-24 07:15:47.499 UTC [standardvalues.go:41] [common/config] NewStandardValues -> DEBU 20c Initializing protos for *config.ChannelProtos
2018-02-24 07:15:47.499 UTC [standardvalues.go:79] [common/config] initializeProtosStruct -> DEBU 20d Processing field: HashingAlgorithm
2018-02-24 07:15:47.499 UTC [standardvalues.go:79] [common/config] initializeProtosStruct -> DEBU 20e Processing field: BlockDataHashingStructure
2018-02-24 07:15:47.499 UTC [standardvalues.go:79] [common/config] initializeProtosStruct -> DEBU 20f Processing field: OrdererAddresses
2018-02-24 07:15:47.499 UTC [standardvalues.go:79] [common/config] initializeProtosStruct -> DEBU 210 Processing field: Consortium
2018-02-24 07:15:47.499 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 211 Adding to config map: [Groups] /Channel
2018-02-24 07:15:47.499 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 212 Adding to config map: [Groups] /Channel/Orderer
2018-02-24 07:15:47.499 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 213 Adding to config map: [Groups] /Channel/Orderer/OrdererOrg
2018-02-24 07:15:47.500 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 214 Adding to config map: [Values] /Channel/Orderer/OrdererOrg/MSP
2018-02-24 07:15:47.500 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 215 Adding to config map: [Policy] /Channel/Orderer/OrdererOrg/Admins
2018-02-24 07:15:47.500 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 216 Adding to config map: [Policy] /Channel/Orderer/OrdererOrg/Readers
2018-02-24 07:15:47.500 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 217 Adding to config map: [Policy] /Channel/Orderer/OrdererOrg/Writers
2018-02-24 07:15:47.500 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 218 Adding to config map: [Values] /Channel/Orderer/ConsensusType
2018-02-24 07:15:47.500 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 219 Adding to config map: [Values] /Channel/Orderer/BatchSize
2018-02-24 07:15:47.500 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 21a Adding to config map: [Values] /Channel/Orderer/BatchTimeout
2018-02-24 07:15:47.500 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 21b Adding to config map: [Values] /Channel/Orderer/ChannelRestrictions
2018-02-24 07:15:47.500 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 21c Adding to config map: [Policy] /Channel/Orderer/Admins
2018-02-24 07:15:47.500 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 21d Adding to config map: [Policy] /Channel/Orderer/BlockValidation
2018-02-24 07:15:47.500 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 21e Adding to config map: [Policy] /Channel/Orderer/Readers
2018-02-24 07:15:47.501 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 21f Adding to config map: [Policy] /Channel/Orderer/Writers
2018-02-24 07:15:47.501 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 220 Adding to config map: [Groups] /Channel/Application
2018-02-24 07:15:47.501 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 221 Adding to config map: [Groups] /Channel/Application/Org1MSP
2018-02-24 07:15:47.501 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 222 Adding to config map: [Values] /Channel/Application/Org1MSP/MSP
2018-02-24 07:15:47.501 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 223 Adding to config map: [Policy] /Channel/Application/Org1MSP/Readers
2018-02-24 07:15:47.501 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 224 Adding to config map: [Policy] /Channel/Application/Org1MSP/Writers
2018-02-24 07:15:47.501 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 225 Adding to config map: [Policy] /Channel/Application/Org1MSP/Admins
2018-02-24 07:15:47.501 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 226 Adding to config map: [Groups] /Channel/Application/Org2MSP
2018-02-24 07:15:47.501 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 227 Adding to config map: [Values] /Channel/Application/Org2MSP/MSP
2018-02-24 07:15:47.501 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 228 Adding to config map: [Policy] /Channel/Application/Org2MSP/Readers
2018-02-24 07:15:47.502 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 229 Adding to config map: [Policy] /Channel/Application/Org2MSP/Writers
2018-02-24 07:15:47.502 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 22a Adding to config map: [Policy] /Channel/Application/Org2MSP/Admins
2018-02-24 07:15:47.502 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 22b Adding to config map: [Policy] /Channel/Application/Readers
2018-02-24 07:15:47.502 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 22c Adding to config map: [Policy] /Channel/Application/Admins
2018-02-24 07:15:47.502 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 22d Adding to config map: [Policy] /Channel/Application/Writers
2018-02-24 07:15:47.502 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 22e Adding to config map: [Values] /Channel/HashingAlgorithm
2018-02-24 07:15:47.502 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 22f Adding to config map: [Values] /Channel/BlockDataHashingStructure
2018-02-24 07:15:47.502 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 230 Adding to config map: [Values] /Channel/OrdererAddresses
2018-02-24 07:15:47.502 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 231 Adding to config map: [Values] /Channel/Consortium
2018-02-24 07:15:47.503 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 232 Adding to config map: [Policy] /Channel/Readers
2018-02-24 07:15:47.503 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 233 Adding to config map: [Policy] /Channel/Writers
2018-02-24 07:15:47.503 UTC [configmap.go:74] [common/configtx] addToMap -> DEBU 234 Adding to config map: [Policy] /Channel/Admins
2018-02-24 07:15:47.503 UTC [config.go:269] [common/configtx] processConfig -> DEBU 235 Beginning new config for channel mychannel
2018-02-24 07:15:47.503 UTC [standardvalues.go:41] [common/config] NewStandardValues -> DEBU 236 Initializing protos for *config.ChannelProtos
2018-02-24 07:15:47.503 UTC [standardvalues.go:79] [common/config] initializeProtosStruct -> DEBU 237 Processing field: HashingAlgorithm
2018-02-24 07:15:47.504 UTC [standardvalues.go:79] [common/config] initializeProtosStruct -> DEBU 238 Processing field: BlockDataHashingStructure
2018-02-24 07:15:47.504 UTC [standardvalues.go:79] [common/config] initializeProtosStruct -> DEBU 239 Processing field: OrdererAddresses
2018-02-24 07:15:47.504 UTC [standardvalues.go:79] [common/config] initializeProtosStruct -> DEBU 23a Processing field: Consortium
2018-02-24 07:15:47.505 UTC [standardvalues.go:41] [common/config] NewStandardValues -> DEBU 23b Initializing protos for *config.OrdererProtos
2018-02-24 07:15:47.505 UTC [standardvalues.go:79] [common/config] initializeProtosStruct -> DEBU 23c Processing field: ConsensusType
2018-02-24 07:15:47.505 UTC [standardvalues.go:79] [common/config] initializeProtosStruct -> DEBU 23d Processing field: BatchSize
2018-02-24 07:15:47.505 UTC [standardvalues.go:79] [common/config] initializeProtosStruct -> DEBU 23e Processing field: BatchTimeout
2018-02-24 07:15:47.505 UTC [standardvalues.go:79] [common/config] initializeProtosStruct -> DEBU 23f Processing field: KafkaBrokers
2018-02-24 07:15:47.505 UTC [standardvalues.go:79] [common/config] initializeProtosStruct -> DEBU 240 Processing field: ChannelRestrictions
2018-02-24 07:15:47.505 UTC [standardvalues.go:41] [common/config] NewStandardValues -> DEBU 241 Initializing protos for *config.OrganizationProtos
2018-02-24 07:15:47.505 UTC [standardvalues.go:79] [common/config] initializeProtosStruct -> DEBU 242 Processing field: MSP
2018-02-24 07:15:47.505 UTC [standardvalues.go:41] [common/config] NewStandardValues -> DEBU 243 Initializing protos for *struct {}
2018-02-24 07:15:47.505 UTC [standardvalues.go:41] [common/config] NewStandardValues -> DEBU 244 Initializing protos for *config.OrganizationProtos
2018-02-24 07:15:47.505 UTC [standardvalues.go:79] [common/config] initializeProtosStruct -> DEBU 245 Processing field: MSP
2018-02-24 07:15:47.505 UTC [standardvalues.go:41] [common/config] NewStandardValues -> DEBU 246 Initializing protos for *config.ApplicationOrgProtos
2018-02-24 07:15:47.505 UTC [standardvalues.go:79] [common/config] initializeProtosStruct -> DEBU 247 Processing field: AnchorPeers
2018-02-24 07:15:47.505 UTC [standardvalues.go:41] [common/config] NewStandardValues -> DEBU 248 Initializing protos for *config.OrganizationProtos
2018-02-24 07:15:47.505 UTC [standardvalues.go:79] [common/config] initializeProtosStruct -> DEBU 249 Processing field: MSP
2018-02-24 07:15:47.505 UTC [standardvalues.go:41] [common/config] NewStandardValues -> DEBU 24a Initializing protos for *config.OrganizationProtos
2018-02-24 07:15:47.505 UTC [standardvalues.go:79] [common/config] initializeProtosStruct -> DEBU 24b Processing field: MSP
2018-02-24 07:15:47.505 UTC [standardvalues.go:41] [common/config] NewStandardValues -> DEBU 24c Initializing protos for *config.ApplicationOrgProtos
2018-02-24 07:15:47.505 UTC [standardvalues.go:79] [common/config] initializeProtosStruct -> DEBU 24d Processing field: AnchorPeers
2018-02-24 07:15:47.505 UTC [standardvalues.go:41] [common/config] NewStandardValues -> DEBU 24e Initializing protos for *config.OrganizationProtos
2018-02-24 07:15:47.505 UTC [standardvalues.go:79] [common/config] initializeProtosStruct -> DEBU 24f Processing field: MSP
2018-02-24 07:15:47.505 UTC [organization.go:112] [common/config] validateMSP -> DEBU 250 Setting up MSP for org OrdererOrg
2018-02-24 07:15:47.507 UTC [applicationorg.go:91] [common/config] Validate -> DEBU 251 Anchor peers for org Org1MSP are 
2018-02-24 07:15:47.510 UTC [organization.go:112] [common/config] validateMSP -> DEBU 252 Setting up MSP for org Org1MSP
2018-02-24 07:15:47.513 UTC [applicationorg.go:91] [common/config] Validate -> DEBU 253 Anchor peers for org Org2MSP are 
2018-02-24 07:15:47.513 UTC [organization.go:112] [common/config] validateMSP -> DEBU 254 Setting up MSP for org Org2MSP
2018-02-24 07:15:47.517 UTC [peer.go:342] [peer] updateTrustedRoots -> DEBU 255 Updating trusted root authorities for channel mychannel
2018-02-24 07:15:47.522 UTC [peer.go:132] [peer] InitChain -> DEBU 256 Init chain mychannel
2018-02-24 07:15:47.522 UTC [start.go:228] [nodeCmd] func2 -> DEBU 257 Deploying system CC, for chain <mychannel>
2018-02-24 07:15:47.522 UTC [lockbased_txmgr.go:64] [lockbasedtxmgr] NewTxSimulator -> DEBU 258 constructing new tx simulator
2018-02-24 07:15:47.522 UTC [lockbased_tx_simulator.go:36] [lockbasedtxmgr] newLockBasedTxSimulator -> DEBU 259 constructing new tx simulator [2473ae71-3ace-4e64-913f-420135ca6576]
2018-02-24 07:15:47.522 UTC [ccprovider.go:363] [ccprovider] NewCCContext -> DEBU 25a NewCCCC (chain=mychannel,chaincode=cscc,version=1.0.5,txid=fdae33d6-dcc1-418e-8687-198107f5f0ab,syscc=true,proposal=0x0,canname=cscc:1.0.5
2018-02-24 07:15:47.522 UTC [chaincode_support.go:585] [chaincode] Launch -> DEBU 25b chaincode is running(no need to launch) : cscc:1.0.5
2018-02-24 07:15:47.522 UTC [chaincode_support.go:720] [chaincode] Execute -> DEBU 25c Entry
2018-02-24 07:15:47.522 UTC [chaincode_support.go:723] [chaincode] Execute -> DEBU 25d chaincode canonical name: cscc:1.0.5
2018-02-24 07:15:47.522 UTC [handler.go:1436] [chaincode] sendExecuteMessage -> DEBU 25e [fdae33d6]Inside sendExecuteMessage. Message INIT
2018-02-24 07:15:47.522 UTC [handler.go:1350] [chaincode] setChaincodeProposal -> DEBU 25f Setting chaincode proposal context...
2018-02-24 07:15:47.522 UTC [handler.go:1444] [chaincode] sendExecuteMessage -> DEBU 260 [fdae33d6]sendExecuteMsg trigger event INIT
2018-02-24 07:15:47.522 UTC [handler.go:357] [chaincode] processStream -> DEBU 261 [fdae33d6]Move state message INIT
2018-02-24 07:15:47.522 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 262 [fdae33d6]Fabric side Handling ChaincodeMessage of type: INIT in state ready
2018-02-24 07:15:47.522 UTC [handler.go:1417] [chaincode] filterError -> DEBU 263 Ignoring NoTransitionError: no transition
2018-02-24 07:15:47.523 UTC [handler.go:377] [chaincode] processStream -> DEBU 264 [fdae33d6]sending state message INIT
2018-02-24 07:15:47.523 UTC [chaincode.go:297] [shim] func1 -> DEBU 265 [fdae33d6]Received message INIT from shim
2018-02-24 07:15:47.523 UTC [handler.go:778] [shim] handleMessage -> DEBU 266 [fdae33d6]Handling ChaincodeMessage of type: INIT(state:ready)
2018-02-24 07:15:47.523 UTC [handler.go:265] [shim] beforeInit -> DEBU 267 Entered state ready
2018-02-24 07:15:47.523 UTC [handler.go:271] [shim] beforeInit -> DEBU 268 [fdae33d6]Received INIT, initializing chaincode
2018-02-24 07:15:47.523 UTC [configure.go:62] [cscc] Init -> INFO 269 Init CSCC
2018-02-24 07:15:47.523 UTC [handler.go:243] [shim] func1 -> DEBU 26a [fdae33d6]Init get response status: 200
2018-02-24 07:15:47.523 UTC [handler.go:259] [shim] func1 -> DEBU 26b [fdae33d6]Init succeeded. Sending COMPLETED
2018-02-24 07:15:47.523 UTC [chaincode.go:304] [shim] func1 -> DEBU 26c [fdae33d6]Move state message COMPLETED
2018-02-24 07:15:47.523 UTC [handler.go:778] [shim] handleMessage -> DEBU 26d [fdae33d6]Handling ChaincodeMessage of type: COMPLETED(state:ready)
2018-02-24 07:15:47.523 UTC [chaincode.go:320] [shim] func1 -> DEBU 26e [fdae33d6]send state message COMPLETED
2018-02-24 07:15:47.524 UTC [handler.go:336] [chaincode] processStream -> DEBU 26f [fdae33d6]Received message COMPLETED from shim
2018-02-24 07:15:47.524 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 270 [fdae33d6]Fabric side Handling ChaincodeMessage of type: COMPLETED in state ready
2018-02-24 07:15:47.524 UTC [handler.go:1392] [chaincode] HandleMessage -> DEBU 271 [fdae33d6-dcc1-418e-8687-198107f5f0ab]HandleMessage- COMPLETED. Notify
2018-02-24 07:15:47.524 UTC [handler.go:551] [chaincode] notify -> DEBU 272 notifying Txid:fdae33d6-dcc1-418e-8687-198107f5f0ab
2018-02-24 07:15:47.524 UTC [chaincode_support.go:751] [chaincode] Execute -> DEBU 273 Exit
2018-02-24 07:15:47.524 UTC [sysccapi.go:137] [sccapi] deploySysCC -> INFO 274 system chaincode cscc/mychannel(github.com/hyperledger/fabric/core/scc/cscc) deployed
2018-02-24 07:15:47.524 UTC [lockbased_query_executer.go:62] [lockbasedtxmgr] Done -> DEBU 275 Done with transaction simulation / query execution [2473ae71-3ace-4e64-913f-420135ca6576]
2018-02-24 07:15:47.524 UTC [lockbased_txmgr.go:64] [lockbasedtxmgr] NewTxSimulator -> DEBU 276 constructing new tx simulator
2018-02-24 07:15:47.524 UTC [lockbased_tx_simulator.go:36] [lockbasedtxmgr] newLockBasedTxSimulator -> DEBU 277 constructing new tx simulator [c7929136-8f82-4c01-90cf-e460095dd429]
2018-02-24 07:15:47.524 UTC [ccprovider.go:363] [ccprovider] NewCCContext -> DEBU 278 NewCCCC (chain=mychannel,chaincode=lscc,version=1.0.5,txid=d4903173-a41d-439c-b886-74dbb1572b9e,syscc=true,proposal=0x0,canname=lscc:1.0.5
2018-02-24 07:15:47.524 UTC [chaincode_support.go:585] [chaincode] Launch -> DEBU 279 chaincode is running(no need to launch) : lscc:1.0.5
2018-02-24 07:15:47.524 UTC [chaincode_support.go:720] [chaincode] Execute -> DEBU 27a Entry
2018-02-24 07:15:47.524 UTC [chaincode_support.go:723] [chaincode] Execute -> DEBU 27b chaincode canonical name: lscc:1.0.5
2018-02-24 07:15:47.524 UTC [handler.go:1436] [chaincode] sendExecuteMessage -> DEBU 27c [d4903173]Inside sendExecuteMessage. Message INIT
2018-02-24 07:15:47.524 UTC [handler.go:1350] [chaincode] setChaincodeProposal -> DEBU 27d Setting chaincode proposal context...
2018-02-24 07:15:47.524 UTC [handler.go:1444] [chaincode] sendExecuteMessage -> DEBU 27e [d4903173]sendExecuteMsg trigger event INIT
2018-02-24 07:15:47.524 UTC [handler.go:357] [chaincode] processStream -> DEBU 27f [d4903173]Move state message INIT
2018-02-24 07:15:47.524 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 280 [d4903173]Fabric side Handling ChaincodeMessage of type: INIT in state ready
2018-02-24 07:15:47.524 UTC [handler.go:1417] [chaincode] filterError -> DEBU 281 Ignoring NoTransitionError: no transition
2018-02-24 07:15:47.524 UTC [handler.go:377] [chaincode] processStream -> DEBU 282 [d4903173]sending state message INIT
2018-02-24 07:15:47.524 UTC [chaincode.go:297] [shim] func1 -> DEBU 283 [d4903173]Received message INIT from shim
2018-02-24 07:15:47.524 UTC [handler.go:778] [shim] handleMessage -> DEBU 284 [d4903173]Handling ChaincodeMessage of type: INIT(state:ready)
2018-02-24 07:15:47.524 UTC [handler.go:265] [shim] beforeInit -> DEBU 285 Entered state ready
2018-02-24 07:15:47.524 UTC [handler.go:271] [shim] beforeInit -> DEBU 286 [d4903173]Received INIT, initializing chaincode
2018-02-24 07:15:47.524 UTC [handler.go:243] [shim] func1 -> DEBU 287 [d4903173]Init get response status: 200
2018-02-24 07:15:47.525 UTC [handler.go:259] [shim] func1 -> DEBU 288 [d4903173]Init succeeded. Sending COMPLETED
2018-02-24 07:15:47.525 UTC [chaincode.go:304] [shim] func1 -> DEBU 289 [d4903173]Move state message COMPLETED
2018-02-24 07:15:47.525 UTC [handler.go:778] [shim] handleMessage -> DEBU 28a [d4903173]Handling ChaincodeMessage of type: COMPLETED(state:ready)
2018-02-24 07:15:47.525 UTC [chaincode.go:320] [shim] func1 -> DEBU 28b [d4903173]send state message COMPLETED
2018-02-24 07:15:47.525 UTC [handler.go:336] [chaincode] processStream -> DEBU 28c [d4903173]Received message COMPLETED from shim
2018-02-24 07:15:47.525 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 28d [d4903173]Fabric side Handling ChaincodeMessage of type: COMPLETED in state ready
2018-02-24 07:15:47.525 UTC [handler.go:1392] [chaincode] HandleMessage -> DEBU 28e [d4903173-a41d-439c-b886-74dbb1572b9e]HandleMessage- COMPLETED. Notify
2018-02-24 07:15:47.525 UTC [handler.go:551] [chaincode] notify -> DEBU 28f notifying Txid:d4903173-a41d-439c-b886-74dbb1572b9e
2018-02-24 07:15:47.525 UTC [chaincode_support.go:751] [chaincode] Execute -> DEBU 290 Exit
2018-02-24 07:15:47.525 UTC [sysccapi.go:137] [sccapi] deploySysCC -> INFO 291 system chaincode lscc/mychannel(github.com/hyperledger/fabric/core/scc/lscc) deployed
2018-02-24 07:15:47.525 UTC [lockbased_query_executer.go:62] [lockbasedtxmgr] Done -> DEBU 292 Done with transaction simulation / query execution [c7929136-8f82-4c01-90cf-e460095dd429]
2018-02-24 07:15:47.525 UTC [lockbased_txmgr.go:64] [lockbasedtxmgr] NewTxSimulator -> DEBU 293 constructing new tx simulator
2018-02-24 07:15:47.525 UTC [lockbased_tx_simulator.go:36] [lockbasedtxmgr] newLockBasedTxSimulator -> DEBU 294 constructing new tx simulator [f2386c5a-bab8-4dcb-801b-adfbb879cabb]
2018-02-24 07:15:47.525 UTC [ccprovider.go:363] [ccprovider] NewCCContext -> DEBU 295 NewCCCC (chain=mychannel,chaincode=escc,version=1.0.5,txid=d97a4948-8f3e-4693-bcc4-f8e4c4bc10be,syscc=true,proposal=0x0,canname=escc:1.0.5
2018-02-24 07:15:47.525 UTC [chaincode_support.go:585] [chaincode] Launch -> DEBU 296 chaincode is running(no need to launch) : escc:1.0.5
2018-02-24 07:15:47.525 UTC [chaincode_support.go:720] [chaincode] Execute -> DEBU 297 Entry
2018-02-24 07:15:47.525 UTC [chaincode_support.go:723] [chaincode] Execute -> DEBU 298 chaincode canonical name: escc:1.0.5
2018-02-24 07:15:47.525 UTC [handler.go:1436] [chaincode] sendExecuteMessage -> DEBU 299 [d97a4948]Inside sendExecuteMessage. Message INIT
2018-02-24 07:15:47.525 UTC [handler.go:1350] [chaincode] setChaincodeProposal -> DEBU 29a Setting chaincode proposal context...
2018-02-24 07:15:47.525 UTC [handler.go:1444] [chaincode] sendExecuteMessage -> DEBU 29b [d97a4948]sendExecuteMsg trigger event INIT
2018-02-24 07:15:47.525 UTC [handler.go:357] [chaincode] processStream -> DEBU 29c [d97a4948]Move state message INIT
2018-02-24 07:15:47.525 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 29d [d97a4948]Fabric side Handling ChaincodeMessage of type: INIT in state ready
2018-02-24 07:15:47.525 UTC [handler.go:1417] [chaincode] filterError -> DEBU 29e Ignoring NoTransitionError: no transition
2018-02-24 07:15:47.526 UTC [handler.go:377] [chaincode] processStream -> DEBU 29f [d97a4948]sending state message INIT
2018-02-24 07:15:47.526 UTC [chaincode.go:297] [shim] func1 -> DEBU 2a0 [d97a4948]Received message INIT from shim
2018-02-24 07:15:47.526 UTC [handler.go:778] [shim] handleMessage -> DEBU 2a1 [d97a4948]Handling ChaincodeMessage of type: INIT(state:ready)
2018-02-24 07:15:47.526 UTC [handler.go:265] [shim] beforeInit -> DEBU 2a2 Entered state ready
2018-02-24 07:15:47.526 UTC [handler.go:271] [shim] beforeInit -> DEBU 2a3 [d97a4948]Received INIT, initializing chaincode
2018-02-24 07:15:47.526 UTC [endorser_onevalidsignature.go:40] [escc] Init -> INFO 2a4 Successfully initialized ESCC
2018-02-24 07:15:47.526 UTC [handler.go:243] [shim] func1 -> DEBU 2a5 [d97a4948]Init get response status: 200
2018-02-24 07:15:47.526 UTC [handler.go:259] [shim] func1 -> DEBU 2a6 [d97a4948]Init succeeded. Sending COMPLETED
2018-02-24 07:15:47.526 UTC [chaincode.go:304] [shim] func1 -> DEBU 2a7 [d97a4948]Move state message COMPLETED
2018-02-24 07:15:47.526 UTC [handler.go:778] [shim] handleMessage -> DEBU 2a8 [d97a4948]Handling ChaincodeMessage of type: COMPLETED(state:ready)
2018-02-24 07:15:47.526 UTC [chaincode.go:320] [shim] func1 -> DEBU 2a9 [d97a4948]send state message COMPLETED
2018-02-24 07:15:47.526 UTC [handler.go:336] [chaincode] processStream -> DEBU 2aa [d97a4948]Received message COMPLETED from shim
2018-02-24 07:15:47.526 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 2ab [d97a4948]Fabric side Handling ChaincodeMessage of type: COMPLETED in state ready
2018-02-24 07:15:47.526 UTC [handler.go:1392] [chaincode] HandleMessage -> DEBU 2ac [d97a4948-8f3e-4693-bcc4-f8e4c4bc10be]HandleMessage- COMPLETED. Notify
2018-02-24 07:15:47.526 UTC [handler.go:551] [chaincode] notify -> DEBU 2ad notifying Txid:d97a4948-8f3e-4693-bcc4-f8e4c4bc10be
2018-02-24 07:15:47.526 UTC [chaincode_support.go:751] [chaincode] Execute -> DEBU 2ae Exit
2018-02-24 07:15:47.526 UTC [sysccapi.go:137] [sccapi] deploySysCC -> INFO 2af system chaincode escc/mychannel(github.com/hyperledger/fabric/core/scc/escc) deployed
2018-02-24 07:15:47.526 UTC [lockbased_query_executer.go:62] [lockbasedtxmgr] Done -> DEBU 2b0 Done with transaction simulation / query execution [f2386c5a-bab8-4dcb-801b-adfbb879cabb]
2018-02-24 07:15:47.526 UTC [lockbased_txmgr.go:64] [lockbasedtxmgr] NewTxSimulator -> DEBU 2b1 constructing new tx simulator
2018-02-24 07:15:47.526 UTC [lockbased_tx_simulator.go:36] [lockbasedtxmgr] newLockBasedTxSimulator -> DEBU 2b2 constructing new tx simulator [7ac13f8d-76ef-4f06-933c-8881254a8580]
2018-02-24 07:15:47.526 UTC [ccprovider.go:363] [ccprovider] NewCCContext -> DEBU 2b3 NewCCCC (chain=mychannel,chaincode=vscc,version=1.0.5,txid=dc429285-d3aa-4cad-8920-7cd8cae05493,syscc=true,proposal=0x0,canname=vscc:1.0.5
2018-02-24 07:15:47.526 UTC [chaincode_support.go:585] [chaincode] Launch -> DEBU 2b4 chaincode is running(no need to launch) : vscc:1.0.5
2018-02-24 07:15:47.526 UTC [chaincode_support.go:720] [chaincode] Execute -> DEBU 2b5 Entry
2018-02-24 07:15:47.526 UTC [chaincode_support.go:723] [chaincode] Execute -> DEBU 2b6 chaincode canonical name: vscc:1.0.5
2018-02-24 07:15:47.526 UTC [handler.go:1436] [chaincode] sendExecuteMessage -> DEBU 2b7 [dc429285]Inside sendExecuteMessage. Message INIT
2018-02-24 07:15:47.527 UTC [handler.go:1350] [chaincode] setChaincodeProposal -> DEBU 2b8 Setting chaincode proposal context...
2018-02-24 07:15:47.527 UTC [handler.go:1444] [chaincode] sendExecuteMessage -> DEBU 2b9 [dc429285]sendExecuteMsg trigger event INIT
2018-02-24 07:15:47.527 UTC [handler.go:357] [chaincode] processStream -> DEBU 2ba [dc429285]Move state message INIT
2018-02-24 07:15:47.527 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 2bb [dc429285]Fabric side Handling ChaincodeMessage of type: INIT in state ready
2018-02-24 07:15:47.527 UTC [handler.go:1417] [chaincode] filterError -> DEBU 2bc Ignoring NoTransitionError: no transition
2018-02-24 07:15:47.527 UTC [handler.go:377] [chaincode] processStream -> DEBU 2bd [dc429285]sending state message INIT
2018-02-24 07:15:47.527 UTC [chaincode.go:297] [shim] func1 -> DEBU 2be [dc429285]Received message INIT from shim
2018-02-24 07:15:47.527 UTC [handler.go:778] [shim] handleMessage -> DEBU 2bf [dc429285]Handling ChaincodeMessage of type: INIT(state:ready)
2018-02-24 07:15:47.527 UTC [handler.go:265] [shim] beforeInit -> DEBU 2c0 Entered state ready
2018-02-24 07:15:47.527 UTC [handler.go:271] [shim] beforeInit -> DEBU 2c1 [dc429285]Received INIT, initializing chaincode
2018-02-24 07:15:47.527 UTC [handler.go:243] [shim] func1 -> DEBU 2c2 [dc429285]Init get response status: 200
2018-02-24 07:15:47.527 UTC [handler.go:259] [shim] func1 -> DEBU 2c3 [dc429285]Init succeeded. Sending COMPLETED
2018-02-24 07:15:47.527 UTC [chaincode.go:304] [shim] func1 -> DEBU 2c4 [dc429285]Move state message COMPLETED
2018-02-24 07:15:47.527 UTC [handler.go:778] [shim] handleMessage -> DEBU 2c5 [dc429285]Handling ChaincodeMessage of type: COMPLETED(state:ready)
2018-02-24 07:15:47.527 UTC [chaincode.go:320] [shim] func1 -> DEBU 2c6 [dc429285]send state message COMPLETED
2018-02-24 07:15:47.527 UTC [handler.go:336] [chaincode] processStream -> DEBU 2c7 [dc429285]Received message COMPLETED from shim
2018-02-24 07:15:47.527 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 2c8 [dc429285]Fabric side Handling ChaincodeMessage of type: COMPLETED in state ready
2018-02-24 07:15:47.527 UTC [handler.go:1392] [chaincode] HandleMessage -> DEBU 2c9 [dc429285-d3aa-4cad-8920-7cd8cae05493]HandleMessage- COMPLETED. Notify
2018-02-24 07:15:47.527 UTC [handler.go:551] [chaincode] notify -> DEBU 2ca notifying Txid:dc429285-d3aa-4cad-8920-7cd8cae05493
2018-02-24 07:15:47.527 UTC [chaincode_support.go:751] [chaincode] Execute -> DEBU 2cb Exit
2018-02-24 07:15:47.527 UTC [sysccapi.go:137] [sccapi] deploySysCC -> INFO 2cc system chaincode vscc/mychannel(github.com/hyperledger/fabric/core/scc/vscc) deployed
2018-02-24 07:15:47.527 UTC [lockbased_query_executer.go:62] [lockbasedtxmgr] Done -> DEBU 2cd Done with transaction simulation / query execution [7ac13f8d-76ef-4f06-933c-8881254a8580]
2018-02-24 07:15:47.527 UTC [lockbased_txmgr.go:64] [lockbasedtxmgr] NewTxSimulator -> DEBU 2ce constructing new tx simulator
2018-02-24 07:15:47.528 UTC [lockbased_tx_simulator.go:36] [lockbasedtxmgr] newLockBasedTxSimulator -> DEBU 2cf constructing new tx simulator [c44b2b53-b128-423c-a93f-4a8e676a769b]
2018-02-24 07:15:47.528 UTC [ccprovider.go:363] [ccprovider] NewCCContext -> DEBU 2d0 NewCCCC (chain=mychannel,chaincode=qscc,version=1.0.5,txid=07ac79ae-8bd6-4e5a-ad37-3df08f79157e,syscc=true,proposal=0x0,canname=qscc:1.0.5
2018-02-24 07:15:47.528 UTC [chaincode_support.go:585] [chaincode] Launch -> DEBU 2d1 chaincode is running(no need to launch) : qscc:1.0.5
2018-02-24 07:15:47.528 UTC [chaincode_support.go:720] [chaincode] Execute -> DEBU 2d2 Entry
2018-02-24 07:15:47.528 UTC [chaincode_support.go:723] [chaincode] Execute -> DEBU 2d3 chaincode canonical name: qscc:1.0.5
2018-02-24 07:15:47.528 UTC [handler.go:1436] [chaincode] sendExecuteMessage -> DEBU 2d4 [07ac79ae]Inside sendExecuteMessage. Message INIT
2018-02-24 07:15:47.528 UTC [handler.go:1350] [chaincode] setChaincodeProposal -> DEBU 2d5 Setting chaincode proposal context...
2018-02-24 07:15:47.528 UTC [handler.go:1444] [chaincode] sendExecuteMessage -> DEBU 2d6 [07ac79ae]sendExecuteMsg trigger event INIT
2018-02-24 07:15:47.528 UTC [handler.go:357] [chaincode] processStream -> DEBU 2d7 [07ac79ae]Move state message INIT
2018-02-24 07:15:47.528 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 2d8 [07ac79ae]Fabric side Handling ChaincodeMessage of type: INIT in state ready
2018-02-24 07:15:47.528 UTC [handler.go:1417] [chaincode] filterError -> DEBU 2d9 Ignoring NoTransitionError: no transition
2018-02-24 07:15:47.528 UTC [handler.go:377] [chaincode] processStream -> DEBU 2da [07ac79ae]sending state message INIT
2018-02-24 07:15:47.528 UTC [chaincode.go:297] [shim] func1 -> DEBU 2db [07ac79ae]Received message INIT from shim
2018-02-24 07:15:47.528 UTC [handler.go:778] [shim] handleMessage -> DEBU 2dc [07ac79ae]Handling ChaincodeMessage of type: INIT(state:ready)
2018-02-24 07:15:47.528 UTC [handler.go:265] [shim] beforeInit -> DEBU 2dd Entered state ready
2018-02-24 07:15:47.528 UTC [handler.go:271] [shim] beforeInit -> DEBU 2de [07ac79ae]Received INIT, initializing chaincode
2018-02-24 07:15:47.528 UTC [query.go:59] [qscc] Init -> INFO 2df Init QSCC
2018-02-24 07:15:47.528 UTC [handler.go:243] [shim] func1 -> DEBU 2e0 [07ac79ae]Init get response status: 200
2018-02-24 07:15:47.528 UTC [handler.go:259] [shim] func1 -> DEBU 2e1 [07ac79ae]Init succeeded. Sending COMPLETED
2018-02-24 07:15:47.528 UTC [chaincode.go:304] [shim] func1 -> DEBU 2e2 [07ac79ae]Move state message COMPLETED
2018-02-24 07:15:47.528 UTC [handler.go:778] [shim] handleMessage -> DEBU 2e3 [07ac79ae]Handling ChaincodeMessage of type: COMPLETED(state:ready)
2018-02-24 07:15:47.528 UTC [chaincode.go:320] [shim] func1 -> DEBU 2e4 [07ac79ae]send state message COMPLETED
2018-02-24 07:15:47.528 UTC [handler.go:336] [chaincode] processStream -> DEBU 2e5 [07ac79ae]Received message COMPLETED from shim
2018-02-24 07:15:47.528 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 2e6 [07ac79ae]Fabric side Handling ChaincodeMessage of type: COMPLETED in state ready
2018-02-24 07:15:47.528 UTC [handler.go:1392] [chaincode] HandleMessage -> DEBU 2e7 [07ac79ae-8bd6-4e5a-ad37-3df08f79157e]HandleMessage- COMPLETED. Notify
2018-02-24 07:15:47.528 UTC [handler.go:551] [chaincode] notify -> DEBU 2e8 notifying Txid:07ac79ae-8bd6-4e5a-ad37-3df08f79157e
2018-02-24 07:15:47.528 UTC [chaincode_support.go:751] [chaincode] Execute -> DEBU 2e9 Exit
2018-02-24 07:15:47.529 UTC [sysccapi.go:137] [sccapi] deploySysCC -> INFO 2ea system chaincode qscc/mychannel(github.com/hyperledger/fabric/core/chaincode/qscc) deployed
2018-02-24 07:15:47.529 UTC [lockbased_query_executer.go:62] [lockbasedtxmgr] Done -> DEBU 2eb Done with transaction simulation / query execution [c44b2b53-b128-423c-a93f-4a8e676a769b]
2018-02-24 07:15:47.529 UTC [eventhelper.go:29] [eventhub_producer] SendProducerBlockEvent -> DEBU 2ec Entry
2018-02-24 07:15:47.529 UTC [eventhelper.go:105] [eventhub_producer] SendProducerBlockEvent -> INFO 2ed Channel [mychannel]: Sending event for block number [0]
2018-02-24 07:15:47.529 UTC [events.go:312] [eventhub_producer] Send -> DEBU 2ee Entry
2018-02-24 07:15:47.529 UTC [events.go:335] [eventhub_producer] Send -> DEBU 2ef Event processor timeout > 0
2018-02-24 07:15:47.529 UTC [events.go:343] [eventhub_producer] Send -> DEBU 2f0 Event sent successfully
2018-02-24 07:15:47.529 UTC [events.go:344] [eventhub_producer] Send -> DEBU 2f1 Exit
2018-02-24 07:15:47.529 UTC [eventhelper.go:107] [eventhub_producer] SendProducerBlockEvent -> DEBU 2f2 Exit
2018-02-24 07:15:47.529 UTC [handler.go:326] [shim] func1 -> DEBU 2f3 [72427d9e]Transaction completed. Sending COMPLETED
2018-02-24 07:15:47.529 UTC [chaincode.go:304] [shim] func1 -> DEBU 2f4 [72427d9e]Move state message COMPLETED
2018-02-24 07:15:47.529 UTC [handler.go:778] [shim] handleMessage -> DEBU 2f5 [72427d9e]Handling ChaincodeMessage of type: COMPLETED(state:ready)
2018-02-24 07:15:47.529 UTC [chaincode.go:320] [shim] func1 -> DEBU 2f6 [72427d9e]send state message COMPLETED
2018-02-24 07:15:47.529 UTC [register_internal_events.go:41] [eventhub_producer] getMessageType -> DEBU 2f7 [gerry] Event_Block
2018-02-24 07:15:47.529 UTC [handler.go:336] [chaincode] processStream -> DEBU 2f8 [72427d9e]Received message COMPLETED from shim
2018-02-24 07:15:47.529 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 2f9 [72427d9e]Fabric side Handling ChaincodeMessage of type: COMPLETED in state ready
2018-02-24 07:15:47.529 UTC [handler.go:1392] [chaincode] HandleMessage -> DEBU 2fa [72427d9ece8288e07287264351927092d178edf727eea249b65fa393d953aab0]HandleMessage- COMPLETED. Notify
2018-02-24 07:15:47.529 UTC [handler.go:551] [chaincode] notify -> DEBU 2fb notifying Txid:72427d9ece8288e07287264351927092d178edf727eea249b65fa393d953aab0
2018-02-24 07:15:47.529 UTC [chaincode_support.go:751] [chaincode] Execute -> DEBU 2fc Exit
2018-02-24 07:15:47.529 UTC [endorser.go:156] [endorser] callChaincode -> DEBU 2fd Exit
2018-02-24 07:15:47.529 UTC [endorser.go:265] [endorser] simulateProposal -> DEBU 2fe Exit
2018-02-24 07:15:47.529 UTC [endorser.go:500] [endorser] ProcessProposal -> DEBU 2ff [gerry] 2 -- endorse and get a marshalled ProposalResponse message
2018-02-24 07:15:47.529 UTC [endorser.go:506] [endorser] ProcessProposal -> DEBU 300 chainID is NULL, txid: 72427d9ece8288e07287264351927092d178edf727eea249b65fa393d953aab0
2018-02-24 07:15:47.529 UTC [endorser.go:523] [endorser] ProcessProposal -> DEBU 301 go here txid: 72427d9ece8288e07287264351927092d178edf727eea249b65fa393d953aab0
2018-02-24 07:15:47.529 UTC [endorser.go:530] [endorser] ProcessProposal -> DEBU 302 Exit
2018-02-24 07:15:53.524 UTC [deliveryclient.go:142] [deliveryClient] StartDeliverForChannel -> DEBU 303 This peer will pass blocks from orderer service to other peers for channel mychannel
2018-02-24 07:15:53.531 UTC [client.go:140] [deliveryClient] connect -> DEBU 304 Connected to orderer.example.com:7050
2018-02-24 07:15:53.531 UTC [client.go:146] [deliveryClient] connect -> DEBU 305 Establishing gRPC stream with orderer.example.com:7050 ...
2018-02-24 07:15:53.534 UTC [client.go:165] [deliveryClient] afterConnect -> DEBU 306 Entering
2018-02-24 07:15:53.534 UTC [requester.go:42] [deliveryClient] RequestBlocks -> DEBU 307 Starting deliver with block [1] for channel mychannel
2018-02-24 07:15:53.538 UTC [client.go:192] [deliveryClient] afterConnect -> DEBU 308 Exiting
```

### install chaincode (peer 日志)

```
2018-02-24 07:22:17.166 UTC [endorser.go:373] [endorser] ProcessProposal -> DEBU 309 Entry
2018-02-24 07:22:17.170 UTC [msgvalidation.go:79] [protoutils] ValidateProposalMessage -> DEBU 30a ValidateProposalMessage starts for signed proposal 0xc42213e780
2018-02-24 07:22:17.170 UTC [msgvalidation.go:215] [protoutils] validateChannelHeader -> DEBU 30b validateChannelHeader info: header type 3
2018-02-24 07:22:17.170 UTC [msgvalidation.go:142] [protoutils] checkSignatureFromCreator -> DEBU 30c checkSignatureFromCreator starts
2018-02-24 07:22:17.171 UTC [msgvalidation.go:160] [protoutils] checkSignatureFromCreator -> DEBU 30d checkSignatureFromCreator info: creator is &{Org1MSP 3dc70c07d3096e889a28bca219d07b0d858baede9b7d60760c9bf5a76017c9a1}
2018-02-24 07:22:17.171 UTC [msgvalidation.go:168] [protoutils] checkSignatureFromCreator -> DEBU 30e checkSignatureFromCreator info: creator is valid
2018-02-24 07:22:17.175 UTC [msgvalidation.go:176] [protoutils] checkSignatureFromCreator -> DEBU 30f checkSignatureFromCreator exists successfully
2018-02-24 07:22:17.175 UTC [msgvalidation.go:39] [protoutils] validateChaincodeProposalMessage -> DEBU 310 validateChaincodeProposalMessage starts for proposal 0xc42154d8b0, header 0xc42213e7b0
2018-02-24 07:22:17.175 UTC [msgvalidation.go:51] [protoutils] validateChaincodeProposalMessage -> DEBU 311 validateChaincodeProposalMessage info: header extension references chaincode name:"lscc" 
2018-02-24 07:22:17.175 UTC [endorser.go:412] [endorser] ProcessProposal -> DEBU 312 processing txid: b44dcddb8e6bd971200fdbc60f95cf18ea65f8ad0d65c740da767448285d78b1
2018-02-24 07:22:17.175 UTC [endorser.go:441] [endorser] ProcessProposal -> DEBU 313 [gerry] to get TxSimulator and HistoryQueryExecutor
2018-02-24 07:22:17.175 UTC [endorser.go:472] [endorser] ProcessProposal -> DEBU 314 [gerry] 1 -- simulate
2018-02-24 07:22:17.176 UTC [endorser.go:211] [endorser] simulateProposal -> DEBU 315 Entry - txid: b44dcddb8e6bd971200fdbc60f95cf18ea65f8ad0d65c740da767448285d78b1 channel id: 
2018-02-24 07:22:17.176 UTC [endorser.go:99] [endorser] callChaincode -> DEBU 316 Entry - txid: b44dcddb8e6bd971200fdbc60f95cf18ea65f8ad0d65c740da767448285d78b1 channel id:  version: 1.0.5
2018-02-24 07:22:17.176 UTC [ccprovider.go:363] [ccprovider] NewCCContext -> DEBU 317 NewCCCC (chain=,chaincode=lscc,version=1.0.5,txid=b44dcddb8e6bd971200fdbc60f95cf18ea65f8ad0d65c740da767448285d78b1,syscc=true,proposal=0xc42154d8b0,canname=lscc:1.0.5
2018-02-24 07:22:17.176 UTC [chaincode_support.go:585] [chaincode] Launch -> DEBU 318 chaincode is running(no need to launch) : lscc:1.0.5
2018-02-24 07:22:17.176 UTC [chaincode_support.go:720] [chaincode] Execute -> DEBU 319 Entry
2018-02-24 07:22:17.176 UTC [chaincode_support.go:723] [chaincode] Execute -> DEBU 31a chaincode canonical name: lscc:1.0.5
2018-02-24 07:22:17.176 UTC [handler.go:1436] [chaincode] sendExecuteMessage -> DEBU 31b [b44dcddb]Inside sendExecuteMessage. Message TRANSACTION
2018-02-24 07:22:17.176 UTC [handler.go:1350] [chaincode] setChaincodeProposal -> DEBU 31c Setting chaincode proposal context...
2018-02-24 07:22:17.176 UTC [handler.go:1352] [chaincode] setChaincodeProposal -> DEBU 31d Proposal different from nil. Creating chaincode proposal context...
2018-02-24 07:22:17.176 UTC [handler.go:1444] [chaincode] sendExecuteMessage -> DEBU 31e [b44dcddb]sendExecuteMsg trigger event TRANSACTION
2018-02-24 07:22:17.176 UTC [handler.go:357] [chaincode] processStream -> DEBU 31f [b44dcddb]Move state message TRANSACTION
2018-02-24 07:22:17.176 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 320 [b44dcddb]Fabric side Handling ChaincodeMessage of type: TRANSACTION in state ready
2018-02-24 07:22:17.176 UTC [handler.go:1417] [chaincode] filterError -> DEBU 321 Ignoring NoTransitionError: no transition
2018-02-24 07:22:17.176 UTC [handler.go:377] [chaincode] processStream -> DEBU 322 [b44dcddb]sending state message TRANSACTION
2018-02-24 07:22:17.176 UTC [chaincode.go:297] [shim] func1 -> DEBU 323 [b44dcddb]Received message TRANSACTION from shim
2018-02-24 07:22:17.176 UTC [handler.go:778] [shim] handleMessage -> DEBU 324 [b44dcddb]Handling ChaincodeMessage of type: TRANSACTION(state:ready)
2018-02-24 07:22:17.176 UTC [handler.go:338] [shim] beforeTransaction -> DEBU 325 [b44dcddb]Received TRANSACTION, invoking transaction on chaincode(Src:ready, Dst:ready)
2018-02-24 07:22:17.178 UTC [handler.go:326] [shim] func1 -> DEBU 326 [b44dcddb]Transaction completed. Sending COMPLETED
2018-02-24 07:22:17.178 UTC [chaincode.go:304] [shim] func1 -> DEBU 327 [b44dcddb]Move state message COMPLETED
2018-02-24 07:22:17.178 UTC [handler.go:778] [shim] handleMessage -> DEBU 328 [b44dcddb]Handling ChaincodeMessage of type: COMPLETED(state:ready)
2018-02-24 07:22:17.178 UTC [chaincode.go:320] [shim] func1 -> DEBU 329 [b44dcddb]send state message COMPLETED
2018-02-24 07:22:17.178 UTC [handler.go:336] [chaincode] processStream -> DEBU 32a [b44dcddb]Received message COMPLETED from shim
2018-02-24 07:22:17.178 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 32b [b44dcddb]Fabric side Handling ChaincodeMessage of type: COMPLETED in state ready
2018-02-24 07:22:17.178 UTC [handler.go:1392] [chaincode] HandleMessage -> DEBU 32c [b44dcddb8e6bd971200fdbc60f95cf18ea65f8ad0d65c740da767448285d78b1]HandleMessage- COMPLETED. Notify
2018-02-24 07:22:17.178 UTC [handler.go:551] [chaincode] notify -> DEBU 32d notifying Txid:b44dcddb8e6bd971200fdbc60f95cf18ea65f8ad0d65c740da767448285d78b1
2018-02-24 07:22:17.178 UTC [chaincode_support.go:751] [chaincode] Execute -> DEBU 32e Exit
2018-02-24 07:22:17.178 UTC [endorser.go:156] [endorser] callChaincode -> DEBU 32f Exit
2018-02-24 07:22:17.178 UTC [endorser.go:265] [endorser] simulateProposal -> DEBU 330 Exit
2018-02-24 07:22:17.178 UTC [endorser.go:500] [endorser] ProcessProposal -> DEBU 331 [gerry] 2 -- endorse and get a marshalled ProposalResponse message
2018-02-24 07:22:17.178 UTC [endorser.go:506] [endorser] ProcessProposal -> DEBU 332 chainID is NULL, txid: b44dcddb8e6bd971200fdbc60f95cf18ea65f8ad0d65c740da767448285d78b1
2018-02-24 07:22:17.178 UTC [endorser.go:523] [endorser] ProcessProposal -> DEBU 333 go here txid: b44dcddb8e6bd971200fdbc60f95cf18ea65f8ad0d65c740da767448285d78b1
2018-02-24 07:22:17.178 UTC [endorser.go:530] [endorser] ProcessProposal -> DEBU 334 Exit
```

### instantiate chaincode (peer 日志)

```
2018-02-24 07:26:49.610 UTC [endorser.go:373] [endorser] ProcessProposal -> DEBU 335 Entry
2018-02-24 07:26:49.610 UTC [msgvalidation.go:79] [protoutils] ValidateProposalMessage -> DEBU 336 ValidateProposalMessage starts for signed proposal 0xc421e1b6e0
2018-02-24 07:26:49.610 UTC [msgvalidation.go:215] [protoutils] validateChannelHeader -> DEBU 337 validateChannelHeader info: header type 3
2018-02-24 07:26:49.610 UTC [msgvalidation.go:142] [protoutils] checkSignatureFromCreator -> DEBU 338 checkSignatureFromCreator starts
2018-02-24 07:26:49.610 UTC [msgvalidation.go:160] [protoutils] checkSignatureFromCreator -> DEBU 339 checkSignatureFromCreator info: creator is &{Org1MSP 3dc70c07d3096e889a28bca219d07b0d858baede9b7d60760c9bf5a76017c9a1}
2018-02-24 07:26:49.611 UTC [msgvalidation.go:168] [protoutils] checkSignatureFromCreator -> DEBU 33a checkSignatureFromCreator info: creator is valid
2018-02-24 07:26:49.611 UTC [msgvalidation.go:176] [protoutils] checkSignatureFromCreator -> DEBU 33b checkSignatureFromCreator exists successfully
2018-02-24 07:26:49.611 UTC [msgvalidation.go:39] [protoutils] validateChaincodeProposalMessage -> DEBU 33c validateChaincodeProposalMessage starts for proposal 0xc421e16460, header 0xc421e1b710
2018-02-24 07:26:49.611 UTC [msgvalidation.go:51] [protoutils] validateChaincodeProposalMessage -> DEBU 33d validateChaincodeProposalMessage info: header extension references chaincode name:"lscc" 
2018-02-24 07:26:49.612 UTC [endorser.go:412] [endorser] ProcessProposal -> DEBU 33e processing txid: 862dbe7cb0f7e4f5e4cf3fff5d590eca2f3700cb0d40db28b0bbba7f13a473cf
2018-02-24 07:26:49.612 UTC [endorser.go:414] [endorser] ProcessProposal -> DEBU 33f [gerry] chainID: mychannel
2018-02-24 07:26:49.612 UTC [blockfile_mgr.go:506] [fsblkstorage] retrieveTransactionByID -> DEBU 340 retrieveTransactionByID() - txId = [862dbe7cb0f7e4f5e4cf3fff5d590eca2f3700cb0d40db28b0bbba7f13a473cf]
2018-02-24 07:26:49.612 UTC [endorser.go:441] [endorser] ProcessProposal -> DEBU 341 [gerry] to get TxSimulator and HistoryQueryExecutor
2018-02-24 07:26:49.612 UTC [lockbased_txmgr.go:64] [lockbasedtxmgr] NewTxSimulator -> DEBU 342 constructing new tx simulator
2018-02-24 07:26:49.612 UTC [lockbased_tx_simulator.go:36] [lockbasedtxmgr] newLockBasedTxSimulator -> DEBU 343 constructing new tx simulator [dd3356ae-bd86-4a74-806c-677322bd2860]
2018-02-24 07:26:49.612 UTC [endorser.go:472] [endorser] ProcessProposal -> DEBU 344 [gerry] 1 -- simulate
2018-02-24 07:26:49.612 UTC [endorser.go:211] [endorser] simulateProposal -> DEBU 345 Entry - txid: 862dbe7cb0f7e4f5e4cf3fff5d590eca2f3700cb0d40db28b0bbba7f13a473cf channel id: mychannel
2018-02-24 07:26:49.612 UTC [endorser.go:99] [endorser] callChaincode -> DEBU 346 Entry - txid: 862dbe7cb0f7e4f5e4cf3fff5d590eca2f3700cb0d40db28b0bbba7f13a473cf channel id: mychannel version: 1.0.5
2018-02-24 07:26:49.612 UTC [ccprovider.go:363] [ccprovider] NewCCContext -> DEBU 347 NewCCCC (chain=mychannel,chaincode=lscc,version=1.0.5,txid=862dbe7cb0f7e4f5e4cf3fff5d590eca2f3700cb0d40db28b0bbba7f13a473cf,syscc=true,proposal=0xc421e16460,canname=lscc:1.0.5
2018-02-24 07:26:49.613 UTC [chaincode_support.go:585] [chaincode] Launch -> DEBU 348 chaincode is running(no need to launch) : lscc:1.0.5
2018-02-24 07:26:49.613 UTC [chaincode_support.go:720] [chaincode] Execute -> DEBU 349 Entry
2018-02-24 07:26:49.613 UTC [chaincode_support.go:723] [chaincode] Execute -> DEBU 34a chaincode canonical name: lscc:1.0.5
2018-02-24 07:26:49.613 UTC [handler.go:1436] [chaincode] sendExecuteMessage -> DEBU 34b [862dbe7c]Inside sendExecuteMessage. Message TRANSACTION
2018-02-24 07:26:49.613 UTC [handler.go:1350] [chaincode] setChaincodeProposal -> DEBU 34c Setting chaincode proposal context...
2018-02-24 07:26:49.613 UTC [handler.go:1352] [chaincode] setChaincodeProposal -> DEBU 34d Proposal different from nil. Creating chaincode proposal context...
2018-02-24 07:26:49.613 UTC [handler.go:1444] [chaincode] sendExecuteMessage -> DEBU 34e [862dbe7c]sendExecuteMsg trigger event TRANSACTION
2018-02-24 07:26:49.613 UTC [handler.go:357] [chaincode] processStream -> DEBU 34f [862dbe7c]Move state message TRANSACTION
2018-02-24 07:26:49.613 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 350 [862dbe7c]Fabric side Handling ChaincodeMessage of type: TRANSACTION in state ready
2018-02-24 07:26:49.613 UTC [handler.go:1417] [chaincode] filterError -> DEBU 351 Ignoring NoTransitionError: no transition
2018-02-24 07:26:49.613 UTC [handler.go:377] [chaincode] processStream -> DEBU 352 [862dbe7c]sending state message TRANSACTION
2018-02-24 07:26:49.613 UTC [chaincode.go:297] [shim] func1 -> DEBU 353 [862dbe7c]Received message TRANSACTION from shim
2018-02-24 07:26:49.613 UTC [handler.go:778] [shim] handleMessage -> DEBU 354 [862dbe7c]Handling ChaincodeMessage of type: TRANSACTION(state:ready)
2018-02-24 07:26:49.613 UTC [handler.go:338] [shim] beforeTransaction -> DEBU 355 [862dbe7c]Received TRANSACTION, invoking transaction on chaincode(Src:ready, Dst:ready)
2018-02-24 07:26:49.614 UTC [handler.go:392] [shim] handleGetState -> DEBU 356 [862dbe7c]Sending GET_STATE
2018-02-24 07:26:49.615 UTC [handler.go:336] [chaincode] processStream -> DEBU 357 [862dbe7c]Received message GET_STATE from shim
2018-02-24 07:26:49.615 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 358 [862dbe7c]Fabric side Handling ChaincodeMessage of type: GET_STATE in state ready
2018-02-24 07:26:49.615 UTC [handler.go:580] [chaincode] afterGetState -> DEBU 359 [862dbe7c]Received GET_STATE, invoking get state from ledger
2018-02-24 07:26:49.615 UTC [handler.go:1417] [chaincode] filterError -> DEBU 35a Ignoring NoTransitionError: no transition
2018-02-24 07:26:49.615 UTC [handler.go:634] [chaincode] func1 -> DEBU 35b [862dbe7c] getting state for chaincode lscc, key fabcar, channel mychannel
2018-02-24 07:26:49.615 UTC [stateleveldb.go:89] [stateleveldb] GetState -> DEBU 35c GetState(). ns=lscc, key=fabcar
2018-02-24 07:26:49.615 UTC [handler.go:650] [chaincode] func1 -> DEBU 35d [862dbe7c]No state associated with key: fabcar. Sending RESPONSE with an empty payload
2018-02-24 07:26:49.615 UTC [handler.go:621] [chaincode] 1 -> DEBU 35e [862dbe7c]handleGetState serial send RESPONSE
2018-02-24 07:26:49.615 UTC [chaincode.go:297] [shim] func1 -> DEBU 35f [862dbe7c]Received message RESPONSE from shim
2018-02-24 07:26:49.615 UTC [handler.go:778] [shim] handleMessage -> DEBU 360 [862dbe7c]Handling ChaincodeMessage of type: RESPONSE(state:ready)
2018-02-24 07:26:49.615 UTC [handler.go:115] [shim] sendChannel -> DEBU 361 [862dbe7c]before send
2018-02-24 07:26:49.615 UTC [handler.go:117] [shim] sendChannel -> DEBU 362 [862dbe7c]after send
2018-02-24 07:26:49.615 UTC [handler.go:356] [shim] afterResponse -> DEBU 363 [862dbe7c]Received RESPONSE, communicated (state:ready)
2018-02-24 07:26:49.615 UTC [handler.go:402] [shim] handleGetState -> DEBU 364 [862dbe7c]GetState received payload RESPONSE
2018-02-24 07:26:49.617 UTC [handler.go:418] [shim] handlePutState -> DEBU 365 [862dbe7c]Inside putstate
2018-02-24 07:26:49.617 UTC [handler.go:434] [shim] handlePutState -> DEBU 366 [862dbe7c]Sending PUT_STATE
2018-02-24 07:26:49.619 UTC [handler.go:336] [chaincode] processStream -> DEBU 367 [862dbe7c]Received message PUT_STATE from shim
2018-02-24 07:26:49.619 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 368 [862dbe7c]Fabric side Handling ChaincodeMessage of type: PUT_STATE in state ready
2018-02-24 07:26:49.619 UTC [handler.go:1417] [chaincode] filterError -> DEBU 369 Ignoring NoTransitionError: no transition
2018-02-24 07:26:49.619 UTC [handler.go:1140] [chaincode] func1 -> DEBU 36a [862dbe7c]state is ready
2018-02-24 07:26:49.619 UTC [handler.go:1315] [chaincode] func1 -> DEBU 36b [862dbe7c]Completed PUT_STATE. Sending RESPONSE
2018-02-24 07:26:49.619 UTC [handler.go:1159] [chaincode] 1 -> DEBU 36c [862dbe7c]enterBusyState trigger event RESPONSE
2018-02-24 07:26:49.619 UTC [handler.go:357] [chaincode] processStream -> DEBU 36d [862dbe7c]Move state message RESPONSE
2018-02-24 07:26:49.619 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 36e [862dbe7c]Fabric side Handling ChaincodeMessage of type: RESPONSE in state ready
2018-02-24 07:26:49.619 UTC [handler.go:1417] [chaincode] filterError -> DEBU 36f Ignoring NoTransitionError: no transition
2018-02-24 07:26:49.619 UTC [handler.go:377] [chaincode] processStream -> DEBU 370 [862dbe7c]sending state message RESPONSE
2018-02-24 07:26:49.619 UTC [chaincode.go:297] [shim] func1 -> DEBU 371 [862dbe7c]Received message RESPONSE from shim
2018-02-24 07:26:49.619 UTC [handler.go:778] [shim] handleMessage -> DEBU 372 [862dbe7c]Handling ChaincodeMessage of type: RESPONSE(state:ready)
2018-02-24 07:26:49.619 UTC [handler.go:115] [shim] sendChannel -> DEBU 373 [862dbe7c]before send
2018-02-24 07:26:49.619 UTC [handler.go:117] [shim] sendChannel -> DEBU 374 [862dbe7c]after send
2018-02-24 07:26:49.619 UTC [handler.go:356] [shim] afterResponse -> DEBU 375 [862dbe7c]Received RESPONSE, communicated (state:ready)
2018-02-24 07:26:49.619 UTC [handler.go:444] [shim] handlePutState -> DEBU 376 [862dbe7c]Received RESPONSE. Successfully updated state
2018-02-24 07:26:49.620 UTC [handler.go:326] [shim] func1 -> DEBU 377 [862dbe7c]Transaction completed. Sending COMPLETED
2018-02-24 07:26:49.620 UTC [chaincode.go:304] [shim] func1 -> DEBU 378 [862dbe7c]Move state message COMPLETED
2018-02-24 07:26:49.620 UTC [handler.go:778] [shim] handleMessage -> DEBU 379 [862dbe7c]Handling ChaincodeMessage of type: COMPLETED(state:ready)
2018-02-24 07:26:49.620 UTC [chaincode.go:320] [shim] func1 -> DEBU 37a [862dbe7c]send state message COMPLETED
2018-02-24 07:26:49.620 UTC [handler.go:336] [chaincode] processStream -> DEBU 37b [862dbe7c]Received message COMPLETED from shim
2018-02-24 07:26:49.620 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 37c [862dbe7c]Fabric side Handling ChaincodeMessage of type: COMPLETED in state ready
2018-02-24 07:26:49.620 UTC [handler.go:1392] [chaincode] HandleMessage -> DEBU 37d [862dbe7cb0f7e4f5e4cf3fff5d590eca2f3700cb0d40db28b0bbba7f13a473cf]HandleMessage- COMPLETED. Notify
2018-02-24 07:26:49.620 UTC [handler.go:551] [chaincode] notify -> DEBU 37e notifying Txid:862dbe7cb0f7e4f5e4cf3fff5d590eca2f3700cb0d40db28b0bbba7f13a473cf
2018-02-24 07:26:49.620 UTC [chaincode_support.go:751] [chaincode] Execute -> DEBU 37f Exit
2018-02-24 07:26:49.620 UTC [ccprovider.go:363] [ccprovider] NewCCContext -> DEBU 380 NewCCCC (chain=mychannel,chaincode=fabcar,version=1.0,txid=862dbe7cb0f7e4f5e4cf3fff5d590eca2f3700cb0d40db28b0bbba7f13a473cf,syscc=false,proposal=0xc421e16460,canname=fabcar:1.0
2018-02-24 07:26:49.621 UTC [chaincode_support.go:656] [chaincode] Launch -> DEBU 381 launchAndWaitForRegister fetched 2713 bytes from file system
2018-02-24 07:26:49.621 UTC [chaincode_support.go:441] [chaincode] launchAndWaitForRegister -> DEBU 382 chaincode fabcar:1.0 is being launched
2018-02-24 07:26:49.621 UTC [chaincode_support.go:406] [chaincode] getArgsAndEnv -> DEBU 383 Executable is chaincode
2018-02-24 07:26:49.621 UTC [chaincode_support.go:407] [chaincode] getArgsAndEnv -> DEBU 384 Args [chaincode -peer.address=peer0.org1.example.com:7051]
2018-02-24 07:26:49.621 UTC [chaincode_support.go:464] [chaincode] launchAndWaitForRegister -> DEBU 385 start container: fabcar:1.0(networkid:dev,peerid:peer0.org1.example.com)
2018-02-24 07:26:49.621 UTC [chaincode_support.go:465] [chaincode] launchAndWaitForRegister -> DEBU 386 start container with args: chaincode -peer.address=peer0.org1.example.com:7051
2018-02-24 07:26:49.621 UTC [chaincode_support.go:466] [chaincode] launchAndWaitForRegister -> DEBU 387 start container with env:
        CORE_CHAINCODE_ID_NAME=fabcar:1.0
        CORE_PEER_TLS_ENABLED=false
        CORE_CHAINCODE_LOGGING_LEVEL=debug
        CORE_CHAINCODE_LOGGING_SHIM=debug
        CORE_CHAINCODE_LOGGING_FORMAT=%{color}%{time:2006-01-02 15:04:05.000 MST} [%{module}] %{shortfunc} -> %{level:.4s} %{id:03x}%{color:reset} %{message}
2018-02-24 07:26:49.621 UTC [controller.go:91] [container] lockContainer -> DEBU 388 waiting for container(dev-peer0.org1.example.com-fabcar-1.0) lock
2018-02-24 07:26:49.621 UTC [controller.go:93] [container] lockContainer -> DEBU 389 got container (dev-peer0.org1.example.com-fabcar-1.0) lock
2018-02-24 07:26:49.621 UTC [dockercontroller.go:234] [dockercontroller] Start -> DEBU 38a Cleanup container dev-peer0.org1.example.com-fabcar-1.0
2018-02-24 07:26:49.628 UTC [dockercontroller.go:381] [dockercontroller] stopInternal -> DEBU 38b Stop container dev-peer0.org1.example.com-fabcar-1.0(No such container: dev-peer0.org1.example.com-fabcar-1.0)
2018-02-24 07:26:49.629 UTC [dockercontroller.go:388] [dockercontroller] stopInternal -> DEBU 38c Kill container dev-peer0.org1.example.com-fabcar-1.0 (No such container: dev-peer0.org1.example.com-fabcar-1.0)
2018-02-24 07:26:49.630 UTC [dockercontroller.go:396] [dockercontroller] stopInternal -> DEBU 38d Remove container dev-peer0.org1.example.com-fabcar-1.0 (No such container: dev-peer0.org1.example.com-fabcar-1.0)
2018-02-24 07:26:49.630 UTC [dockercontroller.go:237] [dockercontroller] Start -> DEBU 38e Start container dev-peer0.org1.example.com-fabcar-1.0
2018-02-24 07:26:49.630 UTC [dockercontroller.go:120] [dockercontroller] getDockerHostConfig -> DEBU 38f docker container hostconfig NetworkMode: net_basic
2018-02-24 07:26:49.631 UTC [dockercontroller.go:159] [dockercontroller] createContainer -> DEBU 390 Create container: dev-peer0.org1.example.com-fabcar-1.0
2018-02-24 07:26:49.633 UTC [dockercontroller.go:244] [dockercontroller] Start -> DEBU 391 start-could not find image <dev-peer0.org1.example.com-fabcar-1.0-5c906e402ed29f20260ae42283216aa75549c571e2e380f3615826365d8269ba> (container id <dev-peer0.org1.example.com-fabcar-1.0>), because of <no such image>...attempt to recreate image
2018-02-24 07:26:49.634 UTC [platforms.go:139] [chaincode-platform] generateDockerfile -> DEBU 392 
FROM hyperledger/fabric-baseos:x86_64-0.3.2
ADD binpackage.tar /usr/local/bin
LABEL org.hyperledger.fabric.chaincode.id.name="fabcar" \
      org.hyperledger.fabric.chaincode.id.version="1.0" \
      org.hyperledger.fabric.chaincode.type="GOLANG" \
      org.hyperledger.fabric.version="1.0.5" \
      org.hyperledger.fabric.base.version="0.3.2"
ENV CORE_CHAINCODE_BUILDLEVEL=1.0.5
2018-02-24 07:26:49.672 UTC [utils.go:156] [util] DockerBuild -> DEBU 393 Attempting build with image hyperledger/fabric-ccenv:x86_64-1.0.5
2018-02-24 07:27:08.944 UTC [dockercontroller.go:188] [dockercontroller] deployImage -> DEBU 394 Created image: dev-peer0.org1.example.com-fabcar-1.0-5c906e402ed29f20260ae42283216aa75549c571e2e380f3615826365d8269ba
2018-02-24 07:27:08.944 UTC [dockercontroller.go:256] [dockercontroller] Start -> DEBU 395 start-recreated image successfully
2018-02-24 07:27:08.944 UTC [dockercontroller.go:159] [dockercontroller] createContainer -> DEBU 396 Create container: dev-peer0.org1.example.com-fabcar-1.0
2018-02-24 07:27:08.960 UTC [dockercontroller.go:164] [dockercontroller] createContainer -> DEBU 397 Created container: dev-peer0.org1.example.com-fabcar-1.0-5c906e402ed29f20260ae42283216aa75549c571e2e380f3615826365d8269ba
2018-02-24 07:27:09.227 UTC [dockercontroller.go:354] [dockercontroller] Start -> DEBU 398 Started container dev-peer0.org1.example.com-fabcar-1.0
2018-02-24 07:27:09.227 UTC [controller.go:104] [container] unlockContainer -> DEBU 399 container lock deleted(dev-peer0.org1.example.com-fabcar-1.0)
2018-02-24 07:27:09.232 UTC [handler.go:397] [chaincode] HandleChaincodeStream -> DEBU 39a Current context deadline = 0001-01-01 00:00:00 +0000 UTC, ok = false
2018-02-24 07:27:09.233 UTC [handler.go:336] [chaincode] processStream -> DEBU 39b []Received message REGISTER from shim
2018-02-24 07:27:09.233 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 39c []Fabric side Handling ChaincodeMessage of type: REGISTER in state created
2018-02-24 07:27:09.233 UTC [handler.go:510] [chaincode] beforeRegisterEvent -> DEBU 39d Received REGISTER in state created
2018-02-24 07:27:09.233 UTC [chaincode_support.go:291] [chaincode] registerHandler -> DEBU 39e registered handler complete for chaincode fabcar:1.0
2018-02-24 07:27:09.233 UTC [handler.go:536] [chaincode] beforeRegisterEvent -> DEBU 39f Got REGISTER for chaincodeID = name:"fabcar:1.0" , sending back REGISTERED
2018-02-24 07:27:09.233 UTC [handler.go:484] [chaincode] notifyDuringStartup -> DEBU 3a0 Notifying during startup
2018-02-24 07:27:09.233 UTC [chaincode_support.go:452] [chaincode] func1 -> DEBU 3a1 chaincode fabcar:1.0 launch seq completed
2018-02-24 07:27:09.233 UTC [handler.go:1371] [chaincode] ready -> DEBU 3a2 sending READY
2018-02-24 07:27:09.233 UTC [handler.go:1350] [chaincode] setChaincodeProposal -> DEBU 3a3 Setting chaincode proposal context...
2018-02-24 07:27:09.233 UTC [handler.go:1352] [chaincode] setChaincodeProposal -> DEBU 3a4 Proposal different from nil. Creating chaincode proposal context...
2018-02-24 07:27:09.233 UTC [handler.go:357] [chaincode] processStream -> DEBU 3a5 [862dbe7c]Move state message READY
2018-02-24 07:27:09.233 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 3a6 [862dbe7c]Fabric side Handling ChaincodeMessage of type: READY in state established
2018-02-24 07:27:09.233 UTC [handler.go:1332] [chaincode] enterReadyState -> DEBU 3a7 [862dbe7c]Entered state ready
2018-02-24 07:27:09.233 UTC [handler.go:551] [chaincode] notify -> DEBU 3a8 notifying Txid:862dbe7cb0f7e4f5e4cf3fff5d590eca2f3700cb0d40db28b0bbba7f13a473cf
2018-02-24 07:27:09.233 UTC [handler.go:377] [chaincode] processStream -> DEBU 3a9 [862dbe7c]sending state message READY
2018-02-24 07:27:09.234 UTC [chaincode_support.go:681] [chaincode] Launch -> DEBU 3aa sending init completed
2018-02-24 07:27:09.234 UTC [chaincode_support.go:684] [chaincode] Launch -> DEBU 3ab LaunchChaincode complete
2018-02-24 07:27:09.234 UTC [chaincode_support.go:720] [chaincode] Execute -> DEBU 3ac Entry
2018-02-24 07:27:09.234 UTC [chaincode_support.go:723] [chaincode] Execute -> DEBU 3ad chaincode canonical name: fabcar:1.0
2018-02-24 07:27:09.234 UTC [handler.go:1436] [chaincode] sendExecuteMessage -> DEBU 3ae [862dbe7c]Inside sendExecuteMessage. Message INIT
2018-02-24 07:27:09.234 UTC [handler.go:1350] [chaincode] setChaincodeProposal -> DEBU 3af Setting chaincode proposal context...
2018-02-24 07:27:09.234 UTC [handler.go:1352] [chaincode] setChaincodeProposal -> DEBU 3b0 Proposal different from nil. Creating chaincode proposal context...
2018-02-24 07:27:09.235 UTC [handler.go:1444] [chaincode] sendExecuteMessage -> DEBU 3b1 [862dbe7c]sendExecuteMsg trigger event INIT
2018-02-24 07:27:09.235 UTC [handler.go:357] [chaincode] processStream -> DEBU 3b2 [862dbe7c]Move state message INIT
2018-02-24 07:27:09.235 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 3b3 [862dbe7c]Fabric side Handling ChaincodeMessage of type: INIT in state ready
2018-02-24 07:27:09.235 UTC [handler.go:1417] [chaincode] filterError -> DEBU 3b4 Ignoring NoTransitionError: no transition
2018-02-24 07:27:09.235 UTC [handler.go:377] [chaincode] processStream -> DEBU 3b5 [862dbe7c]sending state message INIT
2018-02-24 07:27:09.237 UTC [handler.go:336] [chaincode] processStream -> DEBU 3b6 [862dbe7c]Received message COMPLETED from shim
2018-02-24 07:27:09.237 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 3b7 [862dbe7c]Fabric side Handling ChaincodeMessage of type: COMPLETED in state ready
2018-02-24 07:27:09.237 UTC [handler.go:1392] [chaincode] HandleMessage -> DEBU 3b8 [862dbe7cb0f7e4f5e4cf3fff5d590eca2f3700cb0d40db28b0bbba7f13a473cf]HandleMessage- COMPLETED. Notify
2018-02-24 07:27:09.237 UTC [handler.go:551] [chaincode] notify -> DEBU 3b9 notifying Txid:862dbe7cb0f7e4f5e4cf3fff5d590eca2f3700cb0d40db28b0bbba7f13a473cf
2018-02-24 07:27:09.237 UTC [chaincode_support.go:751] [chaincode] Execute -> DEBU 3ba Exit
2018-02-24 07:27:09.237 UTC [endorser.go:156] [endorser] callChaincode -> DEBU 3bb Exit
2018-02-24 07:27:09.237 UTC [lockbased_tx_simulator.go:72] [lockbasedtxmgr] GetTxSimulationResults -> DEBU 3bc Simulation completed, getting simulation results
2018-02-24 07:27:09.237 UTC [lockbased_query_executer.go:62] [lockbasedtxmgr] Done -> DEBU 3bd Done with transaction simulation / query execution [dd3356ae-bd86-4a74-806c-677322bd2860]
2018-02-24 07:27:09.237 UTC [endorser.go:265] [endorser] simulateProposal -> DEBU 3be Exit
2018-02-24 07:27:09.237 UTC [endorser.go:500] [endorser] ProcessProposal -> DEBU 3bf [gerry] 2 -- endorse and get a marshalled ProposalResponse message
2018-02-24 07:27:09.237 UTC [endorser.go:279] [endorser] endorseProposal -> DEBU 3c0 Entry - txid: 862dbe7cb0f7e4f5e4cf3fff5d590eca2f3700cb0d40db28b0bbba7f13a473cf channel id: mychannel chaincode id: name:"lscc" 
2018-02-24 07:27:09.237 UTC [endorser.go:282] [endorser] endorseProposal -> DEBU 3c1 [gerry] *ccprovider.ChaincodeData: <nil>
2018-02-24 07:27:09.237 UTC [endorser.go:299] [endorser] endorseProposal -> DEBU 3c2 info: escc for chaincode id name:"lscc"  is escc
2018-02-24 07:27:09.237 UTC [endorser.go:99] [endorser] callChaincode -> DEBU 3c3 Entry - txid: 862dbe7cb0f7e4f5e4cf3fff5d590eca2f3700cb0d40db28b0bbba7f13a473cf channel id: mychannel version: 1.0.5
2018-02-24 07:27:09.238 UTC [ccprovider.go:363] [ccprovider] NewCCContext -> DEBU 3c4 NewCCCC (chain=mychannel,chaincode=escc,version=1.0.5,txid=862dbe7cb0f7e4f5e4cf3fff5d590eca2f3700cb0d40db28b0bbba7f13a473cf,syscc=true,proposal=0xc421e16460,canname=escc:1.0.5
2018-02-24 07:27:09.238 UTC [chaincode_support.go:585] [chaincode] Launch -> DEBU 3c5 chaincode is running(no need to launch) : escc:1.0.5
2018-02-24 07:27:09.238 UTC [chaincode_support.go:720] [chaincode] Execute -> DEBU 3c6 Entry
2018-02-24 07:27:09.238 UTC [chaincode_support.go:723] [chaincode] Execute -> DEBU 3c7 chaincode canonical name: escc:1.0.5
2018-02-24 07:27:09.238 UTC [handler.go:1436] [chaincode] sendExecuteMessage -> DEBU 3c8 [862dbe7c]Inside sendExecuteMessage. Message TRANSACTION
2018-02-24 07:27:09.238 UTC [handler.go:1350] [chaincode] setChaincodeProposal -> DEBU 3c9 Setting chaincode proposal context...
2018-02-24 07:27:09.238 UTC [handler.go:1352] [chaincode] setChaincodeProposal -> DEBU 3ca Proposal different from nil. Creating chaincode proposal context...
2018-02-24 07:27:09.238 UTC [handler.go:1444] [chaincode] sendExecuteMessage -> DEBU 3cb [862dbe7c]sendExecuteMsg trigger event TRANSACTION
2018-02-24 07:27:09.238 UTC [handler.go:357] [chaincode] processStream -> DEBU 3cc [862dbe7c]Move state message TRANSACTION
2018-02-24 07:27:09.238 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 3cd [862dbe7c]Fabric side Handling ChaincodeMessage of type: TRANSACTION in state ready
2018-02-24 07:27:09.238 UTC [handler.go:1417] [chaincode] filterError -> DEBU 3ce Ignoring NoTransitionError: no transition
2018-02-24 07:27:09.238 UTC [handler.go:377] [chaincode] processStream -> DEBU 3cf [862dbe7c]sending state message TRANSACTION
2018-02-24 07:27:09.238 UTC [chaincode.go:297] [shim] func1 -> DEBU 3d0 [862dbe7c]Received message TRANSACTION from shim
2018-02-24 07:27:09.238 UTC [handler.go:778] [shim] handleMessage -> DEBU 3d1 [862dbe7c]Handling ChaincodeMessage of type: TRANSACTION(state:ready)
2018-02-24 07:27:09.238 UTC [handler.go:338] [shim] beforeTransaction -> DEBU 3d2 [862dbe7c]Received TRANSACTION, invoking transaction on chaincode(Src:ready, Dst:ready)
2018-02-24 07:27:09.238 UTC [endorser_onevalidsignature.go:74] [escc] Invoke -> DEBU 3d3 ESCC starts: 8 args
2018-02-24 07:27:09.241 UTC [endorser_onevalidsignature.go:177] [escc] Invoke -> DEBU 3d4 ESCC exits successfully
2018-02-24 07:27:09.242 UTC [handler.go:326] [shim] func1 -> DEBU 3d5 [862dbe7c]Transaction completed. Sending COMPLETED
2018-02-24 07:27:09.242 UTC [chaincode.go:304] [shim] func1 -> DEBU 3d6 [862dbe7c]Move state message COMPLETED
2018-02-24 07:27:09.242 UTC [handler.go:778] [shim] handleMessage -> DEBU 3d7 [862dbe7c]Handling ChaincodeMessage of type: COMPLETED(state:ready)
2018-02-24 07:27:09.242 UTC [chaincode.go:320] [shim] func1 -> DEBU 3d8 [862dbe7c]send state message COMPLETED
2018-02-24 07:27:09.242 UTC [handler.go:336] [chaincode] processStream -> DEBU 3d9 [862dbe7c]Received message COMPLETED from shim
2018-02-24 07:27:09.242 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 3da [862dbe7c]Fabric side Handling ChaincodeMessage of type: COMPLETED in state ready
2018-02-24 07:27:09.242 UTC [handler.go:1392] [chaincode] HandleMessage -> DEBU 3db [862dbe7cb0f7e4f5e4cf3fff5d590eca2f3700cb0d40db28b0bbba7f13a473cf]HandleMessage- COMPLETED. Notify
2018-02-24 07:27:09.242 UTC [handler.go:551] [chaincode] notify -> DEBU 3dc notifying Txid:862dbe7cb0f7e4f5e4cf3fff5d590eca2f3700cb0d40db28b0bbba7f13a473cf
2018-02-24 07:27:09.242 UTC [chaincode_support.go:751] [chaincode] Execute -> DEBU 3dd Exit
2018-02-24 07:27:09.242 UTC [endorser.go:156] [endorser] callChaincode -> DEBU 3de Exit
2018-02-24 07:27:09.242 UTC [endorser.go:368] [endorser] endorseProposal -> DEBU 3df Exit
2018-02-24 07:27:09.242 UTC [endorser.go:523] [endorser] ProcessProposal -> DEBU 3e0 go here txid: 862dbe7cb0f7e4f5e4cf3fff5d590eca2f3700cb0d40db28b0bbba7f13a473cf
2018-02-24 07:27:09.242 UTC [lockbased_query_executer.go:62] [lockbasedtxmgr] Done -> DEBU 3e1 Done with transaction simulation / query execution [dd3356ae-bd86-4a74-806c-677322bd2860]
2018-02-24 07:27:09.242 UTC [endorser.go:530] [endorser] ProcessProposal -> DEBU 3e2 Exit
2018-02-24 07:27:11.277 UTC [blocksprovider.go:184] [blocksProvider] DeliverBlocks -> DEBU 3e3 [mychannel] Adding payload locally, buffer seqNum = [1], peers number [1]
2018-02-24 07:27:11.285 UTC [blocksprovider.go:191] [blocksProvider] DeliverBlocks -> DEBU 3e4 [mychannel] Gossiping block [1], peers number [1]
2018-02-24 07:27:11.288 UTC [committer_impl.go:73] [committer] Commit -> DEBU 3e5 Validating block
2018-02-24 07:27:11.288 UTC [validator.go:146] [txvalidator] Validate -> DEBU 3e6 START Block Validation
2018-02-24 07:27:11.288 UTC [validator.go:165] [txvalidator] Validate -> DEBU 3e7 Validating transaction peer.ValidateTransaction()
2018-02-24 07:27:11.288 UTC [msgvalidation.go:362] [protoutils] ValidateTransaction -> DEBU 3e8 ValidateTransactionEnvelope starts for envelope 0xc4214b7e30
2018-02-24 07:27:11.289 UTC [msgvalidation.go:377] [protoutils] ValidateTransaction -> DEBU 3e9 Header is channel_header:"\010\003\032\014\010\271\247\304\324\005\020\240\237\275\241\002\"\tmychannel*@862dbe7cb0f7e4f5e4cf3fff5d590eca2f3700cb0d40db28b0bbba7f13a473cf:\010\022\006\022\004lscc" signature_header:"\n\214\006\n\007Org1MSP\022\200\006-----BEGIN -----\nMIICGTCCAb+gAwIBAgIQU2EEaekZq1LsI6bZEdL9rzAKBggqhkjOPQQDAjBzMQsw\nCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy\nYW5jaXNjbzEZMBcGA1UEChMQb3JnMS5leGFtcGxlLmNvbTEcMBoGA1UEAxMTY2Eu\nb3JnMS5leGFtcGxlLmNvbTAeFw0xODAyMDcwMDE1MTRaFw0yODAyMDUwMDE1MTRa\nMFsxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1T\nYW4gRnJhbmNpc2NvMR8wHQYDVQQDDBZBZG1pbkBvcmcxLmV4YW1wbGUuY29tMFkw\nEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE01kUw4VY/YsrMtL8eZxZchnBuzZNPOfj\nJN97M78wxDGWVVnHDhrMFemCJ0r3MLJw2J/AJ869y9HdUrDWX0lo+6NNMEswDgYD\nVR0PAQH/BAQDAgeAMAwGA1UdEwEB/wQCMAAwKwYDVR0jBCQwIoAgT8YQQJTfOVXI\nEk6JW0JtqfNLN5Zs9iPXOc3LEMJGYE0wCgYIKoZIzj0EAwIDSAAwRQIhAJpvtr6J\nEKkSikx802ESGmwdxejmU2HT4vjySUluYCS7AiBAO7bP3X2sIFYCF/A94bBjfcQR\nXNvYJqPJ+Y/zLYo2PQ==\n-----END -----\n\022\030\023\261\266\345\005\255be\026T2S\331\255f\030Y\350\257\215R0\273%" 
2018-02-24 07:27:11.291 UTC [msgvalidation.go:215] [protoutils] validateChannelHeader -> DEBU 3ea validateChannelHeader info: header type 3
2018-02-24 07:27:11.291 UTC [msgvalidation.go:142] [protoutils] checkSignatureFromCreator -> DEBU 3eb checkSignatureFromCreator starts
2018-02-24 07:27:11.293 UTC [msgvalidation.go:160] [protoutils] checkSignatureFromCreator -> DEBU 3ec checkSignatureFromCreator info: creator is &{Org1MSP 3dc70c07d3096e889a28bca219d07b0d858baede9b7d60760c9bf5a76017c9a1}
2018-02-24 07:27:11.293 UTC [msgvalidation.go:168] [protoutils] checkSignatureFromCreator -> DEBU 3ed checkSignatureFromCreator info: creator is valid
2018-02-24 07:27:11.296 UTC [msgvalidation.go:176] [protoutils] checkSignatureFromCreator -> DEBU 3ee checkSignatureFromCreator exists successfully
2018-02-24 07:27:11.299 UTC [msgvalidation.go:279] [protoutils] validateEndorserTransaction -> DEBU 3ef validateEndorserTransaction starts for data 0xc421af3400, header channel_header:"\010\003\032\014\010\271\247\304\324\005\020\240\237\275\241\002\"\tmychannel*@862dbe7cb0f7e4f5e4cf3fff5d590eca2f3700cb0d40db28b0bbba7f13a473cf:\010\022\006\022\004lscc" signature_header:"\n\214\006\n\007Org1MSP\022\200\006-----BEGIN -----\nMIICGTCCAb+gAwIBAgIQU2EEaekZq1LsI6bZEdL9rzAKBggqhkjOPQQDAjBzMQsw\nCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy\nYW5jaXNjbzEZMBcGA1UEChMQb3JnMS5leGFtcGxlLmNvbTEcMBoGA1UEAxMTY2Eu\nb3JnMS5leGFtcGxlLmNvbTAeFw0xODAyMDcwMDE1MTRaFw0yODAyMDUwMDE1MTRa\nMFsxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1T\nYW4gRnJhbmNpc2NvMR8wHQYDVQQDDBZBZG1pbkBvcmcxLmV4YW1wbGUuY29tMFkw\nEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE01kUw4VY/YsrMtL8eZxZchnBuzZNPOfj\nJN97M78wxDGWVVnHDhrMFemCJ0r3MLJw2J/AJ869y9HdUrDWX0lo+6NNMEswDgYD\nVR0PAQH/BAQDAgeAMAwGA1UdEwEB/wQCMAAwKwYDVR0jBCQwIoAgT8YQQJTfOVXI\nEk6JW0JtqfNLN5Zs9iPXOc3LEMJGYE0wCgYIKoZIzj0EAwIDSAAwRQIhAJpvtr6J\nEKkSikx802ESGmwdxejmU2HT4vjySUluYCS7AiBAO7bP3X2sIFYCF/A94bBjfcQR\nXNvYJqPJ+Y/zLYo2PQ==\n-----END -----\n\022\030\023\261\266\345\005\255be\026T2S\331\255f\030Y\350\257\215R0\273%" 
2018-02-24 07:27:11.313 UTC [msgvalidation.go:306] [protoutils] validateEndorserTransaction -> DEBU 3f0 validateEndorserTransaction info: there are 1 actions
2018-02-24 07:27:11.313 UTC [msgvalidation.go:327] [protoutils] validateEndorserTransaction -> DEBU 3f1 validateEndorserTransaction info: signature header is valid
2018-02-24 07:27:11.313 UTC [msgvalidation.go:412] [protoutils] ValidateTransaction -> DEBU 3f2 ValidateTransactionEnvelope returns err %!s(<nil>)
2018-02-24 07:27:11.313 UTC [validator.go:184] [txvalidator] Validate -> DEBU 3f3 Transaction is for chain mychannel
2018-02-24 07:27:11.315 UTC [blockfile_mgr.go:506] [fsblkstorage] retrieveTransactionByID -> DEBU 3f4 retrieveTransactionByID() - txId = [862dbe7cb0f7e4f5e4cf3fff5d590eca2f3700cb0d40db28b0bbba7f13a473cf]
2018-02-24 07:27:11.315 UTC [validator.go:202] [txvalidator] Validate -> DEBU 3f5 Validating transaction vscc tx validate
2018-02-24 07:27:11.315 UTC [lockbased_txmgr.go:64] [lockbasedtxmgr] NewTxSimulator -> DEBU 3f6 constructing new tx simulator
2018-02-24 07:27:11.315 UTC [lockbased_tx_simulator.go:36] [lockbasedtxmgr] newLockBasedTxSimulator -> DEBU 3f7 constructing new tx simulator [56b4dc9f-0f52-4365-aa8c-0ad833a7f334]
2018-02-24 07:27:11.315 UTC [ccprovider.go:363] [ccprovider] NewCCContext -> DEBU 3f8 NewCCCC (chain=mychannel,chaincode=vscc,version=1.0.5,txid=b6487542-4718-4baf-bf91-c03e2a78be6d,syscc=true,proposal=0x0,canname=vscc:1.0.5
2018-02-24 07:27:11.315 UTC [validator.go:622] [txvalidator] VSCCValidateTxForCC -> DEBU 3f9 Invoking VSCC txid 862dbe7cb0f7e4f5e4cf3fff5d590eca2f3700cb0d40db28b0bbba7f13a473cf chaindID mychannel
2018-02-24 07:27:11.315 UTC [chaincode_support.go:585] [chaincode] Launch -> DEBU 3fa chaincode is running(no need to launch) : vscc:1.0.5
2018-02-24 07:27:11.315 UTC [chaincode_support.go:720] [chaincode] Execute -> DEBU 3fb Entry
2018-02-24 07:27:11.316 UTC [chaincode_support.go:723] [chaincode] Execute -> DEBU 3fc chaincode canonical name: vscc:1.0.5
2018-02-24 07:27:11.316 UTC [handler.go:1436] [chaincode] sendExecuteMessage -> DEBU 3fd [b6487542]Inside sendExecuteMessage. Message TRANSACTION
2018-02-24 07:27:11.316 UTC [handler.go:1350] [chaincode] setChaincodeProposal -> DEBU 3fe Setting chaincode proposal context...
2018-02-24 07:27:11.316 UTC [handler.go:1444] [chaincode] sendExecuteMessage -> DEBU 3ff [b6487542]sendExecuteMsg trigger event TRANSACTION
2018-02-24 07:27:11.316 UTC [handler.go:357] [chaincode] processStream -> DEBU 400 [b6487542]Move state message TRANSACTION
2018-02-24 07:27:11.316 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 401 [b6487542]Fabric side Handling ChaincodeMessage of type: TRANSACTION in state ready
2018-02-24 07:27:11.316 UTC [handler.go:1417] [chaincode] filterError -> DEBU 402 Ignoring NoTransitionError: no transition
2018-02-24 07:27:11.316 UTC [handler.go:377] [chaincode] processStream -> DEBU 403 [b6487542]sending state message TRANSACTION
2018-02-24 07:27:11.316 UTC [chaincode.go:297] [shim] func1 -> DEBU 404 [b6487542]Received message TRANSACTION from shim
2018-02-24 07:27:11.316 UTC [handler.go:778] [shim] handleMessage -> DEBU 405 [b6487542]Handling ChaincodeMessage of type: TRANSACTION(state:ready)
2018-02-24 07:27:11.317 UTC [handler.go:338] [shim] beforeTransaction -> DEBU 406 [b6487542]Received TRANSACTION, invoking transaction on chaincode(Src:ready, Dst:ready)
2018-02-24 07:27:11.317 UTC [validator_onevalidsignature.go:91] [vscc] Invoke -> DEBU 407 VSCC invoked
2018-02-24 07:27:11.317 UTC [validator_onevalidsignature.go:457] [vscc] deduplicateIdentity -> DEBU 408 Signature set is of size 1 out of 1 endorsement(s)
2018-02-24 07:27:11.318 UTC [validator_onevalidsignature.go:166] [vscc] Invoke -> DEBU 409 VSCC info: doing special validation for LSCC
2018-02-24 07:27:11.319 UTC [validator_onevalidsignature.go:240] [vscc] ValidateLSCCInvocation -> DEBU 40a VSCC info: ValidateLSCCInvocation acting on deploy [][]uint8{[]uint8{0x6d, 0x79, 0x63, 0x68, 0x61, 0x6e, 0x6e, 0x65, 0x6c}, []uint8{0xa, 0x15, 0x8, 0x1, 0x12, 0xd, 0x12, 0x6, 0x66, 0x61, 0x62, 0x63, 0x61, 0x72, 0x1a, 0x3, 0x31, 0x2e, 0x30, 0x1a, 0x2, 0xa, 0x0}, []uint8{0x12, 0xc, 0x12, 0xa, 0x8, 0x2, 0x12, 0x2, 0x8, 0x0, 0x12, 0x2, 0x8, 0x1, 0x1a, 0xb, 0x12, 0x9, 0xa, 0x7, 0x4f, 0x72, 0x67, 0x31, 0x4d, 0x53, 0x50, 0x1a, 0xb, 0x12, 0x9, 0xa, 0x7, 0x4f, 0x72, 0x67, 0x32, 0x4d, 0x53, 0x50}, []uint8{0x65, 0x73, 0x63, 0x63}, []uint8{0x76, 0x73, 0x63, 0x63}}
2018-02-24 07:27:11.319 UTC [validator_onevalidsignature.go:244] [vscc] ValidateLSCCInvocation -> DEBU 40b VSCC info: validating invocation of lscc function deploy on arguments [][]uint8{[]uint8{0x6d, 0x79, 0x63, 0x68, 0x61, 0x6e, 0x6e, 0x65, 0x6c}, []uint8{0xa, 0x15, 0x8, 0x1, 0x12, 0xd, 0x12, 0x6, 0x66, 0x61, 0x62, 0x63, 0x61, 0x72, 0x1a, 0x3, 0x31, 0x2e, 0x30, 0x1a, 0x2, 0xa, 0x0}, []uint8{0x12, 0xc, 0x12, 0xa, 0x8, 0x2, 0x12, 0x2, 0x8, 0x0, 0x12, 0x2, 0x8, 0x1, 0x1a, 0xb, 0x12, 0x9, 0xa, 0x7, 0x4f, 0x72, 0x67, 0x31, 0x4d, 0x53, 0x50, 0x1a, 0xb, 0x12, 0x9, 0xa, 0x7, 0x4f, 0x72, 0x67, 0x32, 0x4d, 0x53, 0x50}, []uint8{0x65, 0x73, 0x63, 0x63}, []uint8{0x76, 0x73, 0x63, 0x63}}
2018-02-24 07:27:11.319 UTC [validator_onevalidsignature.go:280] [vscc] ValidateLSCCInvocation -> DEBU 40c Namespace lscc
2018-02-24 07:27:11.319 UTC [lockbased_query_executer.go:33] [lockbasedtxmgr] newQueryExecutor -> DEBU 40d constructing new query executor [6c50de34-8153-4376-b043-f00f0537f010]
2018-02-24 07:27:11.319 UTC [stateleveldb.go:89] [stateleveldb] GetState -> DEBU 40e GetState(). ns=lscc, key=fabcar
2018-02-24 07:27:11.319 UTC [lockbased_query_executer.go:62] [lockbasedtxmgr] Done -> DEBU 40f Done with transaction simulation / query execution [6c50de34-8153-4376-b043-f00f0537f010]
2018-02-24 07:27:11.319 UTC [validator_onevalidsignature.go:329] [vscc] ValidateLSCCInvocation -> DEBU 410 Validating deploy for cc fabcar version 1.0
2018-02-24 07:27:11.320 UTC [validator_onevalidsignature.go:195] [vscc] checkInstantiationPolicy -> DEBU 411 VSCC info: checkInstantiationPolicy starts, policy is &cauthdsl.policy{evaluator:(func([]*common.SignedData, []bool) bool)(0x65bc40)}
2018-02-24 07:27:11.320 UTC [validator_onevalidsignature.go:176] [vscc] Invoke -> DEBU 412 VSCC exists successfully
2018-02-24 07:27:11.321 UTC [handler.go:326] [shim] func1 -> DEBU 413 [b6487542]Transaction completed. Sending COMPLETED
2018-02-24 07:27:11.321 UTC [chaincode.go:304] [shim] func1 -> DEBU 414 [b6487542]Move state message COMPLETED
2018-02-24 07:27:11.322 UTC [handler.go:778] [shim] handleMessage -> DEBU 415 [b6487542]Handling ChaincodeMessage of type: COMPLETED(state:ready)
2018-02-24 07:27:11.326 UTC [chaincode.go:320] [shim] func1 -> DEBU 416 [b6487542]send state message COMPLETED
2018-02-24 07:27:11.326 UTC [handler.go:336] [chaincode] processStream -> DEBU 417 [b6487542]Received message COMPLETED from shim
2018-02-24 07:27:11.326 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 418 [b6487542]Fabric side Handling ChaincodeMessage of type: COMPLETED in state ready
2018-02-24 07:27:11.326 UTC [handler.go:1392] [chaincode] HandleMessage -> DEBU 419 [b6487542-4718-4baf-bf91-c03e2a78be6d]HandleMessage- COMPLETED. Notify
2018-02-24 07:27:11.326 UTC [handler.go:551] [chaincode] notify -> DEBU 41a notifying Txid:b6487542-4718-4baf-bf91-c03e2a78be6d
2018-02-24 07:27:11.326 UTC [chaincode_support.go:751] [chaincode] Execute -> DEBU 41b Exit
2018-02-24 07:27:11.326 UTC [lockbased_query_executer.go:62] [lockbasedtxmgr] Done -> DEBU 41c Done with transaction simulation / query execution [56b4dc9f-0f52-4365-aa8c-0ad833a7f334]
2018-02-24 07:27:11.326 UTC [validator.go:271] [txvalidator] Validate -> DEBU 41d END Block Validation
2018-02-24 07:27:11.327 UTC [kv_ledger.go:215] [kvledger] Commit -> DEBU 41e Channel [mychannel]: Validating block [1]
2018-02-24 07:27:11.327 UTC [lockbased_txmgr.go:72] [lockbasedtxmgr] ValidateAndPrepare -> DEBU 41f Validating new block with num trans = [1]
2018-02-24 07:27:11.327 UTC [state_based_validator.go:78] [statevalidator] ValidateAndPrepareBatch -> DEBU 420 New block arrived for validation:&common.Block{Header:(*common.BlockHeader)(0xc421490c40), Data:(*common.BlockData)(0xc4219454c0), Metadata:(*common.BlockMetadata)(0xc421945500)}, doMVCCValidation=true
2018-02-24 07:27:11.327 UTC [state_based_validator.go:80] [statevalidator] ValidateAndPrepareBatch -> DEBU 421 Validating a block with [1] transactions
2018-02-24 07:27:11.327 UTC [stateleveldb.go:89] [stateleveldb] GetState -> DEBU 422 GetState(). ns=lscc, key=fabcar
2018-02-24 07:27:11.327 UTC [state_based_validator.go:139] [statevalidator] ValidateAndPrepareBatch -> DEBU 423 Block [1] Transaction index [0] TxId [862dbe7cb0f7e4f5e4cf3fff5d590eca2f3700cb0d40db28b0bbba7f13a473cf] marked as valid by state validator
2018-02-24 07:27:11.327 UTC [kv_ledger.go:221] [kvledger] Commit -> DEBU 424 Channel [mychannel]: Committing block [1] to storage
2018-02-24 07:27:11.328 UTC [blockindex.go:98] [fsblkstorage] indexBlock -> DEBU 425 Indexing block [blockNum=1, blockHash=[]byte{0x2e, 0x2c, 0xe8, 0x97, 0xbc, 0x74, 0x6c, 0x52, 0x97, 0xff, 0x56, 0x15, 0x25, 0xa3, 0x69, 0xa6, 0x8a, 0x4a, 0xdf, 0x70, 0x4, 0x93, 0x5a, 0x78, 0x4b, 0xe1, 0xac, 0xfe, 0x2b, 0x1c, 0x6a, 0x30} txOffsets=
txId=862dbe7cb0f7e4f5e4cf3fff5d590eca2f3700cb0d40db28b0bbba7f13a473cf locPointer=offset=70, bytesLength=3334
]
2018-02-24 07:27:11.328 UTC [blockindex.go:122] [fsblkstorage] indexBlock -> DEBU 426 Adding txLoc [fileSuffixNum=0, offset=11983, bytesLength=3334] for tx ID: [862dbe7cb0f7e4f5e4cf3fff5d590eca2f3700cb0d40db28b0bbba7f13a473cf] to index
2018-02-24 07:27:11.328 UTC [blockindex.go:135] [fsblkstorage] indexBlock -> DEBU 427 Adding txLoc [fileSuffixNum=0, offset=11983, bytesLength=3334] for tx number:[0] ID: [862dbe7cb0f7e4f5e4cf3fff5d590eca2f3700cb0d40db28b0bbba7f13a473cf] to blockNumTranNum index
2018-02-24 07:27:11.329 UTC [blockfile_mgr.go:430] [fsblkstorage] updateCheckpoint -> DEBU 428 Broadcasting about update checkpointInfo: latestFileChunkSuffixNum=[0], latestFileChunksize=[17073], isChainEmpty=[false], lastBlockNumber=[1]
2018-02-24 07:27:11.329 UTC [kv_ledger.go:225] [kvledger] Commit -> INFO 429 Channel [mychannel]: Created block [1] with 1 transaction(s)
2018-02-24 07:27:11.330 UTC [kv_ledger.go:227] [kvledger] Commit -> DEBU 42a Channel [mychannel]: Committing block [1] transactions to state database
2018-02-24 07:27:11.330 UTC [lockbased_txmgr.go:89] [lockbasedtxmgr] Commit -> DEBU 42b Committing updates to state database
2018-02-24 07:27:11.330 UTC [lockbased_txmgr.go:92] [lockbasedtxmgr] Commit -> DEBU 42c Write lock acquired for committing updates to state database
2018-02-24 07:27:11.331 UTC [stateleveldb.go:141] [stateleveldb] ApplyUpdates -> DEBU 42d Channel [mychannel]: Applying key=[[]byte{0x6c, 0x73, 0x63, 0x63, 0x0, 0x66, 0x61, 0x62, 0x63, 0x61, 0x72}]
2018-02-24 07:27:11.333 UTC [lockbased_txmgr.go:101] [lockbasedtxmgr] Commit -> DEBU 42e Updates committed to state database
2018-02-24 07:27:11.333 UTC [kv_ledger.go:234] [kvledger] Commit -> DEBU 42f Channel [mychannel]: Committing block [1] transactions to history database
2018-02-24 07:27:11.333 UTC [historyleveldb.go:93] [historyleveldb] Commit -> DEBU 430 Channel [mychannel]: Updating history database for blockNo [1] with [1] transactions
2018-02-24 07:27:11.338 UTC [historyleveldb.go:177] [historyleveldb] Commit -> DEBU 431 Channel [mychannel]: Updates committed to history database for blockNo [1]
2018-02-24 07:27:11.338 UTC [eventhelper.go:29] [eventhub_producer] SendProducerBlockEvent -> DEBU 432 Entry
2018-02-24 07:27:11.338 UTC [eventhelper.go:55] [eventhub_producer] SendProducerBlockEvent -> DEBU 433 Channel [mychannel]: Block event for block number [1] contains transaction id: 862dbe7cb0f7e4f5e4cf3fff5d590eca2f3700cb0d40db28b0bbba7f13a473cf
2018-02-24 07:27:11.338 UTC [eventhelper.go:105] [eventhub_producer] SendProducerBlockEvent -> INFO 434 Channel [mychannel]: Sending event for block number [1]
2018-02-24 07:27:11.338 UTC [events.go:312] [eventhub_producer] Send -> DEBU 435 Entry
2018-02-24 07:27:11.338 UTC [events.go:335] [eventhub_producer] Send -> DEBU 436 Event processor timeout > 0
2018-02-24 07:27:11.338 UTC [events.go:343] [eventhub_producer] Send -> DEBU 437 Event sent successfully
2018-02-24 07:27:11.338 UTC [events.go:344] [eventhub_producer] Send -> DEBU 438 Exit
2018-02-24 07:27:11.338 UTC [eventhelper.go:107] [eventhub_producer] SendProducerBlockEvent -> DEBU 439 Exit
2018-02-24 07:27:11.339 UTC [register_internal_events.go:41] [eventhub_producer] getMessageType -> DEBU 43a [gerry] Event_Block
```

### instantiate chaincode (order 日志)

```
2018-02-24 07:26:49.602 UTC [orderer/main] /opt/gopath/src/github.com/hyperledger/fabric/orderer/server.go:74 Broadcast -> DEBU 911 Starting new Broadcast handler
2018-02-24 07:26:49.602 UTC [orderer/common/broadcast] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/broadcast/broadcast.go:77 Handle -> DEBU 912 Starting new broadcast loop
2018-02-24 07:27:09.244 UTC [orderer/common/broadcast] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/broadcast/broadcast.go:138 Handle -> DEBU 913 [channel: mychannel] Broadcast is filtering message of type ENDORSER_TRANSACTION
2018-02-24 07:27:09.246 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 914 Returning policy Writers for evaluation
2018-02-24 07:27:09.246 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:58 func1 -> DEBU 915 0xc420a5c038 gate 1519457229246961438 evaluation starts
2018-02-24 07:27:09.247 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:83 func2 -> DEBU 916 0xc420a5c038 signed by 0 principal evaluation starts (used [false])
2018-02-24 07:27:09.247 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:91 func2 -> DEBU 917 0xc420a5c038 processing identity 0 with bytes of 0a074f7267314d53501280062d2d2d2d2d424547494e202d2d2d2d2d0a4d4949434754434341622b6741774942416749515532454561656b5a71314c734936625a45644c39727a414b42676771686b6a4f50515144416a427a4d5173770a435159445651514745774a56557a45544d4245474131554543424d4b5132467361575a76636d3570595445574d4251474131554542784d4e5532467549455a790a5957356a61584e6a627a455a4d4263474131554543684d5162334a6e4d53356c654746746347786c4c6d4e76625445634d426f474131554541784d54593245750a62334a6e4d53356c654746746347786c4c6d4e7662544165467730784f4441794d4463774d4445314d545261467730794f4441794d4455774d4445314d5452610a4d467378437a414a42674e5642415954416c56544d524d77455159445651514945777044595778705a6d3979626d6c684d5259774641594456515148457731540a59573467526e4a68626d4e7063324e764d523877485159445651514444425a425a473170626b4276636d63784c6d56345957317762475575593239744d466b770a457759484b6f5a497a6a3043415159494b6f5a497a6a3044415163445167414530316b55773456592f5973724d744c38655a785a63686e42757a5a4e504f666a0a4a4e39374d3738777844475756566e484468724d46656d434a3072334d4c4a77324a2f414a383639793948645572445758306c6f2b364e4e4d457377446759440a565230504151482f42415144416765414d41774741315564457745422f7751434d4141774b7759445652306a42435177496f416754385951514a54664f5658490a456b364a57304a7471664e4c4e355a73396950584f63334c454d4a4759453077436759494b6f5a497a6a3045417749445341417752514968414a70767472364a0a454b6b53696b783830324553476d776478656a6d5532485434766a7953556c7559435337416942414f3762503358327349465943462f41393462426a666351520a584e76594a71504a2b592f7a4c596f3250513d3d0a2d2d2d2d2d454e44202d2d2d2d2d0a
2018-02-24 07:27:09.247 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:42 newIdentity -> DEBU 918 Creating identity instance for ID -----BEGIN CERTIFICATE-----
MIICGTCCAb+gAwIBAgIQU2EEaekZq1LsI6bZEdL9rzAKBggqhkjOPQQDAjBzMQsw
CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy
YW5jaXNjbzEZMBcGA1UEChMQb3JnMS5leGFtcGxlLmNvbTEcMBoGA1UEAxMTY2Eu
b3JnMS5leGFtcGxlLmNvbTAeFw0xODAyMDcwMDE1MTRaFw0yODAyMDUwMDE1MTRa
MFsxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1T
YW4gRnJhbmNpc2NvMR8wHQYDVQQDDBZBZG1pbkBvcmcxLmV4YW1wbGUuY29tMFkw
EwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE01kUw4VY/YsrMtL8eZxZchnBuzZNPOfj
JN97M78wxDGWVVnHDhrMFemCJ0r3MLJw2J/AJ869y9HdUrDWX0lo+6NNMEswDgYD
VR0PAQH/BAQDAgeAMAwGA1UdEwEB/wQCMAAwKwYDVR0jBCQwIoAgT8YQQJTfOVXI
Ek6JW0JtqfNLN5Zs9iPXOc3LEMJGYE0wCgYIKoZIzj0EAwIDSAAwRQIhAJpvtr6J
EKkSikx802ESGmwdxejmU2HT4vjySUluYCS7AiBAO7bP3X2sIFYCF/A94bBjfcQR
XNvYJqPJ+Y/zLYo2PQ==
-----END CERTIFICATE-----
2018-02-24 07:27:09.247 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:489 SatisfiesPrincipal -> DEBU 919 Checking if identity satisfies MEMBER role for Org1MSP
2018-02-24 07:27:09.247 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:391 Validate -> DEBU 91a MSP Org1MSP validating identity
2018-02-24 07:27:09.247 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:103 func2 -> DEBU 91b 0xc420a5c038 principal matched by identity 0
2018-02-24 07:27:09.248 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:129 Verify -> DEBU 91c Verify: digest = 00000000  6b a1 71 85 01 f0 41 3d  b0 57 74 13 36 74 02 bd  |k.q...A=.Wt.6t..|
00000010  b2 b0 33 dc 3f 6f bf 8d  2f 39 c3 75 ea d6 91 21  |..3.?o../9.u...!|
2018-02-24 07:27:09.248 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:130 Verify -> DEBU 91d Verify: sig = 00000000  30 45 02 21 00 9a f4 51  d8 c2 e5 bf 8d f8 1e b2  |0E.!...Q........|
00000010  21 8f a5 09 b4 71 ba 55  57 ff 21 10 97 5e 17 53  |!....q.UW.!..^.S|
00000020  e1 72 e1 29 13 02 20 17  74 eb 7b a6 c0 87 e9 15  |.r.).. .t.{.....|
00000030  2f 73 f5 ef ea 67 77 2b  37 09 9a 8d 54 2f 3e 7a  |/s...gw+7...T/>z|
00000040  41 5b 79 8a f4 15 5b                              |A[y...[|
2018-02-24 07:27:09.248 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:109 func2 -> DEBU 91e 0xc420a5c038 principal evaluation succeeds for identity 0
2018-02-24 07:27:09.249 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:70 func1 -> DEBU 91f 0xc420a5c038 gate 1519457229246961438 evaluation succeeds
2018-02-24 07:27:09.249 UTC [orderer/common/sigfilter] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/sigfilter/sigfilter.go:69 Apply -> DEBU 920 Forwarding validly signed message for policy &{%!s(*common.ImplicitMetaPolicy=&{Writers 0}) %!s(int=1) [%!s(*policies.implicitMetaPolicy=&{0xc4209ae200 1 [0xc420a5d120 0xc420a5d1e0]}) %!s(*policies.implicitMetaPolicy=&{0xc4209af160 1 [0xc420a5d258]})]}
2018-02-24 07:27:09.249 UTC [orderer/common/broadcast] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/broadcast/broadcast.go:153 Handle -> DEBU 921 [channel: mychannel] Broadcast has successfully enqueued message of type ENDORSER_TRANSACTION
2018-02-24 07:27:09.250 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 922 Returning policy Writers for evaluation
2018-02-24 07:27:09.250 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:58 func1 -> DEBU 923 0xc420a5c068 gate 1519457229250294113 evaluation starts
2018-02-24 07:27:09.250 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:83 func2 -> DEBU 924 0xc420a5c068 signed by 0 principal evaluation starts (used [false])
2018-02-24 07:27:09.250 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:91 func2 -> DEBU 925 0xc420a5c068 processing identity 0 with bytes of 0a074f7267314d53501280062d2d2d2d2d424547494e202d2d2d2d2d0a4d4949434754434341622b6741774942416749515532454561656b5a71314c734936625a45644c39727a414b42676771686b6a4f50515144416a427a4d5173770a435159445651514745774a56557a45544d4245474131554543424d4b5132467361575a76636d3570595445574d4251474131554542784d4e5532467549455a790a5957356a61584e6a627a455a4d4263474131554543684d5162334a6e4d53356c654746746347786c4c6d4e76625445634d426f474131554541784d54593245750a62334a6e4d53356c654746746347786c4c6d4e7662544165467730784f4441794d4463774d4445314d545261467730794f4441794d4455774d4445314d5452610a4d467378437a414a42674e5642415954416c56544d524d77455159445651514945777044595778705a6d3979626d6c684d5259774641594456515148457731540a59573467526e4a68626d4e7063324e764d523877485159445651514444425a425a473170626b4276636d63784c6d56345957317762475575593239744d466b770a457759484b6f5a497a6a3043415159494b6f5a497a6a3044415163445167414530316b55773456592f5973724d744c38655a785a63686e42757a5a4e504f666a0a4a4e39374d3738777844475756566e484468724d46656d434a3072334d4c4a77324a2f414a383639793948645572445758306c6f2b364e4e4d457377446759440a565230504151482f42415144416765414d41774741315564457745422f7751434d4141774b7759445652306a42435177496f416754385951514a54664f5658490a456b364a57304a7471664e4c4e355a73396950584f63334c454d4a4759453077436759494b6f5a497a6a3045417749445341417752514968414a70767472364a0a454b6b53696b783830324553476d776478656a6d5532485434766a7953556c7559435337416942414f3762503358327349465943462f41393462426a666351520a584e76594a71504a2b592f7a4c596f3250513d3d0a2d2d2d2d2d454e44202d2d2d2d2d0a
2018-02-24 07:27:09.250 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:42 newIdentity -> DEBU 926 Creating identity instance for ID -----BEGIN CERTIFICATE-----
MIICGTCCAb+gAwIBAgIQU2EEaekZq1LsI6bZEdL9rzAKBggqhkjOPQQDAjBzMQsw
CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy
YW5jaXNjbzEZMBcGA1UEChMQb3JnMS5leGFtcGxlLmNvbTEcMBoGA1UEAxMTY2Eu
b3JnMS5leGFtcGxlLmNvbTAeFw0xODAyMDcwMDE1MTRaFw0yODAyMDUwMDE1MTRa
MFsxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1T
YW4gRnJhbmNpc2NvMR8wHQYDVQQDDBZBZG1pbkBvcmcxLmV4YW1wbGUuY29tMFkw
EwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE01kUw4VY/YsrMtL8eZxZchnBuzZNPOfj
JN97M78wxDGWVVnHDhrMFemCJ0r3MLJw2J/AJ869y9HdUrDWX0lo+6NNMEswDgYD
VR0PAQH/BAQDAgeAMAwGA1UdEwEB/wQCMAAwKwYDVR0jBCQwIoAgT8YQQJTfOVXI
Ek6JW0JtqfNLN5Zs9iPXOc3LEMJGYE0wCgYIKoZIzj0EAwIDSAAwRQIhAJpvtr6J
EKkSikx802ESGmwdxejmU2HT4vjySUluYCS7AiBAO7bP3X2sIFYCF/A94bBjfcQR
XNvYJqPJ+Y/zLYo2PQ==
-----END CERTIFICATE-----
2018-02-24 07:27:09.255 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:489 SatisfiesPrincipal -> DEBU 927 Checking if identity satisfies MEMBER role for Org1MSP
2018-02-24 07:27:09.255 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:391 Validate -> DEBU 928 MSP Org1MSP validating identity
2018-02-24 07:27:09.255 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:103 func2 -> DEBU 929 0xc420a5c068 principal matched by identity 0
2018-02-24 07:27:09.255 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:129 Verify -> DEBU 92a Verify: digest = 00000000  6b a1 71 85 01 f0 41 3d  b0 57 74 13 36 74 02 bd  |k.q...A=.Wt.6t..|
00000010  b2 b0 33 dc 3f 6f bf 8d  2f 39 c3 75 ea d6 91 21  |..3.?o../9.u...!|
2018-02-24 07:27:09.255 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:130 Verify -> DEBU 92b Verify: sig = 00000000  30 45 02 21 00 9a f4 51  d8 c2 e5 bf 8d f8 1e b2  |0E.!...Q........|
00000010  21 8f a5 09 b4 71 ba 55  57 ff 21 10 97 5e 17 53  |!....q.UW.!..^.S|
00000020  e1 72 e1 29 13 02 20 17  74 eb 7b a6 c0 87 e9 15  |.r.).. .t.{.....|
00000030  2f 73 f5 ef ea 67 77 2b  37 09 9a 8d 54 2f 3e 7a  |/s...gw+7...T/>z|
00000040  41 5b 79 8a f4 15 5b                              |A[y...[|
2018-02-24 07:27:09.256 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:109 func2 -> DEBU 92c 0xc420a5c068 principal evaluation succeeds for identity 0
2018-02-24 07:27:09.256 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:70 func1 -> DEBU 92d 0xc420a5c068 gate 1519457229250294113 evaluation succeeds
2018-02-24 07:27:09.256 UTC [orderer/common/sigfilter] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/sigfilter/sigfilter.go:69 Apply -> DEBU 92e Forwarding validly signed message for policy &{%!s(*common.ImplicitMetaPolicy=&{Writers 0}) %!s(int=1) [%!s(*policies.implicitMetaPolicy=&{0xc4209ae200 1 [0xc420a5d120 0xc420a5d1e0]}) %!s(*policies.implicitMetaPolicy=&{0xc4209af160 1 [0xc420a5d258]})]}
2018-02-24 07:27:09.256 UTC [orderer/common/blockcutter] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/blockcutter/blockcutter.go:136 Ordered -> DEBU 92f Enqueuing message into batch
2018-02-24 07:27:09.256 UTC [orderer/common/broadcast] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/broadcast/broadcast.go:85 Handle -> WARN 930 Error reading from stream: rpc error: code = Canceled desc = context canceled
2018-02-24 07:27:09.256 UTC [orderer/main] /opt/gopath/src/github.com/hyperledger/fabric/orderer/server.go:79 func1 -> DEBU 931 Closing Broadcast stream
2018-02-24 07:27:11.256 UTC [orderer/solo] /opt/gopath/src/github.com/hyperledger/fabric/orderer/solo/consensus.go:112 main -> DEBU 932 Batch timer expired, creating block
2018-02-24 07:27:11.256 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_mgr.go:454 retrieveBlockByNumber -> DEBU 933 retrieveBlockByNumber() - blockNum = [0]
2018-02-24 07:27:11.256 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:66 newBlockfileStream -> DEBU 934 newBlockfileStream(): filePath=[/var/hyperledger/production/orderer/chains/mychannel/blockfile_000000], startOffset=[0]
2018-02-24 07:27:11.260 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:114 nextBlockBytesAndPlacementInfo -> DEBU 935 Remaining bytes=[11912], Going to peek [8] bytes
2018-02-24 07:27:11.261 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:147 nextBlockBytesAndPlacementInfo -> DEBU 936 Returning blockbytes - length=[11910], placementInfo={fileNum=[0], startOffset=[0], bytesOffset=[2]}
2018-02-24 07:27:11.261 UTC [orderer/multichain] /opt/gopath/src/github.com/hyperledger/fabric/orderer/multichain/chainsupport.go:228 addBlockSignature -> DEBU 937 &{ledgerResources:0xc42098b7a0 chain:0xc42097e0c0 cutter:0xc4202df590 filters:0xc42098bcc0 signer:0x128b4a8 lastConfig:0 lastConfigSeq:1}
2018-02-24 07:27:11.261 UTC [orderer/multichain] /opt/gopath/src/github.com/hyperledger/fabric/orderer/multichain/chainsupport.go:229 addBlockSignature -> DEBU 938 &{}
2018-02-24 07:27:11.261 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mgmt/mgmt.go:149 GetLocalMSP -> DEBU 939 Returning existing local MSP
2018-02-24 07:27:11.261 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:369 GetDefaultSigningIdentity -> DEBU 93a Obtaining default signing identity
2018-02-24 07:27:11.261 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mgmt/mgmt.go:149 GetLocalMSP -> DEBU 93b Returning existing local MSP
2018-02-24 07:27:11.261 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:369 GetDefaultSigningIdentity -> DEBU 93c Obtaining default signing identity
2018-02-24 07:27:11.261 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:208 Sign -> DEBU 93d Sign: plaintext: 0AFF050A0A4F7264657265724D535012...5B064B2ACB2DC28C5697B22AA27F0B9A 
2018-02-24 07:27:11.261 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:210 Sign -> DEBU 93e Sign: digest: 1CB015F994E72CEEF09819D6E1862646BE3480F3788C71F874DB9272C0C900B1 
2018-02-24 07:27:11.261 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mgmt/mgmt.go:149 GetLocalMSP -> DEBU 93f Returning existing local MSP
2018-02-24 07:27:11.261 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:369 GetDefaultSigningIdentity -> DEBU 940 Obtaining default signing identity
2018-02-24 07:27:11.261 UTC [orderer/multichain] /opt/gopath/src/github.com/hyperledger/fabric/orderer/multichain/chainsupport.go:262 addLastConfigSignature -> DEBU 941 [channel: mychannel] About to write block, setting its LAST_CONFIG to 0
2018-02-24 07:27:11.261 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mgmt/mgmt.go:149 GetLocalMSP -> DEBU 942 Returning existing local MSP
2018-02-24 07:27:11.261 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:369 GetDefaultSigningIdentity -> DEBU 943 Obtaining default signing identity
2018-02-24 07:27:11.261 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:208 Sign -> DEBU 944 Sign: plaintext: 0AFF050A0A4F7264657265724D535012...5B064B2ACB2DC28C5697B22AA27F0B9A 
2018-02-24 07:27:11.262 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:210 Sign -> DEBU 945 Sign: digest: ED27485728FADCD64111BD6DAE163F336C80988F9F6716787571F63B58F1851E 
2018-02-24 07:27:11.264 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockindex.go:98 indexBlock -> DEBU 946 Indexing block [blockNum=1, blockHash=[]byte{0x2e, 0x2c, 0xe8, 0x97, 0xbc, 0x74, 0x6c, 0x52, 0x97, 0xff, 0x56, 0x15, 0x25, 0xa3, 0x69, 0xa6, 0x8a, 0x4a, 0xdf, 0x70, 0x4, 0x93, 0x5a, 0x78, 0x4b, 0xe1, 0xac, 0xfe, 0x2b, 0x1c, 0x6a, 0x30} txOffsets=
txId=862dbe7cb0f7e4f5e4cf3fff5d590eca2f3700cb0d40db28b0bbba7f13a473cf locPointer=offset=70, bytesLength=3334
]
2018-02-24 07:27:11.265 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_mgr.go:430 updateCheckpoint -> DEBU 947 Broadcasting about update checkpointInfo: latestFileChunkSuffixNum=[0], latestFileChunksize=[17071], isChainEmpty=[false], lastBlockNumber=[1]
2018-02-24 07:27:11.265 UTC [orderer/multichain] /opt/gopath/src/github.com/hyperledger/fabric/orderer/multichain/chainsupport.go:289 WriteBlock -> DEBU 948 [channel: mychannel] Wrote block 1
2018-02-24 07:27:11.265 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_mgr.go:454 retrieveBlockByNumber -> DEBU 949 retrieveBlockByNumber() - blockNum = [1]
2018-02-24 07:27:11.265 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:66 newBlockfileStream -> DEBU 94a newBlockfileStream(): filePath=[/var/hyperledger/production/orderer/chains/mychannel/blockfile_000000], startOffset=[11912]
2018-02-24 07:27:11.265 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:114 nextBlockBytesAndPlacementInfo -> DEBU 94b Remaining bytes=[5159], Going to peek [8] bytes
2018-02-24 07:27:11.265 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:147 nextBlockBytesAndPlacementInfo -> DEBU 94c Returning blockbytes - length=[5157], placementInfo={fileNum=[0], startOffset=[11912], bytesOffset=[11914]}
2018-02-24 07:27:11.265 UTC [orderer/common/deliver] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/deliver/deliver.go:191 Handle -> DEBU 94d [channel: mychannel] Delivering block for (0xc42032a640)
2018-02-24 07:27:11.265 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_mgr.go:454 retrieveBlockByNumber -> DEBU 94e retrieveBlockByNumber() - blockNum = [1]
2018-02-24 07:27:11.271 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:66 newBlockfileStream -> DEBU 94f newBlockfileStream(): filePath=[/var/hyperledger/production/orderer/chains/mychannel/blockfile_000000], startOffset=[11912]
2018-02-24 07:27:11.271 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:114 nextBlockBytesAndPlacementInfo -> DEBU 950 Remaining bytes=[5159], Going to peek [8] bytes
2018-02-24 07:27:11.271 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:147 nextBlockBytesAndPlacementInfo -> DEBU 951 Returning blockbytes - length=[5157], placementInfo={fileNum=[0], startOffset=[11912], bytesOffset=[11914]}
2018-02-24 07:27:11.271 UTC [orderer/common/deliver] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/deliver/deliver.go:191 Handle -> DEBU 952 [channel: mychannel] Delivering block for (0xc4209d8560)
2018-02-24 07:27:11.274 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_mgr.go:454 retrieveBlockByNumber -> DEBU 953 retrieveBlockByNumber() - blockNum = [1]
2018-02-24 07:27:11.274 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:66 newBlockfileStream -> DEBU 954 newBlockfileStream(): filePath=[/var/hyperledger/production/orderer/chains/mychannel/blockfile_000000], startOffset=[11912]
2018-02-24 07:27:11.275 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:114 nextBlockBytesAndPlacementInfo -> DEBU 955 Remaining bytes=[5159], Going to peek [8] bytes
2018-02-24 07:27:11.275 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:147 nextBlockBytesAndPlacementInfo -> DEBU 956 Returning blockbytes - length=[5157], placementInfo={fileNum=[0], startOffset=[11912], bytesOffset=[11914]}
2018-02-24 07:27:11.276 UTC [orderer/common/deliver] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/deliver/deliver.go:191 Handle -> DEBU 957 [channel: mychannel] Delivering block for (0xc42085a3c0)
```

### invork (执行peer 日志)

```
2018-02-24 07:37:41.584 UTC [endorser.go:373] [endorser] ProcessProposal -> DEBU 43b Entry
2018-02-24 07:37:41.584 UTC [msgvalidation.go:79] [protoutils] ValidateProposalMessage -> DEBU 43c ValidateProposalMessage starts for signed proposal 0xc422017d70
2018-02-24 07:37:41.584 UTC [msgvalidation.go:215] [protoutils] validateChannelHeader -> DEBU 43d validateChannelHeader info: header type 3
2018-02-24 07:37:41.584 UTC [msgvalidation.go:142] [protoutils] checkSignatureFromCreator -> DEBU 43e checkSignatureFromCreator starts
2018-02-24 07:37:41.584 UTC [msgvalidation.go:160] [protoutils] checkSignatureFromCreator -> DEBU 43f checkSignatureFromCreator info: creator is &{Org1MSP 3dc70c07d3096e889a28bca219d07b0d858baede9b7d60760c9bf5a76017c9a1}
2018-02-24 07:37:41.586 UTC [msgvalidation.go:168] [protoutils] checkSignatureFromCreator -> DEBU 440 checkSignatureFromCreator info: creator is valid
2018-02-24 07:37:41.586 UTC [msgvalidation.go:176] [protoutils] checkSignatureFromCreator -> DEBU 441 checkSignatureFromCreator exists successfully
2018-02-24 07:37:41.586 UTC [msgvalidation.go:39] [protoutils] validateChaincodeProposalMessage -> DEBU 442 validateChaincodeProposalMessage starts for proposal 0xc421ae1220, header 0xc422017da0
2018-02-24 07:37:41.586 UTC [msgvalidation.go:51] [protoutils] validateChaincodeProposalMessage -> DEBU 443 validateChaincodeProposalMessage info: header extension references chaincode name:"fabcar" 
2018-02-24 07:37:41.586 UTC [endorser.go:412] [endorser] ProcessProposal -> DEBU 444 processing txid: 420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf
2018-02-24 07:37:41.586 UTC [endorser.go:414] [endorser] ProcessProposal -> DEBU 445 [gerry] chainID: mychannel
2018-02-24 07:37:41.586 UTC [blockfile_mgr.go:506] [fsblkstorage] retrieveTransactionByID -> DEBU 446 retrieveTransactionByID() - txId = [420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf]
2018-02-24 07:37:41.587 UTC [endorser.go:441] [endorser] ProcessProposal -> DEBU 447 [gerry] to get TxSimulator and HistoryQueryExecutor
2018-02-24 07:37:41.588 UTC [lockbased_txmgr.go:64] [lockbasedtxmgr] NewTxSimulator -> DEBU 448 constructing new tx simulator
2018-02-24 07:37:41.588 UTC [lockbased_tx_simulator.go:36] [lockbasedtxmgr] newLockBasedTxSimulator -> DEBU 449 constructing new tx simulator [8d7f5967-1199-48c8-bd4a-20bd0aceb703]
2018-02-24 07:37:41.588 UTC [endorser.go:472] [endorser] ProcessProposal -> DEBU 44a [gerry] 1 -- simulate
2018-02-24 07:37:41.588 UTC [endorser.go:211] [endorser] simulateProposal -> DEBU 44b Entry - txid: 420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf channel id: mychannel
2018-02-24 07:37:41.588 UTC [ccprovider.go:363] [ccprovider] NewCCContext -> DEBU 44c NewCCCC (chain=mychannel,chaincode=lscc,version=1.0.5,txid=420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf,syscc=true,proposal=0xc421ae1220,canname=lscc:1.0.5
2018-02-24 07:37:41.588 UTC [chaincode_support.go:585] [chaincode] Launch -> DEBU 44d chaincode is running(no need to launch) : lscc:1.0.5
2018-02-24 07:37:41.588 UTC [chaincode_support.go:720] [chaincode] Execute -> DEBU 44e Entry
2018-02-24 07:37:41.588 UTC [chaincode_support.go:723] [chaincode] Execute -> DEBU 44f chaincode canonical name: lscc:1.0.5
2018-02-24 07:37:41.588 UTC [handler.go:1436] [chaincode] sendExecuteMessage -> DEBU 450 [420ef361]Inside sendExecuteMessage. Message TRANSACTION
2018-02-24 07:37:41.588 UTC [handler.go:1350] [chaincode] setChaincodeProposal -> DEBU 451 Setting chaincode proposal context...
2018-02-24 07:37:41.588 UTC [handler.go:1352] [chaincode] setChaincodeProposal -> DEBU 452 Proposal different from nil. Creating chaincode proposal context...
2018-02-24 07:37:41.588 UTC [handler.go:1444] [chaincode] sendExecuteMessage -> DEBU 453 [420ef361]sendExecuteMsg trigger event TRANSACTION
2018-02-24 07:37:41.588 UTC [handler.go:357] [chaincode] processStream -> DEBU 454 [420ef361]Move state message TRANSACTION
2018-02-24 07:37:41.589 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 455 [420ef361]Fabric side Handling ChaincodeMessage of type: TRANSACTION in state ready
2018-02-24 07:37:41.589 UTC [handler.go:1417] [chaincode] filterError -> DEBU 456 Ignoring NoTransitionError: no transition
2018-02-24 07:37:41.589 UTC [handler.go:377] [chaincode] processStream -> DEBU 457 [420ef361]sending state message TRANSACTION
2018-02-24 07:37:41.589 UTC [chaincode.go:297] [shim] func1 -> DEBU 458 [420ef361]Received message TRANSACTION from shim
2018-02-24 07:37:41.589 UTC [handler.go:778] [shim] handleMessage -> DEBU 459 [420ef361]Handling ChaincodeMessage of type: TRANSACTION(state:ready)
2018-02-24 07:37:41.589 UTC [handler.go:338] [shim] beforeTransaction -> DEBU 45a [420ef361]Received TRANSACTION, invoking transaction on chaincode(Src:ready, Dst:ready)
2018-02-24 07:37:41.590 UTC [handler.go:392] [shim] handleGetState -> DEBU 45b [420ef361]Sending GET_STATE
2018-02-24 07:37:41.591 UTC [handler.go:336] [chaincode] processStream -> DEBU 45c [420ef361]Received message GET_STATE from shim
2018-02-24 07:37:41.593 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 45d [420ef361]Fabric side Handling ChaincodeMessage of type: GET_STATE in state ready
2018-02-24 07:37:41.593 UTC [handler.go:580] [chaincode] afterGetState -> DEBU 45e [420ef361]Received GET_STATE, invoking get state from ledger
2018-02-24 07:37:41.593 UTC [handler.go:1417] [chaincode] filterError -> DEBU 45f Ignoring NoTransitionError: no transition
2018-02-24 07:37:41.593 UTC [handler.go:634] [chaincode] func1 -> DEBU 460 [420ef361] getting state for chaincode lscc, key fabcar, channel mychannel
2018-02-24 07:37:41.593 UTC [stateleveldb.go:89] [stateleveldb] GetState -> DEBU 461 GetState(). ns=lscc, key=fabcar
2018-02-24 07:37:41.593 UTC [util.go:58] [kvledger.util] DecodeOrderPreservingVarUint64 -> DEBU 462 proto.DecodeVarint(bytes):1
2018-02-24 07:37:41.593 UTC [util.go:58] [kvledger.util] DecodeOrderPreservingVarUint64 -> DEBU 463 proto.DecodeVarint(bytes):0
2018-02-24 07:37:41.593 UTC [handler.go:655] [chaincode] func1 -> DEBU 464 [420ef361]Got state. Sending RESPONSE
2018-02-24 07:37:41.593 UTC [handler.go:621] [chaincode] 1 -> DEBU 465 [420ef361]handleGetState serial send RESPONSE
2018-02-24 07:37:41.593 UTC [chaincode.go:297] [shim] func1 -> DEBU 466 [420ef361]Received message RESPONSE from shim
2018-02-24 07:37:41.593 UTC [handler.go:778] [shim] handleMessage -> DEBU 467 [420ef361]Handling ChaincodeMessage of type: RESPONSE(state:ready)
2018-02-24 07:37:41.593 UTC [handler.go:115] [shim] sendChannel -> DEBU 468 [420ef361]before send
2018-02-24 07:37:41.593 UTC [handler.go:117] [shim] sendChannel -> DEBU 469 [420ef361]after send
2018-02-24 07:37:41.593 UTC [handler.go:356] [shim] afterResponse -> DEBU 46a [420ef361]Received RESPONSE, communicated (state:ready)
2018-02-24 07:37:41.593 UTC [handler.go:402] [shim] handleGetState -> DEBU 46b [420ef361]GetState received payload RESPONSE
2018-02-24 07:37:41.593 UTC [handler.go:326] [shim] func1 -> DEBU 46c [420ef361]Transaction completed. Sending COMPLETED
2018-02-24 07:37:41.593 UTC [chaincode.go:304] [shim] func1 -> DEBU 46d [420ef361]Move state message COMPLETED
2018-02-24 07:37:41.594 UTC [handler.go:778] [shim] handleMessage -> DEBU 46e [420ef361]Handling ChaincodeMessage of type: COMPLETED(state:ready)
2018-02-24 07:37:41.594 UTC [chaincode.go:320] [shim] func1 -> DEBU 46f [420ef361]send state message COMPLETED
2018-02-24 07:37:41.594 UTC [handler.go:336] [chaincode] processStream -> DEBU 470 [420ef361]Received message COMPLETED from shim
2018-02-24 07:37:41.594 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 471 [420ef361]Fabric side Handling ChaincodeMessage of type: COMPLETED in state ready
2018-02-24 07:37:41.594 UTC [handler.go:1392] [chaincode] HandleMessage -> DEBU 472 [420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf]HandleMessage- COMPLETED. Notify
2018-02-24 07:37:41.594 UTC [handler.go:551] [chaincode] notify -> DEBU 473 notifying Txid:420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf
2018-02-24 07:37:41.594 UTC [chaincode_support.go:751] [chaincode] Execute -> DEBU 474 Exit
2018-02-24 07:37:41.594 UTC [ccprovider.go:207] [ccprovider] GetChaincodeData -> DEBU 475 Getting chaincode data for <fabcar, 1.0> from cache
2018-02-24 07:37:41.594 UTC [endorser.go:99] [endorser] callChaincode -> DEBU 476 Entry - txid: 420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf channel id: mychannel version: 1.0
2018-02-24 07:37:41.594 UTC [ccprovider.go:363] [ccprovider] NewCCContext -> DEBU 477 NewCCCC (chain=mychannel,chaincode=fabcar,version=1.0,txid=420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf,syscc=false,proposal=0xc421ae1220,canname=fabcar:1.0
2018-02-24 07:37:41.594 UTC [chaincode_support.go:585] [chaincode] Launch -> DEBU 478 chaincode is running(no need to launch) : fabcar:1.0
2018-02-24 07:37:41.594 UTC [chaincode_support.go:720] [chaincode] Execute -> DEBU 479 Entry
2018-02-24 07:37:41.594 UTC [chaincode_support.go:723] [chaincode] Execute -> DEBU 47a chaincode canonical name: fabcar:1.0
2018-02-24 07:37:41.594 UTC [handler.go:1436] [chaincode] sendExecuteMessage -> DEBU 47b [420ef361]Inside sendExecuteMessage. Message TRANSACTION
2018-02-24 07:37:41.594 UTC [handler.go:1350] [chaincode] setChaincodeProposal -> DEBU 47c Setting chaincode proposal context...
2018-02-24 07:37:41.594 UTC [handler.go:1352] [chaincode] setChaincodeProposal -> DEBU 47d Proposal different from nil. Creating chaincode proposal context...
2018-02-24 07:37:41.594 UTC [handler.go:1444] [chaincode] sendExecuteMessage -> DEBU 47e [420ef361]sendExecuteMsg trigger event TRANSACTION
2018-02-24 07:37:41.594 UTC [handler.go:357] [chaincode] processStream -> DEBU 47f [420ef361]Move state message TRANSACTION
2018-02-24 07:37:41.594 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 480 [420ef361]Fabric side Handling ChaincodeMessage of type: TRANSACTION in state ready
2018-02-24 07:37:41.594 UTC [handler.go:1417] [chaincode] filterError -> DEBU 481 Ignoring NoTransitionError: no transition
2018-02-24 07:37:41.594 UTC [handler.go:377] [chaincode] processStream -> DEBU 482 [420ef361]sending state message TRANSACTION
2018-02-24 07:37:41.596 UTC [handler.go:336] [chaincode] processStream -> DEBU 483 [420ef361]Received message PUT_STATE from shim
2018-02-24 07:37:41.597 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 484 [420ef361]Fabric side Handling ChaincodeMessage of type: PUT_STATE in state ready
2018-02-24 07:37:41.597 UTC [handler.go:1417] [chaincode] filterError -> DEBU 485 Ignoring NoTransitionError: no transition
2018-02-24 07:37:41.597 UTC [handler.go:1140] [chaincode] func1 -> DEBU 486 [420ef361]state is ready
2018-02-24 07:37:41.597 UTC [handler.go:1315] [chaincode] func1 -> DEBU 487 [420ef361]Completed PUT_STATE. Sending RESPONSE
2018-02-24 07:37:41.597 UTC [handler.go:1159] [chaincode] 1 -> DEBU 488 [420ef361]enterBusyState trigger event RESPONSE
2018-02-24 07:37:41.597 UTC [handler.go:357] [chaincode] processStream -> DEBU 489 [420ef361]Move state message RESPONSE
2018-02-24 07:37:41.597 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 48a [420ef361]Fabric side Handling ChaincodeMessage of type: RESPONSE in state ready
2018-02-24 07:37:41.597 UTC [handler.go:1417] [chaincode] filterError -> DEBU 48b Ignoring NoTransitionError: no transition
2018-02-24 07:37:41.597 UTC [handler.go:377] [chaincode] processStream -> DEBU 48c [420ef361]sending state message RESPONSE
2018-02-24 07:37:41.599 UTC [handler.go:336] [chaincode] processStream -> DEBU 48d [420ef361]Received message COMPLETED from shim
2018-02-24 07:37:41.600 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 48e [420ef361]Fabric side Handling ChaincodeMessage of type: COMPLETED in state ready
2018-02-24 07:37:41.600 UTC [handler.go:1392] [chaincode] HandleMessage -> DEBU 48f [420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf]HandleMessage- COMPLETED. Notify
2018-02-24 07:37:41.600 UTC [handler.go:551] [chaincode] notify -> DEBU 490 notifying Txid:420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf
2018-02-24 07:37:41.600 UTC [chaincode_support.go:751] [chaincode] Execute -> DEBU 491 Exit
2018-02-24 07:37:41.600 UTC [endorser.go:156] [endorser] callChaincode -> DEBU 492 Exit
2018-02-24 07:37:41.600 UTC [lockbased_tx_simulator.go:72] [lockbasedtxmgr] GetTxSimulationResults -> DEBU 493 Simulation completed, getting simulation results
2018-02-24 07:37:41.600 UTC [lockbased_query_executer.go:62] [lockbasedtxmgr] Done -> DEBU 494 Done with transaction simulation / query execution [8d7f5967-1199-48c8-bd4a-20bd0aceb703]
2018-02-24 07:37:41.600 UTC [endorser.go:265] [endorser] simulateProposal -> DEBU 495 Exit
2018-02-24 07:37:41.600 UTC [endorser.go:500] [endorser] ProcessProposal -> DEBU 496 [gerry] 2 -- endorse and get a marshalled ProposalResponse message
2018-02-24 07:37:41.600 UTC [endorser.go:279] [endorser] endorseProposal -> DEBU 497 Entry - txid: 420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf channel id: mychannel chaincode id: name:"fabcar" 
2018-02-24 07:37:41.600 UTC [endorser.go:282] [endorser] endorseProposal -> DEBU 498 [gerry] *ccprovider.ChaincodeData: name:"fabcar" version:"1.0" escc:"escc" vscc:"vscc" policy:"\022\014\022\n\010\002\022\002\010\000\022\002\010\001\032\013\022\t\n\007Org1MSP\032\013\022\t\n\007Org2MSP" data:"\n n\231\207\243{A\374\345\227\302\253\245 oA\261\\]A\247\233\253T=\312\312\212\240\034\367\027\343\022 \255\351\262)\230hJ\315\037\\\326v\374~2,*\242\365\275\255\361\261@\241\341\307\365v0\026\035" id:"\375\265\206\375\214=\206\2232\300)\240Iw1E\013\036\260\256&\267\335\342\341\344\001\361\010e8^" instantiation_policy:"\022\014\022\n\010\001\022\002\010\000\022\002\010\001\032\r\022\013\n\007Org1MSP\020\001\032\r\022\013\n\007Org2MSP\020\001" 
2018-02-24 07:37:41.600 UTC [endorser.go:299] [endorser] endorseProposal -> DEBU 499 info: escc for chaincode id name:"fabcar"  is escc
2018-02-24 07:37:41.600 UTC [endorser.go:99] [endorser] callChaincode -> DEBU 49a Entry - txid: 420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf channel id: mychannel version: 1.0.5
2018-02-24 07:37:41.600 UTC [ccprovider.go:363] [ccprovider] NewCCContext -> DEBU 49b NewCCCC (chain=mychannel,chaincode=escc,version=1.0.5,txid=420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf,syscc=true,proposal=0xc421ae1220,canname=escc:1.0.5
2018-02-24 07:37:41.600 UTC [chaincode_support.go:585] [chaincode] Launch -> DEBU 49c chaincode is running(no need to launch) : escc:1.0.5
2018-02-24 07:37:41.600 UTC [chaincode_support.go:720] [chaincode] Execute -> DEBU 49d Entry
2018-02-24 07:37:41.600 UTC [chaincode_support.go:723] [chaincode] Execute -> DEBU 49e chaincode canonical name: escc:1.0.5
2018-02-24 07:37:41.600 UTC [handler.go:1436] [chaincode] sendExecuteMessage -> DEBU 49f [420ef361]Inside sendExecuteMessage. Message TRANSACTION
2018-02-24 07:37:41.600 UTC [handler.go:1350] [chaincode] setChaincodeProposal -> DEBU 4a0 Setting chaincode proposal context...
2018-02-24 07:37:41.600 UTC [handler.go:1352] [chaincode] setChaincodeProposal -> DEBU 4a1 Proposal different from nil. Creating chaincode proposal context...
2018-02-24 07:37:41.600 UTC [handler.go:1444] [chaincode] sendExecuteMessage -> DEBU 4a2 [420ef361]sendExecuteMsg trigger event TRANSACTION
2018-02-24 07:37:41.600 UTC [handler.go:357] [chaincode] processStream -> DEBU 4a3 [420ef361]Move state message TRANSACTION
2018-02-24 07:37:41.600 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 4a4 [420ef361]Fabric side Handling ChaincodeMessage of type: TRANSACTION in state ready
2018-02-24 07:37:41.600 UTC [handler.go:1417] [chaincode] filterError -> DEBU 4a5 Ignoring NoTransitionError: no transition
2018-02-24 07:37:41.600 UTC [handler.go:377] [chaincode] processStream -> DEBU 4a6 [420ef361]sending state message TRANSACTION
2018-02-24 07:37:41.601 UTC [chaincode.go:297] [shim] func1 -> DEBU 4a7 [420ef361]Received message TRANSACTION from shim
2018-02-24 07:37:41.601 UTC [handler.go:778] [shim] handleMessage -> DEBU 4a8 [420ef361]Handling ChaincodeMessage of type: TRANSACTION(state:ready)
2018-02-24 07:37:41.601 UTC [handler.go:338] [shim] beforeTransaction -> DEBU 4a9 [420ef361]Received TRANSACTION, invoking transaction on chaincode(Src:ready, Dst:ready)
2018-02-24 07:37:41.601 UTC [endorser_onevalidsignature.go:74] [escc] Invoke -> DEBU 4aa ESCC starts: 8 args
2018-02-24 07:37:41.601 UTC [endorser_onevalidsignature.go:177] [escc] Invoke -> DEBU 4ab ESCC exits successfully
2018-02-24 07:37:41.601 UTC [handler.go:326] [shim] func1 -> DEBU 4ac [420ef361]Transaction completed. Sending COMPLETED
2018-02-24 07:37:41.601 UTC [chaincode.go:304] [shim] func1 -> DEBU 4ad [420ef361]Move state message COMPLETED
2018-02-24 07:37:41.601 UTC [handler.go:778] [shim] handleMessage -> DEBU 4ae [420ef361]Handling ChaincodeMessage of type: COMPLETED(state:ready)
2018-02-24 07:37:41.601 UTC [chaincode.go:320] [shim] func1 -> DEBU 4af [420ef361]send state message COMPLETED
2018-02-24 07:37:41.601 UTC [handler.go:336] [chaincode] processStream -> DEBU 4b0 [420ef361]Received message COMPLETED from shim
2018-02-24 07:37:41.601 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 4b1 [420ef361]Fabric side Handling ChaincodeMessage of type: COMPLETED in state ready
2018-02-24 07:37:41.601 UTC [handler.go:1392] [chaincode] HandleMessage -> DEBU 4b2 [420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf]HandleMessage- COMPLETED. Notify
2018-02-24 07:37:41.601 UTC [handler.go:551] [chaincode] notify -> DEBU 4b3 notifying Txid:420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf
2018-02-24 07:37:41.601 UTC [chaincode_support.go:751] [chaincode] Execute -> DEBU 4b4 Exit
2018-02-24 07:37:41.601 UTC [endorser.go:156] [endorser] callChaincode -> DEBU 4b5 Exit
2018-02-24 07:37:41.601 UTC [endorser.go:368] [endorser] endorseProposal -> DEBU 4b6 Exit
2018-02-24 07:37:41.601 UTC [endorser.go:523] [endorser] ProcessProposal -> DEBU 4b7 go here txid: 420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf
2018-02-24 07:37:41.601 UTC [lockbased_query_executer.go:62] [lockbasedtxmgr] Done -> DEBU 4b8 Done with transaction simulation / query execution [8d7f5967-1199-48c8-bd4a-20bd0aceb703]
2018-02-24 07:37:41.601 UTC [endorser.go:530] [endorser] ProcessProposal -> DEBU 4b9 Exit
2018-02-24 07:37:43.635 UTC [blocksprovider.go:184] [blocksProvider] DeliverBlocks -> DEBU 4ba [mychannel] Adding payload locally, buffer seqNum = [2], peers number [1]
2018-02-24 07:37:43.639 UTC [blocksprovider.go:191] [blocksProvider] DeliverBlocks -> DEBU 4bb [mychannel] Gossiping block [2], peers number [1]
2018-02-24 07:37:43.639 UTC [committer_impl.go:73] [committer] Commit -> DEBU 4bc Validating block
2018-02-24 07:37:43.639 UTC [validator.go:146] [txvalidator] Validate -> DEBU 4bd START Block Validation
2018-02-24 07:37:43.639 UTC [validator.go:165] [txvalidator] Validate -> DEBU 4be Validating transaction peer.ValidateTransaction()
2018-02-24 07:37:43.639 UTC [msgvalidation.go:362] [protoutils] ValidateTransaction -> DEBU 4bf ValidateTransactionEnvelope starts for envelope 0xc422302270
2018-02-24 07:37:43.639 UTC [msgvalidation.go:377] [protoutils] ValidateTransaction -> DEBU 4c0 Header is channel_header:"\010\003\032\014\010\305\254\304\324\005\020\332\200\260\225\002\"\tmychannel*@420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf:\n\022\010\022\006fabcar" signature_header:"\n\214\006\n\007Org1MSP\022\200\006-----BEGIN -----\nMIICGTCCAb+gAwIBAgIQU2EEaekZq1LsI6bZEdL9rzAKBggqhkjOPQQDAjBzMQsw\nCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy\nYW5jaXNjbzEZMBcGA1UEChMQb3JnMS5leGFtcGxlLmNvbTEcMBoGA1UEAxMTY2Eu\nb3JnMS5leGFtcGxlLmNvbTAeFw0xODAyMDcwMDE1MTRaFw0yODAyMDUwMDE1MTRa\nMFsxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1T\nYW4gRnJhbmNpc2NvMR8wHQYDVQQDDBZBZG1pbkBvcmcxLmV4YW1wbGUuY29tMFkw\nEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE01kUw4VY/YsrMtL8eZxZchnBuzZNPOfj\nJN97M78wxDGWVVnHDhrMFemCJ0r3MLJw2J/AJ869y9HdUrDWX0lo+6NNMEswDgYD\nVR0PAQH/BAQDAgeAMAwGA1UdEwEB/wQCMAAwKwYDVR0jBCQwIoAgT8YQQJTfOVXI\nEk6JW0JtqfNLN5Zs9iPXOc3LEMJGYE0wCgYIKoZIzj0EAwIDSAAwRQIhAJpvtr6J\nEKkSikx802ESGmwdxejmU2HT4vjySUluYCS7AiBAO7bP3X2sIFYCF/A94bBjfcQR\nXNvYJqPJ+Y/zLYo2PQ==\n-----END -----\n\022\030\274\2548\331}\236!W\257\264\302\322\332\237\376R\r4\351\307\237\375\243\376" 
2018-02-24 07:37:43.640 UTC [msgvalidation.go:215] [protoutils] validateChannelHeader -> DEBU 4c1 validateChannelHeader info: header type 3
2018-02-24 07:37:43.640 UTC [msgvalidation.go:142] [protoutils] checkSignatureFromCreator -> DEBU 4c2 checkSignatureFromCreator starts
2018-02-24 07:37:43.641 UTC [msgvalidation.go:160] [protoutils] checkSignatureFromCreator -> DEBU 4c3 checkSignatureFromCreator info: creator is &{Org1MSP 3dc70c07d3096e889a28bca219d07b0d858baede9b7d60760c9bf5a76017c9a1}
2018-02-24 07:37:43.641 UTC [msgvalidation.go:168] [protoutils] checkSignatureFromCreator -> DEBU 4c4 checkSignatureFromCreator info: creator is valid
2018-02-24 07:37:43.642 UTC [msgvalidation.go:176] [protoutils] checkSignatureFromCreator -> DEBU 4c5 checkSignatureFromCreator exists successfully
2018-02-24 07:37:43.642 UTC [msgvalidation.go:279] [protoutils] validateEndorserTransaction -> DEBU 4c6 validateEndorserTransaction starts for data 0xc4220ce000, header channel_header:"\010\003\032\014\010\305\254\304\324\005\020\332\200\260\225\002\"\tmychannel*@420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf:\n\022\010\022\006fabcar" signature_header:"\n\214\006\n\007Org1MSP\022\200\006-----BEGIN -----\nMIICGTCCAb+gAwIBAgIQU2EEaekZq1LsI6bZEdL9rzAKBggqhkjOPQQDAjBzMQsw\nCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy\nYW5jaXNjbzEZMBcGA1UEChMQb3JnMS5leGFtcGxlLmNvbTEcMBoGA1UEAxMTY2Eu\nb3JnMS5leGFtcGxlLmNvbTAeFw0xODAyMDcwMDE1MTRaFw0yODAyMDUwMDE1MTRa\nMFsxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1T\nYW4gRnJhbmNpc2NvMR8wHQYDVQQDDBZBZG1pbkBvcmcxLmV4YW1wbGUuY29tMFkw\nEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE01kUw4VY/YsrMtL8eZxZchnBuzZNPOfj\nJN97M78wxDGWVVnHDhrMFemCJ0r3MLJw2J/AJ869y9HdUrDWX0lo+6NNMEswDgYD\nVR0PAQH/BAQDAgeAMAwGA1UdEwEB/wQCMAAwKwYDVR0jBCQwIoAgT8YQQJTfOVXI\nEk6JW0JtqfNLN5Zs9iPXOc3LEMJGYE0wCgYIKoZIzj0EAwIDSAAwRQIhAJpvtr6J\nEKkSikx802ESGmwdxejmU2HT4vjySUluYCS7AiBAO7bP3X2sIFYCF/A94bBjfcQR\nXNvYJqPJ+Y/zLYo2PQ==\n-----END -----\n\022\030\274\2548\331}\236!W\257\264\302\322\332\237\376R\r4\351\307\237\375\243\376" 
2018-02-24 07:37:43.648 UTC [msgvalidation.go:306] [protoutils] validateEndorserTransaction -> DEBU 4c7 validateEndorserTransaction info: there are 1 actions
2018-02-24 07:37:43.648 UTC [msgvalidation.go:327] [protoutils] validateEndorserTransaction -> DEBU 4c8 validateEndorserTransaction info: signature header is valid
2018-02-24 07:37:43.648 UTC [msgvalidation.go:412] [protoutils] ValidateTransaction -> DEBU 4c9 ValidateTransactionEnvelope returns err %!s(<nil>)
2018-02-24 07:37:43.648 UTC [validator.go:184] [txvalidator] Validate -> DEBU 4ca Transaction is for chain mychannel
2018-02-24 07:37:43.648 UTC [blockfile_mgr.go:506] [fsblkstorage] retrieveTransactionByID -> DEBU 4cb retrieveTransactionByID() - txId = [420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf]
2018-02-24 07:37:43.648 UTC [validator.go:202] [txvalidator] Validate -> DEBU 4cc Validating transaction vscc tx validate
2018-02-24 07:37:43.648 UTC [lockbased_query_executer.go:33] [lockbasedtxmgr] newQueryExecutor -> DEBU 4cd constructing new query executor [eab315bd-c827-4dea-a98e-ea329615df10]
2018-02-24 07:37:43.649 UTC [stateleveldb.go:89] [stateleveldb] GetState -> DEBU 4ce GetState(). ns=lscc, key=fabcar
2018-02-24 07:37:43.649 UTC [util.go:58] [kvledger.util] DecodeOrderPreservingVarUint64 -> DEBU 4cf proto.DecodeVarint(bytes):1
2018-02-24 07:37:43.649 UTC [util.go:58] [kvledger.util] DecodeOrderPreservingVarUint64 -> DEBU 4d0 proto.DecodeVarint(bytes):0
2018-02-24 07:37:43.649 UTC [lockbased_query_executer.go:62] [lockbasedtxmgr] Done -> DEBU 4d1 Done with transaction simulation / query execution [eab315bd-c827-4dea-a98e-ea329615df10]
2018-02-24 07:37:43.649 UTC [lockbased_txmgr.go:64] [lockbasedtxmgr] NewTxSimulator -> DEBU 4d2 constructing new tx simulator
2018-02-24 07:37:43.649 UTC [lockbased_tx_simulator.go:36] [lockbasedtxmgr] newLockBasedTxSimulator -> DEBU 4d3 constructing new tx simulator [7475a2b6-c666-4045-a926-20944f660e2a]
2018-02-24 07:37:43.649 UTC [ccprovider.go:363] [ccprovider] NewCCContext -> DEBU 4d4 NewCCCC (chain=mychannel,chaincode=vscc,version=1.0.5,txid=8c793010-8808-429c-b05c-7fd54d3c0af2,syscc=true,proposal=0x0,canname=vscc:1.0.5
2018-02-24 07:37:43.649 UTC [validator.go:622] [txvalidator] VSCCValidateTxForCC -> DEBU 4d5 Invoking VSCC txid 420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf chaindID mychannel
2018-02-24 07:37:43.649 UTC [chaincode_support.go:585] [chaincode] Launch -> DEBU 4d6 chaincode is running(no need to launch) : vscc:1.0.5
2018-02-24 07:37:43.649 UTC [chaincode_support.go:720] [chaincode] Execute -> DEBU 4d7 Entry
2018-02-24 07:37:43.649 UTC [chaincode_support.go:723] [chaincode] Execute -> DEBU 4d8 chaincode canonical name: vscc:1.0.5
2018-02-24 07:37:43.649 UTC [handler.go:1436] [chaincode] sendExecuteMessage -> DEBU 4d9 [8c793010]Inside sendExecuteMessage. Message TRANSACTION
2018-02-24 07:37:43.649 UTC [handler.go:1350] [chaincode] setChaincodeProposal -> DEBU 4da Setting chaincode proposal context...
2018-02-24 07:37:43.649 UTC [handler.go:1444] [chaincode] sendExecuteMessage -> DEBU 4db [8c793010]sendExecuteMsg trigger event TRANSACTION
2018-02-24 07:37:43.649 UTC [handler.go:357] [chaincode] processStream -> DEBU 4dc [8c793010]Move state message TRANSACTION
2018-02-24 07:37:43.650 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 4dd [8c793010]Fabric side Handling ChaincodeMessage of type: TRANSACTION in state ready
2018-02-24 07:37:43.650 UTC [handler.go:1417] [chaincode] filterError -> DEBU 4de Ignoring NoTransitionError: no transition
2018-02-24 07:37:43.650 UTC [handler.go:377] [chaincode] processStream -> DEBU 4df [8c793010]sending state message TRANSACTION
2018-02-24 07:37:43.650 UTC [chaincode.go:297] [shim] func1 -> DEBU 4e0 [8c793010]Received message TRANSACTION from shim
2018-02-24 07:37:43.650 UTC [handler.go:778] [shim] handleMessage -> DEBU 4e1 [8c793010]Handling ChaincodeMessage of type: TRANSACTION(state:ready)
2018-02-24 07:37:43.650 UTC [handler.go:338] [shim] beforeTransaction -> DEBU 4e2 [8c793010]Received TRANSACTION, invoking transaction on chaincode(Src:ready, Dst:ready)
2018-02-24 07:37:43.650 UTC [validator_onevalidsignature.go:91] [vscc] Invoke -> DEBU 4e3 VSCC invoked
2018-02-24 07:37:43.650 UTC [validator_onevalidsignature.go:457] [vscc] deduplicateIdentity -> DEBU 4e4 Signature set is of size 1 out of 1 endorsement(s)
2018-02-24 07:37:43.651 UTC [validator_onevalidsignature.go:150] [vscc] Invoke -> WARN 4e5 Endorsement policy failure for transaction txid=420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf, err: Failed to authenticate policy
2018-02-24 07:37:43.653 UTC [handler.go:326] [shim] func1 -> DEBU 4e6 [8c793010]Transaction completed. Sending COMPLETED
2018-02-24 07:37:43.653 UTC [chaincode.go:304] [shim] func1 -> DEBU 4e7 [8c793010]Move state message COMPLETED
2018-02-24 07:37:43.653 UTC [handler.go:778] [shim] handleMessage -> DEBU 4e8 [8c793010]Handling ChaincodeMessage of type: COMPLETED(state:ready)
2018-02-24 07:37:43.653 UTC [chaincode.go:320] [shim] func1 -> DEBU 4e9 [8c793010]send state message COMPLETED
2018-02-24 07:37:43.654 UTC [handler.go:336] [chaincode] processStream -> DEBU 4ea [8c793010]Received message COMPLETED from shim
2018-02-24 07:37:43.656 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 4eb [8c793010]Fabric side Handling ChaincodeMessage of type: COMPLETED in state ready
2018-02-24 07:37:43.657 UTC [handler.go:1392] [chaincode] HandleMessage -> DEBU 4ec [8c793010-8808-429c-b05c-7fd54d3c0af2]HandleMessage- COMPLETED. Notify
2018-02-24 07:37:43.657 UTC [handler.go:551] [chaincode] notify -> DEBU 4ed notifying Txid:8c793010-8808-429c-b05c-7fd54d3c0af2
2018-02-24 07:37:43.657 UTC [chaincode_support.go:751] [chaincode] Execute -> DEBU 4ee Exit
2018-02-24 07:37:43.657 UTC [validator.go:630] [txvalidator] VSCCValidateTxForCC -> ERRO 4ef VSCC check failed for transaction txid=420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf, error VSCC error: policy evaluation failed, err Failed to authenticate policy
2018-02-24 07:37:43.657 UTC [lockbased_query_executer.go:62] [lockbasedtxmgr] Done -> DEBU 4f0 Done with transaction simulation / query execution [7475a2b6-c666-4045-a926-20944f660e2a]
2018-02-24 07:37:43.657 UTC [validator.go:206] [txvalidator] Validate -> ERRO 4f1 VSCCValidateTx for transaction txId = 420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf returned error VSCC error: policy evaluation failed, err Failed to authenticate policy
2018-02-24 07:37:43.657 UTC [validator.go:271] [txvalidator] Validate -> DEBU 4f2 END Block Validation
2018-02-24 07:37:43.657 UTC [kv_ledger.go:215] [kvledger] Commit -> DEBU 4f3 Channel [mychannel]: Validating block [2]
2018-02-24 07:37:43.657 UTC [lockbased_txmgr.go:72] [lockbasedtxmgr] ValidateAndPrepare -> DEBU 4f4 Validating new block with num trans = [1]
2018-02-24 07:37:43.657 UTC [state_based_validator.go:78] [statevalidator] ValidateAndPrepareBatch -> DEBU 4f5 New block arrived for validation:&common.Block{Header:(*common.BlockHeader)(0xc421f7fc00), Data:(*common.BlockData)(0xc4213eaa40), Metadata:(*common.BlockMetadata)(0xc4213eaa80)}, doMVCCValidation=true
2018-02-24 07:37:43.657 UTC [state_based_validator.go:80] [statevalidator] ValidateAndPrepareBatch -> DEBU 4f6 Validating a block with [1] transactions
2018-02-24 07:37:43.657 UTC [state_based_validator.go:95] [statevalidator] ValidateAndPrepareBatch -> WARN 4f7 Block [2] Transaction index [0] marked as invalid by committer. Reason code [10]
2018-02-24 07:37:43.657 UTC [kv_ledger.go:221] [kvledger] Commit -> DEBU 4f8 Channel [mychannel]: Committing block [2] to storage
2018-02-24 07:37:43.658 UTC [blockindex.go:98] [fsblkstorage] indexBlock -> DEBU 4f9 Indexing block [blockNum=2, blockHash=[]byte{0xdd, 0x6d, 0xbb, 0x5e, 0x9d, 0x79, 0xe5, 0xbf, 0x97, 0xab, 0x95, 0x8c, 0x91, 0x96, 0x38, 0x6c, 0x1c, 0x90, 0x18, 0x91, 0x62, 0xc0, 0x67, 0xe9, 0xc1, 0xcb, 0xaf, 0xa1, 0xa7, 0x1, 0x88, 0x38} txOffsets=
txId=420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf locPointer=offset=70, bytesLength=2925
]
2018-02-24 07:37:43.658 UTC [blockindex.go:122] [fsblkstorage] indexBlock -> DEBU 4fa Adding txLoc [fileSuffixNum=0, offset=17143, bytesLength=2925] for tx ID: [420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf] to index
2018-02-24 07:37:43.658 UTC [blockindex.go:135] [fsblkstorage] indexBlock -> DEBU 4fb Adding txLoc [fileSuffixNum=0, offset=17143, bytesLength=2925] for tx number:[0] ID: [420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf] to blockNumTranNum index
2018-02-24 07:37:43.659 UTC [blockfile_mgr.go:430] [fsblkstorage] updateCheckpoint -> DEBU 4fc Broadcasting about update checkpointInfo: latestFileChunkSuffixNum=[0], latestFileChunksize=[21826], isChainEmpty=[false], lastBlockNumber=[2]
2018-02-24 07:37:43.659 UTC [kv_ledger.go:225] [kvledger] Commit -> INFO 4fd Channel [mychannel]: Created block [2] with 1 transaction(s)
2018-02-24 07:37:43.659 UTC [kv_ledger.go:227] [kvledger] Commit -> DEBU 4fe Channel [mychannel]: Committing block [2] transactions to state database
2018-02-24 07:37:43.659 UTC [lockbased_txmgr.go:89] [lockbasedtxmgr] Commit -> DEBU 4ff Committing updates to state database
2018-02-24 07:37:43.659 UTC [lockbased_txmgr.go:92] [lockbasedtxmgr] Commit -> DEBU 500 Write lock acquired for committing updates to state database
2018-02-24 07:37:43.659 UTC [lockbased_txmgr.go:101] [lockbasedtxmgr] Commit -> DEBU 501 Updates committed to state database
2018-02-24 07:37:43.659 UTC [kv_ledger.go:234] [kvledger] Commit -> DEBU 502 Channel [mychannel]: Committing block [2] transactions to history database
2018-02-24 07:37:43.659 UTC [historyleveldb.go:93] [historyleveldb] Commit -> DEBU 503 Channel [mychannel]: Updating history database for blockNo [2] with [1] transactions
2018-02-24 07:37:43.659 UTC [historyleveldb.go:109] [historyleveldb] Commit -> DEBU 504 Channel [mychannel]: Skipping history write for invalid transaction number 0
2018-02-24 07:37:43.660 UTC [historyleveldb.go:177] [historyleveldb] Commit -> DEBU 505 Channel [mychannel]: Updates committed to history database for blockNo [2]
2018-02-24 07:37:43.660 UTC [eventhelper.go:29] [eventhub_producer] SendProducerBlockEvent -> DEBU 506 Entry
2018-02-24 07:37:43.660 UTC [eventhelper.go:55] [eventhub_producer] SendProducerBlockEvent -> DEBU 507 Channel [mychannel]: Block event for block number [2] contains transaction id: 420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf
2018-02-24 07:37:43.660 UTC [eventhelper.go:105] [eventhub_producer] SendProducerBlockEvent -> INFO 508 Channel [mychannel]: Sending event for block number [2]
2018-02-24 07:37:43.660 UTC [events.go:312] [eventhub_producer] Send -> DEBU 509 Entry
2018-02-24 07:37:43.660 UTC [events.go:335] [eventhub_producer] Send -> DEBU 50a Event processor timeout > 0
2018-02-24 07:37:43.660 UTC [events.go:343] [eventhub_producer] Send -> DEBU 50b Event sent successfully
2018-02-24 07:37:43.660 UTC [events.go:344] [eventhub_producer] Send -> DEBU 50c Exit
2018-02-24 07:37:43.660 UTC [eventhelper.go:107] [eventhub_producer] SendProducerBlockEvent -> DEBU 50d Exit
2018-02-24 07:37:43.660 UTC [register_internal_events.go:41] [eventhub_producer] getMessageType -> DEBU 50e [gerry] Event_Block
```

### invork (同步peer 日志)

```
2018-02-24 07:37:43.623 UTC [blocksprovider.go:184] [blocksProvider] DeliverBlocks -> DEBU 38d [mychannel] Adding payload locally, buffer seqNum = [2], peers number [0]
2018-02-24 07:37:43.623 UTC [blocksprovider.go:191] [blocksProvider] DeliverBlocks -> DEBU 38e [mychannel] Gossiping block [2], peers number [0]
2018-02-24 07:37:43.626 UTC [committer_impl.go:73] [committer] Commit -> DEBU 38f Validating block
2018-02-24 07:37:43.626 UTC [validator.go:146] [txvalidator] Validate -> DEBU 390 START Block Validation
2018-02-24 07:37:43.626 UTC [validator.go:165] [txvalidator] Validate -> DEBU 391 Validating transaction peer.ValidateTransaction()
2018-02-24 07:37:43.626 UTC [msgvalidation.go:362] [protoutils] ValidateTransaction -> DEBU 392 ValidateTransactionEnvelope starts for envelope 0xc421968cf0
2018-02-24 07:37:43.626 UTC [msgvalidation.go:377] [protoutils] ValidateTransaction -> DEBU 393 Header is channel_header:"\010\003\032\014\010\305\254\304\324\005\020\332\200\260\225\002\"\tmychannel*@420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf:\n\022\010\022\006fabcar" signature_header:"\n\214\006\n\007Org1MSP\022\200\006-----BEGIN -----\nMIICGTCCAb+gAwIBAgIQU2EEaekZq1LsI6bZEdL9rzAKBggqhkjOPQQDAjBzMQsw\nCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy\nYW5jaXNjbzEZMBcGA1UEChMQb3JnMS5leGFtcGxlLmNvbTEcMBoGA1UEAxMTY2Eu\nb3JnMS5leGFtcGxlLmNvbTAeFw0xODAyMDcwMDE1MTRaFw0yODAyMDUwMDE1MTRa\nMFsxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1T\nYW4gRnJhbmNpc2NvMR8wHQYDVQQDDBZBZG1pbkBvcmcxLmV4YW1wbGUuY29tMFkw\nEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE01kUw4VY/YsrMtL8eZxZchnBuzZNPOfj\nJN97M78wxDGWVVnHDhrMFemCJ0r3MLJw2J/AJ869y9HdUrDWX0lo+6NNMEswDgYD\nVR0PAQH/BAQDAgeAMAwGA1UdEwEB/wQCMAAwKwYDVR0jBCQwIoAgT8YQQJTfOVXI\nEk6JW0JtqfNLN5Zs9iPXOc3LEMJGYE0wCgYIKoZIzj0EAwIDSAAwRQIhAJpvtr6J\nEKkSikx802ESGmwdxejmU2HT4vjySUluYCS7AiBAO7bP3X2sIFYCF/A94bBjfcQR\nXNvYJqPJ+Y/zLYo2PQ==\n-----END -----\n\022\030\274\2548\331}\236!W\257\264\302\322\332\237\376R\r4\351\307\237\375\243\376" 
2018-02-24 07:37:43.626 UTC [msgvalidation.go:215] [protoutils] validateChannelHeader -> DEBU 394 validateChannelHeader info: header type 3
2018-02-24 07:37:43.626 UTC [msgvalidation.go:142] [protoutils] checkSignatureFromCreator -> DEBU 395 checkSignatureFromCreator starts
2018-02-24 07:37:43.626 UTC [msgvalidation.go:160] [protoutils] checkSignatureFromCreator -> DEBU 396 checkSignatureFromCreator info: creator is &{Org1MSP 3dc70c07d3096e889a28bca219d07b0d858baede9b7d60760c9bf5a76017c9a1}
2018-02-24 07:37:43.627 UTC [msgvalidation.go:168] [protoutils] checkSignatureFromCreator -> DEBU 397 checkSignatureFromCreator info: creator is valid
2018-02-24 07:37:43.627 UTC [msgvalidation.go:176] [protoutils] checkSignatureFromCreator -> DEBU 398 checkSignatureFromCreator exists successfully
2018-02-24 07:37:43.627 UTC [msgvalidation.go:279] [protoutils] validateEndorserTransaction -> DEBU 399 validateEndorserTransaction starts for data 0xc421964800, header channel_header:"\010\003\032\014\010\305\254\304\324\005\020\332\200\260\225\002\"\tmychannel*@420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf:\n\022\010\022\006fabcar" signature_header:"\n\214\006\n\007Org1MSP\022\200\006-----BEGIN -----\nMIICGTCCAb+gAwIBAgIQU2EEaekZq1LsI6bZEdL9rzAKBggqhkjOPQQDAjBzMQsw\nCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy\nYW5jaXNjbzEZMBcGA1UEChMQb3JnMS5leGFtcGxlLmNvbTEcMBoGA1UEAxMTY2Eu\nb3JnMS5leGFtcGxlLmNvbTAeFw0xODAyMDcwMDE1MTRaFw0yODAyMDUwMDE1MTRa\nMFsxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1T\nYW4gRnJhbmNpc2NvMR8wHQYDVQQDDBZBZG1pbkBvcmcxLmV4YW1wbGUuY29tMFkw\nEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE01kUw4VY/YsrMtL8eZxZchnBuzZNPOfj\nJN97M78wxDGWVVnHDhrMFemCJ0r3MLJw2J/AJ869y9HdUrDWX0lo+6NNMEswDgYD\nVR0PAQH/BAQDAgeAMAwGA1UdEwEB/wQCMAAwKwYDVR0jBCQwIoAgT8YQQJTfOVXI\nEk6JW0JtqfNLN5Zs9iPXOc3LEMJGYE0wCgYIKoZIzj0EAwIDSAAwRQIhAJpvtr6J\nEKkSikx802ESGmwdxejmU2HT4vjySUluYCS7AiBAO7bP3X2sIFYCF/A94bBjfcQR\nXNvYJqPJ+Y/zLYo2PQ==\n-----END -----\n\022\030\274\2548\331}\236!W\257\264\302\322\332\237\376R\r4\351\307\237\375\243\376" 
2018-02-24 07:37:43.627 UTC [msgvalidation.go:306] [protoutils] validateEndorserTransaction -> DEBU 39a validateEndorserTransaction info: there are 1 actions
2018-02-24 07:37:43.627 UTC [msgvalidation.go:327] [protoutils] validateEndorserTransaction -> DEBU 39b validateEndorserTransaction info: signature header is valid
2018-02-24 07:37:43.627 UTC [msgvalidation.go:412] [protoutils] ValidateTransaction -> DEBU 39c ValidateTransactionEnvelope returns err %!s(<nil>)
2018-02-24 07:37:43.627 UTC [validator.go:184] [txvalidator] Validate -> DEBU 39d Transaction is for chain mychannel
2018-02-24 07:37:43.627 UTC [blockfile_mgr.go:506] [fsblkstorage] retrieveTransactionByID -> DEBU 39e retrieveTransactionByID() - txId = [420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf]
2018-02-24 07:37:43.627 UTC [validator.go:202] [txvalidator] Validate -> DEBU 39f Validating transaction vscc tx validate
2018-02-24 07:37:43.628 UTC [lockbased_query_executer.go:33] [lockbasedtxmgr] newQueryExecutor -> DEBU 3a0 constructing new query executor [de032524-d44b-4690-9f5c-e75d9781866e]
2018-02-24 07:37:43.629 UTC [stateleveldb.go:89] [stateleveldb] GetState -> DEBU 3a1 GetState(). ns=lscc, key=fabcar
2018-02-24 07:37:43.629 UTC [util.go:58] [kvledger.util] DecodeOrderPreservingVarUint64 -> DEBU 3a2 proto.DecodeVarint(bytes):1
2018-02-24 07:37:43.629 UTC [util.go:58] [kvledger.util] DecodeOrderPreservingVarUint64 -> DEBU 3a3 proto.DecodeVarint(bytes):0
2018-02-24 07:37:43.629 UTC [lockbased_query_executer.go:62] [lockbasedtxmgr] Done -> DEBU 3a4 Done with transaction simulation / query execution [de032524-d44b-4690-9f5c-e75d9781866e]
2018-02-24 07:37:43.632 UTC [lockbased_txmgr.go:64] [lockbasedtxmgr] NewTxSimulator -> DEBU 3a5 constructing new tx simulator
2018-02-24 07:37:43.632 UTC [lockbased_tx_simulator.go:36] [lockbasedtxmgr] newLockBasedTxSimulator -> DEBU 3a6 constructing new tx simulator [563ffb5b-37a9-4659-b0ca-ab31515c3230]
2018-02-24 07:37:43.633 UTC [ccprovider.go:363] [ccprovider] NewCCContext -> DEBU 3a7 NewCCCC (chain=mychannel,chaincode=vscc,version=1.0.5,txid=6570281f-d71d-4c49-a765-b0d849405554,syscc=true,proposal=0x0,canname=vscc:1.0.5
2018-02-24 07:37:43.633 UTC [validator.go:622] [txvalidator] VSCCValidateTxForCC -> DEBU 3a8 Invoking VSCC txid 420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf chaindID mychannel
2018-02-24 07:37:43.633 UTC [chaincode_support.go:585] [chaincode] Launch -> DEBU 3a9 chaincode is running(no need to launch) : vscc:1.0.5
2018-02-24 07:37:43.633 UTC [chaincode_support.go:720] [chaincode] Execute -> DEBU 3aa Entry
2018-02-24 07:37:43.633 UTC [chaincode_support.go:723] [chaincode] Execute -> DEBU 3ab chaincode canonical name: vscc:1.0.5
2018-02-24 07:37:43.633 UTC [handler.go:1436] [chaincode] sendExecuteMessage -> DEBU 3ac [6570281f]Inside sendExecuteMessage. Message TRANSACTION
2018-02-24 07:37:43.633 UTC [handler.go:1350] [chaincode] setChaincodeProposal -> DEBU 3ad Setting chaincode proposal context...
2018-02-24 07:37:43.635 UTC [handler.go:1444] [chaincode] sendExecuteMessage -> DEBU 3ae [6570281f]sendExecuteMsg trigger event TRANSACTION
2018-02-24 07:37:43.635 UTC [handler.go:357] [chaincode] processStream -> DEBU 3af [6570281f]Move state message TRANSACTION
2018-02-24 07:37:43.635 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 3b0 [6570281f]Fabric side Handling ChaincodeMessage of type: TRANSACTION in state ready
2018-02-24 07:37:43.635 UTC [handler.go:1417] [chaincode] filterError -> DEBU 3b1 Ignoring NoTransitionError: no transition
2018-02-24 07:37:43.635 UTC [handler.go:377] [chaincode] processStream -> DEBU 3b2 [6570281f]sending state message TRANSACTION
2018-02-24 07:37:43.635 UTC [chaincode.go:297] [shim] func1 -> DEBU 3b3 [6570281f]Received message TRANSACTION from shim
2018-02-24 07:37:43.635 UTC [handler.go:778] [shim] handleMessage -> DEBU 3b4 [6570281f]Handling ChaincodeMessage of type: TRANSACTION(state:ready)
2018-02-24 07:37:43.635 UTC [handler.go:338] [shim] beforeTransaction -> DEBU 3b5 [6570281f]Received TRANSACTION, invoking transaction on chaincode(Src:ready, Dst:ready)
2018-02-24 07:37:43.636 UTC [validator_onevalidsignature.go:91] [vscc] Invoke -> DEBU 3b6 VSCC invoked
2018-02-24 07:37:43.636 UTC [validator_onevalidsignature.go:457] [vscc] deduplicateIdentity -> DEBU 3b7 Signature set is of size 1 out of 1 endorsement(s)
2018-02-24 07:37:43.637 UTC [validator_onevalidsignature.go:150] [vscc] Invoke -> WARN 3b8 Endorsement policy failure for transaction txid=420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf, err: Failed to authenticate policy
2018-02-24 07:37:43.637 UTC [handler.go:326] [shim] func1 -> DEBU 3b9 [6570281f]Transaction completed. Sending COMPLETED
2018-02-24 07:37:43.637 UTC [chaincode.go:304] [shim] func1 -> DEBU 3ba [6570281f]Move state message COMPLETED
2018-02-24 07:37:43.638 UTC [handler.go:778] [shim] handleMessage -> DEBU 3bb [6570281f]Handling ChaincodeMessage of type: COMPLETED(state:ready)
2018-02-24 07:37:43.638 UTC [chaincode.go:320] [shim] func1 -> DEBU 3bc [6570281f]send state message COMPLETED
2018-02-24 07:37:43.638 UTC [handler.go:336] [chaincode] processStream -> DEBU 3bd [6570281f]Received message COMPLETED from shim
2018-02-24 07:37:43.638 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 3be [6570281f]Fabric side Handling ChaincodeMessage of type: COMPLETED in state ready
2018-02-24 07:37:43.638 UTC [handler.go:1392] [chaincode] HandleMessage -> DEBU 3bf [6570281f-d71d-4c49-a765-b0d849405554]HandleMessage- COMPLETED. Notify
2018-02-24 07:37:43.638 UTC [handler.go:551] [chaincode] notify -> DEBU 3c0 notifying Txid:6570281f-d71d-4c49-a765-b0d849405554
2018-02-24 07:37:43.638 UTC [chaincode_support.go:751] [chaincode] Execute -> DEBU 3c1 Exit
2018-02-24 07:37:43.638 UTC [validator.go:630] [txvalidator] VSCCValidateTxForCC -> ERRO 3c2 VSCC check failed for transaction txid=420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf, error VSCC error: policy evaluation failed, err Failed to authenticate policy
2018-02-24 07:37:43.638 UTC [lockbased_query_executer.go:62] [lockbasedtxmgr] Done -> DEBU 3c3 Done with transaction simulation / query execution [563ffb5b-37a9-4659-b0ca-ab31515c3230]
2018-02-24 07:37:43.638 UTC [validator.go:206] [txvalidator] Validate -> ERRO 3c4 VSCCValidateTx for transaction txId = 420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf returned error VSCC error: policy evaluation failed, err Failed to authenticate policy
2018-02-24 07:37:43.638 UTC [validator.go:271] [txvalidator] Validate -> DEBU 3c5 END Block Validation
2018-02-24 07:37:43.638 UTC [kv_ledger.go:215] [kvledger] Commit -> DEBU 3c6 Channel [mychannel]: Validating block [2]
2018-02-24 07:37:43.638 UTC [lockbased_txmgr.go:72] [lockbasedtxmgr] ValidateAndPrepare -> DEBU 3c7 Validating new block with num trans = [1]
2018-02-24 07:37:43.639 UTC [state_based_validator.go:78] [statevalidator] ValidateAndPrepareBatch -> DEBU 3c8 New block arrived for validation:&common.Block{Header:(*common.BlockHeader)(0xc421959980), Data:(*common.BlockData)(0xc42030f600), Metadata:(*common.BlockMetadata)(0xc42030f640)}, doMVCCValidation=true
2018-02-24 07:37:43.639 UTC [state_based_validator.go:80] [statevalidator] ValidateAndPrepareBatch -> DEBU 3c9 Validating a block with [1] transactions
2018-02-24 07:37:43.639 UTC [state_based_validator.go:95] [statevalidator] ValidateAndPrepareBatch -> WARN 3ca Block [2] Transaction index [0] marked as invalid by committer. Reason code [10]
2018-02-24 07:37:43.639 UTC [kv_ledger.go:221] [kvledger] Commit -> DEBU 3cb Channel [mychannel]: Committing block [2] to storage
2018-02-24 07:37:43.640 UTC [blockindex.go:98] [fsblkstorage] indexBlock -> DEBU 3cc Indexing block [blockNum=2, blockHash=[]byte{0xdd, 0x6d, 0xbb, 0x5e, 0x9d, 0x79, 0xe5, 0xbf, 0x97, 0xab, 0x95, 0x8c, 0x91, 0x96, 0x38, 0x6c, 0x1c, 0x90, 0x18, 0x91, 0x62, 0xc0, 0x67, 0xe9, 0xc1, 0xcb, 0xaf, 0xa1, 0xa7, 0x1, 0x88, 0x38} txOffsets=
txId=420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf locPointer=offset=70, bytesLength=2925
]
2018-02-24 07:37:43.640 UTC [blockindex.go:122] [fsblkstorage] indexBlock -> DEBU 3cd Adding txLoc [fileSuffixNum=0, offset=17143, bytesLength=2925] for tx ID: [420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf] to index
2018-02-24 07:37:43.640 UTC [blockindex.go:135] [fsblkstorage] indexBlock -> DEBU 3ce Adding txLoc [fileSuffixNum=0, offset=17143, bytesLength=2925] for tx number:[0] ID: [420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf] to blockNumTranNum index
2018-02-24 07:37:43.641 UTC [blockfile_mgr.go:430] [fsblkstorage] updateCheckpoint -> DEBU 3cf Broadcasting about update checkpointInfo: latestFileChunkSuffixNum=[0], latestFileChunksize=[21826], isChainEmpty=[false], lastBlockNumber=[2]
2018-02-24 07:37:43.641 UTC [kv_ledger.go:225] [kvledger] Commit -> INFO 3d0 Channel [mychannel]: Created block [2] with 1 transaction(s)
2018-02-24 07:37:43.641 UTC [kv_ledger.go:227] [kvledger] Commit -> DEBU 3d1 Channel [mychannel]: Committing block [2] transactions to state database
2018-02-24 07:37:43.641 UTC [lockbased_txmgr.go:89] [lockbasedtxmgr] Commit -> DEBU 3d2 Committing updates to state database
2018-02-24 07:37:43.641 UTC [lockbased_txmgr.go:92] [lockbasedtxmgr] Commit -> DEBU 3d3 Write lock acquired for committing updates to state database
2018-02-24 07:37:43.642 UTC [lockbased_txmgr.go:101] [lockbasedtxmgr] Commit -> DEBU 3d4 Updates committed to state database
2018-02-24 07:37:43.642 UTC [kv_ledger.go:234] [kvledger] Commit -> DEBU 3d5 Channel [mychannel]: Committing block [2] transactions to history database
2018-02-24 07:37:43.642 UTC [historyleveldb.go:93] [historyleveldb] Commit -> DEBU 3d6 Channel [mychannel]: Updating history database for blockNo [2] with [1] transactions
2018-02-24 07:37:43.642 UTC [historyleveldb.go:109] [historyleveldb] Commit -> DEBU 3d7 Channel [mychannel]: Skipping history write for invalid transaction number 0
2018-02-24 07:37:43.643 UTC [historyleveldb.go:177] [historyleveldb] Commit -> DEBU 3d8 Channel [mychannel]: Updates committed to history database for blockNo [2]
2018-02-24 07:37:43.643 UTC [eventhelper.go:29] [eventhub_producer] SendProducerBlockEvent -> DEBU 3d9 Entry
2018-02-24 07:37:43.644 UTC [eventhelper.go:55] [eventhub_producer] SendProducerBlockEvent -> DEBU 3da Channel [mychannel]: Block event for block number [2] contains transaction id: 420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf
2018-02-24 07:37:43.644 UTC [eventhelper.go:105] [eventhub_producer] SendProducerBlockEvent -> INFO 3db Channel [mychannel]: Sending event for block number [2]
2018-02-24 07:37:43.647 UTC [events.go:312] [eventhub_producer] Send -> DEBU 3dc Entry
2018-02-24 07:37:43.648 UTC [events.go:335] [eventhub_producer] Send -> DEBU 3dd Event processor timeout > 0
2018-02-24 07:37:43.648 UTC [events.go:343] [eventhub_producer] Send -> DEBU 3de Event sent successfully
2018-02-24 07:37:43.648 UTC [events.go:344] [eventhub_producer] Send -> DEBU 3df Exit
2018-02-24 07:37:43.648 UTC [eventhelper.go:107] [eventhub_producer] SendProducerBlockEvent -> DEBU 3e0 Exit
2018-02-24 07:37:43.648 UTC [register_internal_events.go:41] [eventhub_producer] getMessageType -> DEBU 3e1 [gerry] Event_Block
```

### invork (order 日志)

```
2018-02-24 07:37:41.576 UTC [orderer/main] /opt/gopath/src/github.com/hyperledger/fabric/orderer/server.go:74 Broadcast -> DEBU 958 Starting new Broadcast handler
2018-02-24 07:37:41.577 UTC [orderer/common/broadcast] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/broadcast/broadcast.go:77 Handle -> DEBU 959 Starting new broadcast loop
2018-02-24 07:37:41.603 UTC [orderer/common/broadcast] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/broadcast/broadcast.go:138 Handle -> DEBU 95a [channel: mychannel] Broadcast is filtering message of type ENDORSER_TRANSACTION
2018-02-24 07:37:41.603 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 95b Returning policy Writers for evaluation
2018-02-24 07:37:41.603 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:58 func1 -> DEBU 95c 0xc420a5c038 gate 1519457861603173881 evaluation starts
2018-02-24 07:37:41.603 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:83 func2 -> DEBU 95d 0xc420a5c038 signed by 0 principal evaluation starts (used [false])
2018-02-24 07:37:41.603 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:91 func2 -> DEBU 95e 0xc420a5c038 processing identity 0 with bytes of 0a074f7267314d53501280062d2d2d2d2d424547494e202d2d2d2d2d0a4d4949434754434341622b6741774942416749515532454561656b5a71314c734936625a45644c39727a414b42676771686b6a4f50515144416a427a4d5173770a435159445651514745774a56557a45544d4245474131554543424d4b5132467361575a76636d3570595445574d4251474131554542784d4e5532467549455a790a5957356a61584e6a627a455a4d4263474131554543684d5162334a6e4d53356c654746746347786c4c6d4e76625445634d426f474131554541784d54593245750a62334a6e4d53356c654746746347786c4c6d4e7662544165467730784f4441794d4463774d4445314d545261467730794f4441794d4455774d4445314d5452610a4d467378437a414a42674e5642415954416c56544d524d77455159445651514945777044595778705a6d3979626d6c684d5259774641594456515148457731540a59573467526e4a68626d4e7063324e764d523877485159445651514444425a425a473170626b4276636d63784c6d56345957317762475575593239744d466b770a457759484b6f5a497a6a3043415159494b6f5a497a6a3044415163445167414530316b55773456592f5973724d744c38655a785a63686e42757a5a4e504f666a0a4a4e39374d3738777844475756566e484468724d46656d434a3072334d4c4a77324a2f414a383639793948645572445758306c6f2b364e4e4d457377446759440a565230504151482f42415144416765414d41774741315564457745422f7751434d4141774b7759445652306a42435177496f416754385951514a54664f5658490a456b364a57304a7471664e4c4e355a73396950584f63334c454d4a4759453077436759494b6f5a497a6a3045417749445341417752514968414a70767472364a0a454b6b53696b783830324553476d776478656a6d5532485434766a7953556c7559435337416942414f3762503358327349465943462f41393462426a666351520a584e76594a71504a2b592f7a4c596f3250513d3d0a2d2d2d2d2d454e44202d2d2d2d2d0a
2018-02-24 07:37:41.603 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:42 newIdentity -> DEBU 95f Creating identity instance for ID -----BEGIN CERTIFICATE-----
MIICGTCCAb+gAwIBAgIQU2EEaekZq1LsI6bZEdL9rzAKBggqhkjOPQQDAjBzMQsw
CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy
YW5jaXNjbzEZMBcGA1UEChMQb3JnMS5leGFtcGxlLmNvbTEcMBoGA1UEAxMTY2Eu
b3JnMS5leGFtcGxlLmNvbTAeFw0xODAyMDcwMDE1MTRaFw0yODAyMDUwMDE1MTRa
MFsxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1T
YW4gRnJhbmNpc2NvMR8wHQYDVQQDDBZBZG1pbkBvcmcxLmV4YW1wbGUuY29tMFkw
EwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE01kUw4VY/YsrMtL8eZxZchnBuzZNPOfj
JN97M78wxDGWVVnHDhrMFemCJ0r3MLJw2J/AJ869y9HdUrDWX0lo+6NNMEswDgYD
VR0PAQH/BAQDAgeAMAwGA1UdEwEB/wQCMAAwKwYDVR0jBCQwIoAgT8YQQJTfOVXI
Ek6JW0JtqfNLN5Zs9iPXOc3LEMJGYE0wCgYIKoZIzj0EAwIDSAAwRQIhAJpvtr6J
EKkSikx802ESGmwdxejmU2HT4vjySUluYCS7AiBAO7bP3X2sIFYCF/A94bBjfcQR
XNvYJqPJ+Y/zLYo2PQ==
-----END CERTIFICATE-----
2018-02-24 07:37:41.603 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:489 SatisfiesPrincipal -> DEBU 960 Checking if identity satisfies MEMBER role for Org1MSP
2018-02-24 07:37:41.603 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:391 Validate -> DEBU 961 MSP Org1MSP validating identity
2018-02-24 07:37:41.604 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:103 func2 -> DEBU 962 0xc420a5c038 principal matched by identity 0
2018-02-24 07:37:41.604 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:129 Verify -> DEBU 963 Verify: digest = 00000000  93 89 b4 89 1f c5 99 cf  8a 0c fb 1b ae be 7f 52  |...............R|
00000010  f0 fb ef 4b 47 87 bb a0  c2 59 44 2a c2 30 d9 14  |...KG....YD*.0..|
2018-02-24 07:37:41.604 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:130 Verify -> DEBU 964 Verify: sig = 00000000  30 45 02 21 00 e8 49 43  07 d4 09 d9 ea 0c 29 58  |0E.!..IC......)X|
00000010  99 1c 78 60 aa ad 28 32  fa 28 7d 91 30 b8 f2 c9  |..x`..(2.(}.0...|
00000020  38 7f 29 4f 7b 02 20 23  5e fb b7 89 e7 e2 99 71  |8.)O{. #^......q|
00000030  e5 77 18 bb 4c 1f 7d 2b  b2 6f 11 db 33 48 ff ca  |.w..L.}+.o..3H..|
00000040  8a 88 ae 70 7d 90 34                              |...p}.4|
2018-02-24 07:37:41.604 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:109 func2 -> DEBU 965 0xc420a5c038 principal evaluation succeeds for identity 0
2018-02-24 07:37:41.604 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:70 func1 -> DEBU 966 0xc420a5c038 gate 1519457861603173881 evaluation succeeds
2018-02-24 07:37:41.604 UTC [orderer/common/sigfilter] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/sigfilter/sigfilter.go:69 Apply -> DEBU 967 Forwarding validly signed message for policy &{%!s(*common.ImplicitMetaPolicy=&{Writers 0}) %!s(int=1) [%!s(*policies.implicitMetaPolicy=&{0xc4209ae200 1 [0xc420a5d120 0xc420a5d1e0]}) %!s(*policies.implicitMetaPolicy=&{0xc4209af160 1 [0xc420a5d258]})]}
2018-02-24 07:37:41.604 UTC [orderer/common/broadcast] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/broadcast/broadcast.go:153 Handle -> DEBU 968 [channel: mychannel] Broadcast has successfully enqueued message of type ENDORSER_TRANSACTION
2018-02-24 07:37:41.604 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 969 Returning policy Writers for evaluation
2018-02-24 07:37:41.604 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:58 func1 -> DEBU 96a 0xc420a5c068 gate 1519457861604866480 evaluation starts
2018-02-24 07:37:41.604 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:83 func2 -> DEBU 96b 0xc420a5c068 signed by 0 principal evaluation starts (used [false])
2018-02-24 07:37:41.604 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:91 func2 -> DEBU 96c 0xc420a5c068 processing identity 0 with bytes of 0a074f7267314d53501280062d2d2d2d2d424547494e202d2d2d2d2d0a4d4949434754434341622b6741774942416749515532454561656b5a71314c734936625a45644c39727a414b42676771686b6a4f50515144416a427a4d5173770a435159445651514745774a56557a45544d4245474131554543424d4b5132467361575a76636d3570595445574d4251474131554542784d4e5532467549455a790a5957356a61584e6a627a455a4d4263474131554543684d5162334a6e4d53356c654746746347786c4c6d4e76625445634d426f474131554541784d54593245750a62334a6e4d53356c654746746347786c4c6d4e7662544165467730784f4441794d4463774d4445314d545261467730794f4441794d4455774d4445314d5452610a4d467378437a414a42674e5642415954416c56544d524d77455159445651514945777044595778705a6d3979626d6c684d5259774641594456515148457731540a59573467526e4a68626d4e7063324e764d523877485159445651514444425a425a473170626b4276636d63784c6d56345957317762475575593239744d466b770a457759484b6f5a497a6a3043415159494b6f5a497a6a3044415163445167414530316b55773456592f5973724d744c38655a785a63686e42757a5a4e504f666a0a4a4e39374d3738777844475756566e484468724d46656d434a3072334d4c4a77324a2f414a383639793948645572445758306c6f2b364e4e4d457377446759440a565230504151482f42415144416765414d41774741315564457745422f7751434d4141774b7759445652306a42435177496f416754385951514a54664f5658490a456b364a57304a7471664e4c4e355a73396950584f63334c454d4a4759453077436759494b6f5a497a6a3045417749445341417752514968414a70767472364a0a454b6b53696b783830324553476d776478656a6d5532485434766a7953556c7559435337416942414f3762503358327349465943462f41393462426a666351520a584e76594a71504a2b592f7a4c596f3250513d3d0a2d2d2d2d2d454e44202d2d2d2d2d0a
2018-02-24 07:37:41.605 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:42 newIdentity -> DEBU 96d Creating identity instance for ID -----BEGIN CERTIFICATE-----
MIICGTCCAb+gAwIBAgIQU2EEaekZq1LsI6bZEdL9rzAKBggqhkjOPQQDAjBzMQsw
CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy
YW5jaXNjbzEZMBcGA1UEChMQb3JnMS5leGFtcGxlLmNvbTEcMBoGA1UEAxMTY2Eu
b3JnMS5leGFtcGxlLmNvbTAeFw0xODAyMDcwMDE1MTRaFw0yODAyMDUwMDE1MTRa
MFsxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1T
YW4gRnJhbmNpc2NvMR8wHQYDVQQDDBZBZG1pbkBvcmcxLmV4YW1wbGUuY29tMFkw
EwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE01kUw4VY/YsrMtL8eZxZchnBuzZNPOfj
JN97M78wxDGWVVnHDhrMFemCJ0r3MLJw2J/AJ869y9HdUrDWX0lo+6NNMEswDgYD
VR0PAQH/BAQDAgeAMAwGA1UdEwEB/wQCMAAwKwYDVR0jBCQwIoAgT8YQQJTfOVXI
Ek6JW0JtqfNLN5Zs9iPXOc3LEMJGYE0wCgYIKoZIzj0EAwIDSAAwRQIhAJpvtr6J
EKkSikx802ESGmwdxejmU2HT4vjySUluYCS7AiBAO7bP3X2sIFYCF/A94bBjfcQR
XNvYJqPJ+Y/zLYo2PQ==
-----END CERTIFICATE-----
2018-02-24 07:37:41.611 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:489 SatisfiesPrincipal -> DEBU 96e Checking if identity satisfies MEMBER role for Org1MSP
2018-02-24 07:37:41.611 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:391 Validate -> DEBU 96f MSP Org1MSP validating identity
2018-02-24 07:37:41.611 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:103 func2 -> DEBU 970 0xc420a5c068 principal matched by identity 0
2018-02-24 07:37:41.611 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:129 Verify -> DEBU 971 Verify: digest = 00000000  93 89 b4 89 1f c5 99 cf  8a 0c fb 1b ae be 7f 52  |...............R|
00000010  f0 fb ef 4b 47 87 bb a0  c2 59 44 2a c2 30 d9 14  |...KG....YD*.0..|
2018-02-24 07:37:41.611 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:130 Verify -> DEBU 972 Verify: sig = 00000000  30 45 02 21 00 e8 49 43  07 d4 09 d9 ea 0c 29 58  |0E.!..IC......)X|
00000010  99 1c 78 60 aa ad 28 32  fa 28 7d 91 30 b8 f2 c9  |..x`..(2.(}.0...|
00000020  38 7f 29 4f 7b 02 20 23  5e fb b7 89 e7 e2 99 71  |8.)O{. #^......q|
00000030  e5 77 18 bb 4c 1f 7d 2b  b2 6f 11 db 33 48 ff ca  |.w..L.}+.o..3H..|
00000040  8a 88 ae 70 7d 90 34                              |...p}.4|
2018-02-24 07:37:41.612 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:109 func2 -> DEBU 973 0xc420a5c068 principal evaluation succeeds for identity 0
2018-02-24 07:37:41.612 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:70 func1 -> DEBU 974 0xc420a5c068 gate 1519457861604866480 evaluation succeeds
2018-02-24 07:37:41.612 UTC [orderer/common/sigfilter] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/sigfilter/sigfilter.go:69 Apply -> DEBU 975 Forwarding validly signed message for policy &{%!s(*common.ImplicitMetaPolicy=&{Writers 0}) %!s(int=1) [%!s(*policies.implicitMetaPolicy=&{0xc4209ae200 1 [0xc420a5d120 0xc420a5d1e0]}) %!s(*policies.implicitMetaPolicy=&{0xc4209af160 1 [0xc420a5d258]})]}
2018-02-24 07:37:41.612 UTC [orderer/common/blockcutter] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/blockcutter/blockcutter.go:136 Ordered -> DEBU 976 Enqueuing message into batch
2018-02-24 07:37:41.612 UTC [orderer/common/broadcast] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/broadcast/broadcast.go:85 Handle -> WARN 977 Error reading from stream: rpc error: code = Canceled desc = context canceled
2018-02-24 07:37:41.612 UTC [orderer/main] /opt/gopath/src/github.com/hyperledger/fabric/orderer/server.go:79 func1 -> DEBU 978 Closing Broadcast stream
2018-02-24 07:37:43.612 UTC [orderer/solo] /opt/gopath/src/github.com/hyperledger/fabric/orderer/solo/consensus.go:112 main -> DEBU 979 Batch timer expired, creating block
2018-02-24 07:37:43.616 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_mgr.go:454 retrieveBlockByNumber -> DEBU 97a retrieveBlockByNumber() - blockNum = [1]
2018-02-24 07:37:43.616 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:66 newBlockfileStream -> DEBU 97b newBlockfileStream(): filePath=[/var/hyperledger/production/orderer/chains/mychannel/blockfile_000000], startOffset=[11912]
2018-02-24 07:37:43.616 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:114 nextBlockBytesAndPlacementInfo -> DEBU 97c Remaining bytes=[5159], Going to peek [8] bytes
2018-02-24 07:37:43.616 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:147 nextBlockBytesAndPlacementInfo -> DEBU 97d Returning blockbytes - length=[5157], placementInfo={fileNum=[0], startOffset=[11912], bytesOffset=[11914]}
2018-02-24 07:37:43.616 UTC [orderer/multichain] /opt/gopath/src/github.com/hyperledger/fabric/orderer/multichain/chainsupport.go:228 addBlockSignature -> DEBU 97e &{ledgerResources:0xc42098b7a0 chain:0xc42097e0c0 cutter:0xc4202df590 filters:0xc42098bcc0 signer:0x128b4a8 lastConfig:0 lastConfigSeq:1}
2018-02-24 07:37:43.616 UTC [orderer/multichain] /opt/gopath/src/github.com/hyperledger/fabric/orderer/multichain/chainsupport.go:229 addBlockSignature -> DEBU 97f &{}
2018-02-24 07:37:43.616 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mgmt/mgmt.go:149 GetLocalMSP -> DEBU 980 Returning existing local MSP
2018-02-24 07:37:43.616 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:369 GetDefaultSigningIdentity -> DEBU 981 Obtaining default signing identity
2018-02-24 07:37:43.616 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mgmt/mgmt.go:149 GetLocalMSP -> DEBU 982 Returning existing local MSP
2018-02-24 07:37:43.616 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:369 GetDefaultSigningIdentity -> DEBU 983 Obtaining default signing identity
2018-02-24 07:37:43.616 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:208 Sign -> DEBU 984 Sign: plaintext: 0AFF050A0A4F7264657265724D535012...1C64B131116654B2F470364C84DC1F55 
2018-02-24 07:37:43.618 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:210 Sign -> DEBU 985 Sign: digest: DD42D939B338DC78EFA1E443A4F8DB1069684E5A89AD5AB9158CC587769BDD84 
2018-02-24 07:37:43.618 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mgmt/mgmt.go:149 GetLocalMSP -> DEBU 986 Returning existing local MSP
2018-02-24 07:37:43.618 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:369 GetDefaultSigningIdentity -> DEBU 987 Obtaining default signing identity
2018-02-24 07:37:43.618 UTC [orderer/multichain] /opt/gopath/src/github.com/hyperledger/fabric/orderer/multichain/chainsupport.go:262 addLastConfigSignature -> DEBU 988 [channel: mychannel] About to write block, setting its LAST_CONFIG to 0
2018-02-24 07:37:43.618 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mgmt/mgmt.go:149 GetLocalMSP -> DEBU 989 Returning existing local MSP
2018-02-24 07:37:43.618 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:369 GetDefaultSigningIdentity -> DEBU 98a Obtaining default signing identity
2018-02-24 07:37:43.618 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:208 Sign -> DEBU 98b Sign: plaintext: 0AFF050A0A4F7264657265724D535012...1C64B131116654B2F470364C84DC1F55 
2018-02-24 07:37:43.618 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:210 Sign -> DEBU 98c Sign: digest: F8122096397CA10E5406E3F212E7283E01C7EB9E60700062F49696A7B5AEFD60 
2018-02-24 07:37:43.620 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockindex.go:98 indexBlock -> DEBU 98d Indexing block [blockNum=2, blockHash=[]byte{0xdd, 0x6d, 0xbb, 0x5e, 0x9d, 0x79, 0xe5, 0xbf, 0x97, 0xab, 0x95, 0x8c, 0x91, 0x96, 0x38, 0x6c, 0x1c, 0x90, 0x18, 0x91, 0x62, 0xc0, 0x67, 0xe9, 0xc1, 0xcb, 0xaf, 0xa1, 0xa7, 0x1, 0x88, 0x38} txOffsets=
txId=420ef361db9483bdef5afc107b5a539f556b7a88971f15459453ad0d43d677cf locPointer=offset=70, bytesLength=2925
]
2018-02-24 07:37:43.621 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_mgr.go:430 updateCheckpoint -> DEBU 98e Broadcasting about update checkpointInfo: latestFileChunkSuffixNum=[0], latestFileChunksize=[21823], isChainEmpty=[false], lastBlockNumber=[2]
2018-02-24 07:37:43.621 UTC [orderer/multichain] /opt/gopath/src/github.com/hyperledger/fabric/orderer/multichain/chainsupport.go:289 WriteBlock -> DEBU 98f [channel: mychannel] Wrote block 2
2018-02-24 07:37:43.621 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_mgr.go:454 retrieveBlockByNumber -> DEBU 990 retrieveBlockByNumber() - blockNum = [2]
2018-02-24 07:37:43.621 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:66 newBlockfileStream -> DEBU 991 newBlockfileStream(): filePath=[/var/hyperledger/production/orderer/chains/mychannel/blockfile_000000], startOffset=[17071]
2018-02-24 07:37:43.621 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:114 nextBlockBytesAndPlacementInfo -> DEBU 992 Remaining bytes=[4752], Going to peek [8] bytes
2018-02-24 07:37:43.621 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:147 nextBlockBytesAndPlacementInfo -> DEBU 993 Returning blockbytes - length=[4750], placementInfo={fileNum=[0], startOffset=[17071], bytesOffset=[17073]}
2018-02-24 07:37:43.621 UTC [orderer/common/deliver] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/deliver/deliver.go:191 Handle -> DEBU 994 [channel: mychannel] Delivering block for (0xc42032a640)
2018-02-24 07:37:43.622 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_mgr.go:454 retrieveBlockByNumber -> DEBU 995 retrieveBlockByNumber() - blockNum = [2]
2018-02-24 07:37:43.622 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:66 newBlockfileStream -> DEBU 996 newBlockfileStream(): filePath=[/var/hyperledger/production/orderer/chains/mychannel/blockfile_000000], startOffset=[17071]
2018-02-24 07:37:43.622 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:114 nextBlockBytesAndPlacementInfo -> DEBU 997 Remaining bytes=[4752], Going to peek [8] bytes
2018-02-24 07:37:43.622 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:147 nextBlockBytesAndPlacementInfo -> DEBU 998 Returning blockbytes - length=[4750], placementInfo={fileNum=[0], startOffset=[17071], bytesOffset=[17073]}
2018-02-24 07:37:43.622 UTC [orderer/common/deliver] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/deliver/deliver.go:191 Handle -> DEBU 999 [channel: mychannel] Delivering block for (0xc42085a3c0)
2018-02-24 07:37:43.627 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_mgr.go:454 retrieveBlockByNumber -> DEBU 99a retrieveBlockByNumber() - blockNum = [2]
2018-02-24 07:37:43.627 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:66 newBlockfileStream -> DEBU 99b newBlockfileStream(): filePath=[/var/hyperledger/production/orderer/chains/mychannel/blockfile_000000], startOffset=[17071]
2018-02-24 07:37:43.627 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:114 nextBlockBytesAndPlacementInfo -> DEBU 99c Remaining bytes=[4752], Going to peek [8] bytes
2018-02-24 07:37:43.628 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:147 nextBlockBytesAndPlacementInfo -> DEBU 99d Returning blockbytes - length=[4750], placementInfo={fileNum=[0], startOffset=[17071], bytesOffset=[17073]}
2018-02-24 07:37:43.629 UTC [orderer/common/deliver] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/deliver/deliver.go:191 Handle -> DEBU 99e [channel: mychannel] Delivering block for (0xc4209d8560)
```

### query (执行peer 日志)

```
2018-02-24 07:43:03.321 UTC [endorser.go:373] [endorser] ProcessProposal -> DEBU 50f Entry
2018-02-24 07:43:03.323 UTC [msgvalidation.go:79] [protoutils] ValidateProposalMessage -> DEBU 510 ValidateProposalMessage starts for signed proposal 0xc421ff5e90
2018-02-24 07:43:03.323 UTC [msgvalidation.go:215] [protoutils] validateChannelHeader -> DEBU 511 validateChannelHeader info: header type 3
2018-02-24 07:43:03.323 UTC [msgvalidation.go:142] [protoutils] checkSignatureFromCreator -> DEBU 512 checkSignatureFromCreator starts
2018-02-24 07:43:03.323 UTC [msgvalidation.go:160] [protoutils] checkSignatureFromCreator -> DEBU 513 checkSignatureFromCreator info: creator is &{Org1MSP 3dc70c07d3096e889a28bca219d07b0d858baede9b7d60760c9bf5a76017c9a1}
2018-02-24 07:43:03.324 UTC [msgvalidation.go:168] [protoutils] checkSignatureFromCreator -> DEBU 514 checkSignatureFromCreator info: creator is valid
2018-02-24 07:43:03.324 UTC [msgvalidation.go:176] [protoutils] checkSignatureFromCreator -> DEBU 515 checkSignatureFromCreator exists successfully
2018-02-24 07:43:03.324 UTC [msgvalidation.go:39] [protoutils] validateChaincodeProposalMessage -> DEBU 516 validateChaincodeProposalMessage starts for proposal 0xc421ff8dc0, header 0xc421ff5ef0
2018-02-24 07:43:03.324 UTC [msgvalidation.go:51] [protoutils] validateChaincodeProposalMessage -> DEBU 517 validateChaincodeProposalMessage info: header extension references chaincode name:"fabcar" 
2018-02-24 07:43:03.325 UTC [endorser.go:412] [endorser] ProcessProposal -> DEBU 518 processing txid: d715b4d1c0c45f9d0ace149349a29961d97d5462e2ac0d8cf86c21a9faaca69f
2018-02-24 07:43:03.325 UTC [endorser.go:414] [endorser] ProcessProposal -> DEBU 519 [gerry] chainID: mychannel
2018-02-24 07:43:03.325 UTC [blockfile_mgr.go:506] [fsblkstorage] retrieveTransactionByID -> DEBU 51a retrieveTransactionByID() - txId = [d715b4d1c0c45f9d0ace149349a29961d97d5462e2ac0d8cf86c21a9faaca69f]
2018-02-24 07:43:03.326 UTC [endorser.go:441] [endorser] ProcessProposal -> DEBU 51b [gerry] to get TxSimulator and HistoryQueryExecutor
2018-02-24 07:43:03.326 UTC [lockbased_txmgr.go:64] [lockbasedtxmgr] NewTxSimulator -> DEBU 51c constructing new tx simulator
2018-02-24 07:43:03.326 UTC [lockbased_tx_simulator.go:36] [lockbasedtxmgr] newLockBasedTxSimulator -> DEBU 51d constructing new tx simulator [23f8d1e0-df23-4df1-93c9-22322087a036]
2018-02-24 07:43:03.327 UTC [endorser.go:472] [endorser] ProcessProposal -> DEBU 51e [gerry] 1 -- simulate
2018-02-24 07:43:03.327 UTC [endorser.go:211] [endorser] simulateProposal -> DEBU 51f Entry - txid: d715b4d1c0c45f9d0ace149349a29961d97d5462e2ac0d8cf86c21a9faaca69f channel id: mychannel
2018-02-24 07:43:03.327 UTC [ccprovider.go:363] [ccprovider] NewCCContext -> DEBU 520 NewCCCC (chain=mychannel,chaincode=lscc,version=1.0.5,txid=d715b4d1c0c45f9d0ace149349a29961d97d5462e2ac0d8cf86c21a9faaca69f,syscc=true,proposal=0xc421ff8dc0,canname=lscc:1.0.5
2018-02-24 07:43:03.327 UTC [chaincode_support.go:585] [chaincode] Launch -> DEBU 521 chaincode is running(no need to launch) : lscc:1.0.5
2018-02-24 07:43:03.327 UTC [chaincode_support.go:720] [chaincode] Execute -> DEBU 522 Entry
2018-02-24 07:43:03.327 UTC [chaincode_support.go:723] [chaincode] Execute -> DEBU 523 chaincode canonical name: lscc:1.0.5
2018-02-24 07:43:03.327 UTC [handler.go:1436] [chaincode] sendExecuteMessage -> DEBU 524 [d715b4d1]Inside sendExecuteMessage. Message TRANSACTION
2018-02-24 07:43:03.327 UTC [handler.go:1350] [chaincode] setChaincodeProposal -> DEBU 525 Setting chaincode proposal context...
2018-02-24 07:43:03.327 UTC [handler.go:1352] [chaincode] setChaincodeProposal -> DEBU 526 Proposal different from nil. Creating chaincode proposal context...
2018-02-24 07:43:03.327 UTC [handler.go:1444] [chaincode] sendExecuteMessage -> DEBU 527 [d715b4d1]sendExecuteMsg trigger event TRANSACTION
2018-02-24 07:43:03.327 UTC [handler.go:357] [chaincode] processStream -> DEBU 528 [d715b4d1]Move state message TRANSACTION
2018-02-24 07:43:03.328 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 529 [d715b4d1]Fabric side Handling ChaincodeMessage of type: TRANSACTION in state ready
2018-02-24 07:43:03.328 UTC [handler.go:1417] [chaincode] filterError -> DEBU 52a Ignoring NoTransitionError: no transition
2018-02-24 07:43:03.331 UTC [handler.go:377] [chaincode] processStream -> DEBU 52b [d715b4d1]sending state message TRANSACTION
2018-02-24 07:43:03.331 UTC [chaincode.go:297] [shim] func1 -> DEBU 52c [d715b4d1]Received message TRANSACTION from shim
2018-02-24 07:43:03.331 UTC [handler.go:778] [shim] handleMessage -> DEBU 52d [d715b4d1]Handling ChaincodeMessage of type: TRANSACTION(state:ready)
2018-02-24 07:43:03.331 UTC [handler.go:338] [shim] beforeTransaction -> DEBU 52e [d715b4d1]Received TRANSACTION, invoking transaction on chaincode(Src:ready, Dst:ready)
2018-02-24 07:43:03.332 UTC [handler.go:392] [shim] handleGetState -> DEBU 52f [d715b4d1]Sending GET_STATE
2018-02-24 07:43:03.333 UTC [handler.go:336] [chaincode] processStream -> DEBU 530 [d715b4d1]Received message GET_STATE from shim
2018-02-24 07:43:03.333 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 531 [d715b4d1]Fabric side Handling ChaincodeMessage of type: GET_STATE in state ready
2018-02-24 07:43:03.333 UTC [handler.go:580] [chaincode] afterGetState -> DEBU 532 [d715b4d1]Received GET_STATE, invoking get state from ledger
2018-02-24 07:43:03.333 UTC [handler.go:1417] [chaincode] filterError -> DEBU 533 Ignoring NoTransitionError: no transition
2018-02-24 07:43:03.333 UTC [handler.go:634] [chaincode] func1 -> DEBU 534 [d715b4d1] getting state for chaincode lscc, key fabcar, channel mychannel
2018-02-24 07:43:03.333 UTC [stateleveldb.go:89] [stateleveldb] GetState -> DEBU 535 GetState(). ns=lscc, key=fabcar
2018-02-24 07:43:03.333 UTC [util.go:58] [kvledger.util] DecodeOrderPreservingVarUint64 -> DEBU 536 proto.DecodeVarint(bytes):1
2018-02-24 07:43:03.333 UTC [util.go:58] [kvledger.util] DecodeOrderPreservingVarUint64 -> DEBU 537 proto.DecodeVarint(bytes):0
2018-02-24 07:43:03.333 UTC [handler.go:655] [chaincode] func1 -> DEBU 538 [d715b4d1]Got state. Sending RESPONSE
2018-02-24 07:43:03.333 UTC [handler.go:621] [chaincode] 1 -> DEBU 539 [d715b4d1]handleGetState serial send RESPONSE
2018-02-24 07:43:03.333 UTC [chaincode.go:297] [shim] func1 -> DEBU 53a [d715b4d1]Received message RESPONSE from shim
2018-02-24 07:43:03.333 UTC [handler.go:778] [shim] handleMessage -> DEBU 53b [d715b4d1]Handling ChaincodeMessage of type: RESPONSE(state:ready)
2018-02-24 07:43:03.333 UTC [handler.go:115] [shim] sendChannel -> DEBU 53c [d715b4d1]before send
2018-02-24 07:43:03.333 UTC [handler.go:117] [shim] sendChannel -> DEBU 53d [d715b4d1]after send
2018-02-24 07:43:03.333 UTC [handler.go:356] [shim] afterResponse -> DEBU 53e [d715b4d1]Received RESPONSE, communicated (state:ready)
2018-02-24 07:43:03.333 UTC [handler.go:402] [shim] handleGetState -> DEBU 53f [d715b4d1]GetState received payload RESPONSE
2018-02-24 07:43:03.333 UTC [handler.go:326] [shim] func1 -> DEBU 540 [d715b4d1]Transaction completed. Sending COMPLETED
2018-02-24 07:43:03.333 UTC [chaincode.go:304] [shim] func1 -> DEBU 541 [d715b4d1]Move state message COMPLETED
2018-02-24 07:43:03.333 UTC [handler.go:778] [shim] handleMessage -> DEBU 542 [d715b4d1]Handling ChaincodeMessage of type: COMPLETED(state:ready)
2018-02-24 07:43:03.334 UTC [chaincode.go:320] [shim] func1 -> DEBU 543 [d715b4d1]send state message COMPLETED
2018-02-24 07:43:03.334 UTC [handler.go:336] [chaincode] processStream -> DEBU 544 [d715b4d1]Received message COMPLETED from shim
2018-02-24 07:43:03.334 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 545 [d715b4d1]Fabric side Handling ChaincodeMessage of type: COMPLETED in state ready
2018-02-24 07:43:03.334 UTC [handler.go:1392] [chaincode] HandleMessage -> DEBU 546 [d715b4d1c0c45f9d0ace149349a29961d97d5462e2ac0d8cf86c21a9faaca69f]HandleMessage- COMPLETED. Notify
2018-02-24 07:43:03.334 UTC [handler.go:551] [chaincode] notify -> DEBU 547 notifying Txid:d715b4d1c0c45f9d0ace149349a29961d97d5462e2ac0d8cf86c21a9faaca69f
2018-02-24 07:43:03.334 UTC [chaincode_support.go:751] [chaincode] Execute -> DEBU 548 Exit
2018-02-24 07:43:03.334 UTC [ccprovider.go:207] [ccprovider] GetChaincodeData -> DEBU 549 Getting chaincode data for <fabcar, 1.0> from cache
2018-02-24 07:43:03.334 UTC [endorser.go:99] [endorser] callChaincode -> DEBU 54a Entry - txid: d715b4d1c0c45f9d0ace149349a29961d97d5462e2ac0d8cf86c21a9faaca69f channel id: mychannel version: 1.0
2018-02-24 07:43:03.334 UTC [ccprovider.go:363] [ccprovider] NewCCContext -> DEBU 54b NewCCCC (chain=mychannel,chaincode=fabcar,version=1.0,txid=d715b4d1c0c45f9d0ace149349a29961d97d5462e2ac0d8cf86c21a9faaca69f,syscc=false,proposal=0xc421ff8dc0,canname=fabcar:1.0
2018-02-24 07:43:03.334 UTC [chaincode_support.go:585] [chaincode] Launch -> DEBU 54c chaincode is running(no need to launch) : fabcar:1.0
2018-02-24 07:43:03.334 UTC [chaincode_support.go:720] [chaincode] Execute -> DEBU 54d Entry
2018-02-24 07:43:03.334 UTC [chaincode_support.go:723] [chaincode] Execute -> DEBU 54e chaincode canonical name: fabcar:1.0
2018-02-24 07:43:03.334 UTC [handler.go:1436] [chaincode] sendExecuteMessage -> DEBU 54f [d715b4d1]Inside sendExecuteMessage. Message TRANSACTION
2018-02-24 07:43:03.334 UTC [handler.go:1350] [chaincode] setChaincodeProposal -> DEBU 550 Setting chaincode proposal context...
2018-02-24 07:43:03.334 UTC [handler.go:1352] [chaincode] setChaincodeProposal -> DEBU 551 Proposal different from nil. Creating chaincode proposal context...
2018-02-24 07:43:03.334 UTC [handler.go:1444] [chaincode] sendExecuteMessage -> DEBU 552 [d715b4d1]sendExecuteMsg trigger event TRANSACTION
2018-02-24 07:43:03.334 UTC [handler.go:357] [chaincode] processStream -> DEBU 553 [d715b4d1]Move state message TRANSACTION
2018-02-24 07:43:03.334 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 554 [d715b4d1]Fabric side Handling ChaincodeMessage of type: TRANSACTION in state ready
2018-02-24 07:43:03.334 UTC [handler.go:1417] [chaincode] filterError -> DEBU 555 Ignoring NoTransitionError: no transition
2018-02-24 07:43:03.334 UTC [handler.go:377] [chaincode] processStream -> DEBU 556 [d715b4d1]sending state message TRANSACTION
2018-02-24 07:43:03.337 UTC [handler.go:336] [chaincode] processStream -> DEBU 557 [d715b4d1]Received message GET_STATE from shim
2018-02-24 07:43:03.337 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 558 [d715b4d1]Fabric side Handling ChaincodeMessage of type: GET_STATE in state ready
2018-02-24 07:43:03.337 UTC [handler.go:580] [chaincode] afterGetState -> DEBU 559 [d715b4d1]Received GET_STATE, invoking get state from ledger
2018-02-24 07:43:03.337 UTC [handler.go:1417] [chaincode] filterError -> DEBU 55a Ignoring NoTransitionError: no transition
2018-02-24 07:43:03.337 UTC [handler.go:634] [chaincode] func1 -> DEBU 55b [d715b4d1] getting state for chaincode fabcar, key CAR100, channel mychannel
2018-02-24 07:43:03.337 UTC [stateleveldb.go:89] [stateleveldb] GetState -> DEBU 55c GetState(). ns=fabcar, key=CAR100
2018-02-24 07:43:03.337 UTC [handler.go:650] [chaincode] func1 -> DEBU 55d [d715b4d1]No state associated with key: CAR100. Sending RESPONSE with an empty payload
2018-02-24 07:43:03.337 UTC [handler.go:621] [chaincode] 1 -> DEBU 55e [d715b4d1]handleGetState serial send RESPONSE
2018-02-24 07:43:03.338 UTC [handler.go:336] [chaincode] processStream -> DEBU 55f [d715b4d1]Received message COMPLETED from shim
2018-02-24 07:43:03.339 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 560 [d715b4d1]Fabric side Handling ChaincodeMessage of type: COMPLETED in state ready
2018-02-24 07:43:03.339 UTC [handler.go:1392] [chaincode] HandleMessage -> DEBU 561 [d715b4d1c0c45f9d0ace149349a29961d97d5462e2ac0d8cf86c21a9faaca69f]HandleMessage- COMPLETED. Notify
2018-02-24 07:43:03.339 UTC [handler.go:551] [chaincode] notify -> DEBU 562 notifying Txid:d715b4d1c0c45f9d0ace149349a29961d97d5462e2ac0d8cf86c21a9faaca69f
2018-02-24 07:43:03.339 UTC [chaincode_support.go:751] [chaincode] Execute -> DEBU 563 Exit
2018-02-24 07:43:03.339 UTC [endorser.go:156] [endorser] callChaincode -> DEBU 564 Exit
2018-02-24 07:43:03.339 UTC [lockbased_tx_simulator.go:72] [lockbasedtxmgr] GetTxSimulationResults -> DEBU 565 Simulation completed, getting simulation results
2018-02-24 07:43:03.339 UTC [lockbased_query_executer.go:62] [lockbasedtxmgr] Done -> DEBU 566 Done with transaction simulation / query execution [23f8d1e0-df23-4df1-93c9-22322087a036]
2018-02-24 07:43:03.339 UTC [endorser.go:265] [endorser] simulateProposal -> DEBU 567 Exit
2018-02-24 07:43:03.339 UTC [endorser.go:500] [endorser] ProcessProposal -> DEBU 568 [gerry] 2 -- endorse and get a marshalled ProposalResponse message
2018-02-24 07:43:03.339 UTC [endorser.go:279] [endorser] endorseProposal -> DEBU 569 Entry - txid: d715b4d1c0c45f9d0ace149349a29961d97d5462e2ac0d8cf86c21a9faaca69f channel id: mychannel chaincode id: name:"fabcar" 
2018-02-24 07:43:03.339 UTC [endorser.go:282] [endorser] endorseProposal -> DEBU 56a [gerry] *ccprovider.ChaincodeData: name:"fabcar" version:"1.0" escc:"escc" vscc:"vscc" policy:"\022\014\022\n\010\002\022\002\010\000\022\002\010\001\032\013\022\t\n\007Org1MSP\032\013\022\t\n\007Org2MSP" data:"\n n\231\207\243{A\374\345\227\302\253\245 oA\261\\]A\247\233\253T=\312\312\212\240\034\367\027\343\022 \255\351\262)\230hJ\315\037\\\326v\374~2,*\242\365\275\255\361\261@\241\341\307\365v0\026\035" id:"\375\265\206\375\214=\206\2232\300)\240Iw1E\013\036\260\256&\267\335\342\341\344\001\361\010e8^" instantiation_policy:"\022\014\022\n\010\001\022\002\010\000\022\002\010\001\032\r\022\013\n\007Org1MSP\020\001\032\r\022\013\n\007Org2MSP\020\001" 
2018-02-24 07:43:03.339 UTC [endorser.go:299] [endorser] endorseProposal -> DEBU 56b info: escc for chaincode id name:"fabcar"  is escc
2018-02-24 07:43:03.339 UTC [endorser.go:99] [endorser] callChaincode -> DEBU 56c Entry - txid: d715b4d1c0c45f9d0ace149349a29961d97d5462e2ac0d8cf86c21a9faaca69f channel id: mychannel version: 1.0.5
2018-02-24 07:43:03.339 UTC [ccprovider.go:363] [ccprovider] NewCCContext -> DEBU 56d NewCCCC (chain=mychannel,chaincode=escc,version=1.0.5,txid=d715b4d1c0c45f9d0ace149349a29961d97d5462e2ac0d8cf86c21a9faaca69f,syscc=true,proposal=0xc421ff8dc0,canname=escc:1.0.5
2018-02-24 07:43:03.339 UTC [chaincode_support.go:585] [chaincode] Launch -> DEBU 56e chaincode is running(no need to launch) : escc:1.0.5
2018-02-24 07:43:03.339 UTC [chaincode_support.go:720] [chaincode] Execute -> DEBU 56f Entry
2018-02-24 07:43:03.339 UTC [chaincode_support.go:723] [chaincode] Execute -> DEBU 570 chaincode canonical name: escc:1.0.5
2018-02-24 07:43:03.339 UTC [handler.go:1436] [chaincode] sendExecuteMessage -> DEBU 571 [d715b4d1]Inside sendExecuteMessage. Message TRANSACTION
2018-02-24 07:43:03.339 UTC [handler.go:1350] [chaincode] setChaincodeProposal -> DEBU 572 Setting chaincode proposal context...
2018-02-24 07:43:03.339 UTC [handler.go:1352] [chaincode] setChaincodeProposal -> DEBU 573 Proposal different from nil. Creating chaincode proposal context...
2018-02-24 07:43:03.339 UTC [handler.go:1444] [chaincode] sendExecuteMessage -> DEBU 574 [d715b4d1]sendExecuteMsg trigger event TRANSACTION
2018-02-24 07:43:03.339 UTC [handler.go:357] [chaincode] processStream -> DEBU 575 [d715b4d1]Move state message TRANSACTION
2018-02-24 07:43:03.339 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 576 [d715b4d1]Fabric side Handling ChaincodeMessage of type: TRANSACTION in state ready
2018-02-24 07:43:03.340 UTC [handler.go:1417] [chaincode] filterError -> DEBU 577 Ignoring NoTransitionError: no transition
2018-02-24 07:43:03.340 UTC [handler.go:377] [chaincode] processStream -> DEBU 578 [d715b4d1]sending state message TRANSACTION
2018-02-24 07:43:03.340 UTC [chaincode.go:297] [shim] func1 -> DEBU 579 [d715b4d1]Received message TRANSACTION from shim
2018-02-24 07:43:03.340 UTC [handler.go:778] [shim] handleMessage -> DEBU 57a [d715b4d1]Handling ChaincodeMessage of type: TRANSACTION(state:ready)
2018-02-24 07:43:03.340 UTC [handler.go:338] [shim] beforeTransaction -> DEBU 57b [d715b4d1]Received TRANSACTION, invoking transaction on chaincode(Src:ready, Dst:ready)
2018-02-24 07:43:03.340 UTC [endorser_onevalidsignature.go:74] [escc] Invoke -> DEBU 57c ESCC starts: 8 args
2018-02-24 07:43:03.340 UTC [endorser_onevalidsignature.go:177] [escc] Invoke -> DEBU 57d ESCC exits successfully
2018-02-24 07:43:03.340 UTC [handler.go:326] [shim] func1 -> DEBU 57e [d715b4d1]Transaction completed. Sending COMPLETED
2018-02-24 07:43:03.340 UTC [chaincode.go:304] [shim] func1 -> DEBU 57f [d715b4d1]Move state message COMPLETED
2018-02-24 07:43:03.340 UTC [handler.go:778] [shim] handleMessage -> DEBU 580 [d715b4d1]Handling ChaincodeMessage of type: COMPLETED(state:ready)
2018-02-24 07:43:03.340 UTC [chaincode.go:320] [shim] func1 -> DEBU 581 [d715b4d1]send state message COMPLETED
2018-02-24 07:43:03.340 UTC [handler.go:336] [chaincode] processStream -> DEBU 582 [d715b4d1]Received message COMPLETED from shim
2018-02-24 07:43:03.340 UTC [handler.go:1389] [chaincode] HandleMessage -> DEBU 583 [d715b4d1]Fabric side Handling ChaincodeMessage of type: COMPLETED in state ready
2018-02-24 07:43:03.340 UTC [handler.go:1392] [chaincode] HandleMessage -> DEBU 584 [d715b4d1c0c45f9d0ace149349a29961d97d5462e2ac0d8cf86c21a9faaca69f]HandleMessage- COMPLETED. Notify
2018-02-24 07:43:03.340 UTC [handler.go:551] [chaincode] notify -> DEBU 585 notifying Txid:d715b4d1c0c45f9d0ace149349a29961d97d5462e2ac0d8cf86c21a9faaca69f
2018-02-24 07:43:03.340 UTC [chaincode_support.go:751] [chaincode] Execute -> DEBU 586 Exit
2018-02-24 07:43:03.340 UTC [endorser.go:156] [endorser] callChaincode -> DEBU 587 Exit
2018-02-24 07:43:03.340 UTC [endorser.go:368] [endorser] endorseProposal -> DEBU 588 Exit
2018-02-24 07:43:03.340 UTC [endorser.go:523] [endorser] ProcessProposal -> DEBU 589 go here txid: d715b4d1c0c45f9d0ace149349a29961d97d5462e2ac0d8cf86c21a9faaca69f
2018-02-24 07:43:03.341 UTC [lockbased_query_executer.go:62] [lockbasedtxmgr] Done -> DEBU 58a Done with transaction simulation / query execution [23f8d1e0-df23-4df1-93c9-22322087a036]
2018-02-24 07:43:03.341 UTC [endorser.go:530] [endorser] ProcessProposal -> DEBU 58b Exit
2018-02-24 07:43:05.381 UTC [blocksprovider.go:184] [blocksProvider] DeliverBlocks -> DEBU 58c [mychannel] Adding payload locally, buffer seqNum = [3], peers number [1]
2018-02-24 07:43:05.381 UTC [blocksprovider.go:191] [blocksProvider] DeliverBlocks -> DEBU 58d [mychannel] Gossiping block [3], peers number [1]
2018-02-24 07:43:05.381 UTC [committer_impl.go:73] [committer] Commit -> DEBU 58e Validating block
2018-02-24 07:43:05.381 UTC [validator.go:146] [txvalidator] Validate -> DEBU 58f START Block Validation
2018-02-24 07:43:05.381 UTC [validator.go:165] [txvalidator] Validate -> DEBU 590 Validating transaction peer.ValidateTransaction()
2018-02-24 07:43:05.381 UTC [msgvalidation.go:362] [protoutils] ValidateTransaction -> DEBU 591 ValidateTransactionEnvelope starts for envelope 0xc422483e60
2018-02-24 07:43:05.381 UTC [msgvalidation.go:377] [protoutils] ValidateTransaction -> DEBU 592 Header is channel_header:"\010\003\032\014\010\207\257\304\324\005\020\322\213\205\231\001\"\tmychannel*@d715b4d1c0c45f9d0ace149349a29961d97d5462e2ac0d8cf86c21a9faaca69f:\n\022\010\022\006fabcar" signature_header:"\n\214\006\n\007Org1MSP\022\200\006-----BEGIN -----\nMIICGTCCAb+gAwIBAgIQU2EEaekZq1LsI6bZEdL9rzAKBggqhkjOPQQDAjBzMQsw\nCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy\nYW5jaXNjbzEZMBcGA1UEChMQb3JnMS5leGFtcGxlLmNvbTEcMBoGA1UEAxMTY2Eu\nb3JnMS5leGFtcGxlLmNvbTAeFw0xODAyMDcwMDE1MTRaFw0yODAyMDUwMDE1MTRa\nMFsxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1T\nYW4gRnJhbmNpc2NvMR8wHQYDVQQDDBZBZG1pbkBvcmcxLmV4YW1wbGUuY29tMFkw\nEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE01kUw4VY/YsrMtL8eZxZchnBuzZNPOfj\nJN97M78wxDGWVVnHDhrMFemCJ0r3MLJw2J/AJ869y9HdUrDWX0lo+6NNMEswDgYD\nVR0PAQH/BAQDAgeAMAwGA1UdEwEB/wQCMAAwKwYDVR0jBCQwIoAgT8YQQJTfOVXI\nEk6JW0JtqfNLN5Zs9iPXOc3LEMJGYE0wCgYIKoZIzj0EAwIDSAAwRQIhAJpvtr6J\nEKkSikx802ESGmwdxejmU2HT4vjySUluYCS7AiBAO7bP3X2sIFYCF/A94bBjfcQR\nXNvYJqPJ+Y/zLYo2PQ==\n-----END -----\n\022\030\002\267\370\363\273\240\324\241\032Y\004SA\0232\256a\031\212\276\230\014\332\344" 
2018-02-24 07:43:05.382 UTC [msgvalidation.go:215] [protoutils] validateChannelHeader -> DEBU 593 validateChannelHeader info: header type 3
2018-02-24 07:43:05.382 UTC [msgvalidation.go:142] [protoutils] checkSignatureFromCreator -> DEBU 594 checkSignatureFromCreator starts
2018-02-24 07:43:05.382 UTC [msgvalidation.go:160] [protoutils] checkSignatureFromCreator -> DEBU 595 checkSignatureFromCreator info: creator is &{Org1MSP 3dc70c07d3096e889a28bca219d07b0d858baede9b7d60760c9bf5a76017c9a1}
2018-02-24 07:43:05.384 UTC [msgvalidation.go:168] [protoutils] checkSignatureFromCreator -> DEBU 596 checkSignatureFromCreator info: creator is valid
2018-02-24 07:43:05.384 UTC [msgvalidation.go:176] [protoutils] checkSignatureFromCreator -> DEBU 597 checkSignatureFromCreator exists successfully
2018-02-24 07:43:05.385 UTC [msgvalidation.go:279] [protoutils] validateEndorserTransaction -> DEBU 598 validateEndorserTransaction starts for data 0xc4220ea000, header channel_header:"\010\003\032\014\010\207\257\304\324\005\020\322\213\205\231\001\"\tmychannel*@d715b4d1c0c45f9d0ace149349a29961d97d5462e2ac0d8cf86c21a9faaca69f:\n\022\010\022\006fabcar" signature_header:"\n\214\006\n\007Org1MSP\022\200\006-----BEGIN -----\nMIICGTCCAb+gAwIBAgIQU2EEaekZq1LsI6bZEdL9rzAKBggqhkjOPQQDAjBzMQsw\nCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy\nYW5jaXNjbzEZMBcGA1UEChMQb3JnMS5leGFtcGxlLmNvbTEcMBoGA1UEAxMTY2Eu\nb3JnMS5leGFtcGxlLmNvbTAeFw0xODAyMDcwMDE1MTRaFw0yODAyMDUwMDE1MTRa\nMFsxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1T\nYW4gRnJhbmNpc2NvMR8wHQYDVQQDDBZBZG1pbkBvcmcxLmV4YW1wbGUuY29tMFkw\nEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE01kUw4VY/YsrMtL8eZxZchnBuzZNPOfj\nJN97M78wxDGWVVnHDhrMFemCJ0r3MLJw2J/AJ869y9HdUrDWX0lo+6NNMEswDgYD\nVR0PAQH/BAQDAgeAMAwGA1UdEwEB/wQCMAAwKwYDVR0jBCQwIoAgT8YQQJTfOVXI\nEk6JW0JtqfNLN5Zs9iPXOc3LEMJGYE0wCgYIKoZIzj0EAwIDSAAwRQIhAJpvtr6J\nEKkSikx802ESGmwdxejmU2HT4vjySUluYCS7AiBAO7bP3X2sIFYCF/A94bBjfcQR\nXNvYJqPJ+Y/zLYo2PQ==\n-----END -----\n\022\030\002\267\370\363\273\240\324\241\032Y\004SA\0232\256a\031\212\276\230\014\332\344" 
2018-02-24 07:43:05.385 UTC [msgvalidation.go:306] [protoutils] validateEndorserTransaction -> DEBU 599 validateEndorserTransaction info: there are 1 actions
2018-02-24 07:43:05.385 UTC [msgvalidation.go:327] [protoutils] validateEndorserTransaction -> DEBU 59a validateEndorserTransaction info: signature header is valid
2018-02-24 07:43:05.385 UTC [msgvalidation.go:412] [protoutils] ValidateTransaction -> DEBU 59b ValidateTransactionEnvelope returns err %!s(<nil>)
2018-02-24 07:43:05.386 UTC [validator.go:184] [txvalidator] Validate -> DEBU 59c Transaction is for chain mychannel
2018-02-24 07:43:05.386 UTC [blockfile_mgr.go:506] [fsblkstorage] retrieveTransactionByID -> DEBU 59d retrieveTransactionByID() - txId = [d715b4d1c0c45f9d0ace149349a29961d97d5462e2ac0d8cf86c21a9faaca69f]
2018-02-24 07:43:05.386 UTC [validator.go:202] [txvalidator] Validate -> DEBU 59e Validating transaction vscc tx validate
2018-02-24 07:43:05.386 UTC [validator.go:271] [txvalidator] Validate -> DEBU 59f END Block Validation
2018-02-24 07:43:05.386 UTC [kv_ledger.go:215] [kvledger] Commit -> DEBU 5a0 Channel [mychannel]: Validating block [3]
2018-02-24 07:43:05.386 UTC [lockbased_txmgr.go:72] [lockbasedtxmgr] ValidateAndPrepare -> DEBU 5a1 Validating new block with num trans = [1]
2018-02-24 07:43:05.386 UTC [state_based_validator.go:78] [statevalidator] ValidateAndPrepareBatch -> DEBU 5a2 New block arrived for validation:&common.Block{Header:(*common.BlockHeader)(0xc421c7e840), Data:(*common.BlockData)(0xc421c864a0), Metadata:(*common.BlockMetadata)(0xc421c864e0)}, doMVCCValidation=true
2018-02-24 07:43:05.386 UTC [state_based_validator.go:80] [statevalidator] ValidateAndPrepareBatch -> DEBU 5a3 Validating a block with [1] transactions
2018-02-24 07:43:05.386 UTC [stateleveldb.go:89] [stateleveldb] GetState -> DEBU 5a4 GetState(). ns=fabcar, key=CAR100
2018-02-24 07:43:05.386 UTC [stateleveldb.go:89] [stateleveldb] GetState -> DEBU 5a5 GetState(). ns=lscc, key=fabcar
2018-02-24 07:43:05.386 UTC [util.go:58] [kvledger.util] DecodeOrderPreservingVarUint64 -> DEBU 5a6 proto.DecodeVarint(bytes):1
2018-02-24 07:43:05.386 UTC [util.go:58] [kvledger.util] DecodeOrderPreservingVarUint64 -> DEBU 5a7 proto.DecodeVarint(bytes):0
2018-02-24 07:43:05.387 UTC [state_based_validator.go:139] [statevalidator] ValidateAndPrepareBatch -> DEBU 5a8 Block [3] Transaction index [0] TxId [d715b4d1c0c45f9d0ace149349a29961d97d5462e2ac0d8cf86c21a9faaca69f] marked as valid by state validator
2018-02-24 07:43:05.387 UTC [kv_ledger.go:221] [kvledger] Commit -> DEBU 5a9 Channel [mychannel]: Committing block [3] to storage
2018-02-24 07:43:05.408 UTC [blockindex.go:98] [fsblkstorage] indexBlock -> DEBU 5aa Indexing block [blockNum=3, blockHash=[]byte{0xec, 0x3a, 0x8f, 0xc9, 0x5d, 0xcd, 0x43, 0x93, 0x83, 0xb1, 0x45, 0xb0, 0x90, 0xba, 0x65, 0xc2, 0xfe, 0x61, 0x30, 0x26, 0xd7, 0x89, 0xc0, 0x77, 0xfc, 0xdf, 0x38, 0xd6, 0x99, 0x2c, 0x24, 0x7a} txOffsets=
txId=d715b4d1c0c45f9d0ace149349a29961d97d5462e2ac0d8cf86c21a9faaca69f locPointer=offset=70, bytesLength=2825
]
2018-02-24 07:43:05.408 UTC [blockindex.go:122] [fsblkstorage] indexBlock -> DEBU 5ab Adding txLoc [fileSuffixNum=0, offset=21896, bytesLength=2825] for tx ID: [d715b4d1c0c45f9d0ace149349a29961d97d5462e2ac0d8cf86c21a9faaca69f] to index
2018-02-24 07:43:05.408 UTC [blockindex.go:135] [fsblkstorage] indexBlock -> DEBU 5ac Adding txLoc [fileSuffixNum=0, offset=21896, bytesLength=2825] for tx number:[0] ID: [d715b4d1c0c45f9d0ace149349a29961d97d5462e2ac0d8cf86c21a9faaca69f] to blockNumTranNum index
2018-02-24 07:43:05.417 UTC [blockfile_mgr.go:430] [fsblkstorage] updateCheckpoint -> DEBU 5ad Broadcasting about update checkpointInfo: latestFileChunkSuffixNum=[0], latestFileChunksize=[26479], isChainEmpty=[false], lastBlockNumber=[3]
2018-02-24 07:43:05.417 UTC [kv_ledger.go:225] [kvledger] Commit -> INFO 5ae Channel [mychannel]: Created block [3] with 1 transaction(s)
2018-02-24 07:43:05.417 UTC [kv_ledger.go:227] [kvledger] Commit -> DEBU 5af Channel [mychannel]: Committing block [3] transactions to state database
2018-02-24 07:43:05.418 UTC [lockbased_txmgr.go:89] [lockbasedtxmgr] Commit -> DEBU 5b0 Committing updates to state database
2018-02-24 07:43:05.418 UTC [lockbased_txmgr.go:92] [lockbasedtxmgr] Commit -> DEBU 5b1 Write lock acquired for committing updates to state database
2018-02-24 07:43:05.418 UTC [lockbased_txmgr.go:101] [lockbasedtxmgr] Commit -> DEBU 5b2 Updates committed to state database
2018-02-24 07:43:05.418 UTC [kv_ledger.go:234] [kvledger] Commit -> DEBU 5b3 Channel [mychannel]: Committing block [3] transactions to history database
2018-02-24 07:43:05.418 UTC [historyleveldb.go:93] [historyleveldb] Commit -> DEBU 5b4 Channel [mychannel]: Updating history database for blockNo [3] with [1] transactions
2018-02-24 07:43:05.419 UTC [historyleveldb.go:177] [historyleveldb] Commit -> DEBU 5b5 Channel [mychannel]: Updates committed to history database for blockNo [3]
2018-02-24 07:43:05.419 UTC [eventhelper.go:29] [eventhub_producer] SendProducerBlockEvent -> DEBU 5b6 Entry
2018-02-24 07:43:05.419 UTC [eventhelper.go:55] [eventhub_producer] SendProducerBlockEvent -> DEBU 5b7 Channel [mychannel]: Block event for block number [3] contains transaction id: d715b4d1c0c45f9d0ace149349a29961d97d5462e2ac0d8cf86c21a9faaca69f
2018-02-24 07:43:05.419 UTC [eventhelper.go:105] [eventhub_producer] SendProducerBlockEvent -> INFO 5b8 Channel [mychannel]: Sending event for block number [3]
2018-02-24 07:43:05.419 UTC [events.go:312] [eventhub_producer] Send -> DEBU 5b9 Entry
2018-02-24 07:43:05.419 UTC [events.go:335] [eventhub_producer] Send -> DEBU 5ba Event processor timeout > 0
2018-02-24 07:43:05.419 UTC [events.go:343] [eventhub_producer] Send -> DEBU 5bb Event sent successfully
2018-02-24 07:43:05.419 UTC [events.go:344] [eventhub_producer] Send -> DEBU 5bc Exit
2018-02-24 07:43:05.419 UTC [eventhelper.go:107] [eventhub_producer] SendProducerBlockEvent -> DEBU 5bd Exit
2018-02-24 07:43:05.419 UTC [register_internal_events.go:41] [eventhub_producer] getMessageType -> DEBU 5be [gerry] Event_Block
```

### query (同步peer 日志)

```
2018-02-24 07:43:05.372 UTC [blocksprovider.go:184] [blocksProvider] DeliverBlocks -> DEBU 3e2 [mychannel] Adding payload locally, buffer seqNum = [3], peers number [0]
2018-02-24 07:43:05.372 UTC [blocksprovider.go:191] [blocksProvider] DeliverBlocks -> DEBU 3e3 [mychannel] Gossiping block [3], peers number [0]
2018-02-24 07:43:05.372 UTC [committer_impl.go:73] [committer] Commit -> DEBU 3e4 Validating block
2018-02-24 07:43:05.372 UTC [validator.go:146] [txvalidator] Validate -> DEBU 3e5 START Block Validation
2018-02-24 07:43:05.373 UTC [validator.go:165] [txvalidator] Validate -> DEBU 3e6 Validating transaction peer.ValidateTransaction()
2018-02-24 07:43:05.373 UTC [msgvalidation.go:362] [protoutils] ValidateTransaction -> DEBU 3e7 ValidateTransactionEnvelope starts for envelope 0xc4218fd0b0
2018-02-24 07:43:05.373 UTC [msgvalidation.go:377] [protoutils] ValidateTransaction -> DEBU 3e8 Header is channel_header:"\010\003\032\014\010\207\257\304\324\005\020\322\213\205\231\001\"\tmychannel*@d715b4d1c0c45f9d0ace149349a29961d97d5462e2ac0d8cf86c21a9faaca69f:\n\022\010\022\006fabcar" signature_header:"\n\214\006\n\007Org1MSP\022\200\006-----BEGIN -----\nMIICGTCCAb+gAwIBAgIQU2EEaekZq1LsI6bZEdL9rzAKBggqhkjOPQQDAjBzMQsw\nCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy\nYW5jaXNjbzEZMBcGA1UEChMQb3JnMS5leGFtcGxlLmNvbTEcMBoGA1UEAxMTY2Eu\nb3JnMS5leGFtcGxlLmNvbTAeFw0xODAyMDcwMDE1MTRaFw0yODAyMDUwMDE1MTRa\nMFsxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1T\nYW4gRnJhbmNpc2NvMR8wHQYDVQQDDBZBZG1pbkBvcmcxLmV4YW1wbGUuY29tMFkw\nEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE01kUw4VY/YsrMtL8eZxZchnBuzZNPOfj\nJN97M78wxDGWVVnHDhrMFemCJ0r3MLJw2J/AJ869y9HdUrDWX0lo+6NNMEswDgYD\nVR0PAQH/BAQDAgeAMAwGA1UdEwEB/wQCMAAwKwYDVR0jBCQwIoAgT8YQQJTfOVXI\nEk6JW0JtqfNLN5Zs9iPXOc3LEMJGYE0wCgYIKoZIzj0EAwIDSAAwRQIhAJpvtr6J\nEKkSikx802ESGmwdxejmU2HT4vjySUluYCS7AiBAO7bP3X2sIFYCF/A94bBjfcQR\nXNvYJqPJ+Y/zLYo2PQ==\n-----END -----\n\022\030\002\267\370\363\273\240\324\241\032Y\004SA\0232\256a\031\212\276\230\014\332\344" 
2018-02-24 07:43:05.373 UTC [msgvalidation.go:215] [protoutils] validateChannelHeader -> DEBU 3e9 validateChannelHeader info: header type 3
2018-02-24 07:43:05.373 UTC [msgvalidation.go:142] [protoutils] checkSignatureFromCreator -> DEBU 3ea checkSignatureFromCreator starts
2018-02-24 07:43:05.374 UTC [msgvalidation.go:160] [protoutils] checkSignatureFromCreator -> DEBU 3eb checkSignatureFromCreator info: creator is &{Org1MSP 3dc70c07d3096e889a28bca219d07b0d858baede9b7d60760c9bf5a76017c9a1}
2018-02-24 07:43:05.379 UTC [msgvalidation.go:168] [protoutils] checkSignatureFromCreator -> DEBU 3ec checkSignatureFromCreator info: creator is valid
2018-02-24 07:43:05.383 UTC [msgvalidation.go:176] [protoutils] checkSignatureFromCreator -> DEBU 3ed checkSignatureFromCreator exists successfully
2018-02-24 07:43:05.383 UTC [msgvalidation.go:279] [protoutils] validateEndorserTransaction -> DEBU 3ee validateEndorserTransaction starts for data 0xc421445800, header channel_header:"\010\003\032\014\010\207\257\304\324\005\020\322\213\205\231\001\"\tmychannel*@d715b4d1c0c45f9d0ace149349a29961d97d5462e2ac0d8cf86c21a9faaca69f:\n\022\010\022\006fabcar" signature_header:"\n\214\006\n\007Org1MSP\022\200\006-----BEGIN -----\nMIICGTCCAb+gAwIBAgIQU2EEaekZq1LsI6bZEdL9rzAKBggqhkjOPQQDAjBzMQsw\nCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy\nYW5jaXNjbzEZMBcGA1UEChMQb3JnMS5leGFtcGxlLmNvbTEcMBoGA1UEAxMTY2Eu\nb3JnMS5leGFtcGxlLmNvbTAeFw0xODAyMDcwMDE1MTRaFw0yODAyMDUwMDE1MTRa\nMFsxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1T\nYW4gRnJhbmNpc2NvMR8wHQYDVQQDDBZBZG1pbkBvcmcxLmV4YW1wbGUuY29tMFkw\nEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE01kUw4VY/YsrMtL8eZxZchnBuzZNPOfj\nJN97M78wxDGWVVnHDhrMFemCJ0r3MLJw2J/AJ869y9HdUrDWX0lo+6NNMEswDgYD\nVR0PAQH/BAQDAgeAMAwGA1UdEwEB/wQCMAAwKwYDVR0jBCQwIoAgT8YQQJTfOVXI\nEk6JW0JtqfNLN5Zs9iPXOc3LEMJGYE0wCgYIKoZIzj0EAwIDSAAwRQIhAJpvtr6J\nEKkSikx802ESGmwdxejmU2HT4vjySUluYCS7AiBAO7bP3X2sIFYCF/A94bBjfcQR\nXNvYJqPJ+Y/zLYo2PQ==\n-----END -----\n\022\030\002\267\370\363\273\240\324\241\032Y\004SA\0232\256a\031\212\276\230\014\332\344" 
2018-02-24 07:43:05.383 UTC [msgvalidation.go:306] [protoutils] validateEndorserTransaction -> DEBU 3ef validateEndorserTransaction info: there are 1 actions
2018-02-24 07:43:05.383 UTC [msgvalidation.go:327] [protoutils] validateEndorserTransaction -> DEBU 3f0 validateEndorserTransaction info: signature header is valid
2018-02-24 07:43:05.384 UTC [msgvalidation.go:412] [protoutils] ValidateTransaction -> DEBU 3f1 ValidateTransactionEnvelope returns err %!s(<nil>)
2018-02-24 07:43:05.384 UTC [validator.go:184] [txvalidator] Validate -> DEBU 3f2 Transaction is for chain mychannel
2018-02-24 07:43:05.384 UTC [blockfile_mgr.go:506] [fsblkstorage] retrieveTransactionByID -> DEBU 3f3 retrieveTransactionByID() - txId = [d715b4d1c0c45f9d0ace149349a29961d97d5462e2ac0d8cf86c21a9faaca69f]
2018-02-24 07:43:05.384 UTC [validator.go:202] [txvalidator] Validate -> DEBU 3f4 Validating transaction vscc tx validate
2018-02-24 07:43:05.384 UTC [validator.go:271] [txvalidator] Validate -> DEBU 3f5 END Block Validation
2018-02-24 07:43:05.385 UTC [kv_ledger.go:215] [kvledger] Commit -> DEBU 3f6 Channel [mychannel]: Validating block [3]
2018-02-24 07:43:05.385 UTC [lockbased_txmgr.go:72] [lockbasedtxmgr] ValidateAndPrepare -> DEBU 3f7 Validating new block with num trans = [1]
2018-02-24 07:43:05.385 UTC [state_based_validator.go:78] [statevalidator] ValidateAndPrepareBatch -> DEBU 3f8 New block arrived for validation:&common.Block{Header:(*common.BlockHeader)(0xc421880080), Data:(*common.BlockData)(0xc4217bc680), Metadata:(*common.BlockMetadata)(0xc4217bc6c0)}, doMVCCValidation=true
2018-02-24 07:43:05.385 UTC [state_based_validator.go:80] [statevalidator] ValidateAndPrepareBatch -> DEBU 3f9 Validating a block with [1] transactions
2018-02-24 07:43:05.385 UTC [stateleveldb.go:89] [stateleveldb] GetState -> DEBU 3fa GetState(). ns=fabcar, key=CAR100
2018-02-24 07:43:05.385 UTC [stateleveldb.go:89] [stateleveldb] GetState -> DEBU 3fb GetState(). ns=lscc, key=fabcar
2018-02-24 07:43:05.385 UTC [util.go:58] [kvledger.util] DecodeOrderPreservingVarUint64 -> DEBU 3fc proto.DecodeVarint(bytes):1
2018-02-24 07:43:05.385 UTC [util.go:58] [kvledger.util] DecodeOrderPreservingVarUint64 -> DEBU 3fd proto.DecodeVarint(bytes):0
2018-02-24 07:43:05.386 UTC [state_based_validator.go:139] [statevalidator] ValidateAndPrepareBatch -> DEBU 3fe Block [3] Transaction index [0] TxId [d715b4d1c0c45f9d0ace149349a29961d97d5462e2ac0d8cf86c21a9faaca69f] marked as valid by state validator
2018-02-24 07:43:05.386 UTC [kv_ledger.go:221] [kvledger] Commit -> DEBU 3ff Channel [mychannel]: Committing block [3] to storage
2018-02-24 07:43:05.387 UTC [blockindex.go:98] [fsblkstorage] indexBlock -> DEBU 400 Indexing block [blockNum=3, blockHash=[]byte{0xec, 0x3a, 0x8f, 0xc9, 0x5d, 0xcd, 0x43, 0x93, 0x83, 0xb1, 0x45, 0xb0, 0x90, 0xba, 0x65, 0xc2, 0xfe, 0x61, 0x30, 0x26, 0xd7, 0x89, 0xc0, 0x77, 0xfc, 0xdf, 0x38, 0xd6, 0x99, 0x2c, 0x24, 0x7a} txOffsets=
txId=d715b4d1c0c45f9d0ace149349a29961d97d5462e2ac0d8cf86c21a9faaca69f locPointer=offset=70, bytesLength=2825
]
2018-02-24 07:43:05.387 UTC [blockindex.go:122] [fsblkstorage] indexBlock -> DEBU 401 Adding txLoc [fileSuffixNum=0, offset=21896, bytesLength=2825] for tx ID: [d715b4d1c0c45f9d0ace149349a29961d97d5462e2ac0d8cf86c21a9faaca69f] to index
2018-02-24 07:43:05.387 UTC [blockindex.go:135] [fsblkstorage] indexBlock -> DEBU 402 Adding txLoc [fileSuffixNum=0, offset=21896, bytesLength=2825] for tx number:[0] ID: [d715b4d1c0c45f9d0ace149349a29961d97d5462e2ac0d8cf86c21a9faaca69f] to blockNumTranNum index
2018-02-24 07:43:05.394 UTC [blockfile_mgr.go:430] [fsblkstorage] updateCheckpoint -> DEBU 403 Broadcasting about update checkpointInfo: latestFileChunkSuffixNum=[0], latestFileChunksize=[26479], isChainEmpty=[false], lastBlockNumber=[3]
2018-02-24 07:43:05.394 UTC [kv_ledger.go:225] [kvledger] Commit -> INFO 404 Channel [mychannel]: Created block [3] with 1 transaction(s)
2018-02-24 07:43:05.395 UTC [kv_ledger.go:227] [kvledger] Commit -> DEBU 405 Channel [mychannel]: Committing block [3] transactions to state database
2018-02-24 07:43:05.395 UTC [lockbased_txmgr.go:89] [lockbasedtxmgr] Commit -> DEBU 406 Committing updates to state database
2018-02-24 07:43:05.395 UTC [lockbased_txmgr.go:92] [lockbasedtxmgr] Commit -> DEBU 407 Write lock acquired for committing updates to state database
2018-02-24 07:43:05.396 UTC [lockbased_txmgr.go:101] [lockbasedtxmgr] Commit -> DEBU 408 Updates committed to state database
2018-02-24 07:43:05.396 UTC [kv_ledger.go:234] [kvledger] Commit -> DEBU 409 Channel [mychannel]: Committing block [3] transactions to history database
2018-02-24 07:43:05.396 UTC [historyleveldb.go:93] [historyleveldb] Commit -> DEBU 40a Channel [mychannel]: Updating history database for blockNo [3] with [1] transactions
2018-02-24 07:43:05.398 UTC [historyleveldb.go:177] [historyleveldb] Commit -> DEBU 40b Channel [mychannel]: Updates committed to history database for blockNo [3]
2018-02-24 07:43:05.398 UTC [eventhelper.go:29] [eventhub_producer] SendProducerBlockEvent -> DEBU 40c Entry
2018-02-24 07:43:05.398 UTC [eventhelper.go:55] [eventhub_producer] SendProducerBlockEvent -> DEBU 40d Channel [mychannel]: Block event for block number [3] contains transaction id: d715b4d1c0c45f9d0ace149349a29961d97d5462e2ac0d8cf86c21a9faaca69f
2018-02-24 07:43:05.398 UTC [eventhelper.go:105] [eventhub_producer] SendProducerBlockEvent -> INFO 40e Channel [mychannel]: Sending event for block number [3]
2018-02-24 07:43:05.398 UTC [events.go:312] [eventhub_producer] Send -> DEBU 40f Entry
2018-02-24 07:43:05.398 UTC [events.go:335] [eventhub_producer] Send -> DEBU 410 Event processor timeout > 0
2018-02-24 07:43:05.398 UTC [events.go:343] [eventhub_producer] Send -> DEBU 411 Event sent successfully
2018-02-24 07:43:05.398 UTC [events.go:344] [eventhub_producer] Send -> DEBU 412 Exit
2018-02-24 07:43:05.398 UTC [eventhelper.go:107] [eventhub_producer] SendProducerBlockEvent -> DEBU 413 Exit
2018-02-24 07:43:05.398 UTC [register_internal_events.go:41] [eventhub_producer] getMessageType -> DEBU 414 [gerry] Event_Block
```

### query (order 日志)

```
2018-02-24 07:43:03.314 UTC [orderer/main] /opt/gopath/src/github.com/hyperledger/fabric/orderer/server.go:74 Broadcast -> DEBU 99f Starting new Broadcast handler
2018-02-24 07:43:03.318 UTC [orderer/common/broadcast] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/broadcast/broadcast.go:77 Handle -> DEBU 9a0 Starting new broadcast loop
2018-02-24 07:43:03.342 UTC [orderer/common/broadcast] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/broadcast/broadcast.go:138 Handle -> DEBU 9a1 [channel: mychannel] Broadcast is filtering message of type ENDORSER_TRANSACTION
2018-02-24 07:43:03.342 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 9a2 Returning policy Writers for evaluation
2018-02-24 07:43:03.342 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:58 func1 -> DEBU 9a3 0xc420a5c038 gate 1519458183342525734 evaluation starts
2018-02-24 07:43:03.342 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:83 func2 -> DEBU 9a4 0xc420a5c038 signed by 0 principal evaluation starts (used [false])
2018-02-24 07:43:03.342 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:91 func2 -> DEBU 9a5 0xc420a5c038 processing identity 0 with bytes of 0a074f7267314d53501280062d2d2d2d2d424547494e202d2d2d2d2d0a4d4949434754434341622b6741774942416749515532454561656b5a71314c734936625a45644c39727a414b42676771686b6a4f50515144416a427a4d5173770a435159445651514745774a56557a45544d4245474131554543424d4b5132467361575a76636d3570595445574d4251474131554542784d4e5532467549455a790a5957356a61584e6a627a455a4d4263474131554543684d5162334a6e4d53356c654746746347786c4c6d4e76625445634d426f474131554541784d54593245750a62334a6e4d53356c654746746347786c4c6d4e7662544165467730784f4441794d4463774d4445314d545261467730794f4441794d4455774d4445314d5452610a4d467378437a414a42674e5642415954416c56544d524d77455159445651514945777044595778705a6d3979626d6c684d5259774641594456515148457731540a59573467526e4a68626d4e7063324e764d523877485159445651514444425a425a473170626b4276636d63784c6d56345957317762475575593239744d466b770a457759484b6f5a497a6a3043415159494b6f5a497a6a3044415163445167414530316b55773456592f5973724d744c38655a785a63686e42757a5a4e504f666a0a4a4e39374d3738777844475756566e484468724d46656d434a3072334d4c4a77324a2f414a383639793948645572445758306c6f2b364e4e4d457377446759440a565230504151482f42415144416765414d41774741315564457745422f7751434d4141774b7759445652306a42435177496f416754385951514a54664f5658490a456b364a57304a7471664e4c4e355a73396950584f63334c454d4a4759453077436759494b6f5a497a6a3045417749445341417752514968414a70767472364a0a454b6b53696b783830324553476d776478656a6d5532485434766a7953556c7559435337416942414f3762503358327349465943462f41393462426a666351520a584e76594a71504a2b592f7a4c596f3250513d3d0a2d2d2d2d2d454e44202d2d2d2d2d0a
2018-02-24 07:43:03.342 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:42 newIdentity -> DEBU 9a6 Creating identity instance for ID -----BEGIN CERTIFICATE-----
MIICGTCCAb+gAwIBAgIQU2EEaekZq1LsI6bZEdL9rzAKBggqhkjOPQQDAjBzMQsw
CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy
YW5jaXNjbzEZMBcGA1UEChMQb3JnMS5leGFtcGxlLmNvbTEcMBoGA1UEAxMTY2Eu
b3JnMS5leGFtcGxlLmNvbTAeFw0xODAyMDcwMDE1MTRaFw0yODAyMDUwMDE1MTRa
MFsxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1T
YW4gRnJhbmNpc2NvMR8wHQYDVQQDDBZBZG1pbkBvcmcxLmV4YW1wbGUuY29tMFkw
EwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE01kUw4VY/YsrMtL8eZxZchnBuzZNPOfj
JN97M78wxDGWVVnHDhrMFemCJ0r3MLJw2J/AJ869y9HdUrDWX0lo+6NNMEswDgYD
VR0PAQH/BAQDAgeAMAwGA1UdEwEB/wQCMAAwKwYDVR0jBCQwIoAgT8YQQJTfOVXI
Ek6JW0JtqfNLN5Zs9iPXOc3LEMJGYE0wCgYIKoZIzj0EAwIDSAAwRQIhAJpvtr6J
EKkSikx802ESGmwdxejmU2HT4vjySUluYCS7AiBAO7bP3X2sIFYCF/A94bBjfcQR
XNvYJqPJ+Y/zLYo2PQ==
-----END CERTIFICATE-----
2018-02-24 07:43:03.343 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:489 SatisfiesPrincipal -> DEBU 9a7 Checking if identity satisfies MEMBER role for Org1MSP
2018-02-24 07:43:03.343 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:391 Validate -> DEBU 9a8 MSP Org1MSP validating identity
2018-02-24 07:43:03.343 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:103 func2 -> DEBU 9a9 0xc420a5c038 principal matched by identity 0
2018-02-24 07:43:03.343 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:129 Verify -> DEBU 9aa Verify: digest = 00000000  d4 30 f0 b9 57 6f eb dc  68 96 2a dd 2f 61 e6 a6  |.0..Wo..h.*./a..|
00000010  d3 ca 9f f4 ff 26 fb d6  71 ce 99 a4 0a 35 ce 38  |.....&..q....5.8|
2018-02-24 07:43:03.343 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:130 Verify -> DEBU 9ab Verify: sig = 00000000  30 44 02 20 7f f4 47 5b  48 24 61 d0 5e 62 ad 99  |0D. ..G[H$a.^b..|
00000010  2d dd 0f 45 78 f3 09 8f  4a 35 f5 9e 90 63 a5 3d  |-..Ex...J5...c.=|
00000020  fb 86 01 c3 02 20 2e 2e  89 c2 3e 2b a9 7c a8 91  |..... ....>+.|..|
00000030  3c 75 e7 7b 00 03 a7 8d  b2 a7 89 7f 58 11 d8 38  |<u.{........X..8|
00000040  28 1f a1 03 b5 2a                                 |(....*|
2018-02-24 07:43:03.343 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:109 func2 -> DEBU 9ac 0xc420a5c038 principal evaluation succeeds for identity 0
2018-02-24 07:43:03.343 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:70 func1 -> DEBU 9ad 0xc420a5c038 gate 1519458183342525734 evaluation succeeds
2018-02-24 07:43:03.343 UTC [orderer/common/sigfilter] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/sigfilter/sigfilter.go:69 Apply -> DEBU 9ae Forwarding validly signed message for policy &{%!s(*common.ImplicitMetaPolicy=&{Writers 0}) %!s(int=1) [%!s(*policies.implicitMetaPolicy=&{0xc4209ae200 1 [0xc420a5d120 0xc420a5d1e0]}) %!s(*policies.implicitMetaPolicy=&{0xc4209af160 1 [0xc420a5d258]})]}
2018-02-24 07:43:03.344 UTC [orderer/common/broadcast] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/broadcast/broadcast.go:153 Handle -> DEBU 9af [channel: mychannel] Broadcast has successfully enqueued message of type ENDORSER_TRANSACTION
2018-02-24 07:43:03.344 UTC [policies] /opt/gopath/src/github.com/hyperledger/fabric/common/policies/policy.go:226 GetPolicy -> DEBU 9b0 Returning policy Writers for evaluation
2018-02-24 07:43:03.349 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:58 func1 -> DEBU 9b1 0xc420a5c068 gate 1519458183349926008 evaluation starts
2018-02-24 07:43:03.350 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:83 func2 -> DEBU 9b2 0xc420a5c068 signed by 0 principal evaluation starts (used [false])
2018-02-24 07:43:03.350 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:91 func2 -> DEBU 9b3 0xc420a5c068 processing identity 0 with bytes of 0a074f7267314d53501280062d2d2d2d2d424547494e202d2d2d2d2d0a4d4949434754434341622b6741774942416749515532454561656b5a71314c734936625a45644c39727a414b42676771686b6a4f50515144416a427a4d5173770a435159445651514745774a56557a45544d4245474131554543424d4b5132467361575a76636d3570595445574d4251474131554542784d4e5532467549455a790a5957356a61584e6a627a455a4d4263474131554543684d5162334a6e4d53356c654746746347786c4c6d4e76625445634d426f474131554541784d54593245750a62334a6e4d53356c654746746347786c4c6d4e7662544165467730784f4441794d4463774d4445314d545261467730794f4441794d4455774d4445314d5452610a4d467378437a414a42674e5642415954416c56544d524d77455159445651514945777044595778705a6d3979626d6c684d5259774641594456515148457731540a59573467526e4a68626d4e7063324e764d523877485159445651514444425a425a473170626b4276636d63784c6d56345957317762475575593239744d466b770a457759484b6f5a497a6a3043415159494b6f5a497a6a3044415163445167414530316b55773456592f5973724d744c38655a785a63686e42757a5a4e504f666a0a4a4e39374d3738777844475756566e484468724d46656d434a3072334d4c4a77324a2f414a383639793948645572445758306c6f2b364e4e4d457377446759440a565230504151482f42415144416765414d41774741315564457745422f7751434d4141774b7759445652306a42435177496f416754385951514a54664f5658490a456b364a57304a7471664e4c4e355a73396950584f63334c454d4a4759453077436759494b6f5a497a6a3045417749445341417752514968414a70767472364a0a454b6b53696b783830324553476d776478656a6d5532485434766a7953556c7559435337416942414f3762503358327349465943462f41393462426a666351520a584e76594a71504a2b592f7a4c596f3250513d3d0a2d2d2d2d2d454e44202d2d2d2d2d0a
2018-02-24 07:43:03.350 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:42 newIdentity -> DEBU 9b4 Creating identity instance for ID -----BEGIN CERTIFICATE-----
MIICGTCCAb+gAwIBAgIQU2EEaekZq1LsI6bZEdL9rzAKBggqhkjOPQQDAjBzMQsw
CQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZy
YW5jaXNjbzEZMBcGA1UEChMQb3JnMS5leGFtcGxlLmNvbTEcMBoGA1UEAxMTY2Eu
b3JnMS5leGFtcGxlLmNvbTAeFw0xODAyMDcwMDE1MTRaFw0yODAyMDUwMDE1MTRa
MFsxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1T
YW4gRnJhbmNpc2NvMR8wHQYDVQQDDBZBZG1pbkBvcmcxLmV4YW1wbGUuY29tMFkw
EwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE01kUw4VY/YsrMtL8eZxZchnBuzZNPOfj
JN97M78wxDGWVVnHDhrMFemCJ0r3MLJw2J/AJ869y9HdUrDWX0lo+6NNMEswDgYD
VR0PAQH/BAQDAgeAMAwGA1UdEwEB/wQCMAAwKwYDVR0jBCQwIoAgT8YQQJTfOVXI
Ek6JW0JtqfNLN5Zs9iPXOc3LEMJGYE0wCgYIKoZIzj0EAwIDSAAwRQIhAJpvtr6J
EKkSikx802ESGmwdxejmU2HT4vjySUluYCS7AiBAO7bP3X2sIFYCF/A94bBjfcQR
XNvYJqPJ+Y/zLYo2PQ==
-----END CERTIFICATE-----
2018-02-24 07:43:03.356 UTC [orderer/common/broadcast] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/broadcast/broadcast.go:85 Handle -> WARN 9b5 Error reading from stream: rpc error: code = Canceled desc = context canceled
2018-02-24 07:43:03.356 UTC [orderer/main] /opt/gopath/src/github.com/hyperledger/fabric/orderer/server.go:79 func1 -> DEBU 9b6 Closing Broadcast stream
2018-02-24 07:43:03.356 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:489 SatisfiesPrincipal -> DEBU 9b7 Checking if identity satisfies MEMBER role for Org1MSP
2018-02-24 07:43:03.356 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:391 Validate -> DEBU 9b8 MSP Org1MSP validating identity
2018-02-24 07:43:03.357 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:103 func2 -> DEBU 9b9 0xc420a5c068 principal matched by identity 0
2018-02-24 07:43:03.357 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:129 Verify -> DEBU 9ba Verify: digest = 00000000  d4 30 f0 b9 57 6f eb dc  68 96 2a dd 2f 61 e6 a6  |.0..Wo..h.*./a..|
00000010  d3 ca 9f f4 ff 26 fb d6  71 ce 99 a4 0a 35 ce 38  |.....&..q....5.8|
2018-02-24 07:43:03.357 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:130 Verify -> DEBU 9bb Verify: sig = 00000000  30 44 02 20 7f f4 47 5b  48 24 61 d0 5e 62 ad 99  |0D. ..G[H$a.^b..|
00000010  2d dd 0f 45 78 f3 09 8f  4a 35 f5 9e 90 63 a5 3d  |-..Ex...J5...c.=|
00000020  fb 86 01 c3 02 20 2e 2e  89 c2 3e 2b a9 7c a8 91  |..... ....>+.|..|
00000030  3c 75 e7 7b 00 03 a7 8d  b2 a7 89 7f 58 11 d8 38  |<u.{........X..8|
00000040  28 1f a1 03 b5 2a                                 |(....*|
2018-02-24 07:43:03.357 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:109 func2 -> DEBU 9bc 0xc420a5c068 principal evaluation succeeds for identity 0
2018-02-24 07:43:03.357 UTC [cauthdsl] /opt/gopath/src/github.com/hyperledger/fabric/common/cauthdsl/cauthdsl.go:70 func1 -> DEBU 9bd 0xc420a5c068 gate 1519458183349926008 evaluation succeeds
2018-02-24 07:43:03.357 UTC [orderer/common/sigfilter] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/sigfilter/sigfilter.go:69 Apply -> DEBU 9be Forwarding validly signed message for policy &{%!s(*common.ImplicitMetaPolicy=&{Writers 0}) %!s(int=1) [%!s(*policies.implicitMetaPolicy=&{0xc4209ae200 1 [0xc420a5d120 0xc420a5d1e0]}) %!s(*policies.implicitMetaPolicy=&{0xc4209af160 1 [0xc420a5d258]})]}
2018-02-24 07:43:03.357 UTC [orderer/common/blockcutter] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/blockcutter/blockcutter.go:136 Ordered -> DEBU 9bf Enqueuing message into batch
2018-02-24 07:43:05.357 UTC [orderer/solo] /opt/gopath/src/github.com/hyperledger/fabric/orderer/solo/consensus.go:112 main -> DEBU 9c0 Batch timer expired, creating block
2018-02-24 07:43:05.361 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_mgr.go:454 retrieveBlockByNumber -> DEBU 9c1 retrieveBlockByNumber() - blockNum = [2]
2018-02-24 07:43:05.361 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:66 newBlockfileStream -> DEBU 9c2 newBlockfileStream(): filePath=[/var/hyperledger/production/orderer/chains/mychannel/blockfile_000000], startOffset=[17071]
2018-02-24 07:43:05.361 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:114 nextBlockBytesAndPlacementInfo -> DEBU 9c3 Remaining bytes=[4752], Going to peek [8] bytes
2018-02-24 07:43:05.361 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:147 nextBlockBytesAndPlacementInfo -> DEBU 9c4 Returning blockbytes - length=[4750], placementInfo={fileNum=[0], startOffset=[17071], bytesOffset=[17073]}
2018-02-24 07:43:05.362 UTC [orderer/multichain] /opt/gopath/src/github.com/hyperledger/fabric/orderer/multichain/chainsupport.go:228 addBlockSignature -> DEBU 9c5 &{ledgerResources:0xc42098b7a0 chain:0xc42097e0c0 cutter:0xc4202df590 filters:0xc42098bcc0 signer:0x128b4a8 lastConfig:0 lastConfigSeq:1}
2018-02-24 07:43:05.362 UTC [orderer/multichain] /opt/gopath/src/github.com/hyperledger/fabric/orderer/multichain/chainsupport.go:229 addBlockSignature -> DEBU 9c6 &{}
2018-02-24 07:43:05.362 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mgmt/mgmt.go:149 GetLocalMSP -> DEBU 9c7 Returning existing local MSP
2018-02-24 07:43:05.362 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:369 GetDefaultSigningIdentity -> DEBU 9c8 Obtaining default signing identity
2018-02-24 07:43:05.362 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mgmt/mgmt.go:149 GetLocalMSP -> DEBU 9c9 Returning existing local MSP
2018-02-24 07:43:05.362 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:369 GetDefaultSigningIdentity -> DEBU 9ca Obtaining default signing identity
2018-02-24 07:43:05.362 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:208 Sign -> DEBU 9cb Sign: plaintext: 0AFF050A0A4F7264657265724D535012...F6683EAA9B3CFE2935D80149DE80C942 
2018-02-24 07:43:05.362 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:210 Sign -> DEBU 9cc Sign: digest: F253897A4FB4D75D555164A5A8B0AB701D2C05E62383B5F35619EDE02121ABCA 
2018-02-24 07:43:05.362 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mgmt/mgmt.go:149 GetLocalMSP -> DEBU 9cd Returning existing local MSP
2018-02-24 07:43:05.363 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:369 GetDefaultSigningIdentity -> DEBU 9ce Obtaining default signing identity
2018-02-24 07:43:05.363 UTC [orderer/multichain] /opt/gopath/src/github.com/hyperledger/fabric/orderer/multichain/chainsupport.go:262 addLastConfigSignature -> DEBU 9cf [channel: mychannel] About to write block, setting its LAST_CONFIG to 0
2018-02-24 07:43:05.363 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mgmt/mgmt.go:149 GetLocalMSP -> DEBU 9d0 Returning existing local MSP
2018-02-24 07:43:05.363 UTC [msp] /opt/gopath/src/github.com/hyperledger/fabric/msp/mspimpl.go:369 GetDefaultSigningIdentity -> DEBU 9d1 Obtaining default signing identity
2018-02-24 07:43:05.363 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:208 Sign -> DEBU 9d2 Sign: plaintext: 0AFF050A0A4F7264657265724D535012...F6683EAA9B3CFE2935D80149DE80C942 
2018-02-24 07:43:05.363 UTC [msp/identity] /opt/gopath/src/github.com/hyperledger/fabric/msp/identities.go:210 Sign -> DEBU 9d3 Sign: digest: CFC4B181643699EFBB210A2DB14AC4C3FFBC64A4D4B7F845FBEF14CE7467CB1E 
2018-02-24 07:43:05.367 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockindex.go:98 indexBlock -> DEBU 9d4 Indexing block [blockNum=3, blockHash=[]byte{0xec, 0x3a, 0x8f, 0xc9, 0x5d, 0xcd, 0x43, 0x93, 0x83, 0xb1, 0x45, 0xb0, 0x90, 0xba, 0x65, 0xc2, 0xfe, 0x61, 0x30, 0x26, 0xd7, 0x89, 0xc0, 0x77, 0xfc, 0xdf, 0x38, 0xd6, 0x99, 0x2c, 0x24, 0x7a} txOffsets=
txId=d715b4d1c0c45f9d0ace149349a29961d97d5462e2ac0d8cf86c21a9faaca69f locPointer=offset=70, bytesLength=2825
]
2018-02-24 07:43:05.369 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_mgr.go:430 updateCheckpoint -> DEBU 9d5 Broadcasting about update checkpointInfo: latestFileChunkSuffixNum=[0], latestFileChunksize=[26475], isChainEmpty=[false], lastBlockNumber=[3]
2018-02-24 07:43:05.370 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_mgr.go:454 retrieveBlockByNumber -> DEBU 9d6 retrieveBlockByNumber() - blockNum = [3]
2018-02-24 07:43:05.370 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_mgr.go:454 retrieveBlockByNumber -> DEBU 9d8 retrieveBlockByNumber() - blockNum = [3]
2018-02-24 07:43:05.370 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:66 newBlockfileStream -> DEBU 9d9 newBlockfileStream(): filePath=[/var/hyperledger/production/orderer/chains/mychannel/blockfile_000000], startOffset=[21823]
2018-02-24 07:43:05.370 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:114 nextBlockBytesAndPlacementInfo -> DEBU 9da Remaining bytes=[4652], Going to peek [8] bytes
2018-02-24 07:43:05.370 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:147 nextBlockBytesAndPlacementInfo -> DEBU 9db Returning blockbytes - length=[4650], placementInfo={fileNum=[0], startOffset=[21823], bytesOffset=[21825]}
2018-02-24 07:43:05.370 UTC [orderer/common/deliver] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/deliver/deliver.go:191 Handle -> DEBU 9dc [channel: mychannel] Delivering block for (0xc42085a3c0)
2018-02-24 07:43:05.373 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/blockfile_mgr.go:454 retrieveBlockByNumber -> DEBU 9dd retrieveBlockByNumber() - blockNum = [3]
2018-02-24 07:43:05.373 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:66 newBlockfileStream -> DEBU 9de newBlockfileStream(): filePath=[/var/hyperledger/production/orderer/chains/mychannel/blockfile_000000], startOffset=[21823]
2018-02-24 07:43:05.373 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:114 nextBlockBytesAndPlacementInfo -> DEBU 9df Remaining bytes=[4652], Going to peek [8] bytes
2018-02-24 07:43:05.373 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:147 nextBlockBytesAndPlacementInfo -> DEBU 9e0 Returning blockbytes - length=[4650], placementInfo={fileNum=[0], startOffset=[21823], bytesOffset=[21825]}
2018-02-24 07:43:05.374 UTC [orderer/common/deliver] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/deliver/deliver.go:191 Handle -> DEBU 9e1 [channel: mychannel] Delivering block for (0xc4209d8560)
2018-02-24 07:43:05.378 UTC [orderer/multichain] /opt/gopath/src/github.com/hyperledger/fabric/orderer/multichain/chainsupport.go:289 WriteBlock -> DEBU 9e2 [channel: mychannel] Wrote block 3
2018-02-24 07:43:05.370 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:66 newBlockfileStream -> DEBU 9d7 newBlockfileStream(): filePath=[/var/hyperledger/production/orderer/chains/mychannel/blockfile_000000], startOffset=[21823]
2018-02-24 07:43:05.378 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:114 nextBlockBytesAndPlacementInfo -> DEBU 9e3 Remaining bytes=[4652], Going to peek [8] bytes
2018-02-24 07:43:05.378 UTC [fsblkstorage] /opt/gopath/src/github.com/hyperledger/fabric/common/ledger/blkstorage/fsblkstorage/block_stream.go:147 nextBlockBytesAndPlacementInfo -> DEBU 9e4 Returning blockbytes - length=[4650], placementInfo={fileNum=[0], startOffset=[21823], bytesOffset=[21825]}
2018-02-24 07:43:05.378 UTC [orderer/common/deliver] /opt/gopath/src/github.com/hyperledger/fabric/orderer/common/deliver/deliver.go:191 Handle -> DEBU 9e5 [channel: mychannel] Delivering block for (0xc42032a640)
```


### commit时

```
2018-02-08 10:06:33.033 UTC [validator.go:630] [txvalidator] VSCCValidateTxForCC -> ERRO 46eb VSCC check failed for transaction txid=ec3837e234e68e0635785d9b8ba6f1a666fc7a64a0c7faa2a12dc5e25923fd0a, error VSCC error: policy evaluation failed, err Failed to authenticate policy
2018-02-08 10:06:33.033 UTC [lockbased_query_executer.go:62] [lockbasedtxmgr] Done -> DEBU 46ec Done with transaction simulation / query execution [ee97c24f-17c8-4b7e-a8ae-5121d036df13]
2018-02-08 10:06:33.033 UTC [validator.go:206] [txvalidator] Validate -> ERRO 46ed VSCCValidateTx for transaction txId = ec3837e234e68e0635785d9b8ba6f1a666fc7a64a0c7faa2a12dc5e25923fd0a returned error VSCC error: policy evaluation failed, err Failed to authenticate policy
2018-02-08 10:06:33.033 UTC [validator.go:271] [txvalidator] Validate -> DEBU 46ee END Block Validation
2018-02-08 10:06:33.033 UTC [kv_ledger.go:215] [kvledger] Commit -> DEBU 46ef Channel [mychannel]: Validating block [16]
2018-02-08 10:06:33.033 UTC [lockbased_txmgr.go:72] [lockbasedtxmgr] ValidateAndPrepare -> DEBU 46f0 Validating new block with num trans = [1]
2018-02-08 10:06:33.033 UTC [state_based_validator.go:78] [statevalidator] ValidateAndPrepareBatch -> DEBU 46f1 New block arrived for validation:&common.Block{Header:(*common.BlockHeader)(0xc421b34ec0), Data:(*common.BlockData)(0xc42212fd40), Metadata:(*common.BlockMetadata)(0xc42212fd80)}, doMVCCValidation=true
2018-02-08 10:06:33.033 UTC [state_based_validator.go:80] [statevalidator] ValidateAndPrepareBatch -> DEBU 46f2 Validating a block with [1] transactions
2018-02-08 10:06:33.033 UTC [state_based_validator.go:95] [statevalidator] ValidateAndPrepareBatch -> WARN 46f3 Block [16] Transaction index [0] marked as invalid by committer. Reason code [10]
2018-02-08 10:06:33.034 UTC [kv_ledger.go:221] [kvledger] Commit -> DEBU 46f4 Channel [mychannel]: Committing block [16] to storage
```

## hyperledger/caliper

Caliper is a blockchain performance benchmark framework, which allows users to test different blockchain solutions with predefined use cases, and get a set of performance test results.

https://github.com/hyperledger/caliper




## Refer

### Fabric官方介绍

http://hyperledger-fabric.readthedocs.io/en/latest/
https://wiki.hyperledger.org/
https://cn.hyperledger.org/
https://www.hyperledger.org/about
https://www.hyperledger.org/blog
https://www.hyperledger.org/projects/fabric
[Some movies](https://www.youtube.com/playlist?list=PL0MZ85B_96CH7wvtrRzV7SvtRY0sI0DEg)

### Demo

[Building Your First Network](http://hyperledger-fabric.readthedocs.io/en/latest/build_network.html)

### 官方文档

[Glossary](http://hyperledger-fabric.readthedocs.io/en/release/glossary.html)
[Ledger](http://hyperledger-fabric.readthedocs.io/en/release/ledger.html)
[Transaction Flow](http://hyperledger-fabric.readthedocs.io/en/release/ledger.html#transaction-flow)
[Transaction Flow-More](http://hyperledger-fabric.readthedocs.io/en/release/txflow.html)
[Endorsement policies](http://hyperledger-fabric.readthedocs.io/en/latest/endorsement-policies.html)
[Read-Write set semantics](http://hyperledger-fabric.readthedocs.io/en/release/readwrite.html)
[CouchDB Configuration](http://hyperledger-fabric.readthedocs.io/en/release/ledger.html#couchdb-configuration)
[A Kafka-based Ordering Service for Fabric](https://docs.google.com/document/d/1vNMaM7XhOlu9tB_10dKnlrhy5d7b1u8lSY8a-kVjCO4/edit)
[Chaincode Tutorials](http://hyperledger-fabric.readthedocs.io/en/latest/chaincode.html)
[Channels](http://hyperledger-fabric.readthedocs.io/en/release/channels.html)
[Gossip data dissemination protocol](http://hyperledger-fabric.readthedocs.io/en/release/gossip.html)
[Hyperledger Fabric CA](http://hyperledger-fabric-ca.readthedocs.io/en/latest/)
[Adding an Org to a Channel](http://hyperledger-fabric.readthedocs.io/en/latest/channel_update.html)

### 智能合约

[learn-chaincode](https://github.com/IBM-Blockchain-Archive/learn-chaincode)

### ProtoBuf

https://developers.google.com/protocol-buffers/docs/proto

### gRPC

https://grpc.io/docs/
https://github.com/grpc
https://github.com/grpc/grpc-go/tree/master/examples/helloworld

### yaml

[Learn X in Y minutes](https://learnxinyminutes.com/docs/yaml/)
https://yeasy.gitbooks.io/docker_practice/content/compose/yaml_file.html

### 社区动态

https://github.com/IBM/todo-list-fabricV1
https://www.altoros.com/blog/hyperledger-fabric-v1-0-to-bring-improved-transactions-and-a-pluggable-data-store/

[Hyperledger Fabric v1.1.0-alpha is now available](https://lists.hyperledger.org/pipermail/hyperledger-fabric/2018-January/002732.html)

[一些closed的问题](https://github.com/yeasy/blockchain_guide/issues?q=is%3Aissue+is%3Aclosed)
[JIRA-Fabric Dashboard](https://jira.hyperledger.org/secure/Dashboard.jspa?selectPageId=10104)
[The new and exciting features in Hyperledger Fabric 1.1(-preview)](https://medium.com/wearetheledger/the-new-and-exciting-features-in-hyperledger-fabric-1-1-preview-4261ece3590d)

[Hyperledger Fabric - Private Channel Data (version2)](https://docs.google.com/document/d/1ShrgrYPWLznZSZrl5cnvmFq9LtLJ3tYUxjv9GN6rxuI/edit)

[FAB-3850 disable java chaincode as its WIP](https://github.com/hyperledger/fabric/commit/29e0c40)
[FAB-3218 Sync Java/Golang ChaincodeStub interface](https://jira.hyperledger.org/browse/FAB-3218)

### Bugs

[Kafka/ZK nodes should be shutdown gracefully and in a rolling fashion during upgrade](https://jira.hyperledger.org/browse/FAB-7330)

[Join channel might fail if events are failed to be published](https://jira.hyperledger.org/browse/FAB-3207)

### 综述

[阮一峰-区块链入门教程](http://www.ruanyifeng.com/blog/2017/12/blockchain-tutorial.html)
[Technical Introduction to Hyperledger Fabric v1.0](https://www.slideshare.net/altoros/technical-introduction-to-hyperledger-fabric-v10)
[Hyperledger Fabric V1.0: Block Structure](https://blockchain-fabric.blogspot.com/2017/04/hyperledger-fabric-v10-block-structure.html)
[Senthilnathan's Blockchain"s Blog To know the internals of certain permissioned blockchain platform](https://blockchain-fabric.blogspot.com/)
[Architecture Explained](http://hyperledger-fabric.readthedocs.io/en/release/arch-deep-dive.html)
https://hyperledgercn.github.io/hyperledgerDocs/arch-deep-dive_zh/
https://www.altoros.com/blog/general-availability-of-hyperledger-fabric-v1-0-what-to-expect-in-2017-and-when/
[IBM Blockchain 101：开发人员快速入门指南](https://www.ibm.com/developerworks/cn/cloud/library/cl-ibm-blockchain-101-quick-start-guide-for-developers-bluemix-trs/index.html)
[Hyperledger Whitepaper](https://docs.google.com/document/d/1Z4M_qwILLRehPbVRUsJ3OF8Iir-gqS-ZYe7W-LE9gnE/pub)
[hyperledger_code_fabric](https://github.com/yeasy/hyperledger_code_fabric/blob/master/SUMMARY.md)
[区块链基础](https://console.bluemix.net/docs/services/blockchain/ibmblockchain_overview.html#architecture?ca=dw-_-bluemix-_-cl-ibm-blockchain-101-quick-start-guide-for-developers-bluemix-trs-_-article)

### kafka相关
[Bringing up a Kafka-based Ordering Service](http://hyperledger-fabric.readthedocs.io/en/latest/kafka.html)
[A Kafka-based Ordering Service for Fabric](https://docs.google.com/document/d/1vNMaM7XhOlu9tB_10dKnlrhy5d7b1u8lSY8a-kVjCO4/edit)
https://stackoverflow.com/questions/40369238/which-directory-does-apache-kafka-store-the-data-in-broker-nodes
https://stackoverflow.com/questions/41334691/leader-election-in-zookeeper-and-kafka
https://www.slideshare.net/gwenshap/kafka-reliability-when-it-absolutely-positively-has-to-be-there
https://www.slideshare.net/HadoopSummit/apache-kafka-best-practices
[kafka 数据可靠性深度解读](http://www.importnew.com/25247.html)
[How does Ordering Nodes Synchronization work?](https://stackoverflow.com/questions/45165378/how-does-ordering-nodes-synchronization-work)
https://stackoverflow.com/questions/33970374/need-to-understand-kafka-broker-property-log-flush-interval-messages
https://kafka.apache.org/documentation/

### 性能测试工具

fabric/test/tools/PTE （There's some report with thousands of tps with concurrent config.）
https://github.com/yeasy/blockchain_guide/issues/51

### Docker

https://docs.docker.com/engine/reference/commandline
https://docs.docker.com/compose/compose-file/compose-file-v2/
https://docs.docker.com/engine/userguide/storagedriver/imagesandcontainers/#container-size-on-disk
https://docs.docker.com/engine/admin/resource_constraints/
https://stackoverflow.com/questions/33013904/how-to-limit-docker-filesystem-space-available-to-containers
https://yeasy.gitbooks.io/docker_practice/content/image/dockerfile/entrypoint.html
[docker-compose 之 tty](https://plus.ooclab.com/note/article/1322)

https://stackoverflow.com/questions/43830372/error-manifest-for-hyperledger-fabric-ordererlatest-not-found
https://hub.docker.com/u/hyperledger/

[在 Docker 中配置时区](https://tommy.net.cn/2015/02/05/config-timezone-in-docker/)
https://forums.docker.com/t/synchronize-timezone-from-host-to-container/39116
https://serverfault.com/questions/683605/docker-container-time-timezone-will-not-reflect-changes

### kubernetes

https://kubernetes.io/docs/concepts/storage/persistent-volumes/
https://github.com/kubernetes/examples/tree/master/staging/volumes/cephfs/
[腾讯云-数据卷概览](https://cloud.tencent.com/document/product/457/9112)
https://kubernetes.io/docs/tasks/configure-pod-container/configure-persistent-volume-storage/
https://github.com/kubernetes/community/blob/master/contributors/design-proposals/storage/persistent-storage.md

### 前端
https://github.com/hyperledger/blockchain-explorer

### SDK
[Nodejs SDK下载路径](https://github.com/hyperledger/fabric-sdk-node)
[Nodejs SDK官方文档说明](https://fabric-sdk-node.github.io/)

### NodeJS

[Installing Node.js via package manage](https://nodejs.org/en/download/package-manager/)

### GoLang

https://golang.org/dl/
https://gobyexample.com/
[Go Data Structures: Interfaces](https://research.swtch.com/interfaces)
https://golang.org/pkg/reflect/
https://golang.org/pkg/encoding/json/#Unmarshal
https://golang.org/pkg/encoding/json/#Marshal


### 论坛
[区块链大学](https://www.qkldx.net/)

### 万达区块链

作者季宙栋，现任万达网络科技集团先进技术研究中心
[Hyperledger Fabric V1.0– 开发者快速入门](https://zhuanlan.zhihu.com/p/25070745)
[Blockchain区块链架构设计之一：成员管理](https://zhuanlan.zhihu.com/p/23356616)
[Blockchain区块链架构设计之二：分布式账本技术对比](https://zhuanlan.zhihu.com/p/23463699)
[Blockchain区块链架构设计之三：Hyperledger Fabric介绍](https://zhuanlan.zhihu.com/p/23947372)
[Blockchain区块链架构设计之四：Fabric多通道和下一代账本设计](https://zhuanlan.zhihu.com/p/24605987)
[Blockchain区块链架构设计之六：Fabric 1.0账本设计（1）](https://zhuanlan.zhihu.com/p/25119939)
[Blockchain区块链架构设计之七：Fabric 1.0 Endorse背书策略](https://zhuanlan.zhihu.com/p/25295608)
[Blockchain区块链架构设计之八：Fabric 1.0交易流程示例](https://zhuanlan.zhihu.com/p/25991010)

### 腾讯区块链

https://cloud.tencent.com/product/tbaas

### 比特币

http://www.8btc.com/wiki/bitcoin-a-peer-to-peer-electronic-cash-system

https://en.bitcoin.it/wiki/Transaction

https://blockgeeks.com/guides/bitcoin-developer/  (必看)

https://blockgeeks.com/guides


### IBM

[区块链技术指南](https://yeasy.gitbooks.io/blockchain_guide/)

[Hyperledger Fabric代码解析](https://www.ibm.com/developerworks/community/wikis/home?lang=ru#!/wiki/W30b0c771924e_49d2_b3b7_88a2a2bc2e43/page/IBM%E5%BC%80%E6%BA%90%E6%8A%80%E6%9C%AF%E5%BE%AE%E8%AE%B2%E5%A0%82)

[IBM开源技术社区 Wiki](https://www.ibm.com/developerworks/community/wikis/home?lang=zh#!/wiki/W30b0c771924e_49d2_b3b7_88a2a2bc2e43/page)

[区块链和HyperLedger](https://www.ibm.com/developerworks/community/wikis/home?lang=ru#!/wiki/W30b0c771924e_49d2_b3b7_88a2a2bc2e43/page/HyperLedger%20Fabric%E7%B3%BB%E5%88%97%E5%BE%AE%E8%AE%B2%E5%A0%82)

[容器技术和微服务”系列课程内容](https://www.ibm.com/developerworks/community/wikis/home?lang=ru#!/wiki/W30b0c771924e_49d2_b3b7_88a2a2bc2e43/page/%E5%AE%B9%E5%99%A8%E6%8A%80%E6%9C%AF%E5%92%8C%E5%BE%AE%E6%9C%8D%E5%8A%A1%E7%B3%BB%E5%88%97%E5%BE%AE%E8%AE%B2%E5%A0%82)

Fabric代码解析第一讲
http://v.youku.com/v_show/id_XMzMzMjQ2MTA1Ng==.html
http://v.youku.com/v_show/id_XMzMzMjUzMTU4OA==.html

### MySQL

https://www.digitalocean.com/community/tutorials/how-to-install-mysql-on-ubuntu-16-04


### Chaincode dev

[Getting Started with Blockchain Development](https://blog.selman.org/2017/07/08/getting-started-with-blockchain-development/)

[Managing external dependencies for chaincode written in Go](http://hyperledger-fabric.readthedocs.io/en/latest/chaincode4ade.html#managing-external-dependencies-for-chaincode-written-in-go)

[Chaincode for Developers](http://hyperledger-fabric.readthedocs.io/en/latest/chaincode4ade.html)

[Chaincode for Operators](http://hyperledger-fabric.readthedocs.io/en/latest/chaincode4noah.html)

[Update](http://hyperledger-fabric.readthedocs.io/en/latest/chaincode4noah.html#upgrade)

### 性能调优

[超线程加快了 Linux 的速度](https://www.ibm.com/developerworks/cn/linux/l-htl/index.html)

[hyperledger/caliper](https://github.com/hyperledger/caliper)

### 容器云

[腾讯云-容器服务](https://cloud.tencent.com/document/product/457)
[腾讯云-查询容器服务监控信息](https://cloud.tencent.com/document/product/457/9505)

### 安全

https://en.wikipedia.org/wiki/Public_key_infrastructure
https://en.wikipedia.org/wiki/X.509


### Q&A

[Difference between chain and state database in Hyperledger fabric?](https://stackoverflow.com/questions/47505084/difference-between-chain-and-state-database-in-hyperledger-fabric)

[Add Org or peer in Org dynamically in Hyperledger fabric](https://stackoverflow.com/questions/43593890/add-org-or-peer-in-org-dynamically-in-hyperledger-fabric)



