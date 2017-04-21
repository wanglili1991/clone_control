#ifndef __HANDLESTAT_RETURNSERVE_H__
#define __HANDLESTAT_RETURNSERVE_H__

#include "HandleStat.h"

class HandleProxy;

class HandleStatReturnServe
        : public HandleStat {
public:
    HandleStatReturnServe(HandleProxy *pHandleProxy)
            : HandleStat(pHandleProxy) {
        reset();
    }

    virtual int getLatestSpeed();

    virtual bool onHandleServe(const Ball &ball);

    virtual bool onHandleTouchNet(const Ball &ball);

    virtual bool onHandleHit(const Ball &ball);

    virtual bool onHandleTouchDown(const Ball &ball);

    virtual bool onHandleOut(const Ball &ball);

    virtual bool onHandleHitGuess(const Ball &ball);
};

#endif // RETURNSERVEMODEL_H__
