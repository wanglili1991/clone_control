#ifndef __ALOXHELPER_H__
#define __ALOXHELPER_H__

#ifdef __ANDROID_LOG_OFF__
#define ALOX_DBG_LOG_OFF

#include <stddef.h>
#define assert(msg)

class TextFileLogger
{
public:
	TextFileLogger()
	{
	}

	~TextFileLogger()
	{

	}
};

#define Log_RemoveLogger( ... ) 
#define Log_Prune( ... )
#define Log_Verbose( ... )
#define Log_Info( ... )
#define Log_Warning( ... )
#define Log_Error(...)
#define Log_Assert( ... )

#else

#ifndef __ANDROID__
#define ALIB_DEBUG_OFF
#define ALIB_DEBUG_GLIB_OFF
#define ALIB_DEBUG_STRINGS_OFF
#define ALIB_FEAT_BOXING_FTYPES_ON
#define ALIB_FEAT_BOXING_STD_VECTOR_ON
#define ALIB_FEAT_SINGLETON_MAPPED_OFF
#define ALIB_FEAT_THREADS_ON
#define ALOX_DBG_LOG_ON
#define ALIB_DBG_LOG_CI_ON
#define ALIB_REL_LOG_ON
#define ALIB_REL_LOG_CI_OFF
#endif

#include "alox/alox.hpp"
#include "alox/loggers/textfilelogger.hpp"

using namespace aworx;

#endif

class ALoxHelper
{
public:
	/**
	* 创建内测版本的日志
	* @param szFileName 日志文件名
	* @return 成功返回 日志指针，否则返回 NULL
	* @caution
	* 使用完成后，需要调用以下方法释放
	* Log_RemoveLogger(m_pTextFileLogger);
	* Log_Prune(delete m_pTextFileLogger; )
	* m_pTextFileLogger = NULL;
	*/
	static TextFileLogger *CreateDbgTextFileLogger(const char* szFileName, const char *szDomain = "TEXTFILE_TEST");

	/**
	* 创建发布版本的日志
	* @param szFileName 日志文件名
	* @return 成功返回 日志指针，否则返回 NULL
	* @caution
	* 使用完成后，需要调用以下方法释放
	* Lox_RemoveLogger(m_pTextFileLogger);
	* Lox_Prune(delete m_pTextFileLogger; )
	* m_pTextFileLogger = NULL;
	*/
	static TextFileLogger *CreateRelTextFileLogger(const char* szFileName, const char *szDomain = "TEXTFILE_TEST");
};

#endif