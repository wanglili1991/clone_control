#include "TennisDrill.h"

#include <TennisBaseHelper.h>

using namespace std;

TennisDrill::TennisDrill()
        : m_uRefCount(0), m_pTennisBase(NULL), m_pTextFileLogger(NULL), m_pHandlerA(NULL), m_pHandlerB(NULL), m_bStart(false),
          m_isBaseInitialized(false), m_isDrillNext(false) {

}

TennisDrill::~TennisDrill() {
    if (m_isBaseInitialized)
        stop();

    m_isBaseInitialized = false;
    m_bStart = false;
    //m_bBilateral = false;

    if (m_pHandlerA != NULL) {
        delete m_pHandlerA;
        m_pHandlerA = NULL;
    }

    if (m_pHandlerB != NULL) {
        delete m_pHandlerB;
        m_pHandlerB = NULL;
    }

    if (m_pTextFileLogger != NULL) {
        Log_RemoveLogger(m_pTextFileLogger);

        Log_Prune(delete m_pTextFileLogger;)
        m_pTextFileLogger = NULL;
    }

    if (m_pTennisBase != NULL) {
        TennisBaseHelper::DestroyTennisBase(m_pTennisBase);
        m_pTennisBase = NULL;
    }
}

bool TennisDrill::init(const InitParam &initParam, I_DrillCallback *pDrillCallback, DrillDifficulty drillDifficulty) {
    std::string strLog = initParam.strTmpPath + "TennisDrill.txt";
    m_pTextFileLogger = ALoxHelper::CreateDbgTextFileLogger(strLog.c_str());

    if (!initHandler(initParam, pDrillCallback, drillDifficulty))
        return false;

    std::string strFileName = initParam.strAppPath + "libtennisbase.so";
    m_pTennisBase = TennisBaseHelper::CreateTennisBase(strFileName.c_str());
    if (m_pTennisBase == NULL)
        return false;

    initCallback();

    return init(initParam);
}

bool TennisDrill::init(const InitParam &initParam)
{
    return m_pTennisBase->init(initParam);
}

bool TennisDrill::start() {
    m_bStart = (m_pTennisBase == NULL) ? false : m_pTennisBase->start();
    m_isDrillNext = true;

    if (m_pHandlerA != NULL) {
        m_pHandlerA->start();
    }

    if (m_pHandlerB != NULL) {
        m_pHandlerB->start();
    }

    return m_bStart;
}

bool TennisDrill::pause() {
    bool result = (m_pTennisBase == NULL) ? false : m_pTennisBase->pause();
    if (result) {
        m_bStart = false;
    }

    return result;
}

bool TennisDrill::resume() {
    bool result = (m_pTennisBase == NULL) ? false : m_pTennisBase->resume();
    if (result) {
        m_bStart = true;
    }

    return result;
}

bool TennisDrill::stop() {
    bool result = (m_pTennisBase == NULL) ? false : m_pTennisBase->stop();
    if (result) {
        m_bStart = false;
    }

    if (m_pHandlerA != NULL) {
        m_pHandlerA->stop();
    }

    if (m_pHandlerB != NULL) {
        m_pHandlerB->stop();
    }

    return result;
}

bool TennisDrill::runOnce() {
    if (!m_bStart)
        return false;

    return (m_pTennisBase == NULL) ? false : m_pTennisBase->runOnce();
}

bool TennisDrill::restart(CourtArea serveArea) {
    bool result = (m_pTennisBase == NULL) ? false : m_pTennisBase->restart(serveArea);
    if (!result) {
        m_bStart = false;
    }

    if (m_pHandlerA != NULL) {
        m_pHandlerA->restart();
    }

    if (m_pHandlerB != NULL) {
        m_pHandlerB->restart();
    }

    return result;
}

bool TennisDrill::restartDrill(CourtArea courtArea) {
    m_isDrillNext = false;

#ifndef __TENNIS_DRILL_TEST__
    m_pTennisBase->restart(courtArea);
#endif

    return true;
}

bool TennisDrill::initHandler(const InitParam &initParam, I_DrillCallback *pDrillCallback, DrillDifficulty drillDifficulty) {
    HandleProxy *pHandleProxy = new HandleProxy(this);
    if (!pHandleProxy->init(initParam, pDrillCallback, drillDifficulty)) {
        delete pHandleProxy;
        return false;
    }

    CourtArea trainedSide = pHandleProxy->getTrainedSide();
    if (trainedSide == COURTAREA_A) {
        if (m_pHandlerA != NULL)
            delete m_pHandlerA;

        m_pHandlerA = pHandleProxy;
    } else if (trainedSide == COURTAREA_B) {
        if (m_pHandlerB != NULL)
            delete m_pHandlerB;

        m_pHandlerB = pHandleProxy;
    }

    return true;
}

void TennisDrill::initCallback() {
    m_pTennisBase->onBall = [this](BallStat stat, Ball ball) {
        if (!m_isDrillNext) {
            return;
        }

        if (!m_bStart) {
            return;
        }

        if (NULL != m_pHandlerA) {
            m_pHandlerA->onBall(stat, ball);
        }

        if (NULL != m_pHandlerB) {
            m_pHandlerB->onBall(stat, ball);
        }
    };

    m_pTennisBase->onError = [this](int errorCode, const char *pErrorMessage) {
        if (NULL != m_pHandlerA) {
            m_pHandlerA->onError(errorCode, pErrorMessage);
        }

        if (NULL != m_pHandlerB) {
            m_pHandlerB->onError(errorCode, pErrorMessage);
        }
    };

    m_pTennisBase->onDrillNext = [this]() {
        m_isDrillNext = true;

        if (NULL != m_pHandlerA) {
            m_pHandlerA->onDrillNext();
        }

        if (NULL != m_pHandlerB) {
            m_pHandlerB->onDrillNext();
        }
    };

    m_pTennisBase->onThread = [this](bool isExit) {
        if (NULL != m_pHandlerA) {
            m_pHandlerA->onThread(isExit);
        }

        if (NULL != m_pHandlerB) {
            m_pHandlerB->onThread(isExit);
        }
    };
}

