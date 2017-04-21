#ifndef __INTERFACE_TENNISBASE_H__
#define __INTERFACE_TENNISBASE_H__

#include <functional>

typedef long long ll;

#if FALSE
#define TENNIS_STDCALL __attribute__(( __stdcall ))
#else
#define TENNIS_STDCALL
#endif


//TODO:
//	CourtSize 球场尺寸初始化的数据应该每次启动程序时从配置文件中，以适应不同球场的变化。这里CourtSize时一般场馆的默认值。

// OLD 做一下调整
// 比赛：pos 发球位置：1或2为A场；3或4为B场
// 训练：pos 发球位置：0为全场；1为A场；2为B场
// NEW 如下：
enum CourtArea
{
	COURTAREA_ANYWHERE,	// 任意位置
	COURTAREA_A1,		// A1面，标准坐标体系：第三象限
	COURTAREA_A2,		// A2面，标准坐标体系：第二象限
	COURTAREA_B1,		// B1面，标准坐标体系：第一象限
	COURTAREA_B2,		// B2面，标准坐标体系：第四象限
	COURTAREA_CENTER_A,	// A面 中间
	COURTAREA_CENTER_B,	// B面 中间	
	COURTAREA_A,
	COURTAREA_B,
	COURTAREA_UNKNOWN,	// 未知
		
	COURTAREA_INVALID = -1, 	// 非法位置
};

enum PlayModel
{
    PLAYMODEL_MATCH_SINGLE = 0x100,                            // 比赛单打
    PLAYMODEL_MATCH_DOUBLE = 0x200,                            // 比赛双打
    PLAYMODEL_DRILL_SINGLE = 0x400,                            // 训练单边
    PLAYMODEL_DRILL_DOUBLE = 0x800,                            // 训练双边

    PLAYMODEL_MASK = 0x0F00,

    PLAYMODEL_DRILL_SERVE = 1,                            // 发球
    PLAYMODEL_DRILL_RECEIVE = 2,                          // 接发球
    PLAYMODEL_DRILL_FOREHAND_HIT = 3,                     // 正手击球
    PLAYMODEL_DRILL_FOREHAND_STRAIGHT = 4,                // 正手直线
    PLAYMODEL_DRILL_FOREHAND_SLASH = 5,                   // 正手斜线
    PLAYMODEL_DRILL_BACKHAND_HIT = 6,                     // 反手击球
    PLAYMODEL_DRILL_BACKHAND_STRAIGHT = 7,                // 反手直线
    PLAYMODEL_DRILL_BACKHAND_SLASH = 8,                   // 反手斜线
    PLAYMODEL_DRILL_VOLLEY = 9,                           // 截击
    PLAYMODEL_DRILL_CASUAL = 10,                           // 随意打
    PLAYMODEL_DRILL_MULTI_BALL = 11,                      // 多球
    PLAYMODEL_DRILL_UNKNOWN = 100,                         // 未知

    PLAYMODEL_DRILL_MODEL_MASK = 0xFF,
};

enum BallStat
{
	BALLSTAT_NULL = 0,
	BALLSTAT_HIT = 1,                                // 击球
	BALLSTAT_SERVE = 2,                              // 发球
	BALLSTAT_TOUCH_DOWN = 3,                         // 落地
	BALLSTAT_TOUCH_NET = 4,                          // 触球网
	BALLSTAT_TOUCH_ANY = 5,                          // 触异物
	BALLSTAT_OUT = 6,                                // 飞出视野
	BALLSTAT_NON_UNIQUENESS = 7,                     // 不唯一
	BALLSTAT_ERROR = 8,                              // 异常
	BALLSTAT_IN = 9,                                 // 飞入视野
	BALLSTAT_HIT_GUESS = 10,                         // 猜测击球
	BALLSTAT_ERROR_HIT = 11,                         // 异常击球
	BALLSTAT_WAITING = 12                            // 等待状态
};

enum BallPower
{
	BALLPOWER_INVALID = 0,
	BALLPOWER_A = 1,		// A面球权
	BALLPOWER_B = 2,		// B面球权
};

enum BallDirection
{
	BALLDIRECTION_INVALID = 0,
	BALLDIRECTION_A = 1,	// 球朝向A面
	BALLDIRECTION_B = 2,	// 球朝向B面
	BALLDIRECTION_UNKNOWN = 3,	// 球未知朝向
};

enum Player
{
	PLAYER_INVALID = 0,
	PLAYER_A = 1,			// A面选手
	PLAYER_B = 2,			// B面选手
};

enum PlayerServe
{
	PALYERSERVE_ACE = 0,		// ACE
	PALYERSERVE_FIRST = 1,		// 一发
	PLAYERSERVE_SECOND = 2,		// 二发
};

#pragma pack(push, 4)
struct Ball
{
    int x;                                            // 球坐标x，单位：毫米
    int y;											  // 球坐标y，单位：毫米
    int z;											  // 球坐标z，单位：毫米
    int speed;										  // 球速度
    ll  timestamp;                                    // 当前时间戳,单位：毫秒
};

struct CourtSize
{
    // unit : mm
    int singleX;               // 单人场地x轴范围
    int doubleX;               // 双人场地x轴范围
    int courtX;                // 场馆x轴范围
    int singleY;                // 单人场地y轴范围
    int doubleY;                // 双人场地y轴范围
    int courtY;                 // 场馆y轴范围

    int severX;                // 发球线 x 坐标，默认值6401
    int baselineX;              // 底线 x 坐标
    int centerLineWidth;        // 分区线的宽度，通常在50-100mm，默认是50mm
};

struct InitParam
{
    PlayModel playModel;             // 打球模式
	CourtArea servePosition;           // 发球位置或被训练的人所在位置
    CourtSize courtSize;                // 球场大小

    std::string strAppPath;            // 程序所在目录，以"/"结尾
    std::string strCfgPath;            // 配置文件目录，以"/"结尾
    std::string strTmpPath;            // 临时目录（轨迹跟踪图片等）
};

// 这是与底层交互的接口，不可以更改！
// 若需要修改，则需要与底层团队协商
///////////////////////////////////////////////////the interface///////////////////////////////////////////////////////////////////////////////////
struct I_TennisBase
{
public:			   

	/**
	* 增加底层引用计数
	* @return 返回 对象被引用几次
	*/
	virtual unsigned int TENNIS_STDCALL addRef() = 0;
	//{
	//	return ++m_uRefCount;
	//}

	/**
	* 释放底层引用计数，当引用计数等于 0 时，释放此对象
	* @return 返回 对象被引用几次
	*/
	virtual unsigned int TENNIS_STDCALL releaseRef() = 0;
	//{
	//	if (--m_uRefCount == 0)
	//	{
	//		delete this;
	//		return 0;
	//	}

	//	return m_uRefCount;
	//}


	/**
	* 初始化
	* @param initParam 初始化参数
	* @return 成功返回 true；失败返回 false
	*/
    virtual bool TENNIS_STDCALL init( const InitParam &initParam ) = 0; 


	/**
	* 启动所有摄像头，捕捉视频并分析
	* @return 成功返回 true；失败返回 false
	*/
    virtual bool TENNIS_STDCALL start()   = 0;              


	/**
	* 通知底层暂时停止数据处理
	* @return 成功返回 true；失败返回 false
	*/
    virtual bool TENNIS_STDCALL pause()   = 0;                           


	/**
     * 通知重新开始处理数据
     * @return 成功返回 true；失败返回 false
     */
    virtual bool TENNIS_STDCALL resume()  = 0;  


	/**
	* 终止底层服务
	* @return 成功返回 true；失败返回 false
	*/
    virtual bool TENNIS_STDCALL stop()    = 0;       


	/**
	* 获取底层数据
	* @return 成功返回 true；失败返回 false
	*/
    virtual bool TENNIS_STDCALL runOnce() = 0;                             


	/**
	* 开始一个新的回合
	* @return 成功返回 true；失败返回 false
	*/
	virtual bool TENNIS_STDCALL restart(CourtArea serveArea) = 0;   

protected:
	virtual ~I_TennisBase()
	{

	}

public:
	/**
	* 每10ms输出一次球的信息
	*/
	std::function<void(BallStat ballStat, const Ball &ball)> onBall;


	/**
	* 任意情况下出现不可修复异常，比如启动时找不到摄像头,分析过程中设备故障等
	*/
	std::function<void(int errorCode, const char *pErrorMessage)> onError;


	/**
	 *  线程创建与退出时，回调
	 */
	std::function<void(bool isExit)> onThread;


	/**
	* 底层已经准备好，可以开始下一场训练
	*/
	std::function<void()> onDrillNext;

//private:
//	unsigned int m_uRefCount;
};
///////////////////////////////////////////////////the interface///////////////////////////////////////////////////////////////////////////////////
#pragma pack(pop)


typedef I_TennisBase *(*CREATE_TENNISBASE_INTERFACE)();

#endif // __INTERFACE_BASECONSTRUCT_H__