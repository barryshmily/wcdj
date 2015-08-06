#ifndef __HMAC_TOOLS_H__
#define __HMAC_TOOLS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <openssl/hmac.h>
#include "Base64.h"

std::string hmac_sha1_base64(std::string &key, std::string &data);
std::string hmac_sha1_hex(std::string &key, std::string &data);

#endif /* __HMAC_TOOLS_H__ */
