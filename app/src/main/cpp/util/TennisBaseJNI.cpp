#include "TennisBaseJNI.h"

TennisBaseJNI TennisBaseJNI::s_instance;

TennisBaseJNI::TennisBaseJNI()
    : m_pJavaVM(NULL)
{
    memset(&m_ball, 0, sizeof(m_ball));
    memset(&m_matchBoxScore, 0, sizeof(m_matchBoxScore));

    memset(&m_ballPoint, 0, sizeof(m_ballPoint));
    memset(&m_drillBoxScore, 0, sizeof(m_drillBoxScore));
}

TennisBaseJNI::~TennisBaseJNI()
{
    JNIEnv *env;
    if (m_pJavaVM->GetEnv((void **) &env, JNI_VERSION_1_6) == JNI_OK) {
        if (m_ball.object != NULL) {
            env->DeleteGlobalRef(m_ball.object);
            m_ball.object = NULL;
        }

        if (m_ball.clazz != NULL) {
            env->DeleteGlobalRef(m_ball.clazz);
            m_ball.clazz = NULL;
        }

        if (m_matchBoxScore.objectA != NULL) {
            env->DeleteGlobalRef(m_matchBoxScore.objectA);
            m_matchBoxScore.objectA = NULL;
        }

        if (m_matchBoxScore.objectB != NULL) {
            env->DeleteGlobalRef(m_matchBoxScore.objectB);
            m_matchBoxScore.objectB = NULL;
        }

        if (m_matchBoxScore.clazz != NULL) {
            env->DeleteGlobalRef(m_matchBoxScore.clazz);
            m_matchBoxScore.clazz = NULL;
        }

        if (m_drillBoxScore.arrBallPointHit != NULL) {
            DeleteGlobalArrayRef(env, m_drillBoxScore.arrBallPointHit);
        }

        if (m_drillBoxScore.arrBallPointHit != NULL) {
            DeleteGlobalArrayRef(env, m_drillBoxScore.arrBallPointHit);
        }

        if (m_drillBoxScore.object != NULL) {
            env->DeleteGlobalRef(m_drillBoxScore.object);
            m_drillBoxScore.object = NULL;
        }

        if (m_drillBoxScore.clazz != NULL) {
            env->DeleteGlobalRef(m_drillBoxScore.clazz);
            m_drillBoxScore.clazz = NULL;
        }
    }

}

bool TennisBaseJNI::getInitParam(JNIEnv *env, jobject pInitParam, InitParam &initParam)
{
	bool bOK = false;
	do
	{
		jclass clazz = env->GetObjectClass(pInitParam);
		if (clazz == NULL)
			break;

        jfieldID fieldID = env->GetFieldID(clazz, "courtSize", "Lcom/zealens/face/core/internal/TennisBase$CourtSize;");
        jobject pCourtSize = env->GetObjectField(pInitParam, fieldID);
        jclass clazzCourtSize = env->GetObjectClass(pCourtSize);
        if (clazzCourtSize == NULL)
            break;

        initParam.playModel = (PlayModel)env->GetIntField(pInitParam, env->GetFieldID(clazz, "playModel", "I"));
        initParam.servePosition = (CourtArea)env->GetIntField(pInitParam, env->GetFieldID(clazz, "servePosition", "I"));

        initParam.courtSize.singleX = env->GetIntField(pCourtSize, env->GetFieldID(clazzCourtSize, "singleX", "I"));
        initParam.courtSize.doubleX = env->GetIntField(pCourtSize, env->GetFieldID(clazzCourtSize, "doubleX", "I"));
        initParam.courtSize.courtX = env->GetIntField(pCourtSize, env->GetFieldID(clazzCourtSize, "courtX", "I"));
        initParam.courtSize.singleY = env->GetIntField(pCourtSize, env->GetFieldID(clazzCourtSize, "singleY", "I"));
        initParam.courtSize.doubleY = env->GetIntField(pCourtSize, env->GetFieldID(clazzCourtSize, "doubleY", "I"));
        initParam.courtSize.courtY = env->GetIntField(pCourtSize, env->GetFieldID(clazzCourtSize, "courtY", "I"));

        initParam.courtSize.severX = env->GetIntField(pCourtSize, env->GetFieldID(clazzCourtSize, "severX", "I"));
        initParam.courtSize.baselineX = env->GetIntField(pCourtSize, env->GetFieldID(clazzCourtSize, "baselineX", "I"));
        initParam.courtSize.centerLineWidth = env->GetIntField(pCourtSize, env->GetFieldID(clazzCourtSize, "centerLineWidth", "I"));

		jstring appPath = (jstring)env->GetObjectField(pInitParam, env->GetFieldID(clazz, "appPath", "Ljava/lang/String;"));
        initParam.strAppPath = getJStringToChar(env, appPath);

		jstring cfgPath = (jstring)env->GetObjectField(pInitParam, env->GetFieldID(clazz, "cfgPath", "Ljava/lang/String;"));
        initParam.strCfgPath = getJStringToChar(env, cfgPath);

		jstring tmpPath = (jstring)env->GetObjectField(pInitParam, env->GetFieldID(clazz, "tmpPath", "Ljava/lang/String;"));
        initParam.strTmpPath = getJStringToChar(env, tmpPath);

		if (initParam.strAppPath.empty()
			|| initParam.strCfgPath.empty()
			|| initParam.strTmpPath.empty())
			break;

		bOK = true;
	} while (false);

	return bOK;
}

bool TennisBaseJNI::setBall(JNIEnv *env)
{
	jclass clazz = env->FindClass("com/zealens/face/core/internal/TennisBase$Ball");
	if (clazz == NULL)
		return false;

	jclass clazzGlobal = (jclass)env->NewGlobalRef(clazz);
	env->DeleteLocalRef(clazz);
    if (clazzGlobal == NULL)
        return false;

	m_ball.x = env->GetFieldID(clazzGlobal, "x", "I");
	m_ball.y = env->GetFieldID(clazzGlobal, "y", "I");
	m_ball.z = env->GetFieldID(clazzGlobal, "z", "I");
	m_ball.speed = env->GetFieldID(clazzGlobal, "speed", "I");
	m_ball.timestamp = env->GetFieldID(clazzGlobal, "timestamp", "J");
    m_ball.construct = env->GetMethodID(clazzGlobal, "<init>", "()V");
    if (m_ball.x == NULL || m_ball.y == NULL || m_ball.z == NULL || m_ball.speed == NULL || m_ball.timestamp == NULL) {
        env->DeleteGlobalRef(clazzGlobal);
        return false;
    }

    jobject object = env->NewObject(clazzGlobal, m_ball.construct);
    m_ball.object = env->NewGlobalRef(object);
    env->DeleteLocalRef(object);
    if (m_ball.object == NULL) {
        env->DeleteGlobalRef(clazzGlobal);
        return false;
    }

    m_ball.clazz = clazzGlobal;

	return true;
}

bool TennisBaseJNI::setMatchBoxScore(JNIEnv *env)
{
    jclass clazz = env->FindClass("com/zealens/face/core/internal/TennisBase$MatchBoxScore");
	if (clazz == NULL)
		return false;

    jclass clazzGlobal = (jclass)env->NewGlobalRef(clazz);
    env->DeleteLocalRef(clazz);
    if (clazzGlobal == NULL)
        return false;

    m_matchBoxScore.score = env->GetFieldID(clazzGlobal, "score", "I");
    m_matchBoxScore.ace = env->GetFieldID(clazzGlobal, "ace", "I");
    m_matchBoxScore.doubleFaultsNum = env->GetFieldID(clazzGlobal, "doubleFaultsNum", "I");
    m_matchBoxScore.n1stServeTotalNum = env->GetFieldID(clazzGlobal, "n1stServeTotalNum", "I");
    m_matchBoxScore.n1stServeSNum = env->GetFieldID(clazzGlobal, "n1stServeSNum", "I");
    m_matchBoxScore.n1stSocreSNum = env->GetFieldID(clazzGlobal, "n1stSocreSNum", "I");
    m_matchBoxScore.n2ndServeTotalNum = env->GetFieldID(clazzGlobal, "n2ndServeTotalNum", "I");
    m_matchBoxScore.n2ndServeSNum = env->GetFieldID(clazzGlobal, "n2ndServeSNum", "I");
    m_matchBoxScore.n2ndSocreSNum = env->GetFieldID(clazzGlobal, "n2ndSocreSNum", "I");
    m_matchBoxScore.winners = env->GetFieldID(clazzGlobal, "winners", "I");
    m_matchBoxScore.speedMax = env->GetFieldID(clazzGlobal, "speedMax", "I");
    m_matchBoxScore.speedSum = env->GetFieldID(clazzGlobal, "speedSum", "I");
    m_matchBoxScore.hitTotalNum = env->GetFieldID(clazzGlobal, "hitTotalNum", "I");

    m_matchBoxScore.hitNumInside = env->GetFieldID(clazzGlobal, "hitNumInside", "I");
    m_matchBoxScore.hitNumBaseline = env->GetFieldID(clazzGlobal, "hitNumBaseline", "I");
    m_matchBoxScore.hitNumOutside = env->GetFieldID(clazzGlobal, "hitNumOutside", "I");

    m_matchBoxScore.area11stInsideNum = env->GetFieldID(clazzGlobal, "area11stInsideNum", "I");
    m_matchBoxScore.area11stOutsideNum = env->GetFieldID(clazzGlobal, "area11stOutsideNum", "I");
    m_matchBoxScore.area21stInsideNum = env->GetFieldID(clazzGlobal, "area21stInsideNum", "I");
    m_matchBoxScore.area21stOutsideNum = env->GetFieldID(clazzGlobal, "area21stOutsideNum", "I");

    m_matchBoxScore.area12stInsideNum = env->GetFieldID(clazzGlobal, "area12stInsideNum", "I");
    m_matchBoxScore.area12stOutsideNum = env->GetFieldID(clazzGlobal, "area12stOutsideNum", "I");
    m_matchBoxScore.area22ndInsideNum = env->GetFieldID(clazzGlobal, "area22ndInsideNum", "I");
    m_matchBoxScore.area22stOutsideNum = env->GetFieldID(clazzGlobal, "area22stOutsideNum", "I");
    m_matchBoxScore.construct = env->GetMethodID(clazzGlobal, "<init>", "()V");
    if (m_matchBoxScore.score == NULL || m_matchBoxScore.ace == NULL || m_matchBoxScore.hitNumInside == NULL ||
        m_matchBoxScore.area11stInsideNum == NULL ||
        m_matchBoxScore.area22stOutsideNum == NULL) {
        env->DeleteGlobalRef(clazzGlobal);
        return false;
    }

    jobject objectA = env->NewObject(clazzGlobal, m_matchBoxScore.construct);
    jobject objectB = env->NewObject(clazzGlobal, m_matchBoxScore.construct);
    m_matchBoxScore.objectA = env->NewGlobalRef(objectA);
    m_matchBoxScore.objectB = env->NewGlobalRef(objectB);
    env->DeleteLocalRef(objectA);
    env->DeleteLocalRef(objectB);
    if (m_matchBoxScore.objectA == NULL || m_matchBoxScore.objectB == NULL) {
        if (m_matchBoxScore.objectA != NULL) {
            env->DeleteGlobalRef(m_matchBoxScore.objectA);
            m_matchBoxScore.objectA = NULL;
        }

        if (m_matchBoxScore.objectB != NULL) {
            env->DeleteGlobalRef(m_matchBoxScore.objectA);
            m_matchBoxScore.objectB = NULL;
        }

        env->DeleteGlobalRef(clazzGlobal);

        return false;
    }

    m_matchBoxScore.clazz = clazzGlobal;;

	return true;
}

bool TennisBaseJNI::setDrillBoxScore(JNIEnv *env) {
    jclass clazz = env->FindClass("com/zealens/face/core/internal/TennisBase$DrillBoxScore");
    if (clazz == NULL)
        return false;

    jclass clazzGlobal = (jclass) env->NewGlobalRef(clazz);
    env->DeleteLocalRef(clazz);
    if (clazzGlobal == NULL)
        return false;

    m_drillBoxScore.totalTimes = env->GetFieldID(clazzGlobal, "totalTimes", "I");
    m_drillBoxScore.successfulTimes = env->GetFieldID(clazzGlobal, "successfulTimes", "I");
    m_drillBoxScore.maxSpeed = env->GetFieldID(clazzGlobal, "maxSpeed", "I");
    m_drillBoxScore.averageSpeed = env->GetFieldID(clazzGlobal, "averageSpeed", "I");
    m_drillBoxScore.successRatio = env->GetFieldID(clazzGlobal, "successRatio", "I");
    m_drillBoxScore.netTimes = env->GetFieldID(clazzGlobal, "netTimes", "I");
    m_drillBoxScore.outTimes = env->GetFieldID(clazzGlobal, "outTimes", "I");
    m_drillBoxScore.lastResult = env->GetFieldID(clazzGlobal, "lastResult", "I");
    m_drillBoxScore.speed = env->GetFieldID(clazzGlobal, "speed", "I");
    m_drillBoxScore.downPointsNum = env->GetFieldID(clazzGlobal, "downPointsNum", "I");
    m_drillBoxScore.hitPointsNum = env->GetFieldID(clazzGlobal, "hitPointsNum", "I");
    m_drillBoxScore.ballPointHit = env->GetFieldID(clazzGlobal, "ballPointHit", "[Lcom/zealens/face/core/internal/TennisBase$BallPoint;");
    m_drillBoxScore.ballPointDown = env->GetFieldID(clazzGlobal, "ballPointDown", "[Lcom/zealens/face/core/internal/TennisBase$BallPoint;");

    m_drillBoxScore.construct = env->GetMethodID(clazzGlobal, "<init>", "()V");
    if (m_drillBoxScore.totalTimes == NULL || m_drillBoxScore.successRatio == NULL || m_drillBoxScore.speed == NULL ||
        m_drillBoxScore.ballPointHit == NULL ||
        m_drillBoxScore.ballPointDown == NULL) {
        env->DeleteGlobalRef(clazzGlobal);
        return false;
    }

    jobject object = env->NewObject(clazzGlobal, m_drillBoxScore.construct);
    m_drillBoxScore.object = env->NewGlobalRef(object);
    env->DeleteLocalRef(object);
    if (m_drillBoxScore.object == NULL) {
        env->DeleteGlobalRef(clazzGlobal);
        return false;
    }

    if (!setBallPoint(env, m_drillBoxScore.arrBallPointDown)) {
        env->DeleteGlobalRef(clazzGlobal);
        return false;
    }

    if (!setBallPoint(env, m_drillBoxScore.arrBallPointHit)) {
        DeleteGlobalArrayRef(env, m_drillBoxScore.arrBallPointDown);
        env->DeleteGlobalRef(clazzGlobal);
        return false;
    }

    m_drillBoxScore.clazz = clazzGlobal;;

    return true;
}

jobject TennisBaseJNI::getBall(JNIEnv *env, const Ball &ball)
{
	env->SetIntField(m_ball.object, m_ball.x, ball.x);
	env->SetIntField(m_ball.object, m_ball.y, ball.y);
	env->SetIntField(m_ball.object, m_ball.z, ball.z);
	env->SetIntField(m_ball.object, m_ball.speed, ball.speed);
	env->SetLongField(m_ball.object, m_ball.timestamp, ball.timestamp);

	return m_ball.object;
}

jobject TennisBaseJNI::getMatchBoxScore(JNIEnv *env, Player player, const MatchBoxScore &matchBoxScore)
{
    jobject object = m_matchBoxScore.objectA;
    if (player == PLAYER_B) {
        object = m_matchBoxScore.objectB;
    }

    env->SetIntField(object, m_matchBoxScore.score, matchBoxScore.score);
    env->SetIntField(object, m_matchBoxScore.ace, matchBoxScore.ace);
    env->SetIntField(object, m_matchBoxScore.doubleFaultsNum, matchBoxScore.doubleFaultsNum);
    env->SetIntField(object, m_matchBoxScore.n1stServeTotalNum, matchBoxScore.n1stServeTotalNum);
    env->SetIntField(object, m_matchBoxScore.n1stServeSNum, matchBoxScore.n1stServeSNum);
    env->SetIntField(object, m_matchBoxScore.n1stSocreSNum, matchBoxScore.n1stSocreSNum);
    env->SetIntField(object, m_matchBoxScore.n2ndServeTotalNum, matchBoxScore.n2ndServeTotalNum);
    env->SetIntField(object, m_matchBoxScore.n2ndServeSNum, matchBoxScore.n2ndServeSNum);
    env->SetIntField(object, m_matchBoxScore.n2ndSocreSNum, matchBoxScore.n2ndSocreSNum);
    env->SetIntField(object, m_matchBoxScore.winners, matchBoxScore.winners);
    env->SetIntField(object, m_matchBoxScore.speedMax, matchBoxScore.speedMax);
    env->SetIntField(object, m_matchBoxScore.speedSum, matchBoxScore.speedSum);
    env->SetIntField(object, m_matchBoxScore.hitTotalNum, matchBoxScore.hitTotalNum);

    env->SetIntField(object, m_matchBoxScore.hitNumInside, matchBoxScore.hitNumInside);
    env->SetIntField(object, m_matchBoxScore.hitNumBaseline, matchBoxScore.hitNumBaseline);
    env->SetIntField(object, m_matchBoxScore.hitNumOutside, matchBoxScore.hitNumOutside);

    env->SetIntField(object, m_matchBoxScore.area11stInsideNum, matchBoxScore.area11stInsideNum);
    env->SetIntField(object, m_matchBoxScore.area11stOutsideNum, matchBoxScore.area11stOutsideNum);
    env->SetIntField(object, m_matchBoxScore.area21stInsideNum, matchBoxScore.area21stInsideNum);
    env->SetIntField(object, m_matchBoxScore.area21stOutsideNum, matchBoxScore.area21stOutsideNum);

    env->SetIntField(object, m_matchBoxScore.area12stInsideNum, matchBoxScore.area12ndInsideNum);
    env->SetIntField(object, m_matchBoxScore.area12stOutsideNum, matchBoxScore.area12ndOutsideNum);
    env->SetIntField(object, m_matchBoxScore.area22ndInsideNum, matchBoxScore.area22ndInsideNum);
    env->SetIntField(object, m_matchBoxScore.area22stOutsideNum, matchBoxScore.area22ndOutsideNum);

	return object;
}

jobject TennisBaseJNI::getDrillBoxScore(JNIEnv *env, const DrillBoxScore &drillBoxScore) {
    jobject object = m_drillBoxScore.object;

    env->SetIntField(object, m_drillBoxScore.totalTimes, drillBoxScore.totalTimes);
    env->SetIntField(object, m_drillBoxScore.successfulTimes, drillBoxScore.successfulTimes);
    env->SetIntField(object, m_drillBoxScore.maxSpeed, drillBoxScore.maxSpeed);
    env->SetIntField(object, m_drillBoxScore.averageSpeed, drillBoxScore.averageSpeed);
    env->SetIntField(object, m_drillBoxScore.successRatio, drillBoxScore.successRatio);
    env->SetIntField(object, m_drillBoxScore.netTimes, drillBoxScore.netTimes);
    env->SetIntField(object, m_drillBoxScore.outTimes, drillBoxScore.outTimes);
    env->SetIntField(object, m_drillBoxScore.lastResult, drillBoxScore.lastResult);
    env->SetIntField(object, m_drillBoxScore.speed, drillBoxScore.speed);
    env->SetIntField(object, m_drillBoxScore.downPointsNum, drillBoxScore.downPointsNum);
    env->SetIntField(object, m_drillBoxScore.hitPointsNum, drillBoxScore.hitPointsNum);

    getBallPoint(env, m_drillBoxScore.arrBallPointDown, drillBoxScore.pBallPointDown, drillBoxScore.downPointsNum);
    env->SetObjectField(object, m_drillBoxScore.ballPointDown, m_drillBoxScore.arrBallPointDown);

    getBallPoint(env, m_drillBoxScore.arrBallPointHit, drillBoxScore.pBallPointHit, drillBoxScore.hitPointsNum);
    env->SetObjectField(object, m_drillBoxScore.ballPointHit, m_drillBoxScore.arrBallPointHit);

    return object;
}

void TennisBaseJNI::getBallPoint(JNIEnv *env, jobjectArray &objectArray, BallPoint *pBallPoint, int size) {
    for (int index = 0; index < size; ++index) {
        jobject object = env->GetObjectArrayElement(objectArray, index);
        env->SetIntField(object, m_ballPoint.x, pBallPoint[index].x);
        env->SetIntField(object, m_ballPoint.y, pBallPoint[index].y);
        env->SetBooleanField(object, m_ballPoint.in, (jboolean) pBallPoint[index].in);
    }
}

bool TennisBaseJNI::setBallPoint(JNIEnv *env, jobjectArray &objectArray) {
    jclass clazz = env->FindClass("com/zealens/face/core/internal/TennisBase$BallPoint");
    if (clazz == NULL)
        return false;

    jclass clazzGlobal = (jclass) env->NewGlobalRef(clazz);
    env->DeleteLocalRef(clazz);
    if (clazzGlobal == NULL)
        return false;

    m_ballPoint.x = env->GetFieldID(clazzGlobal, "x", "I");
    m_ballPoint.y = env->GetFieldID(clazzGlobal, "y", "I");
    m_ballPoint.in = env->GetFieldID(clazzGlobal, "in", "Z");

    m_ballPoint.construct = env->GetMethodID(clazzGlobal, "<init>", "()V");
    if (m_ballPoint.x == NULL || m_ballPoint.y == NULL || m_ballPoint.in == NULL) {
        env->DeleteGlobalRef(clazzGlobal);
        return false;
    }

    jsize size = 255;
    jobjectArray objectArrayLocal = env->NewObjectArray(size, clazzGlobal, NULL);
    objectArray = (jobjectArray) env->NewGlobalRef(objectArrayLocal);
    env->DeleteLocalRef(objectArrayLocal);
    if (objectArray == NULL) {
        env->DeleteGlobalRef(clazzGlobal);
        return false;
    }

    for (int index = 0; index < size; ++index) {
        jobject object = env->NewObject(clazzGlobal, m_ballPoint.construct);
        env->SetObjectArrayElement(objectArray, index, env->NewGlobalRef(object));
        env->DeleteLocalRef(object);
    }

    m_ballPoint.clazz = clazzGlobal;

    return true;
}

std::string TennisBaseJNI::getJStringToChar(JNIEnv * env, jstring str)
{
	const char *buffer = env->GetStringUTFChars(str, NULL);
	std::string str2(buffer);
	env->ReleaseStringUTFChars(str, buffer);

	return str2;
}

void TennisBaseJNI::DeleteGlobalArrayRef(JNIEnv *env, jobjectArray &objectArray) {
    jsize size = env->GetArrayLength(objectArray);
    for (int index = 0; index < size; ++index) {
        jobject object = env->GetObjectArrayElement(objectArray, index);
        env->DeleteGlobalRef(object);
    }

    env->DeleteGlobalRef(objectArray);
    objectArray = NULL;
}