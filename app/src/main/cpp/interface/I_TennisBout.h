#ifndef __INTERFACE_TENNISBOUT_H__
#define __INTERFACE_TENNISBOUT_H__

#include "I_TennisBase.h"

#pragma pack(push, 4)
struct MatchBoxScore
{
public:
    int score;                      // ����
    int ace;                        // ace
    int doubleFaultsNum;            // ����ʧ��
    int n1stServeTotalNum;          // һ���ܴ���		�������Լ�һ���˶��ٴΣ�
    int n1stServeSNum;              // һ���������		��һ��������ɹ���
    int n1stSocreSNum;              // һ���÷ִ���		��һ��������غ� �Լ��÷֣�
    int n2ndServeTotalNum;          // �����ܴ���
    int n2ndServeSNum;              // �����������
    int n2ndSocreSNum;              // �����÷ִ���
    int winners;                    // ��ʤ��
    int speedMax;                   // ��߷����ٶ�
    int speedSum;                   // �����ٶ�֮��
    int hitTotalNum;                // ������������ھ�����Ƶ����Ҫͳ�ƻ���20�����ϵĵ÷ֻغ�

    int hitNumInside;               // ���ڻ������
    int hitNumBaseline;             // ���߻������
    int hitNumOutside;              // ����������

    // һ��
    int area11stInsideNum;          // һ���ڲ����
    int area11stOutsideNum;         // һ��������
    int area21stInsideNum;          // �����ڲ����
    int area21stOutsideNum;         // ����������

    // ����
    int area12ndInsideNum;          // һ���ڲ����2
    int area12ndOutsideNum;         // һ��������2
    int area22ndInsideNum;          // �����ڲ����2
    int area22ndOutsideNum;         // ����������2
};
#pragma pop()

///////////////////////////////////////////////////the interface for Android///////////////////////////////////////////////////////////////////////////////////
class I_BoutCallback
{
public:
	/**
	* ���λ��
	* @param ball ���λ��
	*/
	virtual void onBall(const Ball &ball) = 0;

	/**
	* ace��
	* @param playerServe ������
	*/
	virtual void onBallAce(Player playerServe) = 0;

	/**
	*  ����λ��
	* @param ballServe ����λ��
	*/
	virtual void onServe(const Ball &ballServe) = 0;

	/**
	*  ����ʧ��
	*/
	virtual void onServeFailed() = 0;

	/**
	* ����ɹ�
	*/
	virtual void onServeSuccessful() = 0;

	/**
	*  �������λ��
	* @param player ������
	* @param serveType ��������
	* @param ballServeTouchDown  �������λ��
	*/
	virtual void onServeTouchDownPosition(Player player, PlayerServe serveType, const Ball &ballServeTouchDown) = 0;

	/**
	*  ����λ��
	* @param playerHit    ������
	* @param ballHit  ����λ��
	*/
	virtual void onHitPosition(Player playerHit, const Ball &ballHit) = 0;

    /**
    *  һ�غϼ���ͳ�ƽ��
    * @param player    ��Ա
    * @param matchBoxScoreA A����Ա�ļ���ͳ��
    * @param matchBoxScoreB B����Ա����ͳ��
    * @param ball  ��
    */
    virtual void onAddScore(Player player, const MatchBoxScore &matchBoxScoreA, const MatchBoxScore &matchBoxScoreB, const Ball &ball) = 0;

	/**
	*  �ײ����
	* @param code  ������
	* @param errorMessage  ������Ϣ
	*/
	virtual void onError(int code, const char *errorMessage) = 0;

    /**
     * ֪ͨ�Ƿ��̴߳���
     * @param isExit �߳������˳�
     */
    virtual void onThread(bool isExit) = 0;
};

class I_TennisBout
    : public I_TennisBase
{
public:
	/**
	* ��ʼ��
	* @param initParam ��ʼ������
	* @param pCallback �ص�����
	* @return �ɹ����� true��ʧ�ܷ��� false
	*/
    virtual bool TENNIS_STDCALL init(const InitParam &initParam, I_BoutCallback *pCallback) = 0;


	/**
	* ������Ȩ
	* @param ballPower ��Ȩ
	* @return �ɹ����� true��ʧ�ܷ��� false
	*/
    virtual bool TENNIS_STDCALL ballPower( BallPower ballPower ) = 0;
};
///////////////////////////////////////////////////the interface for Android///////////////////////////////////////////////////////////////////////////////////

#endif // end __INTERFACE_BOUT_H__