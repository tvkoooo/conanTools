#ifndef __MYLOG_H__
#define __MYLOG_H__




#include "stdint.h"
#include <stdarg.h>
#include <memory>
#include <mutex>

#define CMYCHAR_CAP 1024

#define CMYFILE_NAME_MAX_SIZE (128)
#define CMyLog_MAX_SIZE (1024 * 2)
#define CMyLog_FILE_NAME ("mylog/mylog.log")

#define CMyLog_LV_DEBUG 1
#define CMyLog_LV_INFO 2
#define CMyLog_LV_WARN 3
#define CMyLog_LV_ERR 4
#define CMyLog_LV_FAIL 5
#define CMyLog_LV_LIMIT CMyLog_LV_INFO

#define __FILENAME__ (strrchr(__FILE__, '\\') ? (strrchr(__FILE__, '\\') + 1):__FILE__)
#define CMyLog_DEBUG CMyLog(CMyLog_LV_DEBUG,CMyLog_FILE_NAME,__FILENAME__,__LINE__).onWrite
#define CMyLog_INFO CMyLog(CMyLog_LV_INFO,CMyLog_FILE_NAME,__FILENAME__,__LINE__).onWrite
#define CMyLog_WARN CMyLog(CMyLog_LV_WARN,CMyLog_FILE_NAME,__FILENAME__,__LINE__).onWrite
#define CMyLog_ERR CMyLog(CMyLog_LV_ERR,CMyLog_FILE_NAME,__FILENAME__,__LINE__).onWrite
#define CMyLog_FAIL CMyLog(CMyLog_LV_FAIL,CMyLog_FILE_NAME,__FILENAME__,__LINE__).onWrite


#define CMYINSLOG_CONFIG(log_limit,file_name) CMyInsLog::instance().config(file_name,log_limit)
#define CMyInsLog_DEBUG(format, ...) CMyInsLog::instance().log(__FILENAME__,__LINE__,CMyLog_LV_DEBUG, format , ##__VA_ARGS__)
#define CMyInsLog_INFO(format, ...) CMyInsLog::instance().log(__FILENAME__,__LINE__,CMyLog_LV_INFO, format , ##__VA_ARGS__)
#define CMyInsLog_WARN(format, ...) CMyInsLog::instance().log(__FILENAME__,__LINE__,CMyLog_LV_WARN, format , ##__VA_ARGS__)
#define CMyInsLog_ERR(format, ...) CMyInsLog::instance().log(__FILENAME__,__LINE__,CMyLog_LV_ERR, format , ##__VA_ARGS__)
#define CMyInsLog_FAIL(format, ...) CMyInsLog::instance().log(__FILENAME__,__LINE__,CMyLog_LV_FAIL, format , ##__VA_ARGS__)




class FRAMEWORK_TOOL_API CMyLog
{
public:
	CMyLog(int32_t log_lv, const char* file_name, const char* file,int32_t line);
	~CMyLog() {}

	void onWrite(const char* format, ...);
private:
	CMyLog() {}
	int32_t _log_lv = CMyLog_LV_ERR;
	const char* _file_name = CMyLog_FILE_NAME;
	const char* _file = NULL;
	int32_t _line = 0;
};




class FRAMEWORK_TOOL_API CMyChar
{
public:
	CMyChar();
	~CMyChar();

	//用来拼接字符 使用  
	void clearn();
	void append(const char* format, ...);
	void vappend(const char* format, va_list args);
	const char* c_str()const { return _log_data; }
private:
	char*					_log_data = NULL;
	int32_t					_log_len = 0;
	int32_t					_multi = 0;
	char                    _log_base[CMYCHAR_CAP];
};



class FRAMEWORK_TOOL_API CMyInsLog
{
public:
	CMyInsLog();
	~CMyInsLog();
	static CMyInsLog& instance();
	bool config(const char* file_name, int32_t log_limit = 0);
	bool log(const char* file, int32_t line, int log_lv, const char* format, ...);

private:
	int32_t					_log_limit = CMyLog_LV_ERR;
	const char*				_file_name = CMyLog_FILE_NAME;
	FILE*					_fd = NULL;

	struct tm*				_open_time = NULL;
};






#endif //__MYLOG_H__
