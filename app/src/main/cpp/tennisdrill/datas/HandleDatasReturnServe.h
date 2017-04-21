#ifndef __HANDLEDATAS_RETURNSERVE_H__
#define __HANDLEDATAS_RETURNSERVE_H__

#include "HandleDatas.h"

class HandleProxy;

class HandleDatasReturnServe
        : public HandleDatas {
public:
    HandleDatasReturnServe(HandleProxy *pHandleProxy)
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

#endif // RETURNSERVEMODEL_H__
