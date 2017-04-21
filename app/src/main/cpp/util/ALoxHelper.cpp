#include "ALoxHelper.h"

#ifndef ALOX_DBG_LOG_OFF
extern String64 autoSizes;

// globals
String64 autoSizes;

#ifdef __TENNIS_BOUT_MODULE__
#undef __TENNIS_MODULE_NAME__
#define __TENNIS_MODULE_NAME__ ("TennisBout")
#elif defined(__TENNIS_DRILL_MOUDULE__)
#undef __TENNIS_MODULE_NAME__
#define __TENNIS_MODULE_NAME__ ("TennisDrill")
#elif defined(__TENNIS_DRILL_MOUDULE__)
#undef __TENNIS_MODULE_NAME__
#define __TENNIS_MODULE_NAME__ ("TennisBase")
#elif defined(__TENNIS_IPCAMERA_MOUDULE__)
#undef __TENNIS_MODULE_NAME__
#define __TENNIS_MODULE_NAME__ ("IPCamera")
#else
#undef __TENNIS_MODULE_NAME__
#define __TENNIS_MODULE_NAME__ ("TennisUnknown")
#endif

Lox_Prune(Lox s_relaseLox(__TENNIS_MODULE_NAME__);)
#define LOX_LOX s_relaseLox
#endif

TextFileLogger *ALoxHelper::CreateDbgTextFileLogger(const char* szFileName, const char *szDomain)
{
	TextFileLogger *pTextFileLogger = NULL;

#ifndef ALOX_DBG_LOG_OFF
	Log_AddDebugLogger();
	Log_Prune(Log::DebugLogger->AutoSizes.Import(Substring(autoSizes), CurrentData::Keep); )

	Log_SetDomain(szDomain, Scope::Method);

	Log_Prune(pTextFileLogger = new TextFileLogger(szFileName));
	Log_SetVerbosity(pTextFileLogger, Verbosity::Verbose);
	Log_SetVerbosity(pTextFileLogger, Verbosity::Error, ALox::InternalDomains);
#endif
	return pTextFileLogger;
}

TextFileLogger *ALoxHelper::CreateRelTextFileLogger(const char* szFileName, const char *szDomain)
{
	TextFileLogger *pTextFileLogger = NULL;
#ifndef ALOX_DBG_LOG_OFF
	Lox_Prune(pTextFileLogger = new TextFileLogger(szFileName); )

#if !ALOX_REL_LOG_CI
	Lox_Prune(pTextFileLogger->MetaInfo->Format = "[%TC+%TL][%tN]%V[%D]%A1(%#): "; )
#endif

	Lox_SetVerbosity(pTextFileLogger, Verbosity::Verbose);
	Lox_SetVerbosity(pTextFileLogger, Verbosity::Error, ALox::InternalDomains);
#endif

	return pTextFileLogger;
}