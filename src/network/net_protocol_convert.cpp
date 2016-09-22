/**********************************************************
 *  \!file net_protocol_convert.cpp
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#include "net_protocol_convert.h"

namespace Network{
	void convertMsgToPacker(std::string &msg, MPackerQueue &packerQueue){
		if(parseMsg(msg, startIndex, endIndex)){
			;
		}
	}
	void convertPackerToMsg(const ConstPackerPtr &pPacker, MMsgQueue &msgQueue){
		;
	}
}