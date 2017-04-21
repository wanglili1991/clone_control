#ifndef __TENNISDRILL_JNI_IMPL_H__
#define __TENNISDRILL_JNI_IMPL_H__

#include "TennisDrill.h"
#include "TennisBaseJNI.h"

class TennisDrillJNIImpl
        : I_DrillCallback {
public:
    TennisDrillJNIImpl();

    ~TennisDrillJNIImpl();

    bool init(JNIEnv *env, jobject thiz, jobject pInitParam, jobject pCallback, jint drillDifficulty);

public:
    virtual bool init(const InitParam &initParam, I_DrillCallback *pCallback, DrillDifficulty drillDifficulty);

    virtual bool start();

    virtual bool pause();

    virtual bool resume();

    virtual bool stop();

    virtual bool runOnce();

    virtual bool restart(CourtArea serveArea);

    virtual void onBegin();

    virtual void onDataChange(const DrillBoxScore &drillBoxScore);

    virtual void onSpeedChange(int speed);

    virtual void onEnd();

    virtual void onDrillNext();

    virtual void onError(int errorCode, const char *pErrorMessage);

    virtual void onThread(bool isExit);

private:
    JNIEnv *getJNIEnv();

    bool setDrillCallback(JNIEnv *env, jobject pCallback);

public:
    struct JNI_DrillCallback {
        jmethodID onBegin;
        jmethodID onDataChange;
        jmethodID onSpeedChange;
        jmethodID onEnd;
        jmethodID onDrillNext;
        jmethodID onError;
    };

private:
    JNIEnv *m_pJNIEnv;
    jobject m_pJNIthiz;
    jobject m_pJNICallback;

    TennisDrill *m_pTennisDrill;
    JNI_DrillCallback m_drillCallback;
};

#endif