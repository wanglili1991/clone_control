#include "HandleProxy.h"
#include "stat/HandleStatServe.h"
#include "datas/HandleDatasServe.h"
#include "stat/HandleStatReturnServe.h"
#include "datas/HandleDatasReturnServe.h"
#include "stat/HandleStatVolley.h"
#include "datas/HandleDatasVolley.h"
#include "stat/HandleStatHand.h"
#include "datas/HandleDatasHand.h"

#include "TennisDrill.h"

HandleProxy::HandleProxy(TennisDrill *pTennisDrill)
        : m_pTennisDrill(pTennisDrill), m_pDrillCallback(NULL), m_pHandleStat(NULL), m_pDrillDatas(NULL) {
}

HandleProxy::~HandleProxy() {
    if (m_pHandleStat != NULL) {
        delete m_pHandleStat;
        m_pHandleStat = NULL;
    }

    if (m_pDrillDatas != NULL) {
        delete m_pDrillDatas;
        m_pDrillDatas = NULL;
    }
}

bool HandleProxy::init(const InitParam &initParam, I_DrillCallback *pDrillCallback, DrillDifficulty drillDifficulty) {
    assert(initParam.servePosition != COURTAREA_UNKNOWN);
    if (pDrillCallback == NULL)
        return false;

    HandleStat *pHandleStat = NULL;
    HandleDatas *pHandleDatas = NULL;
    switch (initParam.playModel & PLAYMODEL_DRILL_MODEL_MASK) {
        // 发球
        case PLAYMODEL_DRILL_SERVE: {
            pHandleStat = new HandleStatServe(this);
            pHandleDatas = new HandleDatasServe(this);
            break;
        }
            // 接发球
        case PLAYMODEL_DRILL_RECEIVE: {
            pHandleStat = new HandleStatReturnServe(this);
            pHandleDatas = new HandleDatasReturnServe(this);
            break;
        }
            // 截击
        case PLAYMODEL_DRILL_VOLLEY: {
            pHandleStat = new HandleStatVolley(this);
            pHandleDatas = new HandleDatasVolley(this);
            break;
        }
            // 多球暂且不做
        case PLAYMODEL_DRILL_MULTI_BALL:
            break;
            // 目前正手击球，正手直线，正手斜线，反手击球，反手直线，反手斜线可以使用一套逻辑
        case PLAYMODEL_DRILL_FOREHAND_HIT:
        case PLAYMODEL_DRILL_FOREHAND_STRAIGHT:
        case PLAYMODEL_DRILL_FOREHAND_SLASH:
        case PLAYMODEL_DRILL_BACKHAND_HIT:
        case PLAYMODEL_DRILL_BACKHAND_STRAIGHT:
        case PLAYMODEL_DRILL_BACKHAND_SLASH:
        case PLAYMODEL_DRILL_CASUAL: {
            pHandleStat = new HandleStatHand(this);
            pHandleDatas = new HandleDatasHand(this);
            break;
        }
        default:
            break;
    }

    if (pHandleStat == NULL || pHandleDatas == NULL)
        return false;

    m_initParam = initParam;
    m_pDrillCallback = pDrillCallback;
    m_difficultyLevel = drillDifficulty;

    m_pHandleStat = pHandleStat;
    m_pDrillDatas = pHandleDatas;

    return true;
}

bool HandleProxy::start() {
    m_pDrillCallback->onBegin();
    return true;
}

bool HandleProxy::stop() {
    m_pDrillCallback->onEnd();

    return true;
}

bool HandleProxy::restart() {
#ifndef __TENNIS_DRILL_TEST__
    std::lock_guard<std::mutex> lockDatas(m_mutexDatas);
    m_pDrillDatas->reset();
#endif
    return true;
}

void HandleProxy::onBall(BallStat stat, const Ball &ball) {
    {
        std::lock_guard<std::mutex> lockStat(m_mutexStat);
        if (m_pHandleStat->onBall(stat, ball)) {
            m_pTennisDrill->restartDrill(m_initParam.servePosition);

            if (m_pHandleStat->getLatestResult() == ER_INVALID) {
#ifdef __TENNIS_DRILL_TEST__
                m_pDrillCallback->onDataChange(m_pDrillDatas->getDrillBoxScore());
#endif
                m_pHandleStat->reset();

                return;
            }

            {
                std::lock_guard<std::mutex> lockDatas(m_mutexDatas);
                m_pDrillDatas->add(m_pHandleStat->getDataItem());
                m_pDrillCallback->onDataChange(m_pDrillDatas->getDrillBoxScore());
            }

            m_pHandleStat->reset();
        } else {
            m_pDrillDatas->setLatestSpeed(m_pHandleStat->getLatestSpeed());
            m_pDrillCallback->onSpeedChange(m_pHandleStat->getLatestSpeed());
        }
    }
}

void HandleProxy::onError(int errorCode, const char *pErrorMessage) {
    m_pDrillCallback->onError(errorCode, pErrorMessage);
}

void HandleProxy::onDrillNext() {
    m_pDrillCallback->onDrillNext();
}

void HandleProxy::onThread(bool isExit) {
    m_pDrillCallback->onThread(isExit);
}

DrillResult HandleProxy::getDrillResult(const Ball &ballHit, const Ball &ballDown, bool bTouchNet) {
    return getDrillResult(ballHit, ballDown, bTouchNet, m_initParam.playModel, m_difficultyLevel);
}

DrillResult HandleProxy::getDrillResult(const Ball &ballHit, const Ball &ballDown, bool bTouchNet, PlayModel playModel,
                                        DrillDifficulty drillDifficulty) {
    CourtArea courtArea = getCourtArea(ballHit.x, ballHit.y);
    CourtRange courtRange = getCourtRange(playModel, courtArea, drillDifficulty, m_initParam.courtSize);
    if (courtRange.contains(ballDown.x, ballDown.y)) {
        if (bTouchNet && ((playModel & PLAYMODEL_DRILL_MODEL_MASK) == PLAYMODEL_DRILL_SERVE)) {
            return ER_INVALID;
        } else {
            return ER_IN;
        }
    } else {
        return ER_OUT;
    }
}

CourtRange
HandleProxy::getCourtRange(PlayModel playModel, CourtArea courtArea, DrillDifficulty drillDifficulty, const CourtSize &courtSize) {
    switch (playModel & PLAYMODEL_DRILL_MODEL_MASK) {
        case PLAYMODEL_DRILL_SERVE:
            return serveCourtRange(courtArea, drillDifficulty, courtSize);
        case PLAYMODEL_DRILL_BACKHAND_HIT:
        case PLAYMODEL_DRILL_FOREHAND_STRAIGHT:
            return straightHitCourtRange(courtArea, drillDifficulty, courtSize);
        case PLAYMODEL_DRILL_FOREHAND_SLASH:
        case PLAYMODEL_DRILL_BACKHAND_STRAIGHT:
            return slashHitCourtRange(courtArea, drillDifficulty, courtSize);
        case PLAYMODEL_DRILL_RECEIVE:
        case PLAYMODEL_DRILL_FOREHAND_HIT:
        case PLAYMODEL_DRILL_BACKHAND_SLASH:
        case PLAYMODEL_DRILL_VOLLEY:
        case PLAYMODEL_DRILL_CASUAL:
        case PLAYMODEL_DRILL_MULTI_BALL:
            return hitCourtRange(getCourtSide(courtArea), drillDifficulty, courtSize);
        default:
            break;
    }

    return CourtRange();
}

CourtRange HandleProxy::serveCourtRange(CourtArea courtArea, DrillDifficulty drillDifficulty, const CourtSize &courtSize) {
    switch (courtArea) {
        case COURTAREA_A1:
            switch (drillDifficulty) {
                case DD_NORMAL:
                    return CourtRange(0, -courtSize.singleY, -courtSize.severX, 0);
                case DD_INSIDE:
                    return CourtRange(0, -courtSize.singleY / 2, -courtSize.severX, 0);
                case DD_OUTSIDE:
                    return CourtRange(0, -courtSize.singleY, -courtSize.severX, -courtSize.singleY / 2 - 1);
                default:
                    break;
            }
            break;
        case COURTAREA_A2:
            switch (drillDifficulty) {
                case DD_NORMAL:
                    return CourtRange(0, 1, -courtSize.severX, courtSize.singleY);
                case DD_INSIDE:
                    return CourtRange(0, 0, -courtSize.severX, courtSize.singleY / 2);
                case DD_OUTSIDE:
                    return CourtRange(0, courtSize.singleY / 2 + 1, -courtSize.severX, courtSize.singleY);
                default:
                    break;
            }
            break;
        case COURTAREA_B1:
            switch (drillDifficulty) {
                case DD_NORMAL:
                    return CourtRange(courtSize.severX, 1, 0, courtSize.singleY);
                case DD_INSIDE:
                    return CourtRange(courtSize.severX, 0, 0, courtSize.singleY / 2);
                case DD_OUTSIDE:
                    return CourtRange(courtSize.severX, courtSize.singleY / 2 + 1, 0, courtSize.singleY);
                default:
                    break;
            }
            break;
        case COURTAREA_B2:
            switch (drillDifficulty) {
                case DD_NORMAL:
                    return CourtRange(courtSize.severX, -courtSize.singleY, 0, 0);
                case DD_INSIDE:
                    return CourtRange(courtSize.severX, -courtSize.singleY / 2, 0, 0);
                case DD_OUTSIDE:
                    return CourtRange(courtSize.severX, -courtSize.singleY, 0, -courtSize.singleY / 2 - 1);
                default:
                    break;
            }
            break;
        default:
            break;
    }

    return CourtRange();
}

CourtRange HandleProxy::hitCourtRange(CourtArea courtArea, DrillDifficulty drillDifficulty, const CourtSize &courtSize) {
    switch (courtArea) {
        case COURTAREA_A:
            switch (drillDifficulty) {
                case DD_EASY:
                    return CourtRange(0, -courtSize.singleY, -courtSize.baselineX, courtSize.singleY);
                case DD_MEDIUM:
                    return CourtRange(-courtSize.severX, -courtSize.singleY, -courtSize.baselineX, courtSize.singleY);
                case DD_DIFFICULT:
                    return CourtRange(-(courtSize.baselineX + courtSize.severX) / 2, -courtSize.singleY, -courtSize.baselineX,
                                      courtSize.singleY);
                default:
                    break;
            }
            break;
        case COURTAREA_B:
            switch (drillDifficulty) {
                case DD_EASY:
                    return CourtRange(courtSize.baselineX, -courtSize.singleY, 0, courtSize.singleY);
                case DD_MEDIUM:
                    return CourtRange(courtSize.baselineX, -courtSize.singleY, courtSize.severX, courtSize.singleY);
                case DD_DIFFICULT:
                    return CourtRange(courtSize.baselineX, -courtSize.singleY, (courtSize.baselineX + courtSize.severX) / 2,
                                      courtSize.singleY);
                default:
                    break;
            }
            break;
        default:
            break;
    }

    return CourtRange();
}

CourtRange HandleProxy::straightHitCourtRange(CourtArea courtArea, DrillDifficulty drillDifficulty, const CourtSize &courtSize) {
    switch (courtArea) {
        case COURTAREA_A1:
            switch (drillDifficulty) {
                case DD_EASY:
                    return CourtRange(0, 0, -courtSize.baselineX, courtSize.singleY);
                case DD_MEDIUM:
                    return CourtRange(-courtSize.severX, 0, -courtSize.baselineX, courtSize.singleY);
                case DD_DIFFICULT:
                    return CourtRange(-(courtSize.baselineX + courtSize.severX) / 2, 0, -courtSize.baselineX, courtSize.singleY);
                default:
                    break;
            }
            break;
        case COURTAREA_A2:
            switch (drillDifficulty) {
                case DD_EASY:
                    return CourtRange(0, -courtSize.singleY, -courtSize.baselineX, 0);
                case DD_MEDIUM:
                    return CourtRange(-courtSize.severX, -courtSize.singleY, -courtSize.baselineX, 0);
                case DD_DIFFICULT:
                    return CourtRange(-(courtSize.baselineX + courtSize.severX) / 2, -courtSize.singleY, -courtSize.baselineX, 0);
                default:
                    break;
            }
            break;
        case COURTAREA_B1:
            switch (drillDifficulty) {
                case DD_EASY:
                    return CourtRange(courtSize.baselineX, -courtSize.singleY, 0, 0);
                case DD_MEDIUM:
                    return CourtRange(courtSize.baselineX, -courtSize.singleY, courtSize.severX, 0);
                case DD_DIFFICULT:
                    return CourtRange(courtSize.baselineX, -courtSize.singleY, (courtSize.baselineX + courtSize.severX) / 2, 0);
                default:
                    break;
            }
            break;
        case COURTAREA_B2:
            switch (drillDifficulty) {
                case DD_EASY:
                    return CourtRange(courtSize.baselineX, 0, 0, courtSize.singleY);
                case DD_MEDIUM:
                    return CourtRange(courtSize.baselineX, 0, courtSize.severX, courtSize.singleY);
                case DD_DIFFICULT:
                    return CourtRange(courtSize.baselineX, 0, (courtSize.baselineX + courtSize.severX) / 2, courtSize.singleY);
                default:
                    break;
            }
            break;
        default:
            break;
    }

    return CourtRange();
}

CourtRange HandleProxy::slashHitCourtRange(CourtArea courtArea, DrillDifficulty drillDifficulty, const CourtSize &courtSize) {
    switch (courtArea) {
        case COURTAREA_A1:
            switch (drillDifficulty) {
                case DD_EASY:
                    return CourtRange(0, -courtSize.singleY, -courtSize.baselineX, 0);
                case DD_MEDIUM:
                    return CourtRange(-courtSize.severX, -courtSize.singleY, -courtSize.baselineX, 0);
                case DD_DIFFICULT:
                    return CourtRange(-(courtSize.baselineX + courtSize.severX) / 2, -courtSize.singleY, -courtSize.baselineX, 0);
                default:
                    break;
            }
            break;
        case COURTAREA_A2:
            switch (drillDifficulty) {
                case DD_EASY:
                    return CourtRange(0, 0, -courtSize.baselineX, courtSize.singleY);
                case DD_MEDIUM:
                    return CourtRange(-courtSize.severX, 0, -courtSize.baselineX, courtSize.singleY);
                case DD_DIFFICULT:
                    return CourtRange(-(courtSize.baselineX + courtSize.severX) / 2, 0, -courtSize.baselineX, courtSize.singleY);
                default:
                    break;
            }
            break;
        case COURTAREA_B2:
            switch (drillDifficulty) {
                case DD_EASY:
                    return CourtRange(courtSize.baselineX, -courtSize.singleY, 0, 0);
                case DD_MEDIUM:
                    return CourtRange(courtSize.baselineX, -courtSize.singleY, courtSize.severX, 0);
                case DD_DIFFICULT:
                    return CourtRange(courtSize.baselineX, -courtSize.singleY, (courtSize.baselineX + courtSize.severX) / 2, 0);
                default:
                    break;
            }
            break;
        case COURTAREA_B1:
            switch (drillDifficulty) {
                case DD_EASY:
                    return CourtRange(courtSize.baselineX, 0, 0, courtSize.singleY);
                case DD_MEDIUM:
                    return CourtRange(courtSize.baselineX, 0, courtSize.severX, courtSize.singleY);
                case DD_DIFFICULT:
                    return CourtRange(courtSize.baselineX, 0, (courtSize.baselineX + courtSize.severX) / 2, courtSize.singleY);
                default:
                    break;
            }
            break;
        default:
            break;
    }

    return CourtRange();
}
