#include "HandleDatasHand.h"

int HandleDatasHand::getMaxSpeed() {
    int maxSpeed = 0;
    // TODO 最大速度，应该是整个运行过程中的最大速度，目前所有的实现都是击球时刻的，后期需要调整
    for (DataItem *pDataItem : m_datas) {
        if (pDataItem->HandItem.ballHit.speed > maxSpeed) {
            maxSpeed = pDataItem->HandItem.ballHit.speed;
        }
    }

    return maxSpeed;
}

int HandleDatasHand::getAverageSpeed() {
    if (m_datas.empty()) {
        return 0;
    }

    int totalSpeed = 0;
    for (DataItem *pDataItem : m_datas) {
        totalSpeed += pDataItem->HandItem.ballHit.speed;
    }

    return totalSpeed / (int) m_datas.size();
}

int HandleDatasHand::getNetTimes() {
    int netTimes = 0;
    for (DataItem *pDataItem : m_datas) {
        if (pDataItem->drillResult == ER_OUT) {
            if (pDataItem->HandItem.isTouchNet)
                ++netTimes;
        }
    }

    return netTimes;
}

int HandleDatasHand::getOutTimes() {
    int outTimes = 0;
    for (DataItem *pDataItem : m_datas) {
        if (pDataItem->drillResult == ER_OUT) {
            if (!pDataItem->HandItem.isTouchNet)
                ++outTimes;
        }
    }

    return outTimes;
}

int HandleDatasHand::getDownPoints(BallPoint *pPoint, int size) {
    int downPointsCount = 0;
    BallPoint *pBallPoint = pPoint;
    for (DataItem *pDataItem : m_datas) {
        if (m_pHandleProxy->isBallValid(pDataItem->HandItem.ballHitDown)) {
            if (pBallPoint != NULL) {
                pBallPoint->x = pDataItem->HandItem.ballHitDown.x;
                pBallPoint->y = pDataItem->HandItem.ballHitDown.y;

                if (pDataItem->drillResult == ER_IN) {
                    pBallPoint->in = true;
                } else if (pDataItem->drillResult == ER_OUT) {
                    pBallPoint->in = false;
                    if (pDataItem->HandItem.isTouchNet) {
                        pBallPoint->x = 0;
                    }
                }

                pBallPoint++;
                if (downPointsCount > size)
                    break;
            }

            downPointsCount++;
        }

        assert(size == 0 || downPointsCount <= size);
    }

    return downPointsCount;
}

int HandleDatasHand::getHitPoints(BallPoint *pPoint, int size) {
    int hitPointsCount = 0;
    BallPoint *pBallPoint = pPoint;
    for (DataItem *pDataItem : m_datas) {
        if (m_pHandleProxy->isBallValid(pDataItem->HandItem.ballHit)) {
            if (pBallPoint != NULL) {
                pBallPoint->x = pDataItem->HandItem.ballHit.x;
                pBallPoint->y = pDataItem->HandItem.ballHit.y;
                pBallPoint->in = pDataItem->drillResult == ER_IN;

                pBallPoint++;
                if (hitPointsCount > size)
                    break;
            }

            hitPointsCount++;
        }

        assert(size == 0 || hitPointsCount <= size);
    }

    return hitPointsCount;
}

