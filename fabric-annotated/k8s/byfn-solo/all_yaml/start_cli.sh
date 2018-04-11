#!/bin/bash

# 进入cli容器
cli=`kubectl get pods -n byfn-solo | grep cli | awk '{print $1}'` 
echo $cli
kubectl exec -it $cli -n byfn-solo /bin/bash