#include "TennisBout.h"
#include "TennisBaseHelper.h"
#include "ErrorHelper.h"

TennisBout::TennisBout()
        : m_pTennisBase(NULL), m_pBoutCallback(NULL), m_pTextFileLogger(NULL),
          m_ballPower(BALLPOWER_A), m_uRefCount(0), m_isBallNotComeback(false) {
}

TennisBout::~TennisBout() {
    if (m_pTennisBase != NULL) {
        TennisBaseHelper::DestroyTennisBase(m_pTennisBase);
        m_pTennisBase = NULL;
    }

    if (m_pTextFileLogger != NULL) {
        Log_RemoveLogger(m_pTextFileLogger);

        Log_Prune(delete m_pTextFileLogger;)
        m_pTextFileLogger = NULL;
    }
}

bool TennisBout::init(const InitParam &initParam, I_BoutCallback *pCallback) {
    std::string strLog = initParam.strTmpPath + "TennisBout.txt";
    m_pTextFileLogger = ALoxHelper::CreateDbgTextFileLogger(strLog.c_str());

    assert(m_pTennisBase == NULL);
    std::string strFileName = initParam.strAppPath + "libtennisbase.so";
    m_pTennisBase = TennisBaseHelper::CreateTennisBase(strFileName.c_str());
    if (m_pTennisBase == NULL)
        return false;

    m_pBoutCallback = pCallback;

    initCourtRange(initParam);
    initCallback(initParam);
    init(initParam);

    reset(false);

    return true;
}

bool TennisBout::ballPower(BallPower ballPower) {
    m_ballPower = ballPower;
    return true;
}

bool TennisBout::init(const InitParam &initParam) {
    return m_pTennisBase->init(initParam);
}

bool TennisBout::start() {
    return m_pTennisBase == NULL ? false : (m_pTennisBase->start());
}

bool TennisBout::pause() {
    return m_pTennisBase == NULL ? false : (m_pTennisBase->pause());
}

bool TennisBout::resume() {
    return m_pTennisBase == NULL ? false : (m_pTennisBase->resume());
}

bool TennisBout::stop() {
    return m_pTennisBase == NULL ? false : (m_pTennisBase->stop());
}

bool TennisBout::runOnce() {
    return m_pTennisBase == NULL ? false : (m_pTennisBase->runOnce());
}

bool TennisBout::restart(CourtArea serveArea) {
    return m_pTennisBase == NULL ? false : (m_pTennisBase->restart(serveArea));
}

void TennisBout::initCourtRange(const InitParam &initParam) {
    m_playModel = initParam.playModel;
    m_courtSize = initParam.courtSize;

    // 场馆边界
    m_alleyRangeA.start.x = initParam.courtSize.courtX;
    m_alleyRangeA.start.y = -initParam.courtSize.courtY;
    m_alleyRangeA.end.x = 0;
    m_alleyRangeA.end.y = initParam.courtSize.courtY;

    m_alleyRangeB.start.x = 0;
    m_alleyRangeB.start.y = -initParam.courtSize.courtY;
    m_alleyRangeB.end.x = -initParam.courtSize.courtX;
    m_alleyRangeB.end.y = initParam.courtSize.courtY;

    // 场地边界
    if ((initParam.playModel & PLAYMODEL_MASK) == PLAYMODEL_MATCH_SINGLE) {
        m_courtRangeA.start.x = initParam.courtSize.singleX;
        m_courtRangeA.start.y = -initParam.courtSize.singleY;
        m_courtRangeA.end.x = 0;
        m_courtRangeA.end.y = initParam.courtSize.singleY;

        m_courtRangeB.start.x = 0;
        m_courtRangeB.start.y = -initParam.courtSize.singleY;
        m_courtRangeB.end.x = -initParam.courtSize.singleX;
        m_courtRangeB.end.y = initParam.courtSize.singleY;
    } else {
        m_courtRangeA.start.x = initParam.courtSize.doubleX;
        m_courtRangeA.start.y = -initParam.courtSize.doubleY;
        m_courtRangeA.end.x = 0;
        m_courtRangeA.end.y = initParam.courtSize.doubleY;

        m_courtRangeB.start.x = 0;
        m_courtRangeB.start.y = -initParam.courtSize.doubleY;
        m_courtRangeB.end.x = -initParam.courtSize.doubleX;
        m_courtRangeB.end.y = initParam.courtSize.doubleY;
    }
}

void TennisBout::initCallback(const InitParam &initParam) {
    m_pTennisBase->onBall = [this](BallStat ballStat, const Ball &ball) {
        if (ballStat != BALLSTAT_SERVE) {
            // TODO 需要打日志
            if (m_isInvalidData)
                return;
        }

        m_ballCurrent = ball;
        m_ballStatCurrent = ballStat;

        m_pBoutCallback->onBall(m_ballCurrent);
        if (!handleStat()) {
            //如果击球则开启方向的检查(击球事件过了的下一个坐标了)
            if (--m_isCheckDirection == 0) {
                if (m_ballDirectionLast == BALLDIRECTION_INVALID) {
                    m_ballDirectionLast = getBallDirection(m_ballCurrent);
                } else if (m_ballDirectionCurrent == BALLDIRECTION_INVALID) {
                    m_ballDirectionCurrent = getBallDirection(m_ballCurrent);
                } else {
                    m_ballDirectionLast = m_ballDirectionCurrent;
                    m_ballDirectionCurrent = getBallDirection(m_ballCurrent);
                }
            }

            if (--m_isCheckDoubleHit == 0) {
                checkDoubleHit();
            }
        }

        if (m_ballStatCurrent != BALLSTAT_NULL && m_isInvalidData != true) {
            m_ballLast = m_ballCurrent;
            m_ballStatLast = m_ballStatCurrent;
        }
    };

    m_pTennisBase->onError = [this](int errorCode, const char *pErrorMessage) {
        m_pBoutCallback->onError(errorCode, pErrorMessage);
    };

    m_pTennisBase->onThread = [this](bool isExit) {
        m_pBoutCallback->onThread(isExit);
    };
}

bool TennisBout::handleStat() {
    bool bHandled = handleStatError();
    if (bHandled)
        return bHandled;

    double statOutElapsed = handleStatOutElapsed();
    if (statOutElapsed == -1)
        return true;

    switch (m_ballStatCurrent) {
        case BALLSTAT_NULL:
            return false;
        case BALLSTAT_HIT:
        case BALLSTAT_HIT_GUESS:
            bHandled = handleStatHit(statOutElapsed);
            break;
        case BALLSTAT_SERVE:
            bHandled = handleStatServe();
            break;
        case BALLSTAT_TOUCH_DOWN:
            bHandled = handleStatTouchDown();
            m_ballTouchDown = m_ballCurrent;
            break;
        case BALLSTAT_TOUCH_NET:
            break;
        case BALLSTAT_TOUCH_ANY:
            break;
        case BALLSTAT_OUT:
            bHandled = handleStatOut();
            break;
        case BALLSTAT_NON_UNIQUENESS:
            break;
        case BALLSTAT_ERROR:
            break;
        case BALLSTAT_IN:
            bHandled = handleStatIn(statOutElapsed);
            break;
        case BALLSTAT_ERROR_HIT:
            break;
        case BALLSTAT_WAITING:
            break;
        default:
            break;
    }

    return bHandled;
}

bool TennisBout::handleStatError() {
    // TODO 出现不能出现的错误后，目前仅仅是记录日志，应该需要有其他处理办法
    switch (m_ballStatCurrent) {
        case BALLSTAT_HIT:
            switch (m_ballStatLast) {
                case BALLSTAT_TOUCH_DOWN:
                    // 已处理
                    break;
                case BALLSTAT_SERVE:
                case BALLSTAT_OUT:
                    m_pBoutCallback->onError(getErrorCode(m_ballStatLast, m_ballStatCurrent), NULL);
                    m_isInvalidData = true;
                    return true;
                default:
                    break;
            }
            break;
        case BALLSTAT_SERVE:
            switch (m_ballStatLast) {
                case BALLSTAT_HIT:
                case BALLSTAT_SERVE:
                case BALLSTAT_TOUCH_DOWN:
                case BALLSTAT_TOUCH_NET:
                case BALLSTAT_OUT:
                case BALLSTAT_IN:
                case BALLSTAT_HIT_GUESS:
                    m_pBoutCallback->onError(getErrorCode(m_ballStatLast, m_ballStatCurrent), NULL);
                    m_isInvalidData = true;
                    return true;
                default:
                    break;
            }
            break;
        case BALLSTAT_TOUCH_DOWN:
            switch (m_ballStatLast) {
                case BALLSTAT_OUT:
                    m_pBoutCallback->onError(getErrorCode(m_ballStatLast, m_ballStatCurrent), NULL);
                    m_isInvalidData = true;
                    return true;
                default:
                    break;
            }
            break;
        case BALLSTAT_TOUCH_NET:
            switch (m_ballStatLast) {
                case BALLSTAT_TOUCH_DOWN:
                case BALLSTAT_TOUCH_NET:
                case BALLSTAT_OUT:
                    m_pBoutCallback->onError(getErrorCode(m_ballStatLast, m_ballStatCurrent), NULL);
                    m_isInvalidData = true;
                    return true;
                default:
                    break;
            }
            break;
        case BALLSTAT_IN:
            switch (m_ballStatLast) {
                case BALLSTAT_HIT:
                case BALLSTAT_SERVE:
                case BALLSTAT_TOUCH_DOWN:
                case BALLSTAT_TOUCH_NET:
                case BALLSTAT_IN:
                case BALLSTAT_HIT_GUESS:
                    m_pBoutCallback->onError(getErrorCode(m_ballStatLast, m_ballStatCurrent), NULL);
                    m_isInvalidData = true;
                    return true;
                default:
                    break;
            }
            break;
        case BALLSTAT_HIT_GUESS:
            switch (m_ballStatLast) {
                case BALLSTAT_HIT:
                case BALLSTAT_SERVE:
                case BALLSTAT_TOUCH_DOWN:
                case BALLSTAT_TOUCH_NET:
                case BALLSTAT_IN:
                case BALLSTAT_HIT_GUESS:
                    m_pBoutCallback->onError(getErrorCode(m_ballStatLast, m_ballStatCurrent), NULL);
                    return true;
                default:
                    break;
            }
            break;
        case BALLSTAT_TOUCH_ANY:
        case BALLSTAT_OUT:
        case BALLSTAT_NON_UNIQUENESS:
        case BALLSTAT_ERROR:
        case BALLSTAT_ERROR_HIT:
        case BALLSTAT_WAITING:
        default:
            break;
    }

    return false;
}

bool TennisBout::handleStatHit(double statOutElapsed) {
    // TODO ???
    if (m_isBallOutHappened
        && ((statOutElapsed <= 1.0) || ((statOutElapsed <= 2.0) && m_isWaitLongerTime))
        && (m_ballStatLast == BALLSTAT_OUT) && (m_ballStatCurrent == BALLSTAT_HIT_GUESS)) {
        char szLog[255] = {0};
        sprintf(
                szLog,
                "Success to handle stat out when stat hit happened, isWaitLongerTime: %d, statOutElapsed: %f",
                m_isWaitLongerTime, statOutElapsed);
        Log_Warning(std::string(szLog));

        m_isBallOutHappened = false;

        m_ballLast = m_ballOutLast;
        m_ballStatLast = m_ballStatOutLast;
    }

    if (m_isInvalidData)
        return true;

    // TODO 如果发球过去，对方没有接到，那么这里就不会出现hit事件，最终导致这个没有记录
    if (m_isServe) {
        m_isServe = false;
        if (m_serveCount <= PLAYERSERVE_SECOND) {
            // 实际这里的击球者还是发球者，发球与击球是独立的
            m_pBoutCallback->onServeTouchDownPosition(
                    m_playerHit, (PlayerServe) m_serveCount, m_ballServeTouchDown);
        }

        // TODO 发球出现双击，应该算是失误一次，如何处理
        //if (((m_ballCurrent.x > 0) && (m_playerHit == PLAYER_A))
        //	|| ((m_ballCurrent.x < 0) && (m_playerHit == PLAYER_B)))
        //{
        //	addScoreToCompetitor();
        //	return true;
        //}
    }

    if (m_playerHit == PLAYER_A) {
        m_playerHit = PLAYER_B;
    } else if (m_playerHit == PLAYER_B) {
        m_playerHit = PLAYER_A;
    } else {
        assert(false && "Invalid player");
    }

    // 除去网前的双击判断
    // TODO ??? 做了调整，注意测试
    if (((m_ballCurrent.x > 1000) && (m_playerHit == PLAYER_B))
        || ((m_ballCurrent.x < -1000) && (m_playerHit == PLAYER_A))) {
        addScoreToSelf();
        return true;
    }

    // 技术统计
    {
        // TODO 需要根据单双打动态决定这个技术参数，目前是硬编码写死的（目前传入的场地参数不够，故先不调整）
        char szLog[255] = {0};
        sprintf(
                szLog,
                "Hit position,, m_ballCurrent.x: %d, m_ballCurrent.y: %d, m_ballCurrent.z: %d, m_playerHit: %d",
                m_ballCurrent.x, m_ballCurrent.y, m_ballCurrent.z, m_playerHit);
        Log_Verbose(std::string(szLog));

        // TODO ??? 做了调整，注意测试
        int x = abs(m_ballCurrent.x);
        if ((x >= 0) && (x <= 11887))
            m_matchBoxScore[m_playerHit].hitNumInside++;
        else if ((x > 11887) && (x <= 13887))
            m_matchBoxScore[m_playerHit].hitNumBaseline++;
        else if ((x > 13887) && (x <= 15887))
            m_matchBoxScore[m_playerHit].hitNumOutside++;
        m_matchBoxScore[m_playerHit].hitTotalNum++;

        m_ballHit = m_ballCurrent;
        m_pBoutCallback->onHitPosition(m_playerHit, m_ballHit);
    }

    // 发起检测双击
    {
        m_isTouchNet = false;
        m_isCheckDirection = 2;
        CourtArea courtArea = getCourtAreaHit(m_ballHit);
        if (courtArea != COURTAREA_UNKNOWN && m_courtAreaHit == courtArea) {
            m_isCheckDoubleHit = 2;
        } else {
            m_hitCountA = 0;
            m_hitCountB = 0;
        }

        m_courtAreaHit = courtArea;
        if (m_courtAreaHit == COURTAREA_A) {
            m_hitCountA++;
        } else if (m_courtAreaHit == COURTAREA_B) {
            m_hitCountB++;
        }
    }

    return false;
}

bool TennisBout::handleStatServe() {
    m_playerHit = m_ballPower == BALLPOWER_A ? PLAYER_A : PLAYER_B;
    m_courtAreaServe = getCourtArea(m_ballCurrent);
    if (((m_playerHit == PLAYER_A) && (m_ballCurrent.x > 0))
        || ((m_playerHit == PLAYER_B) && (m_ballCurrent.x < 0))) {
        m_isServe = true;
        m_isServeSuccessful = false;

        m_isTouchNet = false;
        m_isInvalidData = false;

        m_serveCount++;
        if (m_serveCount == PALYERSERVE_FIRST) {
            m_matchBoxScore[m_playerHit].n1stServeTotalNum++;
        } else if (m_serveCount == PLAYERSERVE_SECOND) {
            m_matchBoxScore[m_playerHit].n2ndServeTotalNum++;
        }

        if (m_ballCurrent.speed > m_matchBoxScore[m_playerHit].speedMax)
            m_matchBoxScore[m_playerHit].speedMax = m_ballCurrent.speed;

        m_pBoutCallback->onServe(m_ballCurrent);
    } else {
        m_isInvalidData = true;

        // TODO 检测出了问题，需要通知底层发球者错误，需要重新发球
        m_pBoutCallback->onError(ERROR_INVALID_SERVER, "Invalid serve");
    }

    return true;
}

bool TennisBout::handleStatTouchDown() {
    if (m_isServe) {
        handleStatTouchDownServe();
        return true;
    }

    if (m_ballStatLast == BALLSTAT_TOUCH_DOWN) {
        if ((m_ballLast.x * m_ballCurrent.x) < 0) {
            // 出错，只能简单报错，然后猜分
            m_pBoutCallback->onError(getErrorCode(m_ballStatLast, m_ballStatCurrent), NULL);
        }

        // 出现双跳，除了异常之外，都是打过去了，那都是我的分
        m_matchBoxScore[m_playerHit].winners++;

        addScoreToSelf();
        return true;
    }

    CourtRange courtRangeSelf = m_playerHit == PLAYER_A ? m_courtRangeA : m_courtRangeB;
    CourtRange courtRangeCompetitor = m_playerHit == PLAYER_A ? m_courtRangeB : m_courtRangeA;

    Position postion;
    postion.x = m_ballCurrent.x;
    postion.y = m_ballCurrent.y;
    if (m_isBallNotComeback) {
        postion.x = m_ballTouchDown.x;
        postion.y = m_ballTouchDown.y;

        // TODO BUG 这里得分，并不当前球的位置，会导致统计错误
        if (isInCourtRange(postion, courtRangeSelf)) {
            addScoreToSelf();
        } else {
            addScoreToCompetitor();
        }

        m_isBallNotComeback = false;

        return true;
    }

    if (isInCourtRange(postion, courtRangeSelf)
        || !isInCourtRange(postion, courtRangeCompetitor)) {
        addScoreToCompetitor();
    }

    return true;
}

bool TennisBout::handleStatOut() {
    // TODO From Joe: 需要对照老马状态机再做一次Check 6,2000事件
    if (abs(m_ballCurrent.x) == HANDLE_STATOUT_NOT_COMEBACK) {
        m_isBallOutHappened = false;
        m_isBallNotComeback = true;

        char szLog[255] = {0};
        sprintf(
                szLog,
                "Failed to handle stat out, m_ballCurrent.x: %d, m_ballCurrent.y: %d, m_ballCurrent.z: %d, m_isBallNotComeback: %d",
                m_ballCurrent.x, m_ballCurrent.y, m_ballCurrent.z, m_isBallNotComeback);
        Log_Warning(std::string(szLog));

        //m_ballDataLast.ballStat = m_ballStatOutLast.ballStat;
        //m_ballDataLast.ball = m_ballStatOutLast.ball;


        m_ballTouchDown.x = 200000;
        m_ballTouchDown.y = 200000;
        handleStatTouchDown();
        m_ballTouchDown = m_ballCurrent;

        return true;
    }

    m_isBallOutHappened = true;
    if (abs(m_ballCurrent.x) >= HANDLE_STATOUT_MAYBE_COMEBACK)
        m_isWaitLongerTime = true;
    else
        m_isWaitLongerTime = false;

    m_timeBallOutElapsed.reset();

    m_ballOutLast = m_ballLast;
    m_ballStatOutLast = m_ballStatLast;

    return true;
}

bool TennisBout::handleStatIn(double statOutElapsed) {
    // TODO ??? 做了调整，注意测试
    if (m_isBallOutHappened
        && ((statOutElapsed <= 1.0) || ((statOutElapsed <= 2.0) && m_isWaitLongerTime))
        && (m_ballStatLast == BALLSTAT_OUT)) {
        char szLog[255] = {0};
        sprintf(
                szLog,
                "Success to handle stat out when stat in happened, m_isWaitLongerTime: %d, statOutElapsed: %f",
                m_isWaitLongerTime, statOutElapsed);
        Log_Warning(std::string(szLog));

        m_isBallOutHappened = false;
        m_ballLast = m_ballOutLast;
        m_ballStatLast = m_ballStatOutLast;

        return false;
    }

    return true;
}

double TennisBout::handleStatOutElapsed() {
    // TODO ??? 做了调整，注意测试
    // TODO 出视野后，需要等事件进行处理，由于不一定有后续事件，故需要添加定时器
    double statOutElapsed = m_timeBallOutElapsed.elapsed_second();
    if (m_isBallOutHappened
        && (statOutElapsed > 2 || ((statOutElapsed > 1) && !m_isWaitLongerTime))
        && (m_ballStatLast == BALLSTAT_OUT)) {
        char szLog[255] = {0};
        sprintf(
                szLog,
                "Failed to handle stat out, because time out, m_isWaitLongerTime: %d, statOutElapsed: %f",
                m_isWaitLongerTime, statOutElapsed);
        Log_Warning(std::string(szLog));

        m_isBallOutHappened = false;
        m_ballLast = m_ballOutLast;
        m_ballStatLast = m_ballStatOutLast;

        Ball ballCurrent = m_ballCurrent;
        BallStat ballStatCurrent = m_ballStatCurrent;

        // 作为界外落地处理
        m_ballCurrent.x = m_courtRangeA.start.x - HANDLE_STATOUT_MAYBE_COMEBACK;
        m_ballCurrent.y = m_courtRangeA.start.y - HANDLE_STATOUT_NOT_COMEBACK;
        handleStatTouchDown();
        m_ballTouchDown = m_ballCurrent;

        m_ballCurrent = ballCurrent;
        m_ballStatCurrent = ballStatCurrent;

        return 0;
    }

    return statOutElapsed;
}

void TennisBout::handleStatTouchDownServe() {
    if (m_isServeSuccessful) {
        char szLog[255] = {0};
        sprintf(
                szLog,
                "Ace position, m_ballCurrent.x: %d, m_ballCurrent.y: %d, m_ballCurrent.z: %d, m_playerHit: %d",
                m_ballCurrent.x, m_ballCurrent.y, m_ballCurrent.z, m_playerHit);
        Log_Verbose(std::string(szLog));

        m_matchBoxScore[m_playerHit].ace++;
        m_pBoutCallback->onBallAce(m_playerHit);
        m_pBoutCallback->onServeTouchDownPosition(m_playerHit, PALYERSERVE_ACE,
                                                  m_ballServeTouchDown);

        addScoreToSelf();
        return;
    }

    bool isOut = isServeOut(m_ballCurrent);
    if (isOut) {
        m_pBoutCallback->onServeFailed();
        m_isInvalidData = true;     //后面还要判断doubleFaultsNum，不能返回
    } else {
        if (m_ballStatLast == BALLSTAT_TOUCH_NET) {
            // 触网不算双误
            m_serveCount--;
        } else {
            m_ballServeTouchDown = m_ballCurrent;
            m_isServeSuccessful = true;

            m_pBoutCallback->onServeSuccessful();
        }
    }

    if (m_serveCount <= PLAYERSERVE_SECOND) {
        if (m_isServeSuccessful) {
            addBoxScoreArea((PlayerServe) m_serveCount);
            m_pBoutCallback->onServeTouchDownPosition(m_playerHit, (PlayerServe) m_serveCount,
                                                      m_ballServeTouchDown);
        } else if (m_serveCount == PLAYERSERVE_SECOND) {
            char szLog[255] = {0};
            sprintf(
                    szLog,
                    "Double Fault, m_playerHit: %d",
                    m_playerHit);
            Log_Verbose(std::string(szLog));

            m_matchBoxScore[m_playerHit].doubleFaultsNum++;

            addScoreToCompetitor();
            return;
        } else {
            reset(true);
        }
    } else {
        assert(false && "Serve too many times.");
    }
}

void TennisBout::addBoxScoreArea11st(int x, int y) {
    if ((x >= 0) && (x <= 6451)) {
        if ((y >= 0) && (y <= 2058)) {
            // 1区内侧
            m_matchBoxScore[m_playerHit].area11stInsideNum++;
        } else if ((y >= 2059) && (y <= 4165)) {
            // 1区外侧
            m_matchBoxScore[m_playerHit].area11stOutsideNum++;
        }
    }
}

void TennisBout::addBoxScoreArea12nd(int x, int y) {
    if ((x >= 0) && (x <= 6451)) {
        if ((y >= 0) && (y <= 2058)) {
            // 1区内侧
            m_matchBoxScore[m_playerHit].area12ndInsideNum++;
        } else if ((y >= 2059) && (y <= 4165)) {
            // 1区外侧
            m_matchBoxScore[m_playerHit].area12ndOutsideNum++;
        }
    }
}

void TennisBout::addBoxScoreArea21st(int x, int y) {
    if ((x >= 0) && (x <= 6451)) {
        if ((y >= 0) && (y <= 2058)) {
            // 2区内侧
            m_matchBoxScore[m_playerHit].area21stInsideNum++;
        } else if ((y >= 2059) && (y <= 4165)) {
            // 2区外侧
            m_matchBoxScore[m_playerHit].area21stOutsideNum++;
        }
    }
}

void TennisBout::addBoxScoreArea22nd(int x, int y) {
    if ((x >= 0) && (x <= 6451)) {
        if ((y >= 0) && (y <= 2058)) {
            // 2区内侧
            m_matchBoxScore[m_playerHit].area22ndInsideNum++;
        } else if ((y >= 2059) && (y <= 4165)) {
            // 2区外侧
            m_matchBoxScore[m_playerHit].area22ndOutsideNum++;
        }
    }
}

void TennisBout::addBoxScoreArea(PlayerServe playServe) {
    char szLog[255] = {0};
    sprintf(
            szLog,
            "Touchdown position, m_ballCurrent.x: %d, m_ballCurrent.y: %d, m_ballCurrent.z: %d, playServe: %d, m_playerHit: %d.\r\n",
            m_ballCurrent.x, m_ballCurrent.y, m_ballCurrent.z, playServe, m_playerHit);
    Log_Verbose(std::string(szLog));

    if (playServe == PALYERSERVE_FIRST) {
        m_matchBoxScore[m_playerHit].n1stServeSNum++;
    } else if (playServe == PLAYERSERVE_SECOND) {
        m_matchBoxScore[m_playerHit].n2ndServeSNum++;
    }

    int x = abs(m_ballServeTouchDown.x);
    int y = abs(m_ballServeTouchDown.y);
    if (m_courtAreaServe == COURTAREA_A1 || m_courtAreaServe == COURTAREA_B1) {
        if (((m_ballServeTouchDown.x >= 0) && (m_ballServeTouchDown.y >= -50))
            || ((m_ballServeTouchDown.x <= 0) && (m_ballServeTouchDown.y <= 50))) {
            if (playServe == PALYERSERVE_FIRST) {
                addBoxScoreArea11st(x, y);
            } else if (playServe == PLAYERSERVE_SECOND) {
                addBoxScoreArea12nd(x, y);
            }
        }
    } else if (m_courtAreaServe == COURTAREA_A2 || m_courtAreaServe == COURTAREA_B2) {
        if (((m_ballServeTouchDown.x >= 0) && (m_ballServeTouchDown.y <= 50))
            || ((m_ballServeTouchDown.x <= 0) && (m_ballServeTouchDown.y >= -50))) {
            if (playServe == PALYERSERVE_FIRST) {
                addBoxScoreArea21st(x, y);
            } else if (playServe == PLAYERSERVE_SECOND) {
                addBoxScoreArea22nd(x, y);
            }
        }

    } else if (m_courtAreaServe == COURTAREA_CENTER_A || m_courtAreaServe == COURTAREA_CENTER_B) {
        if (((m_ballServeTouchDown.x >= 0) && (m_ballServeTouchDown.y >= 0))
            || ((m_ballServeTouchDown.x <= 0) && (m_ballServeTouchDown.y <= 0))) {
            if (playServe == PALYERSERVE_FIRST) {
                addBoxScoreArea11st(x, y);
            } else if (playServe == PLAYERSERVE_SECOND) {
                addBoxScoreArea12nd(x, y);
            }
        } else {
            if (playServe == PALYERSERVE_FIRST) {
                addBoxScoreArea21st(x, y);
            } else if (playServe == PLAYERSERVE_SECOND) {
                addBoxScoreArea22nd(x, y);
            }
        }
    }
}
