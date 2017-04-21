#include "HandleDatasVolley.h"

int HandleDatasVolley::getMaxSpeed() {
    int maxSpeed = 0;
    for (DataItem *pDataItem : m_datas) {
        if (pDataItem->VolleyItem.ballHit.speed > maxSpeed) {
            maxSpeed = pDataItem->VolleyItem.ballHit.speed;
        }
    }

    return maxSpeed;
}

int HandleDatasVolley::getAverageSpeed() {
    if (m_datas.empty()) {
        return 0;
    }

    int totalSpeed = 0;
    for (DataItem *pDataItem : m_datas) {
        totalSpeed += pDataItem->VolleyItem.ballHit.speed;
    }

    return totalSpeed / (int) m_datas.size();
}

int HandleDatasVolley::getDownPoints(BallPoint *pPoint, int size) {
    int downPointsCount = 0;
    BallPoint *pBallPoint = pPoint;
    for (DataItem *pDataItem : m_datas) {
        if (m_pHandleProxy->isBallValid(pDataItem->VolleyItem.ballHitDown)) {
            if (pBallPoint != NULL) {
                pBallPoint->x = pDataItem->VolleyItem.ballHitDown.x;
                pBallPoint->y = pDataItem->VolleyItem.ballHitDown.y;
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

int HandleDatasVolley::getHitPoints(BallPoint *pPoint, int size) {
    int hitPointsCount = 0;
    BallPoint *pBallPoint = pPoint;
    for (DataItem *pDataItem : m_datas) {
        if (m_pHandleProxy->isBallValid(pDataItem->VolleyItem.ballHit)) {
            if (pBallPoint != NULL) {
                pBallPoint->x = pDataItem->VolleyItem.ballHit.x;
                pBallPoint->y = pDataItem->VolleyItem.ballHit.y;

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

