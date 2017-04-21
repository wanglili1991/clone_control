#ifndef __FILEITEM_H__
#define __FILEITEM_H__

#include "I_TennisBase.h"

class FileItem
{
public:
    FileItem(Ball ball, BallStat ballStat);
    ~FileItem();

public:
    Ball     getBall();
    BallStat getBallStat();

private:
    Ball m_ball;
    BallStat m_ballStat;
};

#endif