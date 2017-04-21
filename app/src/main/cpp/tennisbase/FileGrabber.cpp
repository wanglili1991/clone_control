#include "FileGrabber.h"
#include "inih/INIReader.h"

#include <iostream>
#include <ctime>
#include <thread>
#include <stdlib.h>

#ifdef __ANDROID__
#include <unistd.h>
#endif

using namespace std;

FileGrabber::FileGrabber( void )
	: m_index(0)
	, m_interval(0)
{
}

FileGrabber::~FileGrabber( void )
{
}

bool FileGrabber::init(const InitParam &initParam)
{
	if (!loadConfig(initParam))
		return false;

	return loadContent();
}

bool FileGrabber::next( Ball &ball, BallStat &ballStat )
{
	if (m_index == m_fileItems.size())
		return false;

	if (m_index > 0) {
#ifdef __ANDROID__
#ifdef __TENNIS_BASE_TEST__
		usleep(m_interval);
#else
		sleep(m_interval);
#endif
#endif
	}  

	time_t timestamp;
	time(&timestamp);

	ball = m_fileItems[m_index].getBall();
    ball.timestamp = (long long)timestamp;
    if (m_interval == 0 )
    {
        ball.timestamp += m_index * 200;
    }

	ballStat = m_fileItems[m_index].getBallStat();

	m_index++;

    return true;
}

bool FileGrabber::loadConfig(const InitParam &initParam)
{
	INIReader reader(initParam.strCfgPath + "config.cfg");
	int error = reader.ParseError();
	if (error< 0)
		return false;

	m_interval = reader.GetInteger("mixed", "interval", 1);
    string eventID = reader.Get("mixed", "event_id", "D001");
    m_filePath = initParam.strTmpPath + eventID + ".txt";

	cout << "eventID: " << eventID << endl;
	cout << "interval: " << m_interval << endl;
	cout << "filePath: " << m_filePath << endl;

    return true;
}

bool FileGrabber::loadContent()
{
	FILE *fp;
	if (NULL == (fp = fopen(m_filePath.c_str(), "r")))
		return false;

	m_fileItems.clear();

	int state;
	int x, y, z;
	float speed;
	while (!feof(fp))
	{
		char buffer[100];
		fgets(buffer, 100, fp);

		const char *dims = " ,\r\n";
		char *pToken = strtok(buffer, dims);
		if (pToken == NULL)
			continue;

		x = (int)atof(pToken);
		pToken = strtok(NULL, dims);
		if (pToken == NULL)
			continue;

		y = (int)atof(pToken);
		pToken = strtok(NULL, dims);
		if (pToken == NULL)
			continue;

		z = (int)atof(pToken);
		pToken = strtok(NULL, dims);
		if (pToken == NULL)
			continue;

		state = atoi(pToken);
		pToken = strtok(NULL, dims);
		if (pToken != NULL)
		{
			speed = (float)atof(pToken);
			pToken = strtok(NULL, dims);
		}
		else
		{
			speed = 155;
		}

		Ball ball; BallStat ballStat;
		ball.x = x;
		ball.y = y;
		ball.z = z;
		ball.speed = (int)speed;

		switch (state)
		{
		case 0: ballStat = BALLSTAT_NULL; break;
		case 1: ballStat = BALLSTAT_HIT; break;
		case 2: ballStat = BALLSTAT_SERVE; break;
		case 3: ballStat = BALLSTAT_TOUCH_DOWN; break;
		case 4: ballStat = BALLSTAT_TOUCH_NET; break;
		case 5: ballStat = BALLSTAT_TOUCH_ANY; break;
		case 6: ballStat = BALLSTAT_OUT; break;
		case 7: ballStat = BALLSTAT_NON_UNIQUENESS; break;
		case 8: ballStat = BALLSTAT_ERROR; break;
		case 9: ballStat = BALLSTAT_IN; break;
		case 10: ballStat = BALLSTAT_HIT_GUESS; break;
		default: ballStat = (BallStat)state; break;
		}

		FileItem fileItem = FileItem(ball, ballStat);
		m_fileItems.push_back(fileItem);
	}

	fclose(fp);
	if (m_fileItems.size() == 0)
		return false;

	m_index = 0;
	
	return true;
}