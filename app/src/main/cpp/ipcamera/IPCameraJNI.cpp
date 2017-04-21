#include <string.h>
#include <jni.h>
#include <stdio.h>
#include "IPCamera.h"

static IPCamera *s_pIPCamera = NULL;

std::string getJStringToChar(JNIEnv * env, jstring str);

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_zealens_face_core_internal_IPCamera_init
    ( JNIEnv *env, jobject thiz, jstring tmpPath, jstring ip, jshort port, jstring userName, jstring password )
{
    if (s_pIPCamera == NULL)
    {
        s_pIPCamera = new IPCamera();
        if (s_pIPCamera == NULL)
            return (jboolean) false;
    }

    std::string strTmpPath = getJStringToChar(env, tmpPath);
    std::string strIP = getJStringToChar(env, ip);
    std::string strUserName = getJStringToChar(env, userName);
    std::string strPassword = getJStringToChar(env, password);
    return (jboolean) s_pIPCamera->init(strTmpPath.c_str(), strIP.c_str(), port, strUserName.c_str(), strPassword.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_zealens_face_core_internal_IPCamera_dispose
    ( JNIEnv *env, jobject thiz )
{
    if (s_pIPCamera != NULL)
    {
        s_pIPCamera->dispose();
        s_pIPCamera = NULL;
    }
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_zealens_face_core_internal_IPCamera_startRecord
    ( JNIEnv *env, jobject thiz, jstring savePath )
{
    std::string strSavePath = getJStringToChar(env, savePath);
    return (jboolean)s_pIPCamera->startRecord(strSavePath.c_str());
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_zealens_face_core_internal_IPCamera_stopRecord
    ( JNIEnv *env, jobject thiz )
{
    return (jboolean)s_pIPCamera->stopRecord();
}

std::string getJStringToChar(JNIEnv * env, jstring str)
{
    const char *buffer = env->GetStringUTFChars(str, NULL);
    std::string str2(buffer);
    env->ReleaseStringUTFChars(str, buffer);

    return str2;
}