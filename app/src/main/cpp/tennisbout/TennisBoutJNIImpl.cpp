#include <assert.h>
#include "TennisBoutJNIImpl.h"

TennisBoutJNIImpl::TennisBoutJNIImpl()
        : m_pJNIEnv(NULL), m_pJNIthiz(NULL), m_pJNICallback(NULL), m_pTennisBout(NULL) {
    memset(&m_boutCallback, 0, sizeof(m_boutCallback));
}

TennisBoutJNIImpl::~TennisBoutJNIImpl() {
    if (m_pTennisBout != NULL) {
        m_pTennisBout->releaseRef();
        m_pTennisBout = NULL;
    }

    if (m_pJNICallback != NULL) {
        m_pJNIEnv->DeleteGlobalRef(m_pJNICallback);
        m_pJNICallback = NULL;
    }
}

bool TennisBoutJNIImpl::init(JNIEnv *env, jobject thiz, jobject pInitParam, jobject pCallback) {
    InitParam initParam;
    if (!TennisBaseJNI::getInstance().getInitParam(env, pInitParam, initParam))
        return false;

    if (!setBoutCallback(env, pCallback))
        return false;

    m_pJNIEnv = env;
    m_pJNIthiz = thiz;

    return init(initParam, this);
}

bool TennisBoutJNIImpl::init(const InitParam &initParam, I_BoutCallback *pCallback) {
    assert (m_pTennisBout == NULL);

    m_pTennisBout = new TennisBout();
    if (m_pTennisBout == NULL)
        return false;

    m_pTennisBout->addRef();

    if (!m_pTennisBout->init(initParam, pCallback)) {
        m_pTennisBout->releaseRef();
        m_pTennisBout = NULL;

        return false;
    }

    return true;
}

bool TennisBoutJNIImpl::ballPower(BallPower ballPower) {
    if (m_pTennisBout == NULL)
        return false;

    return m_pTennisBout->ballPower(ballPower);
}

bool TennisBoutJNIImpl::start() {
    if (m_pTennisBout == NULL)
        return false;

    return m_pTennisBout->start();
}

bool TennisBoutJNIImpl::pause() {
    if (m_pTennisBout == NULL)
        return false;

    return m_pTennisBout->pause();
}

bool TennisBoutJNIImpl::resume() {
    if (m_pTennisBout == NULL)
        return false;

    return m_pTennisBout->resume();
}

bool TennisBoutJNIImpl::stop() {
    if (m_pTennisBout == NULL)
        return false;

    return m_pTennisBout->stop();
}

bool TennisBoutJNIImpl::runOnce() {
    if (m_pTennisBout == NULL)
        return false;

    return m_pTennisBout->runOnce();
}

bool TennisBoutJNIImpl::restart(CourtArea serveArea) {
    if (m_pTennisBout == NULL)
        return false;

    return m_pTennisBout->restart(serveArea);
}

void TennisBoutJNIImpl::onBall(const Ball &ball) {
    JNIEnv *pJNIEnv = getJNIEnv();
    if (pJNIEnv == NULL)
        return;

    jobject object = TennisBaseJNI::getInstance().getBall(pJNIEnv, ball);
    pJNIEnv->CallVoidMethod(m_pJNICallback, m_boutCallback.onBall, object);
}

void TennisBoutJNIImpl::onBallAce(Player playerServe) {
    JNIEnv *pJNIEnv = getJNIEnv();
    if (pJNIEnv == NULL)
        return;

    pJNIEnv->CallVoidMethod(m_pJNICallback, m_boutCallback.onBallAce, playerServe);
}

void TennisBoutJNIImpl::onServe(const Ball &ballServe) {
    JNIEnv *pJNIEnv = getJNIEnv();
    if (pJNIEnv == NULL)
        return;

    jobject object = TennisBaseJNI::getInstance().getBall(pJNIEnv, ballServe);
    pJNIEnv->CallVoidMethod(m_pJNICallback, m_boutCallback.onServe, object);
}

void TennisBoutJNIImpl::onServeFailed() {
    JNIEnv *pJNIEnv = getJNIEnv();
    if (pJNIEnv == NULL)
        return;

    pJNIEnv->CallVoidMethod(m_pJNICallback, m_boutCallback.onServeFailed);
}

void TennisBoutJNIImpl::onServeSuccessful() {
    JNIEnv *pJNIEnv = getJNIEnv();
    if (pJNIEnv == NULL)
        return;

    pJNIEnv->CallVoidMethod(m_pJNICallback, m_boutCallback.onServeSuccessful);
}

void TennisBoutJNIImpl::onServeTouchDownPosition(Player player, PlayerServe serveType, const Ball &ballServeTouchDown) {
    JNIEnv *pJNIEnv = getJNIEnv();
    if (pJNIEnv == NULL)
        return;

    jobject object = TennisBaseJNI::getInstance().getBall(pJNIEnv, ballServeTouchDown);
    pJNIEnv->CallVoidMethod(m_pJNICallback, m_boutCallback.onServeTouchDownPosition, player, serveType, object);
}

void TennisBoutJNIImpl::onHitPosition(Player playerHit, const Ball &ballHit) {
    JNIEnv *pJNIEnv = getJNIEnv();
    if (pJNIEnv == NULL)
        return;

    jobject object = TennisBaseJNI::getInstance().getBall(pJNIEnv, ballHit);
    pJNIEnv->CallVoidMethod(m_pJNICallback, m_boutCallback.onHitPosition, playerHit, object);
}

void
TennisBoutJNIImpl::onAddScore(Player player, const MatchBoxScore &matchBoxScoreA, const MatchBoxScore &matchBoxScoreB, const Ball &ball) {
    JNIEnv *pJNIEnv = getJNIEnv();
    if (pJNIEnv == NULL)
        return;

    jobject jmatchBoxScoreA = NULL;
    jobject jmatchBoxScoreB = NULL;
    jobject jball = NULL;
    do {
        jmatchBoxScoreA = TennisBaseJNI::getInstance().getMatchBoxScore(pJNIEnv, PLAYER_A, matchBoxScoreA);
        if (jmatchBoxScoreA == NULL)
            break;

        jmatchBoxScoreB = TennisBaseJNI::getInstance().getMatchBoxScore(pJNIEnv, PLAYER_B, matchBoxScoreB);
        if (jmatchBoxScoreB == NULL)
            break;

        jball = TennisBaseJNI::getInstance().getBall(pJNIEnv, ball);
        if (jball == NULL)
            break;

        pJNIEnv->CallVoidMethod(m_pJNICallback, m_boutCallback.onAddScore, player, jmatchBoxScoreA, jmatchBoxScoreB, jball);
    } while (false);
}

void TennisBoutJNIImpl::onError(int code, const char *errorMessage) {
    JNIEnv *pJNIEnv = getJNIEnv();
    if (pJNIEnv == NULL)
        return;

    jstring jname = NULL;
    if (errorMessage != NULL) {
        jname = pJNIEnv->NewStringUTF(errorMessage);
    }

    pJNIEnv->CallVoidMethod(m_pJNICallback, m_boutCallback.onError, code, jname);
}

void TennisBoutJNIImpl::onThread(bool isExit) {
    if (isExit) {
        TennisBaseJNI::getInstance().getJavaVM()->DetachCurrentThread();
    } else {
        JNIEnv *pJNIEnv = getJNIEnv();
    }
}

JNIEnv *TennisBoutJNIImpl::getJNIEnv() {
    JNIEnv *pJNIEnv = NULL;
    int status = TennisBaseJNI::getInstance().getJavaVM()->GetEnv((void **) &pJNIEnv, JNI_VERSION_1_6);
    if (status != JNI_OK) {
        status = TennisBaseJNI::getInstance().getJavaVM()->AttachCurrentThread(&pJNIEnv, NULL);
        assert(pJNIEnv != NULL);
        if (status != JNI_OK)
            return NULL;
    }

    return pJNIEnv;
}

bool TennisBoutJNIImpl::setBoutCallback(JNIEnv *env, jobject pCallback) {
    bool bOK = false;
    jclass clazz = NULL;
    jobject pJNICallback = NULL;
    do {
        pJNICallback = env->NewGlobalRef(pCallback);
        if (pJNICallback == NULL)
            break;

        clazz = env->GetObjectClass(pJNICallback);
        if (clazz == NULL)
            break;

#ifdef _OBSCURE
        m_boutCallback.onBall = env->GetMethodID(clazz, "a", "(Lcom/zealens/face/core/internal/TennisBase$Ball;)V");
#else
        m_boutCallback.onBall = env->GetMethodID(clazz, "onBall", "(Lcom/zealens/face/core/internal/TennisBase$Ball;)V");
#endif
        if (m_boutCallback.onBall == NULL)
            break;

#ifdef _OBSCURE
        m_boutCallback.onBallAce = env->GetMethodID(clazz, "a", "(I)V");
#else
        m_boutCallback.onBallAce = env->GetMethodID(clazz, "onBallAce", "(I)V");
#endif
        if (m_boutCallback.onBallAce == NULL)
            break;

#ifdef _OBSCURE
        m_boutCallback.onServe = env->GetMethodID(clazz, "a", "(Lcom/zealens/face/core/internal/TennisBase$Ball;)V");
#else
        m_boutCallback.onServe = env->GetMethodID(clazz, "onServe", "(Lcom/zealens/face/core/internal/TennisBase$Ball;)V");
#endif
        if (m_boutCallback.onServe == NULL)
            break;

#ifdef _OBSCURE
        m_boutCallback.onServeFailed = env->GetMethodID(clazz, "a", "()V");
#else
        m_boutCallback.onServeFailed = env->GetMethodID(clazz, "onServeFailed", "()V");
#endif
        if (m_boutCallback.onServeFailed == NULL)
            break;


#ifdef _OBSCURE
        m_boutCallback.onServeSuccessful = env->GetMethodID(clazz, "a", "()V");
#else
        m_boutCallback.onServeSuccessful = env->GetMethodID(clazz, "onServeSuccessful", "()V");
#endif
        if (m_boutCallback.onServeSuccessful == NULL)
            break;

#ifdef _OBSCURE
        m_boutCallback.onServeTouchDownPosition = env->GetMethodID(clazz, "a", "(IILcom/zealens/face/core/internal/TennisBase$Ball;)V");
#else
        m_boutCallback.onServeTouchDownPosition = env->GetMethodID(clazz, "onServeTouchDownPosition",
                                                                   "(IILcom/zealens/face/core/internal/TennisBase$Ball;)V");
#endif
        if (m_boutCallback.onServeTouchDownPosition == NULL)
            break;

#ifdef _OBSCURE
        m_boutCallback.onHitPosition = env->GetMethodID(clazz, "a", "(ILcom/zealens/face/core/internal/TennisBase$Ball;)V");
#else
        m_boutCallback.onHitPosition = env->GetMethodID(clazz, "onHitPosition", "(ILcom/zealens/face/core/internal/TennisBase$Ball;)V");
#endif
        if (m_boutCallback.onHitPosition == NULL)
            break;

#ifdef _OBSCURE
        m_boutCallback.onAddScore = env->GetMethodID(clazz, "a", "(ILcom/zealens/face/core/internal/TennisBase$MatchBoxScore;Lcom/zealens/face/core/internal/TennisBase$MatchBoxScore;Lcom/zealens/face/core/internal/TennisBase$Ball;)V");
#else
        m_boutCallback.onAddScore = env->GetMethodID(clazz, "onAddScore",
                                                     "(ILcom/zealens/face/core/internal/TennisBase$MatchBoxScore;Lcom/zealens/face/core/internal/TennisBase$MatchBoxScore;Lcom/zealens/face/core/internal/TennisBase$Ball;)V");
#endif
        if (m_boutCallback.onAddScore == NULL)
            break;

#ifdef _OBSCURE
        m_boutCallback.onError = env->GetMethodID(clazz, "a", "(ILjava/lang/String;)V");
#else
        m_boutCallback.onError = env->GetMethodID(clazz, "onError", "(ILjava/lang/String;)V");
#endif
        if (m_boutCallback.onError == NULL)
            break;

        bOK = true;
        m_pJNICallback = pJNICallback;
    } while (false);

    if (clazz != NULL) {
        env->DeleteLocalRef(clazz);
    }

    if (!bOK) {
        if (pJNICallback != NULL) {
            env->DeleteGlobalRef(pJNICallback);
        }
    }

    return bOK;
}
