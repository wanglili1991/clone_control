#ifndef __TENNISBOUT_JNI_IMPL_H__
#define __TENNISBOUT_JNI_IMPL_H__

#include "TennisBout.h"
#include "TennisBaseJNI.h"

class TennisBoutJNIImpl
        : I_BoutCallback {
public:
    TennisBoutJNIImpl();

    ~TennisBoutJNIImpl();

    bool init(JNIEnv *env, jobject thiz, jobject pInitParam, jobject pCallback);

public:
    virtual bool init(const InitParam &initParam, I_BoutCallback *pCallback);

    virtual bool ballPower(BallPower ballPower);

    virtual bool start();

    virtual bool pause();

    virtual bool resume();

    virtual bool stop();

    virtual bool runOnce();

    virtual bool restart(CourtArea serveArea);

    virtual void onBall(const Ball &ball);

    virtual void onBallAce(Player playerServe);

    virtual void onServe(const Ball &ballServe);

    virtual void onServeFailed();

    virtual void onServeSuccessful();

    virtual void onServeTouchDownPosition(Player player, PlayerServe serveType, const Ball &ballServeTouchDown);

    virtual void onHitPosition(Player playerHit, const Ball &ballHit);

    virtual void onAddScore(Player player, const MatchBoxScore &matchBoxScoreA, const MatchBoxScore &matchBoxScoreB, const Ball &ball);

    virtual void onError(int code, const char *errorMessage);

    virtual void onThread(bool isExit);

private:
    JNIEnv *getJNIEnv();

    bool setBoutCallback(JNIEnv *env, jobject pCallback);

public:
    struct JNI_BoutCallback {
        jmethodID onBall;
        jmethodID onBallAce;
        jmethodID onServe;
        jmethodID onServeFailed;
        jmethodID onServeSuccessful;
        jmethodID onServeTouchDownPosition;
        jmethodID onHitPosition;
        jmethodID onAddScore;
        jmethodID onError;
    };

private:
    JNIEnv *m_pJNIEnv;
    jobject m_pJNIthiz;
    jobject m_pJNICallback;

    TennisBout *m_pTennisBout;
    JNI_BoutCallback m_boutCallback;
};

#endif