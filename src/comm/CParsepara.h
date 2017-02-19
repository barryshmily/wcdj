#ifndef __PARSEPARA_H__
#define __PARSEPARA_H__

#include <vector>
#include <string>
#include <stdlib.h>
#include <string.h>

namespace wcdj
{ 
	namespace util
	{

		/*
		 * CSpacePara class is used to parse A=B or A[tab]B, etc. 
		 * */
		class CSpacePara {
		public:
			CSpacePara() { _split = " ,#\t\r\n"; }
			CSpacePara(const std::string& s) : _split(s) {}
			~CSpacePara() {}
		public:
			/*
			 * decode each para to _paralist
			 * */
			void decode(const std::string& s);
			/*
			 * to get all paras
			 * */
			const std::vector<std::string>& get_pairs() const { return _paralist; }

		public: 
			/*
			 * to set split, and default is " ,#\t\r\n"
			 * */
			void set_splitchar(const std::string& s = " ,#\t\r\n") { _split = s; }

			/*
			 * check c is split or not
			 * */
			bool is_split(unsigned char c);


		private:
			std::string _split;
			std::vector<std::string> _paralist;
		};
	};
}

#endif


