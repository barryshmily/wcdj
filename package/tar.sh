#!/bin/sh

:<< WCDJ_COMMENT
tar.sh is used to package wcdj repo
WCDJ_COMMENT

PACKAGE_PATH=../package

mkdir -p ${PACKAGE_PATH} && ls | grep -v "README" | grep -v "tar.sh"| xargs rm -rf

cp -r ../src   ${PACKAGE_PATH}
cp -r ../inc   ${PACKAGE_PATH}
cp -r ../lib   ${PACKAGE_PATH}
cp -r ../conf  ${PACKAGE_PATH}
cp -r ../doc   ${PACKAGE_PATH}
cp -r ../bin   ${PACKAGE_PATH}
cp -r ../tools ${PACKAGE_PATH}

cd ${PACKAGE_PATH} && tar -zcvf WCDJ_${os_ver}_${os_date}.tgz * 

