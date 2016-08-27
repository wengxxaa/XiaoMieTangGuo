/*!
  @file  		GC_Tool.cpp
  @brief  		GC_Tool.cpp
  @details 		（快捷工具）
  @date 		 2013/08/16
  @author 		 Tyson
 */
#include "GC_Tool.h"
#include "cocos2d.h"
#include <stdlib.h>
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <sys/timeb.h>
#endif
USING_NS_CC;

#define    RAND_LIMIT    32767
#define MAX_STRING_LEN (1024*100)

static string formatToString(const char* format, ...){
	
	string ret;
	va_list ap;
	va_start(ap, format);
	char* pBuf = (char*)malloc(MAX_STRING_LEN);
	if (pBuf != NULL)
	{
		vsnprintf(pBuf, MAX_STRING_LEN, format, ap);
		ret = pBuf;
		free(pBuf);
	}
	va_end(ap);
	return ret;
}
GC_Tool::~GC_Tool(){	
	
}
void GC_Tool::releaseSelf()
{
	CC_SAFE_DELETE(mInstance);
}
//获得单例
GC_Tool *  GC_Tool::sharedTool()
{
	static GC_Tool instance;
	if (NULL == mInstance)
	{
		mInstance = &instance;
		srand(time(NULL));//重载随机数种子
	}
	return &instance;
}

long long GC_Tool::millisecondNow()
{
	//struct timeval now;
	//cocos2d::gettimeofday(&now,NULL);
	////CCLOG(" tv_sec  = %ld ",now.tv_sec);
	//return (now.tv_sec * 1000 + now.tv_usec / 1000);
	struct timeval tv;
	gettimeofday(&tv,NULL);

	return ((long long)tv.tv_sec * 1000 + tv.tv_usec / 1000);

}

int GC_Tool::secondNow(){
	time_t now_time;
	now_time = time(NULL);
	return now_time;
}


string GC_Tool::getDate()
{
	string d = "";
	struct tm *tm;
	time_t tmep ;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	time(&tmep);
	tm =localtime(&tmep);	
#else
	struct timeval now;
	gettimeofday(&now,NULL);
	tm =localtime(&now.tv_sec);	
#endif	
	int year = tm->tm_year +1900;
	d.append((intToString(year) ).c_str());
	int month = tm->tm_mon +1;
	if(month<10)
	{
		d.append(("0"+intToString(month) ).c_str());
	}else
	{
		d.append((intToString(month)).c_str());
	}
	int day = tm->tm_mday;
	if(day < 10)
	{
		d.append(("0" + intToString(day)) .c_str());
	}else
	{
		d.append(intToString(day) .c_str());
	}	
	return d;
}


string GC_Tool::longToString(long data)
{
	return formatToString("%ld",data);
}
long GC_Tool::stringToLong(string data)
{
	
	return atol(data.c_str());
}

string GC_Tool::boolToString(bool data)
{
	if(data)
	{
		return string("true");
	}
	else
	{
		return string("false");
	}
}

bool GC_Tool::stringToBool(string data)
{
	if (data.length() == 0)
	{
		return false;
	}

	if (0 == strcmp(data.c_str(), "0") || 0 == strcmp(data.c_str(), "false"))
	{
		return false;
	}
	return true;
}

//long long 转string
string GC_Tool::longLongToString( long long data)
{
	return formatToString("%lld",data);
}
//string转long long
 long long GC_Tool::stringToLongLong(string data)
{
	
	long long l = 0;
	sscanf(data.c_str(), "%lld", &l);	
	return l;
}
/*int 转 String*/
string GC_Tool::intToString(int data)
{
	std::string ret;
	char* pBuf = (char*)malloc(MAX_STRING_LEN);
	if(pBuf != NULL){
		sprintf(pBuf,"%d",data);
		ret = pBuf;
		free(pBuf);
	}
	return ret;
}

int GC_Tool::stringToInt(string data)
{
	return atoi(data.c_str());
}

float GC_Tool::stringTofloat(string str)
{
	return atof(str.c_str());
}


float GC_Tool::randomFloat(float lo,float hi){
	float r = (float)(std::rand() & (RAND_LIMIT));
	r /= RAND_LIMIT;
	r = (hi - lo) * r + lo;
	return r;
}

float GC_Tool::randomFloat(){
	float r = (float)(std::rand() & (RAND_LIMIT));
	r /= RAND_LIMIT;
	r = 2.0f * r - 1.0f;
	return r;
}

bool GC_Tool::random_bool( float rate )
{
	if (rate<=0)
		return false;
	if(rate>=1)
		return true;
	srand(time(0));
	int i = rand()%100;//0-99
	int seprate = (int)(100.0f * rate);
	if (i<seprate)
		return true;
	else
		return false;
}

int GC_Tool::randomInt()
{
	return std::rand();
}

int GC_Tool::randomInt( int lo, int hi )
{
	return std::rand() % (hi-lo+1) + lo;
}

//utf8字符长度1-6，可以根据每个字符第一个字节判断整个字符长度
//0xxxxxxx
//110xxxxx 10xxxxxx
//1110xxxx 10xxxxxx 10xxxxxx
//11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
//111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
//1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
//
//定义查找表，长度256，表中的数值表示以此为起始字节的utf8字符长度
unsigned char utf8_look_for_table[] = 
{
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 1, 1
};

#define UTFLEN(x)  utf8_look_for_table[(x)]

//计算str字符数目
int GC_Tool::getUtf8Length(const char *str)
{
	int clen = strlen(str);
	int len = 0;

	for(char *ptr = (char*)str; 
		*ptr!=0&&len<clen; 
		len++, ptr+=UTFLEN((unsigned char)*ptr));

	return len;
}

//get子串
string GC_Tool::subUtfString(const char *str, unsigned int start, unsigned int end)
{
	int len = getUtf8Length(str);

	if((int)start >= len) return NULL;
	if((int)end > len) end = len;

	char *sptr = (char*)str;
	for(int i = 0; i < (int)start; ++i,sptr+=UTFLEN((unsigned char)*sptr));

	char *eptr = sptr;
	for(int i = start; i < (int)end; ++i,eptr += UTFLEN((unsigned char)*eptr));

	int retLen = eptr - sptr;
	char *retStr = (char*)malloc(retLen+1);
	memcpy(retStr, sptr, retLen);
	retStr[retLen] = 0;
	string ret = retStr;
	free(retStr);
	return ret;
}
//获取串中asc字符长度
unsigned int GC_Tool::getAscLength(const char* str){
	int i =0;
	unsigned char b = *(str);
	unsigned int length = 0;
	while(b!='\0' && b!='\n'){
		if(b<128){//（0-127）
			length++;
		}
		b=*(str+(++i));
	}
	return length;
}

/*!
  @brief  	截取字符串（并保证utf8中文字符的完整）
  @details  	
  @param  	str 输入的串
  @param	length 截取的长度
  @return 	返回值说明
 */
string GC_Tool::subStringAdvance(const char* str, unsigned int length){
	if(!str)
		return "";
	unsigned int ascLen = getAscLength(str);
	unsigned int chLen = getUtf8Length(str)-ascLen;
	unsigned int retLen = length;
	if(chLen>0 && length>3){//包含中文字符时
		retLen = length - (length-ascLen)%3;
	}
	char *retStr = (char*)malloc(retLen+1);
	memcpy(retStr, str, retLen);
	retStr[retLen] = 0;
	string ret = retStr;
	free(retStr);
	return ret;
}

/*!
  @brief  	以应用的规则获取字符长度
  @details  	（英文和数字是1个字符，汉字两个字符）
  @param  	aParam 参数说明
  @return 	返回值说明
 */
unsigned int GC_Tool::getStringLengthAdvance( const char* str )
{
	if(!str)
		return 0;
	int length = getUtf8Length(str);
	int ascLength = getAscLength(str);
	int chLength = length-ascLength;
	return chLength*2+ascLength;
}

// void GC_Tool::purgeTool()
// {
// 	CC_SAFE_DELETE(mInstance);
// }

Time4f GC_Tool::Time4fFromSeconds(int seconds)
{
	Time4f t;

	t.d=seconds/(60*60*24);
	t.h=seconds%(60*60*24)/(60*60);
	t.m=seconds%(60*60*24)%(60*60)/60;
	t.s=seconds%(60*60*24)%(60*60)%60;

	return t;
}

std::string GC_Tool::convertTimeToString(time_t t)
{
	struct tm *time_str=gmtime(&t);
	char buf[20];

	strftime(buf, 20, "%a, %d.%m.%Y %H:%M:%S", time_str);

	return std::string(buf);
}

time_t GC_Tool::convertStringToTime(const std::string& str)
{
	struct tm time_str;
	//     strptime(str.c_str(), "%a, %d.%m.%Y %H:%M:%S", &time_str);
	return mktime(&time_str);
}

std::string GC_Tool::convertSecondsToHours(int seconds)
{
	int d=seconds/(60*60*24);
	int h=seconds%(60*60*24)/(60*60);
	int m=seconds%(60*60*24)%(60*60)/60;
	int s=seconds%(60*60*24)%(60*60)%60;

	if (d>0)
		return formatToString("%dd%dh",d,h);
	else if (h>0)
		return formatToString("%dh%dm",h,m);
	else if (m>0)
		return formatToString("%dm%ds",m,s);
	else
		return formatToString("%ds",s);
}


GC_Tool* GC_Tool:: mInstance;














