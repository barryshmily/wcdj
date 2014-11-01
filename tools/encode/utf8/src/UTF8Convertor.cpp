#include <string>
//#include "win32linux.h"
#if defined(_WIN32)
	#include <malloc.h>
	#include <iconv.h.win32>
	#define _my_allocator malloc
	#define _my_free(p) free(p)
#else
	#include <alloca.h>
	#define _my_allocator alloca
	#define _my_free(p)  
#endif

#include "UTF8Convertor.h"

UTF8Convertor::UTF8Convertor(const char *localcharset)
{
	m_ready = false;
	//m_to_utf8_conv = iconv_open("UTF-8", localcharset);
	m_to_utf8_conv = iconv_open("UTF-8//IGNORE", localcharset);
	m_utf8_to_conv = iconv_open(localcharset, "UTF-8");
	if( (m_to_utf8_conv == (iconv_t)-1) || (m_to_utf8_conv == (iconv_t)-1) ){
		return;
	}
	m_ready = true;
}

UTF8Convertor::~UTF8Convertor()
{
	if(m_ready){
		iconv_close(m_to_utf8_conv);
		iconv_close(m_utf8_to_conv);
		m_ready = false;
	}
}

::std::string UTF8Convertor::ToUTF8(const ::std::string& instring)
{
	m_utf8 = "";

	if(!m_ready) return m_utf8;

	//reset.
	iconv(m_to_utf8_conv, NULL, NULL, NULL, NULL);

	const char * instr = instring.c_str();
	size_t	inlen = instring.size();
	size_t maxlen = inlen*2;
	char *	outstr = (char*)_my_allocator(maxlen+16);
	if(!outstr) return m_utf8;
	
	char * pout = outstr;
	size_t	outlen = maxlen;
	size_t	ret_conv = 0;
#if defined(_WIN32)
	ret_conv = iconv(m_to_utf8_conv, &instr, &inlen, &outstr, &outlen);
#else
	ret_conv = iconv(m_to_utf8_conv, (char**)&instr, &inlen, &outstr, &outlen);
#endif
	if(outlen<0) outlen = 0;
	pout[maxlen-outlen] = '\0';
	m_utf8 = pout;
	_my_free(pout);
	return m_utf8;
}

::std::string UTF8Convertor::UTF8To(const ::std::string& instring)
{
	m_local = "";
	if(!m_ready) return m_local;
	//reset.
	iconv(m_utf8_to_conv, NULL, NULL, NULL, NULL);

	const char * instr = instring.c_str();
	size_t	inlen = instring.size();
	size_t maxlen = inlen*4;
	char *	outstr = (char*)_my_allocator(maxlen+16);
	if(!outstr) return m_local;

	char * pout = outstr;
	size_t	outlen = maxlen;
	size_t	ret_conv = 0;

#if defined(_WIN32)
	ret_conv = iconv(m_utf8_to_conv, &instr, &inlen, &outstr, &outlen);
#else
	ret_conv = iconv(m_utf8_to_conv, (char**)&instr, &inlen, &outstr, &outlen);
#endif
	if(outlen<0) outlen = 0;
	*((unsigned long*)(&(pout[maxlen-outlen]))) = 0;
	m_local = pout;
	_my_free(pout);
	return m_local;
}

int UTF8Convertor::GBK2UNICODE(const std::string& instring, unsigned char* unicode,int& len)
{
	iconv_t h = iconv_open("UNICODE","GBK");
	if (h ==(iconv_t)-1)
		return -1;	

	char * instr =(char*) instring.c_str();
	size_t	inlen = instring.size();
	size_t maxlen = inlen*4+16;
	if (maxlen > (size_t)len)
	{
		maxlen = len;
		inlen = (len-16)/4;
	}

	char * p = (char*)unicode;
	size_t	outlen =maxlen;// maxlen;
	size_t	ret_conv = 0;

#if defined(_WIN32)
	ret_conv = iconv(h, &instr, &inlen, &outstr, &outlen);
#else
	ret_conv = iconv(h, &instr, &inlen, &p, &outlen);
#endif
	len = maxlen-outlen;
	_my_free(pout);
	iconv_close(h);
	return 0;
}
