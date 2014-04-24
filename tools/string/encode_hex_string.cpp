#include <cstdio>
#include <cstring>

int encode_hex_string(const unsigned char *src, int len, unsigned char *dst);

int main(int argc, char **argv)
{
	const unsigned char src[] = "123abc=-)(*.456.";
	unsigned char dst[128]    = {0};

	encode_hex_string(src, sizeof(src), dst);
	printf("src[%s]\ndst[%s]\n", src, dst);

	return 0;
}

int encode_hex_string(const unsigned char *src, int len, unsigned char *dst)
{
	unsigned char szHexTable[] = "0123456789abcdef";

	for (int i = 0; i < len; ++i)
	{
		*dst = szHexTable[(src[i] >> 4) & 0x0f];
		++dst;
		*dst = szHexTable[src[i] & 0x0f];
		++dst;
	}
	*dst = '\0';

	return 0;
}
/* g++ -o encode_hex_string encode_hex_string.cpp 
 *
 * output:
 * src[123abc=-)(*.456.]
 * dst[3132336162633d2d29282a2e3435362e00]
 *
 * */
