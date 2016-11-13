/**********************************************************
 * \file main.cpp
 * \brief 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#include <iostream>

#include "fr_public/pub_define.h"
#include "fr_public/pub_string.h"
#include "fr_public/pub_tool.h"
#include "fr_public/pub_json.h"
#include "fr_public/pub_thread.h"

using namespace std;
using namespace Universal;

class ThreadA : public FrThread{
	public:
		ThreadA():bS(true){}
		virtual ~ThreadA(){
			bS = false;
		}
	private:
		virtual void execute(){
			while(bS){
				cout << " start sleep" << endl;
				frSleep(1000);
			}
		}
		bool bS;
};

int main( int agrc, char **argv ){
	ThreadA thread1;
	thread1.start();
	frSleep(5 * 1000);

	cout << "123" << endl;

	return 0;
}

