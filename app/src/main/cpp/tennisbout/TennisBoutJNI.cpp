#include "TennisBoutJNIImpl.h"

static TennisBoutJNIImpl *s_pTennisBoutJNIImpl = NULL;

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_zealens_face_core_internal_TennisBout_init
        (JNIEnv *env, jobject thiz, jobject pInitParam, jobject pCallback) {
    assert(s_pTennisBoutJNIImpl == NULL);

    if (s_pTennisBoutJNIImpl != NULL) {
        delete s_pTennisBoutJNIImpl;
    }

    s_pTennisBoutJNIImpl = new TennisBoutJNIImpl();
    if (s_pTennisBoutJNIImpl == NULL)
        return (jboolean) false;

    return (jboolean) s_pTennisBoutJNIImpl->init(env, thiz, pInitParam, pCallback);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_zealens_face_core_internal_TennisBout_dispose
        (JNIEnv *env, jobject thiz) {
    if (s_pTennisBoutJNIImpl != NULL) {
        delete s_pTennisBoutJNIImpl;
        s_pTennisBoutJNIImpl = NULL;
    }
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_zealens_face_core_internal_TennisBout_ballPower
        (JNIEnv *env, jobject thiz, jint bollPower) {
    if (s_pTennisBoutJNIImpl == NULL)
        return (jboolean) false;

    return (jboolean) s_pTennisBoutJNIImpl->ballPower((BallPower) bollPower);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_zealens_face_core_internal_TennisBout_start
        (JNIEnv *env, jobject thiz) {
    if (s_pTennisBoutJNIImpl == NULL)
        return (jboolean) false;

    return (jboolean) s_pTennisBoutJNIImpl->start();
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_zealens_face_core_internal_TennisBout_pause
        (JNIEnv *env, jobject thiz) {
    if (s_pTennisBoutJNIImpl == NULL)
        return (jboolean) false;

    return (jboolean) s_pTennisBoutJNIImpl->pause();
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_zealens_face_core_internal_TennisBout_resume
        (JNIEnv *env, jobject thiz) {
    if (s_pTennisBoutJNIImpl == NULL)
        return (jboolean) false;

    return (jboolean) s_pTennisBoutJNIImpl->resume();
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_zealens_face_core_internal_TennisBout_stop
        (JNIEnv *env, jobject thiz) {
    if (s_pTennisBoutJNIImpl == NULL)
        return (jboolean) false;

    return (jboolean) s_pTennisBoutJNIImpl->stop();
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_zealens_face_core_internal_TennisBout_runOnce
        (JNIEnv *env, jobject thiz) {
    if (s_pTennisBoutJNIImpl == NULL)
        return (jboolean) false;

    return (jboolean) s_pTennisBoutJNIImpl->runOnce();
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_zealens_face_core_internal_TennisBout_restart
        (JNIEnv *env, jobject thiz, jint serveArea) {
    if (s_pTennisBoutJNIImpl == NULL)
        return (jboolean) false;

    return (jboolean) s_pTennisBoutJNIImpl->restart((CourtArea) serveArea);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved) {
    JNIEnv *env;
    if (jvm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK)
        return (jint) -1;

    if (!TennisBaseJNI::getInstance().setBall(env))
        return (jint) -2;

    if (!TennisBaseJNI::getInstance().setMatchBoxScore(env))
        return (jint) -3;

    TennisBaseJNI::getInstance().setJavaVM(jvm);

    return JNI_VERSION_1_6;
}