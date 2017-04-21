#ifndef __HANDLEDATAS_HAND_H__
#define __HANDLEDATAS_HAND_H__

#include "HandleDatas.h"

class HandleProxy;

class HandleDatasHand
        : public HandleDatas {
public:
    HandleDatasHand(HandleProxy *pHandleProxy)
            : HandleDatas(pHandleProxy) {
    }

public:
    virtual int getMaxSpeed();

    virtual int getAverageSpeed();

    virtual int getNetTimes();

    virtual int getOutTimes();

    virtual int getDownPoints(BallPoint *pPoint, int size);

    virtual int getHitPoints(BallPoint *pPoint, int size);
};

#endif
