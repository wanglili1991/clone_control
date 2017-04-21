#ifndef __TENNISDRILL_H
#define __TENNISDRILL_H

#include "HandleProxy.h"

class TennisDrill
        : public I_TennisDrill {
public:
    TennisDrill();

    virtual ~TennisDrill();

public:
    virtual bool TENNIS_STDCALL init(const InitParam &initParam, I_DrillCallback *pDrillCallback, DrillDifficulty drillDifficulty);

public:
    virtual bool TENNIS_STDCALL init(const InitParam &initParam);

    virtual bool TENNIS_STDCALL start();

    virtual bool TENNIS_STDCALL pause();

    virtual bool TENNIS_STDCALL resume();

    virtual bool TENNIS_STDCALL stop();

    virtual bool TENNIS_STDCALL runOnce();

    virtual bool TENNIS_STDCALL restart(CourtArea serveArea);

public:
    bool restartDrill(CourtArea courtArea);

private:
    void initCallback();

    bool initHandler(const InitParam &initParam, I_DrillCallback *pDrillCallback, DrillDifficulty drillDifficulty);

public:
    virtual unsigned int TENNIS_STDCALL addRef() {
        return ++m_uRefCount;
    }

    virtual unsigned int TENNIS_STDCALL releaseRef() {
        if (--m_uRefCount == 0) {
            delete this;
            return 0;
        }

        return m_uRefCount;
    }

private:
    unsigned int m_uRefCount;

    I_TennisBase *m_pTennisBase;
    TextFileLogger *m_pTextFileLogger;

    HandleProxy *m_pHandlerA;
    HandleProxy *m_pHandlerB;

    //bool m_bBilateral = false;           //为双边练做准备，暂时不用

    bool m_bStart;
    bool m_isBaseInitialized;
    bool m_isDrillNext;
};

#endif

