#ifndef __HANDLEDATAS_VOLLEY_H__
#define __HANDLEDATAS_VOLLEY_H__

#include "HandleDatas.h"

class HandleProxy;

class HandleDatasVolley
        : public HandleDatas {
public:
    HandleDatasVolley(HandleProxy *pHandleProxy)
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

#endif // VOLLEYMODEL_H__
