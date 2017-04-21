#ifndef __TENNISBOUT_H__
#define __TENNISBOUT_H__

#include "I_TennisBout.h"
#include "ALoxHelper.h"
#include "Timer.h"

#include <string.h>

static const int HANDLE_STATOUT_MAYBE_COMEBACK = 11000;
static const int HANDLE_STATOUT_NOT_COMEBACK = 20000;

// TODO 
// 1、发球期的连击没有考虑
// 2、首次接球没有考虑连击的事情
class TennisBout
        : public I_TennisBout {
public:
    TennisBout();

    virtual ~TennisBout();

public:
    virtual bool TENNIS_STDCALL init(const InitParam &initParam, I_BoutCallback *pCallback);

    virtual bool TENNIS_STDCALL ballPower(BallPower ballPower);

public:
    virtual bool TENNIS_STDCALL init(const InitParam &initParam);

    virtual bool TENNIS_STDCALL start();

    virtual bool TENNIS_STDCALL pause();

    virtual bool TENNIS_STDCALL resume();

    virtual bool TENNIS_STDCALL stop();

    virtual bool TENNIS_STDCALL runOnce();

    virtual bool TENNIS_STDCALL restart(CourtArea serveArea);

private:
    void initCourtRange(const InitParam &initParam);

    void initCallback(const InitParam &initParam);

    bool handleStat();

    bool handleStatError();

    bool handleStatHit(double statOutElapsed);

    bool handleStatServe();

    bool handleStatTouchDown();

    bool handleStatOut();

    bool handleStatIn(double statOutElapsed);

    double handleStatOutElapsed();

    void handleStatTouchDownServe();

    void addBoxScoreArea11st(int x, int y);

    void addBoxScoreArea12nd(int x, int y);

    void addBoxScoreArea21st(int x, int y);

    void addBoxScoreArea22nd(int x, int y);

    void addBoxScoreArea(PlayerServe playServe);

public:
    virtual unsigned int TENNIS_STDCALL addRef() {
        return ++m_uRefCount;
    }

    virtual unsigned int TENNIS_STDCALL releaseRef() {
        if (--m_uRefCount == 0) {
            delete this;
            return 0;
        }

        return m_uRefCount;
    }

private:
    struct Position {
        int x;
        int y;
    };

    struct CourtRange {
        Position start;
        Position end;
    };

    inline void reset(bool isReserve) {
        m_isServe = false;
        m_isServeSuccessful = false;

        m_isTouchNet = false;
        m_isInvalidData = false;
        m_isCheckDirection = 0;
        m_isCheckDoubleHit = 0;

        m_courtAreaHit = COURTAREA_ANYWHERE;
        m_courtAreaServe = COURTAREA_ANYWHERE;

        memset(&m_ballTouchDown, 0, sizeof(m_ballTouchDown));

        m_ballStatLast = BALLSTAT_NULL;
        m_ballStatCurrent = BALLSTAT_NULL;

        m_ballDirectionLast = BALLDIRECTION_INVALID;
        m_ballDirectionCurrent = BALLDIRECTION_INVALID;

        m_hitCountA = 0;
        m_hitCountB = 0;

        m_playerHit = PLAYER_INVALID;

        m_isWaitLongerTime = false;
        m_isBallOutHappened = false;

        if (!isReserve) {
            m_serveCount = 0;
            memset(&m_matchBoxScore, 0, sizeof(m_matchBoxScore));
        }
    }

    inline bool isInCourtRange(const Position &postion, const CourtRange &courtRange) {
        if (postion.x > courtRange.start.x)
            return false;
        if (postion.x < courtRange.end.x)
            return false;
        if (postion.y < courtRange.start.y)
            return false;
        if (postion.y > courtRange.end.y)
            return false;
        return true;
    }

    inline bool isServeOut(const Ball &ball) {
        switch (m_courtAreaServe) {
            case COURTAREA_A1:
                if ((ball.x > 0) || (ball.y > 0) || (ball.x < -m_courtSize.singleX) ||
                    (ball.y < -m_courtSize.singleY))
                    return true;
                else
                    break;
            case COURTAREA_A2:
                if ((ball.x > 0) || (ball.y < 0) || (ball.x < -m_courtSize.singleX) ||
                    (ball.y > m_courtSize.singleY))
                    return true;
                else
                    break;
            case COURTAREA_B1:
                if ((ball.x < 0) || (ball.y < 0) || (ball.x > m_courtSize.singleX) ||
                    (ball.y > m_courtSize.singleY))
                    return true;
                else
                    break;
            case COURTAREA_B2:
                if ((ball.x < 0) || (ball.y > 0) || (ball.x > m_courtSize.singleX) ||
                    (ball.y < -m_courtSize.singleY))
                    return true;
                else
                    break;
            case COURTAREA_CENTER_A:
                if ((ball.x > 0) || (ball.y < -m_courtSize.singleY) ||
                    (ball.y > m_courtSize.singleY) || (ball.x < -m_courtSize.singleX))
                    return true;
                else
                    break;
            case COURTAREA_CENTER_B:
                if ((ball.x < 0) || (ball.y < -m_courtSize.singleY) ||
                    (ball.y > m_courtSize.singleY) || (ball.x > m_courtSize.singleX))
                    return true;
                else
                    break;
            default:
                // TODO 需要进一步处理未知的情况，无论在哪里击球，只要落地在场内，就可以
                break;
        }

        return false;
    }

    inline CourtArea getCourtArea(const Ball &ball) {
        if (ball.x > 0) {
            if (ball.y > 0)
                return COURTAREA_A1;
            else if (ball.y < 0)
                return COURTAREA_A2;
        } else if (ball.x < 0) {
            if (ball.y > 0)
                return COURTAREA_B2;
            else if (ball.y < 0)
                return COURTAREA_B1;
        } else {
            return COURTAREA_UNKNOWN;
        }

        if (m_ballPower == BALLPOWER_A)
            return COURTAREA_CENTER_A;
        else if (m_ballPower == BALLPOWER_B)
            return COURTAREA_CENTER_B;

        return COURTAREA_UNKNOWN;
    }

    inline BallDirection getBallDirection(const Ball &ball) {
        BallDirection ballDirection = BALLDIRECTION_INVALID;
        // ??? 代码调整，注意测试
        if (ball.x > m_ballLast.x) {
            ballDirection = BALLDIRECTION_A;
        } else if (ball.x < m_ballLast.x) {
            ballDirection = BALLDIRECTION_B;
        } else {
            ballDirection = BALLDIRECTION_UNKNOWN;
        }

        return ballDirection;
    }

    inline CourtArea getCourtAreaHit(const Ball &ball) {
        Position position;
        position.x = ball.x;
        position.y = ball.y;
        // A 面的球馆，不仅仅包含A面场地
        if (isInCourtRange(position, m_alleyRangeA))
            return COURTAREA_A;

        // B 面的球馆，不仅仅包含B面场地
        if (isInCourtRange(position, m_alleyRangeB))
            return COURTAREA_B;

        return COURTAREA_UNKNOWN;
    }

    inline void checkDoubleHitAtAreaA() {
        // 1、B面球员过网击球；2、A面球员网前连击；
        if ((m_ballHit.x > 0) && (m_ballHit.x < 1000)) {
            if (m_ballDirectionLast == BALLDIRECTION_B) {
                if (m_ballDirectionCurrent == BALLDIRECTION_A) {
                    addScoreToCompetitor();
                } else if (m_ballDirectionCurrent == BALLDIRECTION_B) {
                    addScoreToSelf();
                }
            }
        }
    }

    inline void checkDoubleHitAtAreaB() {
        // 1、A面球员过网击球；2、B面球员网前连击；
        if ((m_ballHit.x < 0) && (m_ballHit.x > -1000)) {
            if (m_ballDirectionLast == BALLDIRECTION_A) {
                if (m_ballDirectionCurrent == BALLDIRECTION_B) {
                    addScoreToCompetitor();
                } else if (m_ballDirectionCurrent == BALLDIRECTION_A) {
                    addScoreToSelf();
                }
            }
        }
    }

    inline void checkDoubleHit() {
        if (m_ballDirectionCurrent == BALLDIRECTION_INVALID
            || m_ballDirectionLast == BALLDIRECTION_INVALID)
            return;

        if (m_hitCountA >= 2) {
            checkDoubleHitAtAreaA();
        } else if (m_hitCountB >= 2) {
            checkDoubleHitAtAreaB();
        }
    }

    inline void addRoundScore(Player player) {
        m_matchBoxScore[player].score++;
        if (((player == PLAYER_A) && (m_ballPower == BALLPOWER_A))
            || ((player == PLAYER_B) && (m_ballPower == BALLPOWER_B))) {
            if (m_serveCount == PALYERSERVE_FIRST)
                m_matchBoxScore[player].n1stSocreSNum++;
            if (m_serveCount == PLAYERSERVE_SECOND)
                m_matchBoxScore[player].n2ndSocreSNum++;
        }

        m_pBoutCallback->onAddScore(player, m_matchBoxScore[PLAYER_A], m_matchBoxScore[PLAYER_B],
                                    m_ballCurrent);

        reset(false);
        m_isInvalidData = true;
    }

    inline void addScoreToCompetitor() {
        Player player = m_playerHit;
        if (player == PLAYER_A)
            player = PLAYER_B;
        else if (player == PLAYER_B)
            player = PLAYER_A;
        else
            return;

        addRoundScore(player);
    }

    inline void addScoreToSelf() {
        addRoundScore(m_playerHit);
    }

private:
    I_TennisBase *m_pTennisBase;
    I_BoutCallback *m_pBoutCallback;

    TextFileLogger *m_pTextFileLogger;

    unsigned int m_uRefCount;

    PlayModel m_playModel;
    CourtSize m_courtSize;
    CourtRange m_courtRangeA;
    CourtRange m_courtRangeB;
    CourtRange m_alleyRangeA;
    CourtRange m_alleyRangeB;

    bool m_isServe;
    bool m_isServeSuccessful;

    bool m_isTouchNet;
    bool m_isInvalidData;
    int m_isCheckDirection;
    int m_isCheckDoubleHit;

    BallPower m_ballPower;

    CourtArea m_courtAreaHit;
    CourtArea m_courtAreaServe;

    Ball m_ballHit;
    Ball m_ballServeTouchDown;
    Ball m_ballTouchDown;

    Ball m_ballLast;
    Ball m_ballCurrent;
    BallStat m_ballStatLast;
    BallStat m_ballStatCurrent;
    BallDirection m_ballDirectionLast;
    BallDirection m_ballDirectionCurrent;

    int m_serveCount;
    int m_hitCountA;
    int m_hitCountB;
    Player m_playerHit;
    MatchBoxScore m_matchBoxScore[3];        // 浪费 index = 0 的空间，来减少不必要的重复代码

    // for ball out
    bool m_isWaitLongerTime;
    bool m_isBallOutHappened;

    Ball m_ballOutLast;
    BallStat m_ballStatOutLast;
    Timer m_timeBallOutElapsed;

    bool m_isBallNotComeback;
};

#endif