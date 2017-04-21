#ifndef __HANDLEDATAS_H__
#define __HANDLEDATAS_H__

#include "../HandleProxy.h"

class HandleProxy;

class HandleDatas {
public:
    HandleDatas(HandleProxy *pHandleProxy)
            : m_pHandleProxy(pHandleProxy) {
        reset();
    }

    virtual ~HandleDatas();

    void reset();

    void add(const DataItem &dataItem);

    int getLatestSpeed();                 // 最近一次发球的球速
    void setLatestSpeed(int speed);

    int getTotalTimes();     // 总次数
    int getSuccessTimes();     // 成功次数
    int getSuccessRatio();

    DrillResult getLatestResult();     // 获取最后一次结果

    const DrillBoxScore &getDrillBoxScore();

public:
    virtual int getMaxSpeed()     = 0;     // 最大球速
    virtual int getAverageSpeed() = 0;     // 平均球速
    virtual int getNetTimes()     = 0;     // 下网次数
    virtual int getOutTimes()     = 0;     // 出界次数


    virtual int getDownPoints(BallPoint *pPoint, int size) = 0;        //获取落点数据
    virtual int getHitPoints(BallPoint *pPoint, int size)  = 0;        //获取击球点数据

protected:
    HandleProxy *m_pHandleProxy;

    DrillBoxScore m_drillBoxSocre;
    std::list<DataItem *> m_datas;
};

#endif // DRILLDATABASE_H__
