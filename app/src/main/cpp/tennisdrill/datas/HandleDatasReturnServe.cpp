#include "HandleDatasReturnServe.h"

int HandleDatasReturnServe::getMaxSpeed() {
    int maxSpeed = 0;
    // TODO 最大速度，应该是整个运行过程中的最大速度，目前所有的实现都是击球时刻的，后期需要调整
    for (DataItem *pDataItem : m_datas) {
        if (pDataItem->ReturnServeItem.ballServe.speed > maxSpeed) {
            maxSpeed = pDataItem->ReturnServeItem.ballServe.speed;
        }

        if (pDataItem->ReturnServeItem.ballHit.speed > maxSpeed) {
            maxSpeed = pDataItem->ReturnServeItem.ballHit.speed;
        }
    }

    return maxSpeed;
}

int HandleDatasReturnServe::getAverageSpeed() {
    if (m_datas.empty()) {
        return 0;
    }

    int totalSpeed = 0;
    for (DataItem *pDataItem : m_datas) {
        totalSpeed += pDataItem->ReturnServeItem.ballServe.speed;
        totalSpeed += pDataItem->ReturnServeItem.ballHit.speed;
    }

    return totalSpeed / (2 * (int) m_datas.size());
}

int HandleDatasReturnServe::getNetTimes() {
    int netTimes = 0;
    for (DataItem *pDataItem : m_datas) {
        if (pDataItem->drillResult == ER_OUT) {
            if (pDataItem->ReturnServeItem.isTouchNet)
                ++netTimes;
        }
    }

    return netTimes;
}

int HandleDatasReturnServe::getOutTimes() {
    int outTimes = 0;
    for (DataItem *pDataItem : m_datas) {
        if (pDataItem->drillResult == ER_OUT) {
            if (!pDataItem->ReturnServeItem.isTouchNet)
                ++outTimes;
        }
    }

    return outTimes;
}

int HandleDatasReturnServe::getDownPoints(BallPoint *pPoint, int size) {
    int downPointsCount = 0;
    BallPoint *pBallPoint = pPoint;
    for (DataItem *pDataItem : m_datas) {
        if (m_pHandleProxy->isBallValid(pDataItem->ReturnServeItem.ballHitDown)) {
            if (pBallPoint != NULL) {
                pBallPoint->x = pDataItem->ReturnServeItem.ballHitDown.x;
                pBallPoint->y = pDataItem->ReturnServeItem.ballHitDown.y;

                if (pDataItem->drillResult == ER_IN) {
                    pBallPoint->in = true;
                } else if (pDataItem->drillResult == ER_OUT) {
                    pBallPoint->in = false;
                    if (pDataItem->ReturnServeItem.isTouchNet) {
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

int HandleDatasReturnServe::getHitPoints(BallPoint *pPoint, int size) {
    int hitPointsCount = 0;
    BallPoint *pBallPoint = pPoint;
    for (DataItem *pDataItem : m_datas) {
        if (m_pHandleProxy->isBallValid(pDataItem->ReturnServeItem.ballHit)) {
            if (pBallPoint != NULL) {
                pBallPoint->x = pDataItem->ReturnServeItem.ballHit.x;
                pBallPoint->y = pDataItem->ReturnServeItem.ballHit.y;
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

