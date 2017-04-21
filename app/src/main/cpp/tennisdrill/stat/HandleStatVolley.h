#ifndef __HANDLESTAT_VOLLEY_H__
#define __HANDLESTAT_VOLLEY_H__

#include "HandleStat.h"

class HandleProxy;

class HandleStatVolley
        : public HandleStat {
public:
    HandleStatVolley(HandleProxy *pHandleProxy)
            : HandleStat(pHandleProxy) {
        reset();
    }

public:
    virtual int getLatestSpeed();

    virtual bool onHandleHit(const Ball &ball);

    virtual bool onHandleTouchDown(const Ball &ball);

    virtual bool onHandleOut(const Ball &ball);
};

#endif // VOLLEYMODEL_H__
