/**********************************************************
 * \file logic/tool/CommonTool.cpp
 * \brief 
 * \note 
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#include "fr_public/pub_tool.h"

#include "fr_public/pub_string.h"
#include "boost/regex.hpp"
#include "fr_public/pub_md5.h"
#include "fr_public/pub_rc4.h"

#ifdef WIN32
#ifdef _MFC
#include <afxwin.h>
#else
#include <windows.h>
#endif
#else
#include <unistd.h>
#endif

using namespace std;
using namespace boost;

/*
 * other function
 */
namespace Universal{
#ifdef LINUX
	bool execShellCmd(const string &cmd){
		FILE* fp = NULL;
		if((fp=popen(cmd.c_str(), "r")) != NULL){
			pclose(fp);  fp = NULL;
			return true;
		}
		return false;
	}
#endif

	string getMobileType(const string &mobile){
		string head1 = mobile.substr(0, 1);
		string head2 = mobile.substr(0, 2);
		string head3 = mobile.substr(0, 3);
		string head4 = mobile.substr(0, 4);

		if(head3 == "134" || head3 == "135" || head3 == "136" || head3 == "137" || head3 == "138" || head3 == "139" || head3 == "147" || head3 == "150" || 
			head3 == "151" || head3 == "152" || head3 == "157" || head3 == "158" || head3 == "159" || head3 == "182" || head3 == "183" || head3 == "184" || 
			head3 == "187" || head3 == "188" || head3 == "178" || 
			head4 == "1705"
				){
			return _PTHONETYPE_MOBILE;
		}
		else if(head3 == "130" || head3 == "131" || head3 == "132" || head3 == "155" || head3 == "156" || head3 == "185" || head3 == "186" ||
				 head4 == "1709"){
			return _PTHONETYPE_UNICOM;
		}
		else if(head2 == "05" || head4 == "1700" ||
				 head3 == "133" || head3 == "153" || head3 == "180" || head3 == "181" || head3 == "189" || head3 == "169" || head3 == "177"){
			return _PTHONETYPE_CDMA;
		}
		else{
			return _PTHONETYPE_QT;
		}
		return _STRINGFALSE;
	}


	string findDataFromMap(const map<string, string> &mapData, const string &key){
		if(mapData.find(key) != mapData.end()){
			return mapData.find(key)->second;
		}
		return _STRINGFALSE;
	}

	string md5(const std::string str){
		return MD5(str).md5();
	}

	string md5_16(const std::string str){
		return MD5(str).md5().substr(8,16);
	}

	bool rc4_encrypt(string &content, const string &sKey){
		return rc4_encrypt(const_cast<char*>(content.c_str()), (int32)content.size(), sKey.c_str(), (int32)sKey.size());
	}
	bool rc4_encrypt(Byte* content, uint32 size, const string &sKey){
		RC4_KEY key;
		RC4Init(sKey.c_str(), sKey.size(), &key);
		RC4Works(content, size, &key);
		return true;
	}

#ifndef SWAP_VALUE
	#define SWAP_VALUE(x, y) { t = *x; *x = *y; *y = t; }
#endif
	bool rc4_encrypt(char *content, int32 contentLength, const char* sKey, int32 ketLength){
		char t;
		unsigned char box[256];
		int index = 0;

		//生成对应的Key
		int seed_len = ketLength;
		unsigned char *seed = (unsigned char *)sKey;

		int src_len = contentLength;
		unsigned char *src = (unsigned char *)content;

		int i;
		for(i = 0; i < 256; i++){
			box[i] = i;
		}

		int j = 0;
		for (int i = 0; i < 256; i++) {
			j = (j + seed[i % seed_len] + box[i]) % 256;
			SWAP_VALUE(&box[i], &box[j]);
		}

		int x = 0; int y = 0;
		for (int i = 0; i < src_len; i++) {
			x = 0; y = 0;
			x = (x + 1) % 256;
			y = (box[x] + y) % 256;
			SWAP_VALUE(&box[x], &box[y]);
			index = (box[x] + box[y]) % 256;
			t = *(src + i);
			t ^= box[index];
			*(src + i) = t;
		}
		return true;
	}

	bool myStrEncrypt(std::string &content, const std::string &key){
		if(rc4_encrypt(content, key)){
			return convertBinaryToHexString(content);
		}
		return false;
	}

	bool myStrDecrypt(std::string &content, const std::string &key){
		if(convertHexStringToBinary(content)){
			return rc4_encrypt(content, key);
		}
		return false;
	}

	bool convertBinaryToHexString(string &content){
		auto funcBinaryToStr = [](char c)->char{
			if(0 <= c && c <= 9){
				return c + '0';
			}
			else if(10 <= c && c <= 15){
				return c - 10 + 'A';
			}
			return 'G';
		};

		string tmp;
		for(auto iterContent = content.begin(); iterContent != content.end(); ++iterContent){
			tmp += funcBinaryToStr((*iterContent & 0xF0) >> 4);
			tmp += funcBinaryToStr(*iterContent & 0x0F);
		}
		content = tmp;
		return true;
	}

	bool convertHexStringToBinary(string &content){
		auto funcHexCharToChar = [](char c)->char{
			switch(c){
				case '1': return 1; case '2': return 2; case '3': return 3; case '4': return 4; case '5': return 5; case '6': return 6; case '7': return 7; case '8': return 8; case '9': return 9; 
				case 'A': return 10; case 'B': return 11; case 'C': return 12; case 'D': return 13; case 'E': return 14; case 'F': return 15;
				default: return 0;
			}
		};

		string tmp;
		auto iterContent = content.begin();
		while(iterContent != content.end()){
			char high = iterContent != content.end() ? *(iterContent++) : 0;
			char low = iterContent != content.end() ? *(iterContent++) : 0;
			char data = (funcHexCharToChar(high) << 4) + funcHexCharToChar(low);

			tmp += data;
		}
		content = tmp;
		return true;
	}
}  // namespace : Universal


//
// 字符验证。。。
//
namespace Universal{
	bool checkNumber(const string &number){
		if(number.empty()){ return false; }

		stringstream sin(number);
		double doubleTmp;
		char charTmp;
		if(!(sin >> doubleTmp))
			return false;
		if(sin >> charTmp)
			return false;

		return true;
	}


	bool checkDate(const string &date){
		regex reg("^(?<year>(1[8-9]\\d{2})|([2-9]\\d{3}))(-|/|.|)(((?<month>10|12|0?[13578])(-|/|.|)(?<day> 3[01]|[12][0-9]|0?[1-9]))|((?<month>11|0?[469])(-|/|.|)(?<day>30|[12][0-9]|0?[1-9]))|((?<month>0?[2])(-|/|.|)(?<day>0[1-9]|1[0-9]|2[0-8])))(\\s((?<hour>(0[1-9])|([1-9])|(1[0-2]))\\:(?<min>[0-5][0-9])((\\:)(?<sec>[0-5][0-9]))?(\\s[AM|PM|am|pm]{2,2})?))?$");
		return regex_match(date, reg);
	}


	bool checkMobile(const string &mobile){
		if(mobile.size() != 11){  return false;  }

		string head3 = mobile.substr(0, 3);
		string head4 = mobile.substr(0, 4);
		if(
			head3 == "130" || head3 == "131" || head3 == "132" || head3 == "133" || head3 == "134" || head3 == "135" || head3 == "136" || 
			head3 == "137" || head3 == "138" || head3 == "139" || head3 == "147" || head3 == "150" || head3 == "151" || head3 == "152" || 
			head3 == "153" || head3 == "155" || head3 == "156" || head3 == "157" || head3 == "158" || head3 == "159" || head3 == "169" || 
			head3 == "177" || head3 == "178" || head3 == "180" || head3 == "181" || head3 == "182" || head3 == "183" || head3 == "184" || 
			head3 == "185" || head3 == "186" || head3 == "187" || head3 == "188" || head3 == "189" ||
			head4 == "1700" || head4 == "1705" || head4 == "1709" 
			){ 
			return true;
		}
		return false;
	}

}  // namespace : Universal

namespace Universal{
	void frSleep(unsigned long time){
#ifdef WIN32
		Sleep(time);
#else
		usleep(time * 1000);
#endif
	}

#ifdef LINUX
	void frUSleep(unsigned long time){
		usleep(time * 1000);
	}
#endif
}  // namespace : Universal

namespace Universal{
	void getIp(std::string &ip){
		ip.clear();
#ifdef WIN32
		/*
		// 获得本机主机名  
		char hostname[MAX_PATH] = { 0 };
		gethostname(hostname, MAX_PATH);
		struct hostent FAR* lpHostEnt = gethostbyname(hostname);
		if(lpHostEnt == NULL)
		{
			return DEFAULT_IP;
		}

		// 取得IP地址列表中的第一个为返回的IP(因为一台主机可能会绑定多个IP)  
		LPSTR lpAddr = lpHostEnt->h_addr_list[0];

		// 将IP地址转化成字符串形式  
		struct in_addr inAddr;
		memmove(&inAddr, lpAddr, 4);

		return CString(inet_ntoa(inAddr));
		*/
#else
#endif
	}

#ifdef WIN32
	// #include <windows.h>
	string GBKToUTF8(const string& strGBK){
		string strOutUTF8 = ""; 
		WCHAR * str1;
		int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0); 
		str1 = new WCHAR[n]; 
		MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n); 
		n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
		char * str2 = new char[n]; 
		WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL); 
		strOutUTF8 = str2; 
		delete[]str1; 
		str1 = NULL; 
		delete[]str2;
		str2 = NULL; 
		return strOutUTF8;
	}

	string UTF8ToGBK(const string& strUTF8){
		int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
		unsigned short * wszGBK = new unsigned short[len + 1]; 
		memset(wszGBK, 0, len * 2 + 2); 
		MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUTF8.c_str(), -1, (LPWSTR)wszGBK, len);

		len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
		char *szGBK = new char[len + 1];
		memset(szGBK, 0, len + 1); 
		WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBK, len, NULL, NULL); //strUTF8 = szGBK; 
		std::string strTemp(szGBK); 
		delete[]szGBK; 
		delete[]wszGBK; 
		return strTemp; 
	}
#endif
}  // namespace : Universal



