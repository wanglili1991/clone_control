#include <assert.h>
#include "TennisDrillJNIImpl.h"

TennisDrillJNIImpl::TennisDrillJNIImpl()
        : m_pJNIEnv(NULL), m_pJNIthiz(NULL), m_pJNICallback(NULL), m_pTennisDrill(NULL) {
    memset(&m_drillCallback, 0, sizeof(m_drillCallback));
}

TennisDrillJNIImpl::~TennisDrillJNIImpl() {
    if (m_pTennisDrill != NULL) {
        m_pTennisDrill->releaseRef();
        m_pTennisDrill = NULL;
    }

    if (m_pJNICallback != NULL) {
        m_pJNIEnv->DeleteGlobalRef(m_pJNICallback);
        m_pJNICallback = NULL;
    }
}

bool TennisDrillJNIImpl::init(JNIEnv *env, jobject thiz, jobject pInitParam, jobject pCallback, jint drillDifficulty) {
    InitParam initParam;
    if (!TennisBaseJNI::getInstance().getInitParam(env, pInitParam, initParam))
        return false;

    if (!setDrillCallback(env, pCallback))
        return false;

    m_pJNIEnv = env;
    m_pJNIthiz = thiz;

    return init(initParam, this, (DrillDifficulty) drillDifficulty);
}

bool TennisDrillJNIImpl::init(const InitParam &initParam, I_DrillCallback *pCallback, DrillDifficulty drillDifficulty) {
    assert (m_pTennisDrill == NULL);

    m_pTennisDrill = new TennisDrill();
    if (m_pTennisDrill == NULL)
        return false;

    m_pTennisDrill->addRef();

    if (!m_pTennisDrill->init(initParam, pCallback, drillDifficulty)) {
        m_pTennisDrill->releaseRef();
        m_pTennisDrill = NULL;

        return false;
    }

    return true;
}

bool TennisDrillJNIImpl::start() {
    if (m_pTennisDrill == NULL)
        return false;

    return m_pTennisDrill->start();
}

bool TennisDrillJNIImpl::pause() {
    if (m_pTennisDrill == NULL)
        return false;

    return m_pTennisDrill->pause();
}

bool TennisDrillJNIImpl::resume() {
    if (m_pTennisDrill == NULL)
        return false;

    return m_pTennisDrill->resume();
}

bool TennisDrillJNIImpl::stop() {
    if (m_pTennisDrill == NULL)
        return false;

    return m_pTennisDrill->stop();
}

bool TennisDrillJNIImpl::runOnce() {
    if (m_pTennisDrill == NULL)
        return false;

    return m_pTennisDrill->runOnce();
}

bool TennisDrillJNIImpl::restart(CourtArea serveArea) {
    if (m_pTennisDrill == NULL)
        return false;

    return m_pTennisDrill->restart(serveArea);
}

void TennisDrillJNIImpl::onBegin() {
    JNIEnv *pJNIEnv = getJNIEnv();
    if (pJNIEnv == NULL)
        return;

    pJNIEnv->CallVoidMethod(m_pJNICallback, m_drillCallback.onBegin);
}

void TennisDrillJNIImpl::onDataChange(const DrillBoxScore &drillBoxScore) {
    JNIEnv *pJNIEnv = getJNIEnv();
    if (pJNIEnv == NULL)
        return;

    jobject object = TennisBaseJNI::getInstance().getDrillBoxScore(pJNIEnv, drillBoxScore);
    pJNIEnv->CallVoidMethod(m_pJNICallback, m_drillCallback.onDataChange, object);
}

void TennisDrillJNIImpl::onSpeedChange(int speed) {
    JNIEnv *pJNIEnv = getJNIEnv();
    if (pJNIEnv == NULL)
        return;

    pJNIEnv->CallVoidMethod(m_pJNICallback, m_drillCallback.onSpeedChange, speed);
}

void TennisDrillJNIImpl::onEnd() {
    JNIEnv *pJNIEnv = getJNIEnv();
    if (pJNIEnv == NULL)
        return;

    pJNIEnv->CallVoidMethod(m_pJNICallback, m_drillCallback.onEnd);
}

void TennisDrillJNIImpl::onDrillNext() {
    JNIEnv *pJNIEnv = getJNIEnv();
    if (pJNIEnv == NULL)
        return;

    pJNIEnv->CallVoidMethod(m_pJNICallback, m_drillCallback.onDrillNext);
}

void TennisDrillJNIImpl::onError(int code, const char *errorMessage) {
    JNIEnv *pJNIEnv = getJNIEnv();
    if (pJNIEnv == NULL)
        return;

    jstring jname = NULL;
    if (errorMessage != NULL) {
        jname = pJNIEnv->NewStringUTF(errorMessage);
    }

    pJNIEnv->CallVoidMethod(m_pJNICallback, m_drillCallback.onError, code, jname);
}

void TennisDrillJNIImpl::onThread(bool isExit) {
    if (isExit) {
        TennisBaseJNI::getInstance().getJavaVM()->DetachCurrentThread();
    } else {
        JNIEnv *pJNIEnv = getJNIEnv();
    }
}

JNIEnv *TennisDrillJNIImpl::getJNIEnv() {
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

bool TennisDrillJNIImpl::setDrillCallback(JNIEnv *env, jobject pCallback) {
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
        m_drillCallback.onBegin = env->GetMethodID(clazz, "a", "()V");
#else
        m_drillCallback.onBegin = env->GetMethodID(clazz, "onBegin", "()V");
#endif
        if (m_drillCallback.onBegin == NULL)
            break;

#ifdef _OBSCURE
        m_drillCallback.onDataChange = env->GetMethodID(clazz, "a", "(Lcom/zealens/face/core/internal/TennisBase$DrillBoxScores;)V");
#else
        m_drillCallback.onDataChange = env->GetMethodID(clazz, "onDataChange",
                                                        "(Lcom/zealens/face/core/internal/TennisBase$DrillBoxScore;)V");
#endif
        if (m_drillCallback.onDataChange == NULL)
            break;

#ifdef _OBSCURE
        m_drillCallback.onSpeedChange = env->GetMethodID(clazz, "a", "(I)V");
#else
        m_drillCallback.onSpeedChange = env->GetMethodID(clazz, "onSpeedChange", "(I)V");
#endif
        if (m_drillCallback.onSpeedChange == NULL)
            break;

#ifdef _OBSCURE
        m_drillCallback.onEnd = env->GetMethodID(clazz, "a", "()V");
#else
        m_drillCallback.onEnd = env->GetMethodID(clazz, "onEnd", "()V");
#endif
        if (m_drillCallback.onEnd == NULL)
            break;


#ifdef _OBSCURE
        m_drillCallback.onDrillNext = env->GetMethodID(clazz, "a", "()V");
#else
        m_drillCallback.onDrillNext = env->GetMethodID(clazz, "onDrillNext", "()V");
#endif
        if (m_drillCallback.onDrillNext == NULL)
            break;

#ifdef _OBSCURE
        m_drillCallback.onError = env->GetMethodID(clazz, "a", "(ILjava/lang/String;)V");
#else
        m_drillCallback.onError = env->GetMethodID(clazz, "onError", "(ILjava/lang/String;)V");
#endif
        if (m_drillCallback.onError == NULL)
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
