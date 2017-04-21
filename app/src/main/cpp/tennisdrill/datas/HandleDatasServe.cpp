#include "HandleDatasServe.h"

int HandleDatasServe::getMaxSpeed() {
    int maxSpeed = 0;
    for (DataItem *pDataItem : m_datas) {
        if (pDataItem->ServeItem.ballServe.speed > maxSpeed) {
            maxSpeed = pDataItem->ServeItem.ballServe.speed;
        }
    }
    return maxSpeed;
}

int HandleDatasServe::getAverageSpeed() {
    if (m_datas.empty()) {
        return 0;
    }

    int totalSpeed = 0;
    for (DataItem *pDataItem : m_datas) {
        totalSpeed += pDataItem->ServeItem.ballServe.speed;
    }

    return totalSpeed / (int) m_datas.size();
}

int HandleDatasServe::getDownPoints(BallPoint *pPoint, int size) {
    int downPointsCount = 0;
    BallPoint *pBallPoint = pPoint;
    for (DataItem *pDataItem : m_datas) {
        if (m_pHandleProxy->isBallValid(pDataItem->ServeItem.ballServeDown)) {
            if (pBallPoint != NULL) {
                pBallPoint->x = pDataItem->ServeItem.ballServeDown.x;
                pBallPoint->y = pDataItem->ServeItem.ballServeDown.y;
                pBallPoint->in = pDataItem->drillResult == ER_IN;

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

int HandleDatasServe::getHitPoints(BallPoint *pPoint, int size) {
    int hitPointsCount = 0;
    BallPoint *pBallPoint = pPoint;
    for (DataItem *pDataItem : m_datas) {
        if (m_pHandleProxy->isBallValid(pDataItem->ServeItem.ballServe)) {
            if (pBallPoint != NULL) {
                pBallPoint->x = pDataItem->ServeItem.ballServe.x;
                pBallPoint->y = pDataItem->ServeItem.ballServe.y;
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

