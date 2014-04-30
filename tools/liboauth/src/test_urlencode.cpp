#include "sig_check.h"

int main(int argc, char **argv)
{
	string strUrl = "http://www.gerryyang.tk/search?k1=v1&k2=v2";

	NS_SIG_CHECK::CSigCheck sig_check;
	string strUrlencode = sig_check.url_encode(strUrl);

	cout << "strUrl[" << strUrl << "] " << endl;
	cout << "strUrlencode[" << strUrlencode << "] " << endl;

	return 0;
}
/* output:
 * strUrl[http://www.gerryyang.tk/search?k1=v1&k2=v2] 
 * strUrlencode[http%3A%2F%2Fwww.gerryyang.tk%2Fsearch%3Fk1%3Dv1%26k2%3Dv2]
 * */
