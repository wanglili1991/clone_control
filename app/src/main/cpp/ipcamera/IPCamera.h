#ifndef __IPCAMERA_H__
#define __IPCAMERA_H__

#include "I_IPCamera.h"
#include "NetDEVSDK.h"

#include "ALoxHelper.h"

#include <string>

class IPCamera
    : public I_IPCamera
{
public:
    IPCamera();
    virtual ~IPCamera();

    bool TENNIS_STDCALL init( const char *tmpPath, const char *ip, short port, const char *szUserName, const char *szPassword );

	bool TENNIS_STDCALL startRecord( const char *szSavePath );
	bool TENNIS_STDCALL stopRecord();

	void TENNIS_STDCALL dispose();

private:
    LPVOID m_pLoginToken;
    LPVOID m_pRealPlayToken;

    TextFileLogger *m_pTextFileLogger;

    std::string m_strSavePath;
};

#endif
