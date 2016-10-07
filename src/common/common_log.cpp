/**********************************************************
 *  \!file common_log.cpp
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#include "common_log.h"

#include <unistd.h>
#include <iostream>
#include <fstream>
#include "tool/common_file.h"
#include "tool/common_tool.h"

std::ostringstream os31415926_tmp;

using namespace std;
using namespace Universal;
namespace Universal{

	LogServer::LogServer()
		:m_Path(),
		 m_FileName(),
		 m_Cache(),
		 m_MCache()
	{
		;
	}

	LogServer::~LogServer(){
		;
	}

	void LogServer::InitLog(const string &path, const string &fileName){
		m_FileName = fileName;
		m_Path = completePath(path);
	}

	void LogServer::writeLog(const string &time, const eLogLevel &level, const string &funcName, const long &line, const string &msg){
		m_MCache.lock();
		m_Cache << "[" << time << "]"
			<< "[" << pthread_self() << "]"
			<< "[" << level << "]"
			<< "[" << funcName << "]"
			<< "[" << line << "]"
			<< " : " << msg << "\n";
		m_MCache.unlock();
	}

	void LogServer::execute(){
		string curDate;
		ofstream outfile;
		while(1){
			usleep(10 * 1000); // 10ms

			if(!m_FileName.empty() && !m_Path.empty()){
				if(curDate.empty() || curDate != getLocalTime("%Y%m%d")){
					curDate = getLocalTime("%Y%m%d");
					outfile.close();
					outfile.open(string(m_Path + m_FileName + "_" + curDate + ".log").c_str(), ios::app);
				}

				m_MCache.lock();
				if(outfile){
					outfile << m_Cache.str();
				}
				m_Cache.str(""); // clear cache
				m_MCache.unlock();
			}
		}
		outfile.close( );
	}
}

void Log_D(const string &msg, string funcName, long line){
	SingleLogServer::getInstance()->writeLog(getLocalTimeU("%D %T"), eLogLevel_Debug, funcName, line, msg);
}

void Log_I(const string &msg, string funcName, long line){
	SingleLogServer::getInstance()->writeLog(getLocalTimeU("%D %T"), eLogLevel_Info, funcName, line, msg);
}

void Log_W(const string &msg, string funcName, long line){
	SingleLogServer::getInstance()->writeLog(getLocalTimeU("%D %T"), eLogLevel_Warning, funcName, line, msg);
}

void Log_E(const string &msg, string funcName, long line){
	SingleLogServer::getInstance()->writeLog(getLocalTimeU("%D %T"), eLogLevel_Error, funcName, line, msg);
}

void Log_C(const string &msg, string funcName, long line){
	SingleLogServer::getInstance()->writeLog(getLocalTimeU("%D %T"), eLogLevel_Crash, funcName, line, msg);
}
