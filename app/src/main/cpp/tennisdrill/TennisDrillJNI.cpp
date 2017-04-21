#include "TennisDrillJNIImpl.h"

static TennisDrillJNIImpl *s_pTennisDrillJNIImpl = NULL;

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_zealens_face_core_internal_TennisDrill_init
        (JNIEnv *env, jobject thiz, jobject pInitParam, jobject pCallback, jint drillDifficulty) {
    assert(s_pTennisDrillJNIImpl == NULL);

    if (s_pTennisDrillJNIImpl != NULL) {
        delete s_pTennisDrillJNIImpl;
    }

    s_pTennisDrillJNIImpl = new TennisDrillJNIImpl();
    if (s_pTennisDrillJNIImpl == NULL)
        return (jboolean) false;

    return (jboolean) s_pTennisDrillJNIImpl->init(env, thiz, pInitParam, pCallback, drillDifficulty);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_zealens_face_core_internal_TennisDrill_dispose
        (JNIEnv *env, jobject thiz) {
    if (s_pTennisDrillJNIImpl != NULL) {
        delete s_pTennisDrillJNIImpl;
        s_pTennisDrillJNIImpl = NULL;
    }
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_zealens_face_core_internal_TennisDrill_start
        (JNIEnv *env, jobject thiz) {
    if (s_pTennisDrillJNIImpl == NULL)
        return (jboolean) false;

    return (jboolean) s_pTennisDrillJNIImpl->start();
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_zealens_face_core_internal_TennisDrill_pause
        (JNIEnv *env, jobject thiz) {
    if (s_pTennisDrillJNIImpl == NULL)
        return (jboolean) false;

    return (jboolean) s_pTennisDrillJNIImpl->pause();
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_zealens_face_core_internal_TennisDrill_resume
        (JNIEnv *env, jobject thiz) {
    if (s_pTennisDrillJNIImpl == NULL)
        return (jboolean) false;

    return (jboolean) s_pTennisDrillJNIImpl->resume();
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_zealens_face_core_internal_TennisDrill_stop
        (JNIEnv *env, jobject thiz) {
    if (s_pTennisDrillJNIImpl == NULL)
        return (jboolean) false;

    return (jboolean) s_pTennisDrillJNIImpl->stop();
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_zealens_face_core_internal_TennisDrill_runOnce
        (JNIEnv *env, jobject thiz) {
    if (s_pTennisDrillJNIImpl == NULL)
        return (jboolean) false;

    return (jboolean) s_pTennisDrillJNIImpl->runOnce();
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_zealens_face_core_internal_TennisDrill_restart
        (JNIEnv *env, jobject thiz, jint serveArea) {
    if (s_pTennisDrillJNIImpl == NULL)
        return (jboolean) false;

    return (jboolean) s_pTennisDrillJNIImpl->restart((CourtArea) serveArea);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved) {
    JNIEnv *env;
    if (jvm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK)
        return (jint) -1;

    if (!TennisBaseJNI::getInstance().setDrillBoxScore(env))
        return (jint) -2;

    TennisBaseJNI::getInstance().setJavaVM(jvm);

    return JNI_VERSION_1_6;
}