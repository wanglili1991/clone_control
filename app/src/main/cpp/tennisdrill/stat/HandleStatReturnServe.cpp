#include "HandleStatReturnServe.h"

int HandleStatReturnServe::getLatestSpeed() {
    if (m_pHandleProxy->isBallValid(m_dataItem.ReturnServeItem.ballHit))
        return m_dataItem.ReturnServeItem.ballHit.speed;
    else
        return 0;
}

bool HandleStatReturnServe::onHandleServe(const Ball &ball) {
    if (m_pHandleProxy->isBallValid(m_dataItem.ReturnServeItem.ballServe)) {
        reset();
        m_dataItem.drillResult = ER_INVALID;

        return true;
    }

    reset();
    if (!m_pHandleProxy->isSameCourtSide(ball)) {
        m_dataItem.ReturnServeItem.ballServe = ball;
    }

    return false;
}

bool HandleStatReturnServe::onHandleTouchNet(const Ball &ball) {
    if (!m_pHandleProxy->isBallValid(m_dataItem.ReturnServeItem.ballServe)) {
        reset();
        return false;
    }

    if (!m_pHandleProxy->isBallValid(m_dataItem.ReturnServeItem.ballHit)) {
        reset();
        m_dataItem.drillResult = ER_INVALID;
        return true;
    }

    if (isMoveToTrainedSide()) {
        reset();
    } else {
        m_dataItem.ReturnServeItem.isTouchNet = true;
    }

    return false;
}

bool HandleStatReturnServe::onHandleHit(const Ball &ball) {
    if (!m_pHandleProxy->isBallValid(m_dataItem.ReturnServeItem.ballServe)) {
        reset();
        m_dataItem.drillResult = ER_INVALID;
        return true;
    }

    if (!m_pHandleProxy->isBallValid(m_dataItem.ReturnServeItem.ballServeDown)) {
        reset();
        m_dataItem.drillResult = ER_INVALID;
        return true;
    }

    if (m_pHandleProxy->isBallValid(m_dataItem.ReturnServeItem.ballHit)) {
        m_dataItem.drillResult = ER_MULTIHIT;
        return true;
    }

    if (m_pHandleProxy->isSameCourtSide(ball)) {
        m_dataItem.ReturnServeItem.ballHit = ball;
        return false;
    } else {
        reset();
        m_dataItem.drillResult = ER_INVALID;
        return true;
    }
}

bool HandleStatReturnServe::onHandleTouchDown(const Ball &ball) {
    if (!m_pHandleProxy->isBallValid(m_dataItem.ReturnServeItem.ballServe)) {
        reset();
        m_dataItem.drillResult = ER_INVALID;
        return true;
    }

    if (m_pHandleProxy->isBallValid(m_dataItem.ReturnServeItem.ballServeDown)
        && !m_pHandleProxy->isBallValid(m_dataItem.ReturnServeItem.ballHit)) {
        // TODO 出界了，不能直接out，否则数据点数就会有问题
        reset();
        m_dataItem.drillResult = ER_OUT; //drillResult = ER_NOHIT;
        return true;
    }

    if (m_pHandleProxy->isSameCourtSide(ball)) {
        if (!m_pHandleProxy->isBallValid(m_dataItem.ReturnServeItem.ballHit)) {
            DrillResult result = m_pHandleProxy->getDrillResult(m_dataItem.ReturnServeItem.ballServe, ball,
                                                                m_dataItem.ReturnServeItem.isTouchNet, PLAYMODEL_DRILL_SERVE, DD_NORMAL);
            if (result == ER_IN) {
                m_dataItem.ReturnServeItem.ballServeDown = ball;
                return false;
            } else {
                reset();
                m_dataItem.drillResult = ER_INVALID;
                return true;
            }
        } else {
            m_dataItem.ReturnServeItem.ballHitDown = ball;
            m_dataItem.drillResult = ER_OUT;
            return true;
        }
    } else {
        if (!m_pHandleProxy->isBallValid(m_dataItem.ReturnServeItem.ballServeDown) ||
            !m_pHandleProxy->isBallValid(m_dataItem.ReturnServeItem.ballHit)) {
            reset();
            m_dataItem.drillResult = ER_INVALID;
            return true;
        }

        m_dataItem.ReturnServeItem.ballHitDown = ball;
        m_dataItem.drillResult = m_pHandleProxy->getDrillResult(m_dataItem.ReturnServeItem.ballHit, m_dataItem.ReturnServeItem.ballHitDown,
                                                                m_dataItem.ReturnServeItem.isTouchNet);

        return true;
    }
}

bool HandleStatReturnServe::onHandleOut(const Ball &ball) {
    if (!m_pHandleProxy->isBallValid(m_dataItem.ReturnServeItem.ballServe)) {
        reset();
        m_dataItem.drillResult = ER_INVALID;
        return true;
    }

    if (m_pHandleProxy->isIn(ball)) {
        return false;
    }

    if (!m_pHandleProxy->isBallValid(m_dataItem.ReturnServeItem.ballServeDown)) {
        reset();
        m_dataItem.drillResult = ER_INVALID;
        return true;
    }

    if (m_pHandleProxy->isBallValid(m_dataItem.ReturnServeItem.ballHit)) {
        m_dataItem.drillResult = ER_OUT;
    } else if (std::abs(ball.x) == 2000) {
        reset();
        m_dataItem.drillResult = ER_NOHIT;
    } else {
        m_dataItem.ReturnServeItem.isOut = true;
        return false;
    }

    return true;
}

bool HandleStatReturnServe::onHandleHitGuess(const Ball &ball) {
    if (!m_pHandleProxy->isBallValid(m_dataItem.ReturnServeItem.ballServeDown)) {
        reset();
        m_dataItem.drillResult = ER_INVALID;
        return true;
    }

    if (m_pHandleProxy->isBallValid(m_dataItem.ReturnServeItem.ballHit)) {
        m_dataItem.drillResult = ER_MULTIHIT;
        return true;
    }

    if (m_dataItem.ReturnServeItem.isOut && m_pHandleProxy->isSameCourtSide(ball)) {
        m_dataItem.ReturnServeItem.ballHit = ball;
        return false;
    } else {
        reset();
        m_dataItem.drillResult = ER_INVALID;
        return true;
    }
}
