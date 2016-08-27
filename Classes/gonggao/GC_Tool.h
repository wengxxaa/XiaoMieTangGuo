#ifndef _GC_TOOL__
#define _GC_TOOL__

#include <string>
#include <ctime>

using namespace std;

struct Time4f
{
	Time4f()
		:d(0)
		,h(0)
		,m(0)
		,s(0)
	{};

	int d;
	int h;
	int m;
	int s;
};

class GC_Tool{
public:
	GC_Tool(){}
	~GC_Tool();
	static void releaseSelf();
	static GC_Tool* sharedTool();
//	static void purgeTool();
	//返回毫秒级时间
	long long millisecondNow();
	int secondNow();
		
	//返回年月日时间 xxxxxxxx  
	string getDate();

	/*int 转string*/
	string intToString(int data);
	/*string转int*/
	int stringToInt(string data);//
	
	float stringTofloat(string str);
	//long long 转string
	string longLongToString( long long data);

	//string转long long
	 long long stringToLongLong(string data);

	 string longToString(long data);
	 long stringToLong(string data);

	string boolToString(bool data);
	bool stringToBool(string data);

	float randomFloat(float lo,float hi);
	float randomFloat();
	bool random_bool(float rate);
	int randomInt(int lo, int hi);

	int randomInt();
	int getUtf8Length(const char *str);

	string subUtfString(const char *str, unsigned int start, unsigned int end);
	//获得字符串中asc字符长度
	unsigned int getAscLength(const char* str);
	//（英文和数字是1个字符，汉字两个字符）
	unsigned int getStringLengthAdvance(const char* str);
	//截取字符串并保证中文字符的完整
	string subStringAdvance(const char* str, unsigned int length);



	Time4f Time4fFromSeconds(int seconds);

	std::string convertSecondsToHours(int seconds);

	std::string convertTimeToString(time_t t);
	time_t convertStringToTime(const std::string&);

private:
	static GC_Tool*  mInstance;	
	
};
#define GC_STRING_2_INT(str)		(GC_Tool::sharedTool()->stringToInt(str))
#define	GC_INT_2_STRING(integer)	(GC_Tool::sharedTool()->intToString(integer))
#define GC_STRING_2_BOOL(str)		(GC_Tool::sharedTool()->stringToBool(str))
#define	GC_BOOL_2_STRING(integer)	(GC_Tool::sharedTool()->boolToString(integer))

#endif//_GC_TOOL__