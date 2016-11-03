/**********************************************************
 * \file logic/tool/Timer.h
 * \note	时间函数：考虑到通用性问题，将使用boost作为底层。
 * \note	window下仅支持毫秒级计时，所有相关函数会讲纳秒级数据设置为0，但并不表示实际运行中的纳秒时间。
 * \note	部分功能下linux下有更好的扩展，所以并非完全使用boost作为基准.
 * \note	所有time_t 和 秒转日期 都是基于 [1970-01-01 00:00:00.000000] 为基准。
 * \note	该文件内的时间函数，不推荐直接使用在可以让用户调整界面中：如需要，则要阅读对应注释，避免已知的一切异常情况。
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#ifndef _Timer_H
#define _Timer_H

#include "pub_tool.h"
#include <ctime>

//
// 时间通用函数。
//
namespace Universal{
	//! \brief	将日期转化为秒数
	//! \note	格式必须正确，否则会产生异常行为。
	//! \note	已确定的格式支持：%Y 年 %m 月 %d 日 %H 时 %M 分 %S 秒
	time_t dateToTime(const string &date, string dateFormat = "%Y/%m/%d %H:%M:%S");
	//! \brief	将秒数转化为字符串。
	//! \note	已确定的格式支持：%Y 年 %m 月 %d 日 %H 时 %M 分 %S 秒
	//! \note	以下行为会导致函数异常（崩溃，结果出错等无法预知的结果）
	//				* 格式排列数据不是按照 ：年月日时分秒排列（但可以缺少某个格式，例如没有月份直接 %Y%d是可以的）
	//				* 分隔符使用中文或'%'，或超过一个字符（可以无分隔符）。
	string formatDateTime(const time_t &second, string dateFormat = "%Y/%m/%d %H:%M:%S");
	time_t getLocalTime();
	time_t getLocalTimeU();
    //! \param[in] dateFormat 要处理的格式.
	string getLocalTime(const string &dateFormat);
	//! \brief	在格式后增加纳秒数，使用[]包含，暂不支持更多格式。
	//! \note	建议格式中包含到秒。
	//! \note	window下仅支持毫秒级计时，使用TimeU纳秒级数据将默认使用0填充（所以window下纳秒级数据没有实际含义）
	//! \param[in] dateFormat 格式仅对秒之前的有效。
	//! \retval	一定会包含秒后的数据。所以如果不需要秒后时间，不要使用该函数。
	string getLocalTimeU(const string &dateFormat);
	//! \brief	补全剩余的时间。默认使用 0 补充。可以选择最高数补全
	//! \note	仅支持  y-m-d h:m:s  格式。
	//! \param[in]	strtime	表示时间的字符串。可以省略部分尾部时间。
	//! \param[in]	useMaxNum	尾部时间的补全方式：true表示使用最大值补全。false使用最小值补全。
	time_t supplementTime(const string &strtime, bool useMaxNum = false);
}  // namespace : Universal


namespace Universal{
	//! \brief	计时类。
	//! \note	精度由系统决定。linux下是us window下是ms。单因计时类本身也需要时间，所以会存在一定误差。
	//! \note	使用方法：
	//				* 构造对象
	//				* 调用start函数。
	//				* 调用stop函数。
	//				* 如果对象希望反复使用，可以使用 start - stop 组合。
	//! \note	扩展能力：
	//				* 对计时的可阅读性进行优化。
	//				* 可以保存自动保存到日志中、或自己执行的路径下。
	//! \note	不足：
	//				* 微妙级的运行时间采集会存在误差(类本身也需要一定运行时间)。
	//				* 保存独立的计时文件，对计时的准确性没有影响，但有IO操作(写文件)，如果计时非常频繁的话，对程序的运行速度有影响。
	//! \todo	一个对象运行时间的总扩展。
	//！\todo	对象的计时次数，以及平均运行时间。
	class TimeCounter{
		public:
			//! \brief	默认的计时 会将信息打印到日志中。
			TimeCounter();
			//! \brief	指定目录，计时信息将统一保存到固定的文件中（每天一个文件）
			TimeCounter(const string &_logDir);
			~TimeCounter();
		public:
			//! \brief	开始计时
			void start();
			//! \brief	结束计时
			void stop(string file =__FILE__, long line =__LINE__);
			//! \brief	从开始计时运行到现在的毫秒数。
			//! \note	window下 us数据都未0.
			time_t getUsTime();
			//! \brief	设置目录()
			void setDirectory(const string &dir);
			//! \brief	设置文件名称。
			//! \note	如果设置文件名为空，则表示将信息打印到日志中。
			void setFileName(const string &fileName);
		private:
		private:
			clock_t			m_Start;
			clock_t			m_Stop;
			string			m_FileName;
			string			m_Dir;
	};
}  // namespace : Universal

#endif


