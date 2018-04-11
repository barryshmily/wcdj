#!/bin/bash

kubectl delete -n byfn-solo -f ./ 
kubectl create -n byfn-solo -f ./ 