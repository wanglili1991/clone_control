#ifndef __HANDLESTAT_H__
#define __HANDLESTAT_H__

#include "../HandleProxy.h"

class HandleProxy;

struct DataItem;

class HandleStat {
public:
    HandleStat(HandleProxy *pHandleProxy)
            : m_pHandleProxy(pHandleProxy) {
        m_ballDirection = BALLDIRECTION_INVALID;
    }

public:
    bool onBall(BallStat stat, const Ball &ball);

    void setBallDirection(const Ball &ball);

    BallDirection getBallDirection();

    bool isMoveToTrainedSide();

public:
    virtual int getLatestSpeed();

    virtual bool onHandleNull(const Ball &ball);

    virtual bool onHandleHit(const Ball &ball);

    virtual bool onHandleServe(const Ball &ball);

    virtual bool onHandleTouchDown(const Ball &ball);

    virtual bool onHandleTouchNet(const Ball &ball);

    virtual bool onHandleTouchAny(const Ball &ball);

    virtual bool onHandleOut(const Ball &ball);

    virtual bool onHandleNonUniqueness(const Ball &ball);

    virtual bool onHandleError(const Ball &ball);

    virtual bool onHandleIn(const Ball &ball);

    virtual bool onHandleHitGuess(const Ball &ball);

    virtual bool onHandleErrorHit(const Ball &ball);

public:
    virtual ~HandleStat() {
    }

    virtual void reset() {
        memset(&m_ballLast, 0, sizeof(m_ballLast));
        memset(&m_dataItem, 0, sizeof(m_dataItem));
    }

    DrillResult getLatestResult() {
        return m_dataItem.drillResult;
    }

    const DataItem &getDataItem() {
        return m_dataItem;
    }

protected:
    HandleProxy *m_pHandleProxy;

    DataItem m_dataItem;

    Ball m_ballLast;
    BallDirection m_ballDirection;
};

#endif
