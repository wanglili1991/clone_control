#include "HandleStatServe.h"

int HandleStatServe::getLatestSpeed() {
    if (m_pHandleProxy->isBallValid(m_dataItem.ServeItem.ballServe))
        return m_dataItem.ServeItem.ballServe.speed;
    else
        return 0;
}

bool HandleStatServe::onHandleServe(const Ball &ball) {
    if (m_pHandleProxy->isBallValid(m_dataItem.ServeItem.ballServe)) {
        reset();
        m_dataItem.drillResult = ER_INVALID;
        return true;
    }

    reset();
    if (m_pHandleProxy->isSameCourtSide(ball)) {
        m_dataItem.ServeItem.ballServe = ball;
    }

    return false;
}

bool HandleStatServe::onHandleTouchNet(const Ball &ball) {
    if (!m_pHandleProxy->isBallValid(m_dataItem.ServeItem.ballServe)) {
        reset();
        m_dataItem.drillResult = ER_INVALID;
        return true;
    }

    if (isMoveToTrainedSide()) {
        reset();
    } else {
        m_dataItem.ServeItem.isTouchNet = true;
    }

    return false;
}

bool HandleStatServe::onHandleTouchDown(const Ball &ball) {
    if (!m_pHandleProxy->isBallValid(m_dataItem.ServeItem.ballServe)) {
        reset();
        m_dataItem.drillResult = ER_INVALID;

        return true;
    }

    m_dataItem.ServeItem.ballServeDown = ball;
    m_dataItem.drillResult = m_pHandleProxy->getDrillResult(m_dataItem.ServeItem.ballServe, m_dataItem.ServeItem.ballServeDown,
                                                            m_dataItem.ServeItem.isTouchNet);

    return true;
}

bool HandleStatServe::onHandleOut(const Ball &ball) {
    if (!m_pHandleProxy->isBallValid(m_dataItem.ServeItem.ballServe)) {
        reset();
        m_dataItem.drillResult = ER_INVALID;
        return true;
    }

    if (ball.x == 2000) {
        m_dataItem.drillResult = ER_OUT;
        return true;
    }

    if (m_pHandleProxy->isIn(ball)) {
        return false;
    }

    m_dataItem.drillResult = ER_OUT;

    return true;
}

bool HandleStatServe::onHandleHit(const Ball &ball) {
    if (!m_pHandleProxy->isBallValid(m_dataItem.ServeItem.ballServe)) {
        reset();
        return false;
    }

    reset();
    m_dataItem.drillResult = ER_INVALID;

    return true;
}
