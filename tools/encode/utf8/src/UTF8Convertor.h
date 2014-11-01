#ifndef _UTF8CONVERTOR_H
#define _UTF8CONVERTOR_H
#if defined(_WIN32)
	#include <iconv.h.win32>
#else
	#include <iconv.h>
#endif
#include <string>
class UTF8Convertor{
public:
	UTF8Convertor(const char *localcharset);
	virtual ~UTF8Convertor();
	::std::string ToUTF8(const ::std::string& instr);
	::std::string UTF8To(const ::std::string& instr);
	static int GBK2UNICODE(const ::std::string& instr,unsigned char* unicode, int& len);
	bool m_ready;
private:
	iconv_t	m_to_utf8_conv;
	iconv_t m_utf8_to_conv;
	::std::string m_utf8;
	::std::string m_local;
};
#endif

