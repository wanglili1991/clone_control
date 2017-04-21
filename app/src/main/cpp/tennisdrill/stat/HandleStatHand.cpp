#include "HandleStatHand.h"

int HandleStatHand::getLatestSpeed() {
    if (m_pHandleProxy->isBallValid(m_dataItem.HandItem.ballHit))
        return m_dataItem.HandItem.ballHit.speed;
    else
        return 0;
}

bool HandleStatHand::onHandleServe(const Ball &ball) {
    // 错误状态.击球不该有发球事件的响应
    Log_Warning("ErrorMessage: Hit Event should not happened in the Serve moment");

    reset();
    m_dataItem.drillResult = ER_INVALID;

    return true;
}

bool HandleStatHand::onHandleTouchNet(const Ball &ball) {
    m_dataItem.HandItem.isTouchNet = true;
    switch (m_lastBallStat) {
        case BALLSTAT_HIT: {
            if (m_isReady) {
                // 将上一个事件登记为有效事件，并置上一个事件为4
                m_beforeLastBallStat = m_lastBallStat;
                m_beforeLastBall = m_lastBall;

                m_lastBallStat = BALLSTAT_TOUCH_NET;
                m_lastBall = ball;
            } else {
                // 喂球过程中触网，忽略
                // 不在范围内
                reset();
                m_dataItem.drillResult = ER_INVALID;
                return true;
            }
            break;
        }
        case BALLSTAT_IN: {
            if (m_isReady) {
                m_lastBallStat = BALLSTAT_TOUCH_NET;
                m_lastBall = ball;
            } else {
                // 不在范围内
                reset();
                m_dataItem.drillResult = ER_INVALID;
                return true;
            }
            break;
        }
        case BALLSTAT_NULL:
        case BALLSTAT_SERVE:
        case BALLSTAT_TOUCH_NET:
        case BALLSTAT_TOUCH_ANY:
        case BALLSTAT_OUT:
        case BALLSTAT_NON_UNIQUENESS:
        case BALLSTAT_ERROR:
        case BALLSTAT_HIT_GUESS:
        case BALLSTAT_ERROR_HIT:
        case BALLSTAT_TOUCH_DOWN:
        case BALLSTAT_WAITING: {
            reset();
            m_dataItem.drillResult = ER_INVALID;
            return true;
        }
        default:
            break;
    }
    return false;
}

bool HandleStatHand::onHandleHit(const Ball &ball) {
    switch (m_lastBallStat) {
        case BALLSTAT_WAITING: {
            m_lastBallStat = BALLSTAT_HIT;
            m_lastBall = ball;
            break;
        }
        case BALLSTAT_TOUCH_DOWN: {
            if ((ball.x ^ m_dataItem.HandItem.ballComeDown.x) > 0) {
                m_lastBallStat = BALLSTAT_HIT;
                m_lastBall = ball;
                //这个是回球的击球点
                m_dataItem.HandItem.ballHit = ball;
            } else {
                // 置waiting状态
                return updateFinalFailedState();
            }
            break;
        }
        case BALLSTAT_HIT: // TODO : 问题：没法判定是底层给错事件还是真实没等球落地就击回
        {
            Log_Warning(
                    "We have no ideas to judge whether the basic engine error or not");

            //{
            //	// 目前就当作是用户没等球落地就击回，总次数增加，当作没有成功处理

            //	ballHit = ball;
            //	drillResult = ER_OUT;
            //	return true;

            //	break;
            //}
            break;
        }
        case BALLSTAT_SERVE:
            //case BallStat_touch_any:
            // 这里没有落地，先触网就击球，应该和直接击球同样处理
        case BALLSTAT_TOUCH_NET:
        case BALLSTAT_OUT:
            //case BallStat_non_uniqueness:
            //case BallStat_error:
        case BALLSTAT_IN:
        case BALLSTAT_HIT_GUESS: // 一定不出现上一个事件为10的情况
            //case BallStat_error_hit:
        {
            return updateFinalFailedState();
        }
        default:
            break;
    }
    return false;
}

bool HandleStatHand::onHandleTouchDown(const Ball &ball) {
    switch (m_lastBallStat) {
        case BALLSTAT_HIT: {
            m_lastBallStat = BALLSTAT_TOUCH_DOWN;
            m_lastBall = ball;

            if (m_isReady) {
                m_isReady = false;
                m_dataItem.HandItem.ballHitDown = ball;
                if (!m_pHandleProxy->isSameCourtSide(ball)
                    && m_pHandleProxy->getDrillResult(m_dataItem.HandItem.ballHit, m_dataItem.HandItem.ballHitDown,
                                                      m_dataItem.HandItem.isTouchNet) == ER_IN) {
                    if (!m_pHandleProxy->isBallValid(m_dataItem.HandItem.ballComeDown) ||
                        !m_pHandleProxy->isBallValid(m_dataItem.HandItem.ballHit)) {
                        reset();
                        m_dataItem.drillResult = ER_INVALID;
                    } else {
                        // 需要成功次数+1，记录各项统计数据
                        m_dataItem.drillResult = ER_IN;
                    }
                } else {
                    // 不在范围内
                    m_dataItem.drillResult = ER_OUT;
                }
                return true;
            } else {
                // Check area
                if (m_pHandleProxy->isBallValid(ball)) {
                    m_dataItem.HandItem.ballComeDown = ball;

                    // 这里应该是不考虑是否触网，范围最大的情况
                    // TODO: 是否要考虑落点的正负？
                    if (m_pHandleProxy->isIn(ball)) {
                        m_isReady = true;
                    } else {
                        m_isReady = false;
                        // 不在范围内
                        reset();
                        m_dataItem.drillResult = ER_INVALID;
                        return true;
                    }
                } else {
                    // 不在范围内
                    return updateFinalFailedState();
                }
            }
            break;
        }
            // 与之前的有效事件做判断。规则适用将有效事件当做上一个事件的判定规则
        case BALLSTAT_TOUCH_NET:
        case BALLSTAT_IN: {
            m_lastBallStat = m_beforeLastBallStat;
            m_lastBall = m_beforeLastBall;
            return onHandleTouchDown(ball);
        }
        case BALLSTAT_NULL:
        case BALLSTAT_SERVE:
        case BALLSTAT_TOUCH_DOWN:
        case BALLSTAT_TOUCH_ANY:
        case BALLSTAT_OUT:
        case BALLSTAT_NON_UNIQUENESS:
        case BALLSTAT_ERROR:
        case BALLSTAT_ERROR_HIT:
        case BALLSTAT_HIT_GUESS:
        case BALLSTAT_WAITING: {
            // 不在范围内
            return updateFinalFailedState();
        }
        default:
            break;
    }

    return false;
}

bool HandleStatHand::onHandleOut(const Ball &ball) {
    switch (m_lastBallStat) {
        case BALLSTAT_HIT: {
            if (abs(ball.x) == 2000) {
                return updateFinalFailedState();
            } else {
                // TODO: 检查此6是否是从上面出视野, 有待进一步确认小概率事件
                if (m_pHandleProxy->isIn(ball)) {
                    m_beforeLastBall = m_lastBall;
                    m_beforeLastBallStat = m_lastBallStat;

                    m_dataItem.HandItem.ballHitDown = m_lastBall;

                    m_lastBallStat = BALLSTAT_OUT;
                    m_lastBall = ball;
                } else {
                    return updateFinalFailedState();
                }
            }
            break;
        }
        case BALLSTAT_TOUCH_DOWN: {
            if (abs(ball.x) == 2000) {
                return updateFinalFailedState();
            } else {
                m_beforeLastBall = m_lastBall;
                m_beforeLastBallStat = m_lastBallStat;

                m_dataItem.HandItem.ballHitDown = m_lastBall;

                m_lastBallStat = BALLSTAT_OUT;
                m_lastBall = ball;
            }
            break;
        }
        case BALLSTAT_IN:
        case BALLSTAT_TOUCH_NET:
        case BALLSTAT_NULL:
        case BALLSTAT_SERVE:
        case BALLSTAT_TOUCH_ANY:
        case BALLSTAT_OUT:
        case BALLSTAT_NON_UNIQUENESS:
        case BALLSTAT_ERROR:
        case BALLSTAT_HIT_GUESS:
        case BALLSTAT_ERROR_HIT:
        case BALLSTAT_WAITING: {
            return updateFinalFailedState();
        }
        default:
            break;
    }
    return false;
}

bool HandleStatHand::onHandleIn(const Ball &ball) {
    switch (m_lastBallStat) {
        case BALLSTAT_OUT: {
            m_lastBallStat = BALLSTAT_IN;
            m_lastBall = ball;
            break;
        }
        case BALLSTAT_HIT:
        case BALLSTAT_TOUCH_DOWN:
        case BALLSTAT_IN:
        case BALLSTAT_TOUCH_NET:
        case BALLSTAT_NULL:
        case BALLSTAT_SERVE:
        case BALLSTAT_TOUCH_ANY:
        case BALLSTAT_NON_UNIQUENESS:
        case BALLSTAT_ERROR:
        case BALLSTAT_HIT_GUESS:
        case BALLSTAT_ERROR_HIT:
        case BALLSTAT_WAITING: {
            return updateFinalFailedState();
        }
        default:
            break;
    }
    return false;
}

bool HandleStatHand::onHandleHitGuess(const Ball &ball) {
    switch (m_lastBallStat) {
        case BALLSTAT_OUT: {
            if ((m_beforeLastBallStat == BALLSTAT_TOUCH_DOWN) &&
                m_pHandleProxy->isBallValid(m_dataItem.HandItem.ballHitDown)) {
                if ((ball.x ^ m_dataItem.HandItem.ballHitDown.x) > 0) {
                    m_lastBallStat = BALLSTAT_HIT;
                    m_lastBall = ball;
                    m_dataItem.HandItem.ballHit = ball;
                } else {
                    return updateFinalFailedState();
                }
            } else {
                return updateFinalFailedState();
            }
            break;
        }
        case BALLSTAT_HIT:
        case BALLSTAT_TOUCH_DOWN:
        case BALLSTAT_IN:
        case BALLSTAT_TOUCH_NET:
        case BALLSTAT_NULL:
        case BALLSTAT_SERVE:
        case BALLSTAT_TOUCH_ANY:
        case BALLSTAT_NON_UNIQUENESS:
        case BALLSTAT_ERROR:
        case BALLSTAT_HIT_GUESS:
        case BALLSTAT_ERROR_HIT:
        case BALLSTAT_WAITING: {
            return updateFinalFailedState();
        }
        default:
            break;
    }
    return false;
}
