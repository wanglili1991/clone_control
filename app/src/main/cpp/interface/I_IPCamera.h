#ifndef __INTERFACE_IPCAMERA_H__
#define __INTERFACE_IPCAMERA_H__

#include "I_TennisBase.h"

struct I_IPCamera
{
	// 接口层，所有路径的编码，统一采用UTF-8
    virtual bool TENNIS_STDCALL init(const char *tmpPath, const char *ip, short port, const char *szUserName, const char *szPassword ) = 0;

	virtual bool TENNIS_STDCALL startRecord(const char *szSavePath) = 0;
	virtual bool TENNIS_STDCALL stopRecord() = 0;

	virtual void TENNIS_STDCALL dispose() = 0;

protected:
	virtual ~I_IPCamera()
	{

	}
};

#endif