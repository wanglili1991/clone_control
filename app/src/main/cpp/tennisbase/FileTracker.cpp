#include "FileTracker.h"

#include <thread>
#include <iostream>

#ifdef __ANDROID__
#include <unistd.h>
#endif

using namespace std;

FileTracker::FileTracker( void )
	: m_uRefCount(0)
{
}

FileTracker::~FileTracker( void )
{
}

bool FileTracker::init(const InitParam &initParam)
{
	m_initParam = initParam;
    return m_fileGrabber.init(m_initParam);
}

bool FileTracker::start()
{
    std::thread t1(&FileTracker::nextThread, this);
    t1.detach();

    return true;
}

bool FileTracker::pause()
{
	return true;
}

bool FileTracker::resume()
{
	return true;
}

bool FileTracker::stop()
{
    return true;
}

bool FileTracker::runOnce()
{
    std::lock_guard<std::mutex> lock_guard(m_mutexFileGrabber);
    if ( !m_fileGrabber.next( m_ball, m_ballStat ) )
        return false;

    cout << "position: (" << m_ball.x << "," << m_ball.y << "," << m_ball.z << ")" << endl;
    cout << "speed: " << m_ball.speed << endl;
    cout << "timestamp:" << m_ball.timestamp << endl;
    cout << "state: " << m_ballStat << endl;
    onBall( m_ballStat, m_ball );

    return true;
}

bool FileTracker::restart(CourtArea serveArea)
{
    std::thread t1(&FileTracker::restartThread, this);
    t1.detach();

    return true;
}

void FileTracker::drillNextThread()
{
#ifdef __ANDROID__
#ifdef __TENNIS_BASE_TEST__
    usleep(100);
#else
    sleep(1);
#endif
#endif

	if (onDrillNext != NULL)
	{
		onDrillNext();
	}
}

void FileTracker::restartThread() {
    std::lock_guard<std::mutex> lock_guard(m_mutexFileGrabber);

    if (onThread != NULL) {
        onThread(false);
    }

    if ((m_initParam.playModel & PLAYMODEL_MASK) != PLAYMODEL_MATCH_SINGLE
        && (m_initParam.playModel & PLAYMODEL_MASK) != PLAYMODEL_MATCH_DOUBLE) {
        if (!m_fileGrabber.init(m_initParam))
            return;

        drillNextThread();
        start();
    } else {
        if (!m_fileGrabber.init(m_initParam))
            return;

        start();
    }

    if (onThread != NULL) {
        onThread(true);
    }
}

void FileTracker::nextThread()
{
    std::lock_guard<std::mutex> lock_guard(m_mutexFileGrabber);

	if (onThread != NULL)
	{
		onThread(false);
	}

	Ball ball;
	BallStat ballStat = BALLSTAT_NULL;
	while (m_fileGrabber.next(ball, ballStat))
	{
		cout << "position: (" << ball.x << "," << ball.y << "," << ball.z << ")" << endl;
		cout << "speed: " << ball.speed << endl;
		cout << "timestamp:" << ball.timestamp << endl;
		cout << "state: " << ballStat << endl;

		if (onBall != NULL)
		{
			onBall(ballStat, ball);
		}
	}

	if (onThread != NULL)
	{
		onThread(true);
	}
}

static FileTracker *s_pFileTracker = NULL;

extern "C" I_TennisBase* createTennisBaseInterface()
{
	if (s_pFileTracker == NULL)
	{
		s_pFileTracker = new FileTracker();
		if (s_pFileTracker == NULL)
			return NULL;
	}
	
	return s_pFileTracker;
}
