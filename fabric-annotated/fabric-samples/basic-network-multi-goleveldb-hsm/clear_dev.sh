#/bin/bash

set -e

docker rm `docker ps -aq`
docker rmi `docker images dev* -aq`

docker images | grep none | awk -F' ' '{print $3}' | while read f; do docker rmi -f $f; done
