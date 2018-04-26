
# Useful commands for Fabric
---
[TOC]


## check Fabric logger level

```
docker inspect --format={{.LogPath}} peer0.org1.example.com | xargs grep "setModuleLevel" 
```

## clear container or images

```
docker rm `docker ps -aq`
docker rmi `docker images dev* -aq`

docker images | grep none | awk -F' ' '{print $3}' | while read f; do docker rmi -f $f; done
```

## calc Fabric TPS


1. use peer log.

peer log:
```
# 1.0.4
{"log":"2018-02-28 12:28:53.378 UTC [kvledger] Commit -\u003e INFO 039\u001b[0m Channel [mychannel]: Created block [4] with 2 transaction(s)\n","stream":"stderr","time":"2018-02-28T12:28:53.378598294Z"}

# 1.1.0
2018-03-06 07:29:58.620 UTC [kvledger] CommitWithPvtData -> INFO 038 Channel [mychannel]: Committed block [4] with 1 transaction(s)
```

script: 
```
# 1.0.4
# docker inspect --format={{.LogPath}} peer0.org1.example.com | xargs grep "Created" | perl -ne 'if(/(\d\d:\d\d:(\d+)).*with\s(\d+)/)  {$c+=$3; if($pre != $2) {printf("%s qps:%d\n", $1, $c); $c=0; $pre = $2;} }'

10:12:18 qps:1
10:12:33 qps:1
10:12:43 qps:1
12:26:30 qps:1
12:28:53 qps:2

# 1.1.0
# docker inspect --format={{.LogPath}} peer0.org1.example.com | xargs grep "Committed" | perl -ne 'if(/(\d\d:\d\d:(\d+)).*with\s(\d+)/)  {$c+=$3; if($pre != $2) {printf("%s qps:%d\n", $1, $c); $c=0; $pre = $2;} }'
```

2. use [hyperledger/caliper](https://github.com/hyperledger/caliper)


Refer

[区块链专利](http://www.soopat.com/Home/Result?Sort=&View=&Columns=&Valid=&Embed=&Db=&Ids=&FolderIds=&FolderId=&ImportPatentIndex=&Filter=&SearchWord=%E5%8C%BA%E5%9D%97%E9%93%BE+%E5%B9%B6%E8%A1%8C&FMZL=Y&SYXX=Y&WGZL=Y&FMSQ=Y)