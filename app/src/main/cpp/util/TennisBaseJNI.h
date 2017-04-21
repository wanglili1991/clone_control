#ifndef __TENNISBASE_JNI_H__
#define __TENNISBASE_JNI_H__

#include <I_TennisBout.h>
#include <I_TennisDrill.h>

#include <jni.h>
#include <string>


class TennisBaseJNI
{
public:
	TennisBaseJNI();
	~TennisBaseJNI();

public:
	bool setBall(JNIEnv *env);

    bool setMatchBoxScore(JNIEnv *env);

    bool setDrillBoxScore(JNIEnv *env);

public:
	bool getInitParam(JNIEnv *env, jobject pInitParam, InitParam &initParam);

	jobject getBall(JNIEnv *env, const Ball &ball);

    jobject getMatchBoxScore(JNIEnv *env, Player player, const MatchBoxScore &matchBoxScore);

    jobject getDrillBoxScore(JNIEnv *env, const DrillBoxScore &drillBoxScore);

private:
    void getBallPoint(JNIEnv *env, jobjectArray &objectArray, BallPoint *pBallPoint, int size);

    bool setBallPoint(JNIEnv *env, jobjectArray &objectArray);

	std::string getJStringToChar(JNIEnv * env, jstring str);

    void DeleteGlobalArrayRef(JNIEnv *env, jobjectArray &objectArray);

public:
	void setJavaVM(JavaVM* pJavaVM)
	{
		m_pJavaVM = pJavaVM;
	}

	JavaVM *getJavaVM()
	{
		return m_pJavaVM;
	}

public:
	static TennisBaseJNI& getInstance()
	{
		return s_instance;
	}

public:
	struct JNI_Ball {
        jclass  clazz;
        jmethodID construct;
        jobject object;
		jfieldID  x;
		jfieldID  y;
		jfieldID  z;
		jfieldID  speed;
		jfieldID  timestamp;
	};

    struct JNI_MatchBoxScore {
        jclass clazz;
		jmethodID construct;
		jobject objectA;
		jobject objectB;

		jfieldID score;
		jfieldID ace;
		jfieldID doubleFaultsNum;
		jfieldID n1stServeTotalNum;
		jfieldID n1stServeSNum;
		jfieldID n1stSocreSNum;
		jfieldID n2ndServeTotalNum;
		jfieldID n2ndServeSNum;
		jfieldID n2ndSocreSNum;
		jfieldID winners;
		jfieldID speedMax;
		jfieldID speedSum;
		jfieldID hitTotalNum;
		
		jfieldID hitNumInside;
		jfieldID hitNumBaseline;
		jfieldID hitNumOutside;

		// 一发						
		jfieldID area11stInsideNum;
		jfieldID area11stOutsideNum;
		jfieldID area21stInsideNum;
		jfieldID area21stOutsideNum;

		// 二发	
		jfieldID area12stInsideNum;
		jfieldID area12stOutsideNum;
		jfieldID area22ndInsideNum;
		jfieldID area22stOutsideNum;
	};

    struct JNI_BallPoint {
        jclass clazz;
        jmethodID construct;

        jfieldID x;
        jfieldID y;
        jfieldID in;
    };

    struct JNI_DrillBoxScore {
        jclass clazz;
        jmethodID construct;
        jobject object;

        jobjectArray arrBallPointHit;
        jobjectArray arrBallPointDown;

        // DrillBoxSocre
        jfieldID totalTimes;
        jfieldID successfulTimes;
        jfieldID maxSpeed;
        jfieldID averageSpeed;
        jfieldID successRatio;
        jfieldID netTimes;
        jfieldID outTimes;
        jfieldID lastResult;
        jfieldID speed;
        jfieldID downPointsNum;
        jfieldID hitPointsNum;
        jfieldID ballPointHit;
        jfieldID ballPointDown;
    };

private:
	JavaVM* m_pJavaVM;

	JNI_Ball m_ball;
    JNI_MatchBoxScore m_matchBoxScore;

    JNI_BallPoint m_ballPoint;
    JNI_DrillBoxScore m_drillBoxScore;

private:
	static TennisBaseJNI s_instance;
};

#endif