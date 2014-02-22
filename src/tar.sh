#!/bin/sh

PACKAGE_PATH=../package

mkdir -p ${PACKAGE_PATH} && rm ${PACKAGE_PATH}/* -rf 
mkdir ${PACKAGE_PATH}/src
mkdir ${PACKAGE_PATH}/inc
mkdir ${PACKAGE_PATH}/lib
mkdir ${PACKAGE_PATH}/conf
mkdir ${PACKAGE_PATH}/bin

cp ../src/svr/*.cpp  ${PACKAGE_PATH}/src
cp ../inc/*.h        ${PACKAGE_PATH}/inc
#cp ../lib/*          ${PACKAGE_PATH}/lib
#cp ../conf/*         ${PACKAGE_PATH}/conf
cp ../bin/*          ${PACKAGE_PATH}/bin

cd ${PACKAGE_PATH} && tar -zcvf WCDJ_${os_ver}_${os_date}.tgz * 

