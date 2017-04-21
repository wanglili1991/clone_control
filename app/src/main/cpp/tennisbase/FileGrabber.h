#ifndef __FILEGRABBER_H__
#define __FILEGRABBER_H__

#include "FileItem.h"

#include <string>
#include <vector>

using namespace std;

class FileGrabber
{
public:
    FileGrabber( void );
    ~FileGrabber( void );

	bool init(const InitParam &initParam);
    bool next(Ball &ball, BallStat &ballStat);

private:
    bool loadConfig(const InitParam &initParam);
	bool loadContent();

private:
	size_t m_index;
    long m_interval;

	string m_filePath;
	vector<FileItem> m_fileItems;
};

#endif