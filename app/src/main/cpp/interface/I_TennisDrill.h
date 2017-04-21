#ifndef __INTERFACE_DRILL_H__
#define __INTERFACE_DRILL_H__

#include "I_TennisBase.h"

enum DrillResult {
    ER_UNKNOW,
    ER_INVALID,
    ER_IN,            // 界内
    ER_OUT,            // 界外
    ER_NOHIT,       // 没接住
    ER_MULTIHIT,    // 多次接球
};

enum DrillDifficulty {
    DD_UNKNOW,

    DD_EASY,                    // 训练难度：简单
    DD_NORMAL = DD_EASY,

    DD_MEDIUM,                    // 训练难度：容易
    DD_INSIDE = DD_MEDIUM,

    DD_DIFFICULT,                // 训练难度：困难
    DD_OUTSIDE = DD_DIFFICULT,

};

#pragma pack(push, 1)
// 球的落点信息，可能是击球点，也可能是球的落地点
struct BallPoint {
    int x;
    int y;
    bool in;
};

// 训练统计数据
class DrillBoxScore {
public:
    DrillBoxScore() : totalTimes(0), successfulTimes(0), maxSpeed(0),
                      averageSpeed(0), successRatio(0), netTimes(0), outTimes(0), lastResult(ER_UNKNOW),
                      speed(0), downPointsNum(0), hitPointsNum(0) {
    }

    ~DrillBoxScore() {
        if (pBallPointDown != NULL) {
            delete[] pBallPointDown;
            pBallPointDown = NULL;
        }

        if (pBallPointHit != NULL) {
            delete[] pBallPointHit;
            pBallPointHit = NULL;
        }
    }

public:
    int totalTimes;
    int successfulTimes;
    int maxSpeed;
    int averageSpeed;
    int successRatio;
    int netTimes;
    int outTimes;
    DrillResult lastResult;
    int speed;
    int downPointsNum;
    int hitPointsNum;
    BallPoint *pBallPointHit;
    BallPoint *pBallPointDown;
};

struct DataItem {
    union {
        struct {
            bool isTouchNet;

            Ball ballServe;        // 发球位置
            Ball ballServeDown;   // 发球落点
        } ServeItem;

        struct {
            bool isTouchNet;
            bool isOut;

            Ball ballServe;        // 对方发球位置
            Ball ballServeDown;   // 发球落点

            Ball ballHit;          // 击球位置
            Ball ballHitDown;     // 击球落点
        } ReturnServeItem;

        struct {
            bool isTouchNet;

            Ball ballComeDown;    // 来球落点
            Ball ballHit;          // 击球位置
            Ball ballHitDown;     // 击球落点
        } HandItem;

        struct {
            bool isOtherSide;

            Ball ballServe;        // 发球位置
            Ball ballHit;          // 击球位置（截击）
            Ball ballHitDown;     // 击球落点
        } VolleyItem;
    };

    int speed;
    DrillResult drillResult;
};

class I_DrillCallback {
public:


    /**
     * 开始训练后，会触发正式开始（表达底层已经准备好）
     */
    virtual void onBegin() = 0;


    /**
     * 训练过程中，一个回合的训练结束，则触发此接口
     *
     * @param drillBoxScore 训练数据
     */
    virtual void onDataChange(const DrillBoxScore &drillBoxScore)   = 0;


    /**
     * 速度发生变化时，则触发此接口
     *
     * @param speed 当前球速
     */
    virtual void onSpeedChange(int speed)  = 0;


    /**
     * 训练结束后，会触发正式结束（表达底层正式停止）
     */
    virtual void onEnd() = 0;


    /**
     * 下一回合训练已经准备好，可以开始喂球或者发球
     */
    virtual void onDrillNext() = 0;


    /**
     * internal error
     *
     * @param code         error code
     * @param errorMessage
     */
    virtual void onError(int errorCode, const char *pErrorMessage) = 0;


    /**
     * 线程创建与退出
     *
     * @param isExit  线程正在退出
     */
    virtual void onThread(bool isExit) = 0;
};

class I_TennisDrill
        : public I_TennisBase {
public:
    virtual bool TENNIS_STDCALL init(const InitParam &initParam, I_DrillCallback *pDrillCallback, DrillDifficulty drillDifficulty) = 0;
};

#pragma pack(pop)

#endif //__INTERFACE_DRILL_H__
