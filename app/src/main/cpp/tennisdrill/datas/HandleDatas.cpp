#include "HandleDatas.h"

HandleDatas::~HandleDatas() {
    for (DataItem *pDataItem : m_datas) {
        delete pDataItem;
        pDataItem = NULL;
    }

    m_datas.clear();
}

void HandleDatas::reset() {
    memset(&m_drillBoxSocre, 0, sizeof(m_drillBoxSocre));
    m_datas.clear();
}

void HandleDatas::add(const DataItem &dataItem) {
    DataItem *pDataItem = new DataItem(dataItem);
    m_datas.push_back(pDataItem);
}

int HandleDatas::getLatestSpeed() {
    return m_drillBoxSocre.speed;
}

void HandleDatas::setLatestSpeed(int speed) {
    m_drillBoxSocre.speed = speed;
}

int HandleDatas::getTotalTimes() {
    return (int) m_datas.size();
}

int HandleDatas::getSuccessTimes() {
    int successTimes = 0;
    for (DataItem *pDataItem : m_datas) {
        if (pDataItem->drillResult == ER_IN) {
            successTimes++;
        }
    }

    return successTimes;
}

int HandleDatas::getSuccessRatio() {
    if (m_datas.empty()) {
        return 0;
    }

    return (getSuccessTimes() * 100) / getTotalTimes();
}

DrillResult HandleDatas::getLatestResult() {
    DrillResult drillResult = ER_INVALID;
    if (!m_datas.empty()) {
        DataItem *pDataItem = m_datas.back();
        drillResult = pDataItem->drillResult;
    }

    return drillResult;
}

const DrillBoxScore &HandleDatas::getDrillBoxScore() {
    if (m_drillBoxSocre.pBallPointDown != NULL) {
        delete[] m_drillBoxSocre.pBallPointDown;
        m_drillBoxSocre.pBallPointDown = NULL;
    }

    if (m_drillBoxSocre.pBallPointHit != NULL) {
        delete[] m_drillBoxSocre.pBallPointHit;
        m_drillBoxSocre.pBallPointHit = NULL;
    }

    m_drillBoxSocre.totalTimes = getTotalTimes();
    m_drillBoxSocre.successfulTimes = getSuccessTimes();
    m_drillBoxSocre.maxSpeed = getMaxSpeed();
    m_drillBoxSocre.averageSpeed = getAverageSpeed();
    m_drillBoxSocre.successRatio = getSuccessRatio();
    m_drillBoxSocre.netTimes = getNetTimes();
    m_drillBoxSocre.outTimes = getOutTimes();
    m_drillBoxSocre.lastResult = getLatestResult();
    m_drillBoxSocre.speed = getLatestSpeed();

    BallPoint *pBallPoint = NULL;
    int downPointsNum = getDownPoints(NULL, 0);
    pBallPoint = new BallPoint[downPointsNum];
    m_drillBoxSocre.downPointsNum = getDownPoints(pBallPoint, downPointsNum);
    m_drillBoxSocre.pBallPointDown = pBallPoint;

    int hitPointsNum = getHitPoints(NULL, 0);
    pBallPoint = new BallPoint[hitPointsNum];
    m_drillBoxSocre.hitPointsNum = getHitPoints(pBallPoint, hitPointsNum);
    m_drillBoxSocre.pBallPointHit = pBallPoint;

    return m_drillBoxSocre;
}


