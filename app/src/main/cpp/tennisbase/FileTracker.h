#ifndef __FILETRACKER_H__
#define __FILETRACKER_H__

#include <mutex>
#include "I_TennisBase.h"
#include "FileGrabber.h"

using namespace std;

class FileTracker
    : public I_TennisBase
{
public:
    FileTracker( void );
    virtual ~FileTracker( void );

private:
	virtual unsigned int TENNIS_STDCALL addRef()
	{
		return ++m_uRefCount;
	}

	virtual unsigned int TENNIS_STDCALL releaseRef()
	{
		if (--m_uRefCount == 0)
		{
			delete this;
			return 0;
		}

		return m_uRefCount;
	}

	virtual bool TENNIS_STDCALL init(const InitParam &initParam);            
	virtual bool TENNIS_STDCALL start();                    
	virtual bool TENNIS_STDCALL pause();                     
	virtual bool TENNIS_STDCALL resume();             
	virtual bool TENNIS_STDCALL stop();                     
	virtual bool TENNIS_STDCALL runOnce();                    

	virtual bool TENNIS_STDCALL restart(CourtArea serveArea);

    void restartThread();
	void nextThread();
    void drillNextThread();

private:
	unsigned int m_uRefCount;

	InitParam m_initParam;

    Ball m_ball;
    BallStat m_ballStat;

    FileGrabber m_fileGrabber;

    std::mutex m_mutexFileGrabber;
};

#endif