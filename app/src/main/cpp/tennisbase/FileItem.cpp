#include "FileItem.h"

FileItem::FileItem( Ball ball, BallStat ballStat )
{
    m_ball     = ball;
    m_ballStat = ballStat;
}

FileItem::~FileItem()
{
}

Ball FileItem::getBall()
{
    return m_ball;
}

BallStat FileItem::getBallStat()
{
    return m_ballStat;
}