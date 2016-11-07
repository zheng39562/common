/**********************************************************
 * \file logic/tool/CommonTool.cpp
 * \brief 
 * \note 
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#include "pub_tool.h"

#include "pub_string.h"
#include "boost/regex.hpp"
#include "pub_md5.h"

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace std;
using namespace boost;

/*
 * other function
 */
namespace Universal{
#ifndef WIN32
	bool execShellCmd( const string &cmd ){
		FILE* fp = NULL;
		if( (fp=popen(cmd.c_str(), "r" ) ) != NULL ){
			pclose( fp);  fp = NULL;
			return true;
		}
		return false;
	}
#endif

	string getMobileType( const string &mobile ){
		string head1 = mobile.substr( 0, 1 );
		string head2 = mobile.substr( 0, 2 );
		string head3 = mobile.substr( 0, 3 );
		string head4 = mobile.substr( 0, 4 );

		if( head3 == "134" || head3 == "135" || head3 == "136" || head3 == "137" || head3 == "138" || head3 == "139" || head3 == "147" || head3 == "150" || 
			head3 == "151" || head3 == "152" || head3 == "157" || head3 == "158" || head3 == "159" || head3 == "182" || head3 == "183" || head3 == "184" || 
			head3 == "187" || head3 == "188" || head3 == "178" || 
			head4 == "1705"
				){
			return _PTHONETYPE_MOBILE;
		}
		else if( head3 == "130" || head3 == "131" || head3 == "132" || head3 == "155" || head3 == "156" || head3 == "185" || head3 == "186" ||
				 head4 == "1709" ){
			return _PTHONETYPE_UNICOM;
		}
		else if( head2 == "05" || head4 == "1700" ||
				 head3 == "133" || head3 == "153" || head3 == "180" || head3 == "181" || head3 == "189" || head3 == "169" || head3 == "177" ){
			return _PTHONETYPE_CDMA;
		}
		else{
			return _PTHONETYPE_QT;
		}
		return _STRINGFALSE;
	}


	string findDataFromMap( const map<string, string> &mapData, const string &key ){
		if( mapData.find(key) != mapData.end() ){
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
}  // namespace : Universal


//
// 字符验证。。。
//
namespace Universal{
	bool checkNumber( const string &number ){
		if( number.empty() ){ return false; }

		stringstream sin(number);
		double doubleTmp;
		char charTmp;
		if( !(sin >> doubleTmp) )
			return false;
		if( sin >> charTmp )
			return false;

		return true;
	}


	bool checkDate( const string &date ){
		regex reg("^(?<year>(1[8-9]\\d{2})|([2-9]\\d{3}))(-|/|.|)(((?<month>10|12|0?[13578])(-|/|.|)(?<day> 3[01]|[12][0-9]|0?[1-9]))|((?<month>11|0?[469])(-|/|.|)(?<day>30|[12][0-9]|0?[1-9]))|((?<month>0?[2])(-|/|.|)(?<day>0[1-9]|1[0-9]|2[0-8])))(\\s((?<hour>(0[1-9])|([1-9])|(1[0-2]))\\:(?<min>[0-5][0-9])((\\:)(?<sec>[0-5][0-9]))?(\\s[AM|PM|am|pm]{2,2})?))?$");
		return regex_match( date, reg );
	}


	bool checkMobile( const string &mobile ){
		if( mobile.size() != 11 ){  return false;  }

		string head3 = mobile.substr( 0, 3 );
		string head4 = mobile.substr( 0, 4 );
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

	void frUSleep(unsigned long time){
#ifdef WIN32
#else
		usleep(time * 1000);
#endif
	}
}  // namespace : Universal



