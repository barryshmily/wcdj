#/bin/bash

g++ -DAES_INF -I/root/LAMP/openssl/install/openssl-1.0.1g/include -L//root/LAMP/openssl/install/openssl-1.0.1g/lib AES_ecb_encrypt.cpp base64.cpp -lcrypto -ldl
#g++ -DEVP_INF -I/root/LAMP/openssl/install/openssl-1.0.1g/include -L//root/LAMP/openssl/install/openssl-1.0.1g/lib AES_ecb_encrypt.cpp base64.cpp -lcrypto -ldl

