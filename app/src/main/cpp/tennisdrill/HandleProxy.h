#ifndef __HANDLEPROXY_H__
#define __HANDLEPROXY_H__

#include <mutex>
#include <string.h>
#include <cstdlib>
#include <list>

#include "I_TennisDrill.h"
#include "stat/HandleStat.h"
#include "datas/HandleDatas.h"

#include <ALoxHelper.h>

class HandleStat;

class HandleDatas;

class TennisDrill;

class CourtRange {
public:
    CourtRange()
            : m_left(0), m_top(0), m_right(0), m_bottom(0) {};

    CourtRange(int left, int top, int right, int bottom)
            : m_left(left), m_top(top), m_right(right), m_bottom(bottom) {
        assert(m_left >= m_right);
        assert(m_bottom >= m_top);
    };

    bool contains(int x, int y) {
        return x >= m_right && x <= m_left && y >= m_top && y <= m_bottom;
    }

private:
    int m_left;
    int m_top;
    int m_right;
    int m_bottom;
};

class HandleProxy {
public:
    HandleProxy(TennisDrill *pTennisDrill);

    virtual ~HandleProxy();

public:
    bool init(const InitParam &initParam, I_DrillCallback *pDrillCallback, DrillDifficulty drillDifficulty);

    bool start();

    bool stop();

    bool restart();

public:
    void onBall(BallStat stat, const Ball &ball);

    void onError(int errorCode, const char *pErrorMessage);

    void onDrillNext();

    void onThread(bool isExit);

public:
    DrillResult getDrillResult(const Ball &ballHit, const Ball &ballDown, bool bTouchNet);

    DrillResult
    getDrillResult(const Ball &ballHit, const Ball &ballDown, bool bTouchNet, PlayModel playModel, DrillDifficulty drillDifficulty);

private:
    CourtRange getCourtRange(PlayModel playModel, CourtArea courtArea, DrillDifficulty drillDifficulty, const CourtSize &courtSize);

    CourtRange serveCourtRange(CourtArea courtArea, DrillDifficulty drillDifficulty, const CourtSize &courtSize);

    CourtRange hitCourtRange(CourtArea courtArea, DrillDifficulty drillDifficulty, const CourtSize &courtSize);

    CourtRange straightHitCourtRange(CourtArea courtArea, DrillDifficulty drillDifficulty, const CourtSize &courtSize);

    CourtRange slashHitCourtRange(CourtArea courtArea, DrillDifficulty drillDifficulty, const CourtSize &courtSize);

public:
    bool isBallValid(const Ball &ball) {
        return ball.timestamp != 0;
    }

    CourtArea getCourtArea(int x, int y) {
        if (x >= 0) {
            if (y >= 0) {
                return COURTAREA_A1;
            } else {
                return COURTAREA_A2;
            }
        } else {
            if (y >= 0) {
                return COURTAREA_B2;
            } else {
                return COURTAREA_B1;
            }
        }
    }

    CourtArea getCourtSide(CourtArea courtArea) {
        switch (courtArea) {
            case COURTAREA_UNKNOWN:
                break;
            case COURTAREA_A1:
            case COURTAREA_A2:
            case COURTAREA_A:
                return COURTAREA_A;
            case COURTAREA_B1:
            case COURTAREA_B2:
            case COURTAREA_B:
                return COURTAREA_B;
            default:
                break;
        }

        return COURTAREA_UNKNOWN;
    }

    CourtArea getTrainedSide() {
        return getCourtSide(m_initParam.servePosition);
    }

    bool isIn(const Ball &ball) {
        int x = abs(ball.x);
        int y = abs(ball.y);
        switch (m_initParam.playModel & PLAYMODEL_MASK) {
            case PLAYMODEL_DRILL_SINGLE:
                return x < m_initParam.courtSize.baselineX && y < m_initParam.courtSize.singleY;
            case PLAYMODEL_DRILL_DOUBLE:
                return x < m_initParam.courtSize.baselineX && y < m_initParam.courtSize.doubleY;
            default:
                break;
        }

        return false;
    }

    bool isSameCourtArea(const Ball &ball) {
        return getCourtArea(ball.x, ball.y) == m_initParam.servePosition;
    }

    bool isSameCourtSide(const Ball &ball) {
        return getCourtSide(getCourtArea(ball.x, ball.y)) == getCourtSide(m_initParam.servePosition);
    }

public:
    const InitParam &getInitParam() {
        return m_initParam;
    }

protected:
    InitParam m_initParam;
    DrillDifficulty m_difficultyLevel;
    I_DrillCallback *m_pDrillCallback;

    TennisDrill *m_pTennisDrill;

    HandleStat *m_pHandleStat;
    HandleDatas *m_pDrillDatas;

    std::mutex m_mutexStat;
    std::mutex m_mutexDatas;
};

#endif // BASEHANDLER_H__

