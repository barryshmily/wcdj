#ifndef _BASE64_H
#define _BASE64_H

namespace WCDJ 
{
	/** 
	  @brief Encode data buffer to base64 string.
	  @param sSrc: Source data buffer.
	  @param sTarget: To store decode string.
	  @param nSize: The length of the data buffer.
	  @return The length of target string.
	  */
	int EncodeBase64(const unsigned char *sSrc, unsigned char *sTarget, const int iTargetSize, const int nSize);

	/** 
	  @brief Decode base64 string to data.
	  @param sSrc: Source base64 string.
	  @param sTarget: To store data.
	  @param nSize: The length of the data buffer.
	  @return The length of target data.
	  */
	int DecodeBase64(const unsigned char *sSrc, unsigned char *sTarget, const int iTargetSize, const int nSize);

}

#endif /* _BASE64_H */

