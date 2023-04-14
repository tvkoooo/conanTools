#include <stdio.h>
#include "mylog.h"
#include <time.h>
#include<fcntl.h>

#define NEED_TRY_TIMES 10


static char* s_log_show[] =
{
	"",						//0		
	"DEBUG",				//1		  CMyLog_LV_DEBUG 1
	"INFO",					//2		  CMyLog_LV_INFO 2
	"WARN",					//3		  CMyLog_LV_WARN 3
	"ERR",					//4		  CMyLog_LV_ERR 4
	"FAIL",					//5		  CMyLog_LV_FAIL 5
	"",						//n		
};




CMyLog::CMyLog(int32_t log_lv, const char* file_name, const char* function, int32_t line)
{ 
	_log_lv = log_lv;
	_file_name = file_name;
	_file = function;
	_line = line;
}

void CMyLog::onWrite(const char* format, ...)
{
	if (_log_lv < CMyLog_LV_LIMIT)
	{
		return;
	}
	FILE* fd;
	fd = fopen(_file_name, "a+");
	if (fd == NULL) 
	{
		return;
	}
	char _log[CMyLog_MAX_SIZE];
	va_list args;
	va_start(args, format);
	vsnprintf(_log , CMyLog_MAX_SIZE, format, args);
	va_end(args);
	time_t current_time;
	struct tm* now_time;
	time(&current_time);
	now_time = localtime(&current_time);
	fprintf(fd, "%04d/%02d/%02d %02d:%02d:%02d %s %s(%d) %s\r\n", now_time->tm_year + 1900, now_time->tm_mon+1, now_time->tm_mday
		, now_time->tm_hour, now_time->tm_min, now_time->tm_sec
		, s_log_show[_log_lv], _file, _line, _log);
	fflush(fd);
	fclose(fd);
}


CMyChar::CMyChar()
{
	memset(_log_base,0, CMYCHAR_CAP);
	_log_data = _log_base;
	_multi = 1;
	_log_len = 0;
}
CMyChar::~CMyChar()
{
	if (_log_data != _log_base)
	{
		delete[]_log_data;
	}
}

void CMyChar::clearn()
{
	if (_log_data != _log_base)
	{
		delete[]_log_data;
	}
	memset(_log_base, 0, CMYCHAR_CAP);
	_log_data = _log_base;
	_multi = 1;
	_log_len = 0;
}
void CMyChar::append(const char* format, ...)
{
	int32_t ret = 0;
	int32_t try_times = NEED_TRY_TIMES;
	for (; try_times > 0; )
	{
		va_list args;
		va_start(args, format);
		ret = vsnprintf(_log_data + _log_len, CMYCHAR_CAP * _multi - _log_len, format, args);
		va_end(args);

		if (ret <= 0 || ret > CMYCHAR_CAP * _multi - _log_len)//溢出
		{
			_multi *= 2;
			char* new_data = new char[CMYCHAR_CAP * _multi];
			if (_log_len > 0)
			{
				memcpy(new_data, _log_data, _log_len );
			}
			memset(new_data + _log_len,0, CMYCHAR_CAP * _multi - _log_len);

			if (_log_data != _log_base)
			{
				delete[]_log_data;
			}
			_log_data = new_data;
		}
		else
		{
			_log_len += ret;
			try_times = 0;
		}
	}
}
void CMyChar::vappend(const char* format, va_list args)
{
	int32_t ret = 0;
	int32_t try_times = NEED_TRY_TIMES;
	for (; try_times > 0; )
	{
		ret = vsnprintf(_log_data + _log_len, CMYCHAR_CAP * _multi - _log_len, format, args);

		if (ret <= 0 || ret > CMYCHAR_CAP * _multi - _log_len)//溢出
		{
			_multi *= 2;
			char* new_data = new char[CMYCHAR_CAP * _multi];
			if (_log_len > 0)
			{
				memcpy(new_data, _log_data, _log_len);
			}
			memset(new_data + _log_len , 0, CMYCHAR_CAP * _multi - _log_len);

			if (_log_data != _log_base)
			{
				delete[]_log_data;
			}
			_log_data = new_data;
		}
		else
		{
			_log_len += ret;
			try_times = 0;
		}
	}

}






std::mutex some_mutex;
CMyInsLog::CMyInsLog()
{
	_log_limit = CMyLog_LV_ERR;
	_file_name = CMyLog_FILE_NAME;
	_fd = NULL;
	_open_time = NULL;
}
CMyInsLog::~CMyInsLog()
{
	_log_limit = CMyLog_LV_ERR;
	_file_name = CMyLog_FILE_NAME;
	_fd = NULL;
	_open_time = NULL;
}

CMyInsLog& CMyInsLog::instance()
{
	static CMyInsLog s_log;
	return s_log;
}
bool CMyInsLog::config(const char* file_name, int32_t log_limit )
{
	if (log_limit > 0)
	{
		_log_limit = log_limit;
	}

	if (_fd != NULL)
	{
		if (file_name != _file_name)//换文件
		{
			std::lock_guard<std::mutex> guard(some_mutex);
			fclose(_fd);//关掉旧文件
			_fd = NULL;
		}
	}

	time_t current_time;
	time(&current_time);
	_open_time = localtime(&current_time);

	_file_name = file_name;
	char c_name[CMYFILE_NAME_MAX_SIZE] = {0};
	snprintf(c_name, CMYFILE_NAME_MAX_SIZE,"%s_%02d%02d%02d.log", _file_name, _open_time->tm_year + 1900, _open_time->tm_mon + 1, _open_time->tm_mday);

	std::lock_guard<std::mutex> guard(some_mutex);
	_fd = fopen(c_name, "a+");
	if (_fd == NULL)
	{
		return false;
	}
	return true;
}


bool CMyInsLog::log(const char* file, int32_t line, int log_lv, const char* format, ...)
{
	if (log_lv < _log_limit)
	{
		return true;
	}
	if (_fd == NULL)
	{
		return false;
	}

	time_t current_time;
	struct tm* now_time;
	time(&current_time);
	now_time = localtime(&current_time);

	CMyChar mychar;
	mychar.append("%04d/%02d/%02d %02d:%02d:%02d %s %s(%d) ", now_time->tm_year + 1900, now_time->tm_mon + 1, now_time->tm_mday
		, now_time->tm_hour, now_time->tm_min, now_time->tm_sec, s_log_show[log_lv], file, line);

	va_list args;
	va_start(args, format);
	mychar.vappend(format, args);
	va_end(args);

	if (now_time->tm_mday != _open_time->tm_mday || now_time->tm_mon != _open_time->tm_mon || now_time->tm_year != _open_time->tm_year)
	{

		char c_name[CMYFILE_NAME_MAX_SIZE] = { 0 };
		snprintf(c_name, CMYFILE_NAME_MAX_SIZE, "%s_%02d%02d%02d.log", _file_name, _open_time->tm_year + 1900, _open_time->tm_mon + 1, _open_time->tm_mday);

		std::lock_guard<std::mutex> guard(some_mutex);
		fclose(_fd);//关掉旧文件
		_fd = NULL;
		_fd = fopen(c_name, "a+");
		if (_fd == NULL)
		{
			return false;
		}
	}
	std::lock_guard<std::mutex> guard(some_mutex);
	fprintf(_fd, "%s",mychar.c_str());
	fflush(_fd);
	return true;
}