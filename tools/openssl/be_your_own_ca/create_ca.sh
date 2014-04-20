#!/bin/bash
# this script refer to link: http://www.g-loaded.eu/2005/11/10/be-your-own-ca/


OPENSSL_PATH=/Users/gerryyang/LAMP/OpenSSL/install/openssl-1.0.1g
DEMO_PATH=$OPENSSL_PATH/bin


:<<WCDJ
myCA is our Certificate Authority＊s directory.
myCA/certs directory is where our server certificates will be placed.
myCA/newcerts directory is where openssl puts the created certificates in PEM (unencrypted) format and in the form cert_serial_number.pem (eg 07.pem). Openssl needs this directory, so we create it.
myCA/crl is where our certificate revokation list is placed.
myCA/private is the directory where our private keys are placed. Be sure that you set restrictive permissions to all your private keys so that they can be read only by root, or the user with whose priviledges a server runs. If anyone steals your private keys, then things get really bad.
WCDJ


echo "[1] Creating the necessary directories"
mkdir -p -m 0755 $DEMO_PATH/myCA/{private,certs,newcerts,crl}


echo "[2] Initial openssl configuration"
cp $OPENSSL_PATH/ssl/openssl.cnf $DEMO_PATH/myCA/openssl.my.cnf

# This file does not need to be world readable, so we change its attributes
chmod 0600 $DEMO_PATH/myCA/openssl.my.cnf

# We also need to create two other files. This file serves as a database for openssl
touch $DEMO_PATH/myCA/index.txt

# The following file contains the next certificate＊s serial number. Since we have not created any certificates yet, we set it to "01"
echo '01' > $DEMO_PATH/myCA/serial

:<<WCDJ
Things to remember

Here is a small legend with file extensions we will use for the created files and their meaning. All files that will be created will have one of these extensions:

    KEY 每 Private key (Restrictive permissions should be set on this)
    CSR 每 Certificate Request (This will be signed by our CA in order to create the server certificates. Afterwards it is not needed and can be deleted)
    CRT 每 Certificate (This can be publicly distributed)
    PEM 每 We will use this extension for files that contain both the Key and the server Certificate (Some servers need this). Permissions should be restrictive on these files.
    CRL 每 Certificate Revokation List (This can be publicly distributed)
WCDJ


echo "[3] Create the CA Certificate and Key"

# Now, that all initial configuration is done, we may create a self-signed certificate, that will be used as our CA＊s certificate. In other words, we will use this to sign other certificate requests.

# Change to our CA＊s directory. This is where we should issue all the openssl commands because here is our openssl＊s configuration file (openssl.my.cnf).
cd $DEMO_PATH/myCA/

:<< WCDJ
And then create your CA＊s Certificate and Private Key.
This creates a self-signed certificate with the default CA extensions which is valid for 5 years. You will be prompted for a passphrase for your CA＊s private key. Be sure that you set a strong passphrase. Then you will need to provide some info about your CA. Fill in whatever you like. Here is an example:

Country Name (2 letter code) [GB]:GR
State or Province Name (full name) [Berkshire]:Greece
Locality Name (eg, city) [Newbury]:Thessaloniki
Organization Name (eg, company) [My Company Ltd]:My Network
Organizational Unit Name (eg, section) []:My Certificate Authority
Common Name (eg, your name or your server's hostname) []:server.example.com
Email Address []:whatever@server.example.com

WCDJ

openssl req -config openssl.my.cnf -new -x509 -extensions v3_ca -keyout private/myca.key -out certs/myca.crt -days 1825

:<< WCDJ
Two files are created:

    certs/myca.crt 每 This is your CA＊s certificate and can be publicly available and of course world readable.
    private/myca.key 每 This is your CA＊s private key. Although it is protected with a passphrase you should restrict access to it.
WCDJ

chmod 0400 private/myca.key

# Because we use a custom directory for our certificates＊ management, some modifications to openssl.my.cnf are necessary.

:<< WCDJ
Open openssl.my.cnf in your favourite text editor and find the following part (around line 35):
You should modify the following settings in order to coform to our custom directory and our custom CA key and certificate:

[ CA_default ]

dir     = .                # <--CHANGE THIS
certs       = $dir/certs
crl_dir     = $dir/crl
database    = $dir/index.txt
#unique_subject = no

new_certs_dir   = $dir/newcerts

certificate = $dir/certs/myca.crt   # <--CHANGE THIS
serial      = $dir/serial
#crlnumber  = $dir/crlnumber

crl     = $dir/crl.pem
private_key = $dir/private/myca.key    # <--CHANGE THIS
RANDFILE    = $dir/private/.rand

x509_extensions = usr_cert
WCDJ


echo "[4] Create a Server certificate"

# Generate a Certificate Request
:<<WCDJ
The -nodes option is needed so that the private key is not protected with a passphrase. If you do not intend to use the certificate for server authentication, you should not include it in the above command.
You can customize the number of days you want this certificate to be valid for.

You will be prompted for the certificate＊s info. Here is an example:

Country Name (2 letter code) [GB]:GR
State or Province Name (full name) [Berkshire]:Greece
Locality Name (eg, city) [Newbury]:Thessaloniki
Organization Name (eg, company) [My Company Ltd]:My Network
Organizational Unit Name (eg, section) []:My Web Server
Common Name (eg, your name or your server's hostname) []:www.server.example.com
Email Address []:whatever@server.example.com

The Common Name (CN) is the info that uniquely distinguishes your service, so be sure that you type it correctly.
When prompted for some extra attributes (challenge password, optional company name) just hit the [Enter] key.
WCDJ

openssl req -config openssl.my.cnf -new -nodes -keyout private/server.key -out server.csr -days 365

:<< WCDJ
Two files are created:

    server.csr 每 this is the certificate request.
    private/server.key 每 this is the private key, which is not protected with a passphrase.
WCDJ

# Set restrictive permissions on the private key. Only root or the user that is used to run the server should be able to read it.

chown 0400 private/server.key


echo "[5] Sign the Certificate Request"

:<< WCDJ
Now we are going to sign the certificate request and generate the server＊s certificate.
After all this is done two new files are created:

    certs/server.crt 每 this is the server＊s certificate, which can be made available publicly.
    newcerts/01.pem 每 This is exactly the same certificate, but with the certificate＊s serial number as a filename. It is not needed.

WCDJ

openssl ca -config openssl.my.cnf -policy policy_anything -out certs/server.crt -infiles server.csr

# You can now delete the certificate request (server.csr). It＊s no longer needed.
#rm -f server.csr


echo "[6] Verify the certificate"

# You can see the certificate＊s info with the following:
openssl x509 -subject -issuer -enddate -noout -in certs/server.crt
# Or the following:
openssl x509 -in certs/server.crt -noout -text

:<< WCDJ
And verify that the certificate is valid for server authentication with the following:
output likes this:
gerryyang@mba:myCA$openssl verify -purpose sslserver -CAfile certs/myca.crt certs/server.crt
certs/server.crt: OK
WCDJ
openssl verify -purpose sslserver -CAfile certs/myca.crt certs/server.crt


echo "[7] Server certificate and key in one file"

# Some servers, for example vsftpd, require that both the private key and the certificate exist in the same file. In a situation like that just do the following:
cat certs/server.crt private/server.key > private/server-key-cert.pem

# You should restrict access to the final file and delete server.crt and server.key since thay are no longer needed.
chmod 0400 private/server-key-cert.pem
# rm -f certs/server.crt
# rm -f private/server.key


echo "[8] Revoke a Server Certificate"

# If you do not want a certificate to be valid any more, you have to revoke it. This is done with the command:
#openssl ca -config openssl.my.cnf -revoke certs/server.crt

# Then you should generate a new CRL (Certificate Revokation List):
openssl ca -config openssl.my.cnf -gerncrl -out crl/myca.crl


echo "[9] Distribute your certificates and CRL"

:<< WCDJ
Your CA＊s certificate and your servers＊ certificates should be distributed to those who trust you so they can import them in their client software (web browsers, ftp clients, email clients etc). The CRL should also be published.
WCDJ






