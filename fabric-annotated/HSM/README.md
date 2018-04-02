
BCCSP (Blockchain crypto provider)

* SW: a software based crypto provider
* PKCS11: a CA hardware security module crypto provider.

Orderer:
initializeLocalMsp -> LoadLocalMsp -> GetLocalMspConfig -> InitFactories -> initBCCSP


softhsm2-util --init-token --slot 0 --label fabric --pin 123456 --so-pin 123456

https://www.opendnssec.org/documentation/

[pkcs11 wrapper for Go](https://github.com/miekg/pkcs11)

https://everipedia.org/wiki/PKCS_11/




> Signer is an interface for an opaque private key that can be used for signing operations. For example, an RSA key kept in a hardware module.
> https://golang.org/pkg/crypto/#Signer

http://www.blockchainbrother.com/article/203
国密支持的一些思路

## fabric中的PKCS11

* lib and docs

github.com/miekg/pkcs11

https://godoc.org/github.com/miekg/pkcs11

* Demo

https://github.com/miekg/pkcs11#examples

https://godoc.org/github.com/miekg/pkcs11#example-Ctx-Sign



## PKCS11

http://docs.oasis-open.org/pkcs11/pkcs11-base/v2.40/pkcs11-base-v2.40.html (重要)

### Definitions


* Cryptographic Device 

A device storing cryptographic information and possibly performing cryptographic functions.  May be implemented as a smart card, smart disk, PCMCIA card, or with some other technology, including software-only.

* Cryptoki

The Cryptographic Token Interface defined in this standard.

* PIN

Personal Identification Number.

* PKCS

Public-Key Cryptography Standards.

* Slot

A logical reader that potentially contains a token.

*  SO

A Security Officer user.

* Token

The logical view of a cryptographic device defined by Cryptoki.


## support PKCS11

1. 修改编译选项，将静态编译改为动态编译

Makefile中include docker-env.mk，需要修改DOCKER_GO_LDFLAGS

```
DOCKER_GO_LDFLAGS += $(GO_LDFLAGS)
DOCKER_GO_LDFLAGS += -linkmode external -extldflags '-static -lpthread'

DOCKER_GO_LDFLAGS_EXTERNAL += $(GO_LDFLAGS)
DOCKER_GO_LDFLAGS_EXTERNAL += -linkmode external -extldflags '-lpthread'
```

2. 在宿主机创建token

```
softhsm2-util --init-token --free --label fabric --pin 123456 --so-pin 123456
softhsm2-util --show-slots
```

3. 修改容器启动配置

``` yaml
  orderer.example.com:
    container_name: orderer.example.com
    image: hyperledger/fabric-orderer
    #tty: true
    environment:
      - ORDERER_GENERAL_LOGLEVEL=debug
      - ORDERER_GENERAL_LISTENADDRESS=0.0.0.0
      - ORDERER_GENERAL_GENESISMETHOD=file
      - ORDERER_GENERAL_GENESISFILE=/etc/hyperledger/configtx/genesis.block
      - ORDERER_GENERAL_LOCALMSPID=OrdererMSP
      - ORDERER_GENERAL_LOCALMSPDIR=/etc/hyperledger/msp/orderer/msp
      # PKCS11
      - ORDERER_GENERAL_BCCSP_DEFAULT=PKCS11
      - ORDERER_GENERAL_BCCSP_PKCS11_HASH=SHA2
      - ORDERER_GENERAL_BCCSP_PKCS11_SECURITY=256
      - ORDERER_GENERAL_BCCSP_PKCS11_LIBRARY=/usr/lib/libsofthsm2.so
      - ORDERER_GENERAL_BCCSP_PKCS11_PIN=123456
      - ORDERER_GENERAL_BCCSP_PKCS11_LABEL=fabric
      - ORDERER_GENERAL_BCCSP_PKCS11_FILEKEYSTORE_KEYSTORE=/etc/hyperledger/msp/orderer/msp/keystore
      - ORDERER_GENERAL_BCCSP_SW_FILEKEYSTORE_KEYSTORE=/etc/hyperledger/msp/orderer/keystore
    working_dir: /opt/gopath/src/github.com/hyperledger/fabric/orderer
    command: orderer
    ports:
      - 7050:7050
    volumes:
        - ./config/:/etc/hyperledger/configtx
        - ./crypto-config/ordererOrganizations/example.com/orderers/orderer.example.com/:/etc/hyperledger/msp/orderer
        - ./crypto-config/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/:/etc/hyperledger/msp/peerOrg1
        # PKCS11
        - /root/softhsm/SoftHSMv2/src/lib/.libs/libsofthsm2.so:/usr/lib/libsofthsm2.so
        - /usr/lib/x86_64-linux-gnu/libltdl.so.7:/usr/lib/libltdl.so.7
        - /usr/local/bin/softhsm2-util:/usr/local/bin/softhsm2-util
        - /etc/softhsm2.conf:/etc/softhsm2.conf
        - /var/lib/softhsm/tokens:/var/lib/softhsm/tokens
    networks:
      - basic
 ```

4. order改为PKCS11后，不会再用到/etc/hyperledger/msp/orderer/msp/keystore下的私钥

```
# tree crypto-config
crypto-config
├── ordererOrganizations
│   └── example.com
│       ├── ca
│       │   ├── ca.example.com-cert.pem
│       │   └── e74945ed26f287e596ed4c165481a037821a65b99788504746d993e145c93e3b_sk
│       ├── msp
│       │   ├── admincerts
│       │   │   └── Admin@example.com-cert.pem
│       │   ├── cacerts
│       │   │   └── ca.example.com-cert.pem
│       │   └── tlscacerts
│       │       └── tlsca.example.com-cert.pem
│       ├── orderers
│       │   └── orderer.example.com
│       │       ├── keystore
│       │       ├── msp
│       │       │   ├── admincerts
│       │       │   │   └── Admin@example.com-cert.pem
│       │       │   ├── cacerts
│       │       │   │   └── ca.example.com-cert.pem
│       │       │   ├── keystore
│       │       │   │   └── b12d8ca582dd0b6ad76a95b05e2b84a384be91ef51a525f040efa98913ccbda8_sk_del
│       │       │   ├── signcerts
│       │       │   │   └── orderer.example.com-cert.pem
│       │       │   └── tlscacerts
│       │       │       └── tlsca.example.com-cert.pem
│       │       └── tls
│       │           ├── ca.crt
│       │           ├── server.crt
│       │           └── server.key
│       ├── tlsca
```

## SoftHSM

### docs

SoftHSM is an implementation of a cryptographic store accessible through a PKCS#11 interface. You can use SoftHSM as an HSM for OpenDNSSEC.

https://www.opendnssec.org/softhsm/

https://wiki.opendnssec.org/display/SoftHSMDOCS

https://wiki.opendnssec.org/display/DOCREF/HSM+Buyers%27+Guide

### install

https://wiki.opendnssec.org/display/SoftHSMDOCS/SoftHSM+Documentation+v2

```
sudo apt-get autoremove automake
sudo apt-get install automake
sudo apt-get install autoconf

sh ./autogen.sh 
./configure && make && make install
```

### utils

#### softhsm2-util

> softhsm2-util is a support tool mainly for libsofthsm2. It can also be used with other PKCS#11 libraries by using the option --module

softhsm2-util --show-slots



softhsm2-keyconv

softhsm2-migrate




### conf

man softhsm2.conf

```
# cat /etc/softhsm2.conf
# SoftHSM v2 configuration file

directories.tokendir = /var/lib/softhsm/tokens/
objectstore.backend = file

# ERROR, WARNING, INFO, DEBUG
log.level = ERROR

# If CKF_REMOVABLE_DEVICE flag should be set
slots.removable = false
```

```
# softhsm2-util --init-token --slot 0 --label "My token 1"
=== SO PIN (4-255 characters) ===
Please enter SO PIN: ****
Please reenter SO PIN: ****
=== User PIN (4-255 characters) ===
Please enter user PIN: ****
Please reenter user PIN: ****
The token has been initialized and is reassigned to slot 1296418332
```


``` go
// fabric_v1.0.4\bccsp\bccsp.go

// BCCSP is the blockchain cryptographic service provider that offers
// the implementation of cryptographic standards and algorithms.
type BCCSP interface {

	// KeyGen generates a key using opts.
	KeyGen(opts KeyGenOpts) (k Key, err error)

	// KeyDeriv derives a key from k using opts.
	// The opts argument should be appropriate for the primitive used.
	KeyDeriv(k Key, opts KeyDerivOpts) (dk Key, err error)

	// KeyImport imports a key from its raw representation using opts.
	// The opts argument should be appropriate for the primitive used.
	KeyImport(raw interface{}, opts KeyImportOpts) (k Key, err error)

	// GetKey returns the key this CSP associates to
	// the Subject Key Identifier ski.
	GetKey(ski []byte) (k Key, err error)

	// Hash hashes messages msg using options opts.
	// If opts is nil, the default hash function will be used.
	Hash(msg []byte, opts HashOpts) (hash []byte, err error)

	// GetHash returns and instance of hash.Hash using options opts.
	// If opts is nil, the default hash function will be returned.
	GetHash(opts HashOpts) (h hash.Hash, err error)

	// Sign signs digest using key k.
	// The opts argument should be appropriate for the algorithm used.
	//
	// Note that when a signature of a hash of a larger message is needed,
	// the caller is responsible for hashing the larger message and passing
	// the hash (as digest).
	Sign(k Key, digest []byte, opts SignerOpts) (signature []byte, err error)

	// Verify verifies signature against key k and digest
	// The opts argument should be appropriate for the algorithm used.
	Verify(k Key, signature, digest []byte, opts SignerOpts) (valid bool, err error)

	// Encrypt encrypts plaintext using key k.
	// The opts argument should be appropriate for the algorithm used.
	Encrypt(k Key, plaintext []byte, opts EncrypterOpts) (ciphertext []byte, err error)

	// Decrypt decrypts ciphertext using key k.
	// The opts argument should be appropriate for the algorithm used.
	Decrypt(k Key, ciphertext []byte, opts DecrypterOpts) (plaintext []byte, err error)
}
```

``` go
// fabric_v1.0.4\bccsp\pkcs11\conf.go

// PKCS11Opts contains options for the P11Factory
type PKCS11Opts struct {
	// Default algorithms when not specified (Deprecated?)
	SecLevel   int    `mapstructure:"security" json:"security"`
	HashFamily string `mapstructure:"hash" json:"hash"`

	// Keystore options
	Ephemeral     bool               `mapstructure:"tempkeys,omitempty" json:"tempkeys,omitempty"`
	FileKeystore  *FileKeystoreOpts  `mapstructure:"filekeystore,omitempty" json:"filekeystore,omitempty"`
	DummyKeystore *DummyKeystoreOpts `mapstructure:"dummykeystore,omitempty" json:"dummykeystore,omitempty"`

	// PKCS11 options
	Library    string `mapstructure:"library" json:"library"`
	Label      string `mapstructure:"label" json:"label"`
	Pin        string `mapstructure:"pin" json:"pin"`
	Sensitive  bool   `mapstructure:"sensitivekeys,omitempty" json:"sensitivekeys,omitempty"`
	SoftVerify bool   `mapstructure:"softwareverify,omitempty" json:"softwareverify,omitempty"`
}

```

core.xml
``` yaml
    # BCCSP (Blockchain crypto provider): Select which crypto implementation or
    # library to use
    BCCSP:
        Default: SW
        SW:
            # TODO: The default Hash and Security level needs refactoring to be
            # fully configurable. Changing these defaults requires coordination
            # SHA2 is hardcoded in several places, not only BCCSP
            Hash: SHA2
            Security: 256
            # Location of Key Store
            FileKeyStore:
                # If "", defaults to 'mspConfigPath'/keystore
                # TODO: Ensure this is read with fabric/core/config.GetPath() once ready
                KeyStore:
```


## Refer

[关于go的移植性，静态链接和动态链接](https://tonybai.com/2017/06/27/an-intro-about-go-portability/)

> nm -C orderer | grep " U "

[Exploring shared objects in Go](https://blog.ksub.org/bytes/2017/02/12/exploring-shared-objects-in-go/)

[Go Execution Modes](https://docs.google.com/document/d/1nr-TQHw_er6GOQRsF6T43GGhFDelrAP0NqSS_00RgZQ/edit)

[Totally static Go builds](http://blog.wrouesnel.com/articles/Totally%20static%20Go%20builds/)
