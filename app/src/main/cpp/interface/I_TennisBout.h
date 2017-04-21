#ifndef __INTERFACE_TENNISBOUT_H__
#define __INTERFACE_TENNISBOUT_H__

#include "I_TennisBase.h"

#pragma pack(push, 4)
struct MatchBoxScore
{
public:
    int score;                      // 分数
    int ace;                        // ace
    int doubleFaultsNum;            // 两次失误
    int n1stServeTotalNum;          // 一发总次数		（整盘自己一发了多少次）
    int n1stServeSNum;              // 一发进球次数		（一发，发球成功）
    int n1stSocreSNum;              // 一发得分次数		（一发，这个回合 自己得分）
    int n2ndServeTotalNum;          // 二发总次数
    int n2ndServeSNum;              // 二发进球次数
    int n2ndSocreSNum;              // 二发得分次数
    int winners;                    // 制胜分
    int speedMax;                   // 最高发球速度
    int speedSum;                   // 发球速度之和
    int hitTotalNum;                // 击球次数，用于精彩视频里面要统计击球20次以上的得分回合

    int hitNumInside;               // 场内击球次数
    int hitNumBaseline;             // 底线击球次数
    int hitNumOutside;              // 场外击球次数

    // 一发
    int area11stInsideNum;          // 一区内侧次数
    int area11stOutsideNum;         // 一区外侧次数
    int area21stInsideNum;          // 二区内侧次数
    int area21stOutsideNum;         // 二区外侧次数

    // 二发
    int area12ndInsideNum;          // 一区内侧次数2
    int area12ndOutsideNum;         // 一区外侧次数2
    int area22ndInsideNum;          // 二区内侧次数2
    int area22ndOutsideNum;         // 二区外侧次数2
};
#pragma pop()

///////////////////////////////////////////////////the interface for Android///////////////////////////////////////////////////////////////////////////////////
class I_BoutCallback
{
public:
	/**
	* 球的位置
	* @param ball 球的位置
	*/
	virtual void onBall(const Ball &ball) = 0;

	/**
	* ace球
	* @param playerServe 发球者
	*/
	virtual void onBallAce(Player playerServe) = 0;

	/**
	*  发球位置
	* @param ballServe 发球位置
	*/
	virtual void onServe(const Ball &ballServe) = 0;

	/**
	*  发球失败
	*/
	virtual void onServeFailed() = 0;

	/**
	* 发球成功
	*/
	virtual void onServeSuccessful() = 0;

	/**
	*  发球落点位置
	* @param player 发球者
	* @param serveType 发球类型
	* @param ballServeTouchDown  发球落点位置
	*/
	virtual void onServeTouchDownPosition(Player player, PlayerServe serveType, const Ball &ballServeTouchDown) = 0;

	/**
	*  击球位置
	* @param playerHit    击球者
	* @param ballHit  击球位置
	*/
	virtual void onHitPosition(Player playerHit, const Ball &ballHit) = 0;

    /**
    *  一回合技术统计结果
    * @param player    球员
    * @param matchBoxScoreA A面球员的技术统计
    * @param matchBoxScoreB B面球员技术统计
    * @param ball  球
    */
    virtual void onAddScore(Player player, const MatchBoxScore &matchBoxScoreA, const MatchBoxScore &matchBoxScoreB, const Ball &ball) = 0;

	/**
	*  底层错误
	* @param code  错误码
	* @param errorMessage  错误信息
	*/
	virtual void onError(int code, const char *errorMessage) = 0;

    /**
     * 通知是否线程创建
     * @param isExit 线程正在退出
     */
    virtual void onThread(bool isExit) = 0;
};

class I_TennisBout
    : public I_TennisBase
{
public:
	/**
	* 初始化
	* @param initParam 初始化参数
	* @param pCallback 回调函数
	* @return 成功返回 true；失败返回 false
	*/
    virtual bool TENNIS_STDCALL init(const InitParam &initParam, I_BoutCallback *pCallback) = 0;


	/**
	* 设置球权
	* @param ballPower 球权
	* @return 成功返回 true；失败返回 false
	*/
    virtual bool TENNIS_STDCALL ballPower( BallPower ballPower ) = 0;
};
///////////////////////////////////////////////////the interface for Android///////////////////////////////////////////////////////////////////////////////////

#endif // end __INTERFACE_BOUT_H__