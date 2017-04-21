#ifndef __HANDLESTAT_HAND_H__
#define __HANDLESTAT_HAND_H__

#include "HandleStat.h"

class HandleProxy;

class HandleStatHand
        : public HandleStat {
public:
    HandleStatHand(HandleProxy *pHandleProxy)
            : HandleStat(pHandleProxy) {
        reset();
    }

    virtual int getLatestSpeed();

    virtual void reset() {
        HandleStat::reset();

        memset(&m_lastBall, 0, sizeof(Ball));
        memset(&m_beforeLastBall, 0, sizeof(Ball));
        m_lastBallStat = BALLSTAT_WAITING;
        m_beforeLastBallStat = BALLSTAT_WAITING;
        m_isReady = false;
    }

    bool updateFinalFailedState() {
        // 不在范围内
        bool isReady = m_isReady;
        reset();
        if (isReady) {
            m_dataItem.drillResult = ER_OUT;
        } else {
            m_dataItem.drillResult = ER_INVALID;
        }

        return true;
    }

    virtual bool onHandleServe(const Ball &ball);

    virtual bool onHandleTouchNet(const Ball &ball);

    virtual bool onHandleHit(const Ball &ball);

    virtual bool onHandleTouchDown(const Ball &ball);

    virtual bool onHandleOut(const Ball &ball);

    virtual bool onHandleIn(const Ball &ball);

    virtual bool onHandleHitGuess(const Ball &ball);

private:
    BallStat m_lastBallStat;
    Ball m_lastBall;
    BallStat m_beforeLastBallStat;
    Ball m_beforeLastBall;
    bool m_isReady;    // 标示喂球是否成功
};


#endif
