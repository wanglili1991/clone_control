#include "HandleStatVolley.h"

int HandleStatVolley::getLatestSpeed() {
    if (m_pHandleProxy->isBallValid(m_dataItem.VolleyItem.ballHit))
        return m_dataItem.VolleyItem.ballHit.speed;
    else
        return 0;
}

bool HandleStatVolley::onHandleHit(const Ball &ball) {
    if (!m_pHandleProxy->isSameCourtSide(ball)) {
        reset();
        m_dataItem.VolleyItem.ballServe = ball;

        return false;
    } else {
        if (m_pHandleProxy->isBallValid(m_dataItem.VolleyItem.ballHit)) {
            m_dataItem.drillResult = ER_MULTIHIT;
            return true;
        } else {
            if (m_pHandleProxy->isBallValid(m_dataItem.VolleyItem.ballServe)) {
                m_dataItem.VolleyItem.ballHit = ball;
                return false;
            } else {
                reset();
                m_dataItem.drillResult = ER_INVALID;
                return true;
            }
        }
    }
}

bool HandleStatVolley::onHandleTouchDown(const Ball &ball) {
    if (m_pHandleProxy->isBallValid(m_dataItem.VolleyItem.ballHit) && m_pHandleProxy->isBallValid(m_dataItem.VolleyItem.ballServe)) {
        m_dataItem.VolleyItem.ballHitDown = ball;
        m_dataItem.drillResult = m_pHandleProxy->getDrillResult(m_dataItem.VolleyItem.ballHit, m_dataItem.VolleyItem.ballHitDown, false);
        return true;
    } else {
        if (m_pHandleProxy->isSameCourtSide(ball)
            && m_pHandleProxy->isBallValid(ball)
            && m_pHandleProxy->isBallValid(m_dataItem.VolleyItem.ballServe)) {
            m_dataItem.VolleyItem.ballHitDown = ball;
            // TODO: 落地后怎么界定是否非法
            //m_dataItem.drillResult = ER_INVALID;
            m_dataItem.drillResult = ER_OUT;
            return true;
        } else {
            reset();
            m_dataItem.drillResult = ER_INVALID;
            return true;
        }
    }
}

bool HandleStatVolley::onHandleOut(const Ball &ball) {
    if (m_pHandleProxy->isIn(ball)) {
        return false;
    }

    if (m_pHandleProxy->isBallValid(m_dataItem.VolleyItem.ballServe) && m_pHandleProxy->isBallValid(m_dataItem.VolleyItem.ballHit)) {
        m_dataItem.drillResult = ER_OUT;
        return true;
    } else {
        reset();
        m_dataItem.drillResult = ER_INVALID;
        return true;
    }
}