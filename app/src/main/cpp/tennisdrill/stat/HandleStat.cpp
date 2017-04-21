#include "HandleStat.h"

bool HandleStat::onBall(BallStat ballStat, const Ball &ball) {
    assert(m_pHandleProxy->isBallValid(ball));

    setBallDirection(ball);
    switch (ballStat) {
        case BALLSTAT_NULL:
            return onHandleNull(ball);
        case BALLSTAT_HIT:
            return onHandleHit(ball);
        case BALLSTAT_SERVE:
            return onHandleServe(ball);
        case BALLSTAT_TOUCH_DOWN:
            return onHandleTouchDown(ball);
        case BALLSTAT_TOUCH_NET:
            return onHandleTouchNet(ball);
        case BALLSTAT_TOUCH_ANY:
            return onHandleTouchAny(ball);
        case BALLSTAT_OUT:
            return onHandleOut(ball);
        case BALLSTAT_NON_UNIQUENESS:
            return onHandleNonUniqueness(ball);
        case BALLSTAT_ERROR:
            return onHandleError(ball);
        case BALLSTAT_IN:
            return onHandleIn(ball);
        case BALLSTAT_HIT_GUESS:
            return onHandleHitGuess(ball);
        case BALLSTAT_ERROR_HIT:
            return onHandleErrorHit(ball);
        default:
            break;
    }

    return false;
}

void HandleStat::setBallDirection(const Ball &ball) {
    if (m_pHandleProxy->isBallValid(ball)) {
        if (ball.x == m_ballLast.x) {
            m_ballDirection = BALLDIRECTION_UNKNOWN;
        } else {
            if (m_ballLast.x == 0) {
                m_ballDirection = ball.x < m_ballLast.x ? BALLDIRECTION_A : BALLDIRECTION_B;
            } else {
                m_ballDirection = ball.x < m_ballLast.x ? BALLDIRECTION_B : BALLDIRECTION_A;
            }
        }

        m_ballLast = ball;
    } else {
        m_ballDirection = BALLDIRECTION_INVALID;
    }
}

BallDirection HandleStat::getBallDirection() {
    return m_ballDirection;
}

bool HandleStat::isMoveToTrainedSide() {
    CourtArea courtArea = m_pHandleProxy->getTrainedSide();
    if (((courtArea == COURTAREA_A) && (m_ballDirection == BALLDIRECTION_A))
        || ((courtArea == COURTAREA_B) && (m_ballDirection == BALLDIRECTION_B))) {
        return true;
    }
    return false;
}

int HandleStat::getLatestSpeed() {
    return 0;
}

bool HandleStat::onHandleNull(const Ball &ball) {
    return false;
}

bool HandleStat::onHandleHit(const Ball &ball) {
    return false;
}

bool HandleStat::onHandleServe(const Ball &ball) {
    return false;
}

bool HandleStat::onHandleTouchDown(const Ball &ball) {
    return false;
}

bool HandleStat::onHandleTouchNet(const Ball &ball) {
    return false;
}

bool HandleStat::onHandleTouchAny(const Ball &ball) {
    return false;
}

bool HandleStat::onHandleOut(const Ball &ball) {
    return false;
}

bool HandleStat::onHandleNonUniqueness(const Ball &ball) {
    reset();
    return false;
}

bool HandleStat::onHandleError(const Ball &ball) {
    reset();
    return false;
}

bool HandleStat::onHandleIn(const Ball &ball) {
    return false;
}

bool HandleStat::onHandleHitGuess(const Ball &ball) {
    return false;
}

bool HandleStat::onHandleErrorHit(const Ball &ball) {
    return false;
}
