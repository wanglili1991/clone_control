#ifndef __HANDLESTAT_SERVE_H__
#define __HANDLESTAT_SERVE_H__

#include "HandleStat.h"

class HandleProxy;

class HandleStatServe
        : public HandleStat {
public:
    HandleStatServe(HandleProxy *pHandleProxy)
            : HandleStat(pHandleProxy) {
        reset();
    }

public:
    virtual int getLatestSpeed();

    virtual bool onHandleServe(const Ball &ball);

    virtual bool onHandleTouchNet(const Ball &ball);

    virtual bool onHandleTouchDown(const Ball &ball);

    virtual bool onHandleOut(const Ball &ball);

    virtual bool onHandleHit(const Ball &ball);
};

#endif // SERVEMODEL_H__
