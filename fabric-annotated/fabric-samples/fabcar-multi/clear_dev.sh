#/bin/bash

set -e

docker rm `docker ps -aq`
docker rmi `docker images dev* -aq`
