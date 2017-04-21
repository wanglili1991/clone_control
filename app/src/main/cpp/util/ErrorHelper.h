#ifndef __ERRORHELPER_H__
#define __ERRORHELPER_H__

#include "I_TennisBase.h"

#define getErrorCode(ballStatLast, ballStatCurrent) ((ballStatLast * 1000) + ballStatCurrent)

#define ERROR_TENNIS_BASE			100000
#define ERROR_INVALID_SERVER		ERROR_TENNIS_BASE + 1
#endif