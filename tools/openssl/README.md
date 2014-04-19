
Abstract
===
openssl - OpenSSL command line tool
openssl command


*reference*

[1] [openssl公钥,私钥,数字签名,CA,根证书等](http://wujunfeng.blog.51cto.com/3041/804693)
[2] [OpenSSL证书管理](http://wiki.ubuntu.org.cn/Openssl%E8%AF%81%E4%B9%A6%E7%AE%A1%E7%90%86)


# DESCRIPTION

OpenSSL is a cryptography toolkit implementing the Secure Sockets Layer (SSL v2/v3) and Transport Layer Security (TLS v1) network protocols and related cryptography standards required by them.

The openssl program is a command line tool for using the various cryptography functions of OpenSSL's crypto library from the shell.  It can be used for

(1) Creation of RSA, DH and DSA key parameters

(2) Creation of X.509 certificates, CSRs and CRLs

(3) Calculation of Message Digests

(4) Encryption and Decryption with Ciphers

(5) SSL/TLS Client and Server Tests

(6) Handling of S/MIME signed or encrypted mail


# COMMAND SUMMARY

The openssl program provides a rich variety of commands, each of which often has a wealth of options and arguments.


# STANDARD COMMANDS

asn1parse Parse an ASN.1 sequence.

ca        Certificate Authority (CA) Management.

ciphers   Cipher Suite Description Determination.

crl       Certificate Revocation List (CRL) Management.

crl2pkcs7 CRL to PKCS#7 Conversion.

dgst      Message Digest Calculation.

dh        Diffie-Hellman Parameter Management.  Obsoleted by dhparam.

dsa       DSA Data Management.

dsaparam  DSA Parameter Generation.

enc       Encoding with Ciphers.

errstr    Error Number to Error String Conversion.

dhparam   Generation and Management of Diffie-Hellman Parameters.

gendh     Generation of Diffie-Hellman Parameters.  Obsoleted by dhparam.

gendsa    Generation of DSA Parameters.

genrsa    Generation of RSA Parameters.

ocsp      Online Certificate Status Protocol utility.

passwd    Generation of hashed passwords.

pkcs12    PKCS#12 Data Management.

pkcs7     PKCS#7 Data Management.

rand      Generate pseudo-random bytes.

req       X.509 Certificate Signing Request (CSR) Management.

rsa       RSA Data Management.

rsautl    RSA utility for signing, verification, encryption, and decryption.

s_client  This implements a generic SSL/TLS client which can establish a transparent connection to a remote server speaking SSL/TLS. It's intended for testing purposes only and provides only rudimentary interface functionality but internally uses mostly all functionality of the OpenSSL ssl library.

s_server  This implements a generic SSL/TLS server which accepts connections from remote clients speaking SSL/TLS. It's intended for testing purposes only and provides only rudimentary interface functionality but internally uses mostly all functionality of the OpenSSL ssl library.  It provides both an own command line oriented protocol for testing SSL functions and a simple HTTP response facility to emulate an SSL/TLS-aware webserver.

s_time    SSL Connection Timer.

sess_id   SSL Session Data Management.

smime     S/MIME mail processing.

speed     Algorithm Speed Measurement.

verify    X.509 Certificate Verification.

version   OpenSSL Version Information.

x509      X.509 Certificate Data Management.


# MESSAGE DIGEST COMMANDS

md2       MD2 Digest

md5       MD5 Digest

mdc2      MDC2 Digest

rmd160    RMD-160 Digest

sha       SHA Digest

sha1      SHA-1 Digest

sha224    SHA-224 Digest

sha256    SHA-256 Digest

sha384    SHA-384 Digest

sha512    SHA-512 Digest


# ENCODING AND CIPHER COMMANDS

base64    Base64 Encoding

bf bf-cbc bf-cfb bf-ecb bf-ofb Blowfish Cipher

etc.



*END*




