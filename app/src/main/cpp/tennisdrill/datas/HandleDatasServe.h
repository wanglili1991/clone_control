#ifndef __HANDLEDATAS_SERVE_H__
#define __HANDLEDATAS_SERVE_H__

#include "HandleDatas.h"

class HandleProxy;

class HandleDatasServe
        : public HandleDatas {
public:
    HandleDatasServe(HandleProxy *pHandleProxy)
            : HandleDatas(pHandleProxy) {
    }

public:
    virtual int getMaxSpeed();

    virtual int getAverageSpeed();

    virtual int getNetTimes() { return 0; };

    virtual int getOutTimes() { return 0; };

    virtual int getDownPoints(BallPoint *pPoint, int size);

    virtual int getHitPoints(BallPoint *pPoint, int size);
};

#endif // SERVEMODEL_H__
