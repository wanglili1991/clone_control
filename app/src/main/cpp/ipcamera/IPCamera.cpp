#include "IPCamera.h"

#include <android/log.h>

#define LOG_TAG "JNI"
#define SDKLOG(level,fmt,...)  __android_log_print(level, LOG_TAG, "[L%u][%s]: " fmt, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define SDKLOGV(fmt, ...) SDKLOG(ANDROID_LOG_VERBOSE, fmt, ##__VA_ARGS__)
#define SDKLOGD(fmt, ...) SDKLOG(ANDROID_LOG_DEBUG, fmt, ##__VA_ARGS__)
#define SDKLOGI(fmt, ...) SDKLOG(ANDROID_LOG_INFO, fmt, ##__VA_ARGS__)
#define SDKLOGE(fmt, ...) SDKLOG(ANDROID_LOG_ERROR, fmt, ##__VA_ARGS__)

#undef Log_Error
#define Log_Error SDKLOGE

IPCamera::IPCamera()
    : m_pLoginToken( NULL )
    , m_pRealPlayToken( NULL )
    , m_pTextFileLogger( NULL )
{
}

IPCamera::~IPCamera()
{
	//NETDEV_SetLogPath
	//NETDEV_GetLastError
	// NETDEV_Reboot

	NETDEV_CloseSound(m_pRealPlayToken);

	if (m_pRealPlayToken != NULL)
	{
		NETDEV_StopRealPlay(m_pRealPlayToken);
		m_pRealPlayToken = NULL;
	}

	if (m_pLoginToken != NULL)
	{
		NETDEV_Logout(m_pLoginToken);
		m_pLoginToken = NULL;
	}

	NETDEV_Cleanup();

	if (m_pTextFileLogger != NULL)
	{
		Log_RemoveLogger(m_pTextFileLogger);

		Log_Prune(delete m_pTextFileLogger; )
			m_pTextFileLogger = NULL;
	}
}

bool IPCamera::init( const char *tmpPath, const char *ip, short port, const char *szUserName, const char *szPassword )
{
    if ( ( ip == NULL ) || ( *ip == '\0' ) )
        return false;

    if ( ( szUserName == NULL ) || ( *szUserName == '\0' ) )
        return false;

    if ( ( szPassword == NULL ) || ( *szPassword == '\0' ) )
        return false;

    std::string strTmpPath(tmpPath);
    m_pTextFileLogger = ALoxHelper::CreateDbgTextFileLogger( (strTmpPath + "IPCamera.txt").c_str() );
    if ( !NETDEV_Init() )
    {
        Log_Error( "Failed to call NETDEV_Init(), NETDEV_GetLastError = 0x%08x", NETDEV_GetLastError() );
        return false;
    }

    std::string strIP( ip );
    std::string strUserName( szUserName );
    std::string strPassword( szPassword );
    NETDEV_DEVICE_INFO_S ntDeviceInfo = { 0 };
    m_pLoginToken = NETDEV_Login( &strIP[0], port, &strUserName[0], &strPassword[0], &ntDeviceInfo );
    if ( m_pLoginToken == NULL )
    {
        Log_Error( "Failed to call NETDEV_Login(), NETDEV_GetLastError = 0x%08x", NETDEV_GetLastError() );
        return false;
    }

    NETDEV_PREVIEWINFO_S ntPreviewInfo = { 0 };
    ntPreviewInfo.dwChannelID  = 1;
    ntPreviewInfo.dwStreamType = NETDEV_LIVE_STREAM_INDEX_MAIN;
    ntPreviewInfo.dwLinkMode   = NETDEV_TRANSPROTOCAL_RTPTCP;
    m_pRealPlayToken = NETDEV_RealPlay( m_pLoginToken, &ntPreviewInfo, NULL, 0 );
    if ( m_pRealPlayToken == NULL )
    {
        Log_Error( "Failed to call NETDEV_RealPlay(), NETDEV_GetLastError = 0x%08x", NETDEV_GetLastError() );
        return false;
    }

    if ( !NETDEV_OpenSound( m_pRealPlayToken ) )
    {
        Log_Error( "Failed to call NETDEV_OpenSound(), NETDEV_GetLastError = 0x%08x", NETDEV_GetLastError() );
    }

    if ( !NETDEV_SoundVolumeControl( m_pRealPlayToken, NETDEV_MIC_SOUND_MAX_VALUE ) )
    {
        Log_Error( "Failed to call NETDEV_SoundVolumeControl(), NETDEV_GetLastError = 0x%08x", NETDEV_GetLastError() );
    }

    return true;
}

void IPCamera::dispose()
{
    delete this;
}

bool IPCamera::startRecord( const char *szSavePath )
{
    if ( ( szSavePath == NULL ) || ( *szSavePath == '\0' ) )
        return false;

    if ( m_pRealPlayToken == NULL )
        return false;

    if ( m_strSavePath.empty() )
    {
        m_strSavePath = szSavePath;
    }
    else if ( m_strSavePath.compare( szSavePath ) == 0 )
    {
        Log_Error( "Failed to call startRecord(), because you used the same save path." );
        return false;
    }
    else
    {
        m_strSavePath = szSavePath;
    }

    if ( !NETDEV_SaveRealData( m_pRealPlayToken, &m_strSavePath[0], NETDEV_MEDIA_FILE_MP4 ) )
    {
        Log_Error( "Failed to call NETDEV_SaveRealData(), NETDEV_GetLastError = 0x%08x", NETDEV_GetLastError() );
        return false;
    }

    return true;
}

bool IPCamera::stopRecord()
{
    if ( m_pRealPlayToken == NULL )
        return false;

    if ( !NETDEV_StopSaveRealData( m_pRealPlayToken ) )
    {
        Log_Error( "Failed to call NETDEV_SaveRealData() first time, NETDEV_GetLastError = 0x%08x", NETDEV_GetLastError() );
        if ( !NETDEV_StopSaveRealData( m_pRealPlayToken ) )
        {
            Log_Error( "Failed to call NETDEV_SaveRealData() second time, NETDEV_GetLastError = 0x%08x", NETDEV_GetLastError() );
            return false;
        }
    }

    return true;
}

